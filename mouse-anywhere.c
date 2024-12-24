// mouse-anywhere.c
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// Define M_PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846 // π
#endif

// Constants
#define INITIAL_DELAY_MS 3000       // 3-second initial delay
#define SMOOTH_DELAY_MS 5           // Delay between each step in milliseconds
#define MIN_STRENGTH 1
#define MAX_STRENGTH 100
#define TARGET_RADIUS 10            // Radius within which the target is considered hit
#define LOG_FILE "mouse_movement.log"

// Easing Types
typedef enum {
    EASE_LINEAR = 1,
    EASE_QUADRATIC,
    EASE_SINUSOIDAL,
    EASE_CUBIC,
    EASE_EXPONENTIAL
} EasingType;

// Structure to hold a single target
typedef struct Target {
    int x;
    int y;
    bool relative; // true for relative movement, false for absolute
    struct Target* next;
} Target;

// Structure to hold movement parameters with synchronization and queue
typedef struct {
    int target_x;
    int target_y;
    EasingType easing_type;
    int strength;
    bool active;
    bool paused;
    CRITICAL_SECTION cs; // Critical section for thread safety

    // Queue for multiple targets
    Target* queue_head;
    Target* queue_tail;

    // Threads
    HANDLE move_thread;
} MovementParams;

// Function prototypes with export declarations
__declspec(dllexport) void initialize();
__declspec(dllexport) void mouse_shutdown(); // Renamed from 'shutdown'
__declspec(dllexport) void set_cursor_abs(int x, int y);
__declspec(dllexport) void set_cursor_rel(int delta_x, int delta_y);
__declspec(dllexport) void enqueue_target_abs(int x, int y);
__declspec(dllexport) void enqueue_target_rel(int delta_x, int delta_y);

// Internal function prototypes
double ease(double t, EasingType easing_type);
void set_cursor_abs_internal(int x, int y);
void set_cursor_rel_internal(int delta_x, int delta_y);
void enforce_screen_bounds(int* x, int* y);
DWORD WINAPI smooth_move_thread(LPVOID arg);
void log_message(const char* message);
void enqueue_target(MovementParams* params, int x, int y, bool relative);
bool dequeue_target(MovementParams* params, int* x, int* y, bool* relative);

// Global variables
FILE* logFile = NULL;
MovementParams* global_params = NULL;

// Function to calculate eased values (linear, quadratic, sinusoidal, cubic, exponential easing)
double ease(double t, EasingType easing_type) {
    switch (easing_type) {
        case EASE_LINEAR:
            return t;
        case EASE_QUADRATIC:
            return t * t;
        case EASE_SINUSOIDAL:
            return sin(t * (M_PI / 2));
        case EASE_CUBIC:
            return t * t * t;
        case EASE_EXPONENTIAL:
            return (t == 0.0) ? 0.0 : pow(2, 10 * (t - 1));
        default:
            return t;
    }
}

// Function to set cursor to an absolute position using SetCursorPos
void set_cursor_abs_internal(int x, int y) {
    // Enforce screen boundaries
    enforce_screen_bounds(&x, &y);

    BOOL result = SetCursorPos(x, y);
    if (!result) {
        log_message("Error: SetCursorPos failed to move cursor.");
    } else {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Moved cursor to absolute position (%d, %d)", x, y);
        log_message(buffer);
    }
}

// Function to move cursor relative to its current position
void set_cursor_rel_internal(int delta_x, int delta_y) {
    POINT pos;
    if (GetCursorPos(&pos)) {
        pos.x += delta_x;
        pos.y += delta_y;
        // Enforce screen boundaries
        enforce_screen_bounds(&pos.x, &pos.y);
        BOOL result = SetCursorPos(pos.x, pos.y);
        if (!result) {
            log_message("Error: SetCursorPos failed to move cursor relatively.");
        } else {
            char buffer[100];
            snprintf(buffer, sizeof(buffer), "Moved cursor relatively by (%d, %d)", delta_x, delta_y);
            log_message(buffer);
        }
    } else {
        log_message("Error: GetCursorPos failed.");
    }
}

// Function to ensure movement stays within screen boundaries (handles multi-monitor setups)
void enforce_screen_bounds(int* x, int* y) {
    // Get the virtual screen boundaries (all monitors)
    int virtualLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int virtualTop = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int virtualRight = virtualLeft + GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int virtualBottom = virtualTop + GetSystemMetrics(SM_CYVIRTUALSCREEN);

    if (*x < virtualLeft) *x = virtualLeft;
    if (*y < virtualTop) *y = virtualTop;
    if (*x >= virtualRight) *x = virtualRight - 1;
    if (*y >= virtualBottom) *y = virtualBottom - 1;
}

