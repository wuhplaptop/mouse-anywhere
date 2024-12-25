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
    bool smooth;              // Toggle for smooth or instant movement
    CRITICAL_SECTION cs;      // Critical section for thread safety

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
__declspec(dllexport) void perform_click(int button);

// Internal function prototypes
double ease(double t, EasingType easing_type);
void set_cursor_abs_internal(int x, int y, bool smooth);
void set_cursor_rel_internal(int delta_x, int delta_y, bool smooth);
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

// Function to ensure movement stays within screen boundaries (handles multi-monitor setups)
void enforce_screen_bounds(int* x, int* y) {
    int virtualLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int virtualTop = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int virtualRight = virtualLeft + GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int virtualBottom = virtualTop + GetSystemMetrics(SM_CYVIRTUALSCREEN);

    if (*x < virtualLeft) *x = virtualLeft;
    if (*y < virtualTop) *y = virtualTop;
    if (*x >= virtualRight) *x = virtualRight - 1;
    if (*y >= virtualBottom) *y = virtualBottom - 1;
}

// Function to set cursor to an absolute position
void set_cursor_abs_internal(int x, int y, bool smooth) {
    enforce_screen_bounds(&x, &y);

    if (!smooth) {
        SetCursorPos(x, y);
        log_message("Mouse teleported to absolute position.");
        return;
    }

    POINT start_pos;
    if (GetCursorPos(&start_pos)) {
        double delta_x = x - start_pos.x;
        double delta_y = y - start_pos.y;
        int steps = MAX_STRENGTH - global_params->strength + 1;

        for (int i = 1; i <= steps; i++) {
            double progress = ease((double)i / steps, global_params->easing_type);
            int current_x = start_pos.x + (int)(delta_x * progress);
            int current_y = start_pos.y + (int)(delta_y * progress);
            enforce_screen_bounds(&current_x, &current_y);
            SetCursorPos(current_x, current_y);
            Sleep(SMOOTH_DELAY_MS);
        }
    }
}

// Function to move cursor relative to its current position
void set_cursor_rel_internal(int delta_x, int delta_y, bool smooth) {
    POINT pos;
    if (GetCursorPos(&pos)) {
        int final_x = pos.x + delta_x;
        int final_y = pos.y + delta_y;
        set_cursor_abs_internal(final_x, final_y, smooth);
    }
}

// Function to enqueue a new target to the movement queue
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

    log_message("New target enqueued.");
}

// Function to dequeue a target from the movement queue
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

// Logging function to write messages to a log file
void log_message(const char* message) {
    if (logFile) {
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", t);

        fprintf(logFile, "[%s] %s\n", timeStr, message);
        fflush(logFile);
    }
}

// Function to perform mouse clicks
void perform_click(int button) {
    INPUT inputs[2] = {0};

    inputs[0].type = INPUT_MOUSE;
    inputs[1].type = INPUT_MOUSE;

    switch (button) {
        case 1: // Left click
            inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
            break;
        case 2: // Right click
            inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            break;
        case 3: // Middle click
            inputs[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            inputs[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            break;
        default:
            log_message("Invalid button for click.");
            return;
    }

    SendInput(2, inputs, sizeof(INPUT));
    log_message("Mouse click performed.");
}

// Initialization of the DLL
__declspec(dllexport) void initialize() {
    logFile = fopen(LOG_FILE, "a");
    if (!logFile) return;

    log_message("DLL initialized.");

    global_params = (MovementParams*)malloc(sizeof(MovementParams));
    if (!global_params) {
        log_message("Error: Unable to allocate MovementParams.");
        return;
    }

    global_params->active = true;
    global_params->paused = false;
    global_params->smooth = true; // Default to smooth movement
    global_params->easing_type = EASE_SINUSOIDAL;
    global_params->strength = 50;
    global_params->queue_head = NULL;
    global_params->queue_tail = NULL;
    InitializeCriticalSection(&global_params->cs);

    log_message("Global parameters initialized.");
}

// Shutdown and cleanup of the DLL
__declspec(dllexport) void mouse_shutdown() {
    if (global_params) {
        global_params->active = false;

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
    }
}

// DLL exported function to set cursor absolute position
__declspec(dllexport) void set_cursor_abs(int x, int y) {
    if (global_params) {
        set_cursor_abs_internal(x, y, global_params->smooth);
    }
}

// DLL exported function to set cursor relative position
__declspec(dllexport) void set_cursor_rel(int delta_x, int delta_y) {
    if (global_params) {
        set_cursor_rel_internal(delta_x, delta_y, global_params->smooth);
    }
}

// DLL exported function to enqueue an absolute target
__declspec(dllexport) void enqueue_target_abs(int x, int y) {
    if (global_params) {
        enqueue_target(global_params, x, y, false);
    }
}

// DLL exported function to enqueue a relative target
__declspec(dllexport) void enqueue_target_rel(int delta_x, int delta_y) {
    if (global_params) {
        enqueue_target(global_params, delta_x, delta_y, true);
    }
}