// Logging function to write messages to a log file
void log_message(const char* message) {
    if (logFile) {
        // Get current time
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", t);

        fprintf(logFile, "[%s] %s\n", timeStr, message);
        fflush(logFile);
    }
}

// Enqueue a new target to the movement queue
void enqueue_target(MovementParams* params, int x, int y, bool relative) {
    Target* new_target = (Target*)malloc(sizeof(Target));
    if (!new_target) {
        log_message("Error: Memory allocation failed for new target.");
        return;
    }
    new_target->x = x;
    new_target->y = y;
    new_target->relative = relative;
    new_target->next = NULL;

    EnterCriticalSection(&params->cs);
    if (params->queue_tail == NULL) {
        params->queue_head = params->queue_tail = new_target;
    } else {
        params->queue_tail->next = new_target;
        params->queue_tail = new_target;
    }
    LeaveCriticalSection(&params->cs);

    char buffer[100];
    snprintf(buffer, sizeof(buffer),
             "Enqueued new target: (%d, %d) [%s]",
             x, y, relative ? "Relative" : "Absolute");
    log_message(buffer);
}

// Dequeue a target from the movement queue
bool dequeue_target(MovementParams* params, int* x, int* y, bool* relative) {
    EnterCriticalSection(&params->cs);
    if (params->queue_head == NULL) {
        LeaveCriticalSection(&params->cs);
        return false;
    }

    Target* target = params->queue_head;
    *x = target->x;
    *y = target->y;
    *relative = target->relative;

    params->queue_head = target->next;
    if (params->queue_head == NULL) {
        params->queue_tail = NULL;
    }
    LeaveCriticalSection(&params->cs);

    free(target);
    return true;
}

// Thread function for smooth mouse movement
DWORD WINAPI smooth_move_thread(LPVOID arg) {
    MovementParams* params = (MovementParams*)arg;

    // Initial delay before starting movement
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Movement will start in %d seconds...", INITIAL_DELAY_MS / 1000);
    log_message(buffer);
    Sleep(INITIAL_DELAY_MS);

    while (true) {
        // Check if active
        EnterCriticalSection(&params->cs);
        bool is_active = params->active;
        bool is_paused = params->paused;
        LeaveCriticalSection(&params->cs);

        if (!is_active) {
            break;
        }

        if (is_paused) {
            Sleep(100);
            continue;
        }

        // Dequeue the next target
        int target_x, target_y;
        bool relative;
        bool has_target = dequeue_target(params, &target_x, &target_y, &relative);

        if (!has_target) {
            // No targets in the queue, sleep briefly
            Sleep(100);
            continue;
        }

        // Get starting cursor position
        POINT start_pos;
        if (!GetCursorPos(&start_pos)) {
            log_message("Error: GetCursorPos failed.");
            continue;
        }

        // Calculate target position
        int final_x, final_y;
        if (relative) {
            final_x = start_pos.x + target_x;
            final_y = start_pos.y + target_y;
        } else {
            final_x = target_x;
            final_y = target_y;
        }

        // Enforce screen boundaries
        enforce_screen_bounds(&final_x, &final_y);

        // Calculate deltas
        double delta_x = (double)(final_x - start_pos.x);
        double delta_y = (double)(final_y - start_pos.y);

        // Retrieve current parameters
        EnterCriticalSection(&params->cs);
        EasingType easing = params->easing_type;
        int strength = params->strength;
        LeaveCriticalSection(&params->cs);

        // Map strength to steps (higher strength -> fewer steps -> faster movement)
        int steps = MAX_STRENGTH - strength + 1;
        if (steps < 1) steps = 1;

        // Get high-resolution timer frequency
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);

        for (int i = 1; i <= steps; i++) {
            // Check if paused or inactive
            EnterCriticalSection(&params->cs);
            is_active = params->active;
            is_paused = params->paused;
            LeaveCriticalSection(&params->cs);

            if (!is_active) {
                break;
            }

            if (is_paused) {
                Sleep(100);
                i--; // Adjust step count to account for pause
                continue;
            }

            double progress = (double)i / steps;
            double eased_progress = ease(progress, easing);

            // Calculate the current position based on easing
            int current_x = start_pos.x + (int)(delta_x * eased_progress);
            int current_y = start_pos.y + (int)(delta_y * eased_progress);

            // Enforce screen boundaries
            enforce_screen_bounds(&current_x, &current_y);

            // Move the cursor to the current position
            if (relative) {
                int move_x = current_x - start_pos.x;
                int move_y = current_y - start_pos.y;
                set_cursor_rel_internal(move_x, move_y);
            } else {
                set_cursor_abs_internal(current_x, current_y);
            }

            // Log the movement
            snprintf(buffer, sizeof(buffer), "Moved to (%d, %d)", current_x, current_y);
            log_message(buffer);

            // Optional: Check if the cursor is within the target radius
            POINT cursor_pos;
            if (GetCursorPos(&cursor_pos)) {
                double distance = sqrt(pow(cursor_pos.x - final_x, 2) +
                                       pow(cursor_pos.y - final_y, 2));
                if (distance <= TARGET_RADIUS) {
                    log_message("Target reached within radius.");
                    break;
                }
            } else {
                log_message("Error: GetCursorPos failed during movement.");
                break;
            }

            // High-resolution sleep
            LARGE_INTEGER start, end;
            QueryPerformanceCounter(&start);
            double target_time = (double)SMOOTH_DELAY_MS / 1000.0; // in seconds

            // Busy-wait until the target time has passed
            while (true) {
                QueryPerformanceCounter(&end);
                double elapsed = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
                if (elapsed >= target_time) {
                    break;
                }
            }
        }

        snprintf(buffer, sizeof(buffer), "Completed movement to (%d, %d)", final_x, final_y);
        log_message(buffer);
    }

    log_message("Movement thread has terminated.");
    return 0;
}

// DllMain (for initialization and cleanup)
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                      )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Initialization can be done here if needed
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // Cleanup can be done here if needed
        break;
    }
    return TRUE;
}

// Initialize the DLL
__declspec(dllexport) void initialize() {
    // Open log file
    logFile = fopen(LOG_FILE, "a");
    if (!logFile) {
        // Handle error
        return;
    }
    log_message("DLL initialized.");

    // Allocate and initialize MovementParams
    global_params = (MovementParams*)malloc(sizeof(MovementParams));
    if (!global_params) {
        log_message("Error: Unable to allocate MovementParams.");
        return;
    }
    global_params->target_x = 800;
    global_params->target_y = 600;
    global_params->easing_type = EASE_SINUSOIDAL;
    global_params->strength = 50;
    global_params->active = true;
    global_params->paused = false;
    global_params->queue_head = NULL;
    global_params->queue_tail = NULL;
    InitializeCriticalSection(&global_params->cs);
    global_params->move_thread = CreateThread(NULL, 0, smooth_move_thread, global_params, 0, NULL);
    if (global_params->move_thread == NULL) {
        log_message("Error: Unable to create movement thread.");
    }

    log_message("MovementParams initialized and movement thread started.");
}

// Shutdown the DLL
__declspec(dllexport) void mouse_shutdown() { // Renamed from 'shutdown'
    log_message("Shutdown called.");

    if (global_params) {
        EnterCriticalSection(&global_params->cs);
        global_params->active = false;
        LeaveCriticalSection(&global_params->cs);

        // Wait for the movement thread to finish
        if (global_params->move_thread != NULL) {
            WaitForSingleObject(global_params->move_thread, INFINITE);
            CloseHandle(global_params->move_thread);
            global_params->move_thread = NULL;
        }

        // Clean up queue
        Target* current = global_params->queue_head;
        while (current) {
            Target* next = current->next;
            free(current);
            current = next;
        }

        DeleteCriticalSection(&global_params->cs);
        free(global_params);
        global_params = NULL;
    }

    if (logFile) {
        log_message("DLL shutting down.");
        fclose(logFile);
        logFile = NULL;
    }
}

// Function to set cursor to an absolute position
__declspec(dllexport) void set_cursor_abs(int x, int y) {
    set_cursor_abs_internal(x, y);
}

// Function to move cursor relative to current position
__declspec(dllexport) void set_cursor_rel(int delta_x, int delta_y) {
    set_cursor_rel_internal(delta_x, delta_y);
}

// Function to enqueue an absolute target
__declspec(dllexport) void enqueue_target_abs(int x, int y) {
    if (global_params) {
        enqueue_target(global_params, x, y, false);
    } else {
        log_message("Error: DLL not initialized. Call initialize() first.");
    }
}

// Function to enqueue a relative target
__declspec(dllexport) void enqueue_target_rel(int delta_x, int delta_y) {
    if (global_params) {
        enqueue_target(global_params, delta_x, delta_y, true);
    } else {
        log_message("Error: DLL not initialized. Call initialize() first.");
    }
}
