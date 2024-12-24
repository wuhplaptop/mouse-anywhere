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
} MovementParams;

// Function prototypes
double ease(double t, EasingType easing_type);
void set_cursor_abs(int x, int y);
void set_cursor_rel(int delta_x, int delta_y);
void enforce_screen_bounds(int* x, int* y);
DWORD WINAPI smooth_move_thread(LPVOID arg);
DWORD WINAPI input_thread(LPVOID arg);
void log_message(const char* message);
void enqueue_target(MovementParams* params, int x, int y, bool relative);
bool dequeue_target(MovementParams* params, int* x, int* y, bool* relative);
void parse_arguments(int argc, char* argv[], MovementParams* params);

// Global log file pointer
FILE* logFile = NULL;

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

// Function to set cursor to an absolute position using SendInput
void set_cursor_abs(int x, int y) {
    // Get virtual screen resolution
    int virtualLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int virtualTop = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int virtualWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int virtualHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    // Normalize coordinates to [0, 65535]
    double normalizedX = ((double)(x - virtualLeft) * 65535.0) / (virtualWidth - 1);
    double normalizedY = ((double)(y - virtualTop) * 65535.0) / (virtualHeight - 1);

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    input.mi.dx = (LONG)normalizedX;
    input.mi.dy = (LONG)normalizedY;

    UINT sent = SendInput(1, &input, sizeof(INPUT));
    if (sent != 1) {
        log_message("Error: SendInput failed to move cursor.");
    }
}

// Function to move cursor relative to its current position
void set_cursor_rel(int delta_x, int delta_y) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = delta_x;
    input.mi.dy = delta_y;

    UINT sent = SendInput(1, &input, sizeof(INPUT));
    if (sent != 1) {
        log_message("Error: SendInput failed to move cursor relatively.");
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
                set_cursor_rel(move_x, move_y);
            } else {
                set_cursor_abs(current_x, current_y);
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

// Thread function for handling user input
DWORD WINAPI input_thread(LPVOID arg) {
    MovementParams* params = (MovementParams*)arg;
    char command[256];

    printf("Enter commands to add targets or perform actions.\n");
    printf("Commands:\n");
    printf("  add_abs x y       - Add an absolute target at (x, y)\n");
    printf("  add_rel dx dy     - Add a relative movement by (dx, dy)\n");
    printf("  quit              - Exit the program\n");
    printf("Example: add_abs 1000 800\n");
    printf("Example: add_rel -50 50\n");

    while (true) {
        printf("> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            continue;
        }

        // Remove trailing newline
        command[strcspn(command, "\n")] = 0;

        // Parse the command
        if (strncmp(command, "add_abs", 7) == 0) {
            int x, y;
            if (sscanf(command + 7, "%d %d", &x, &y) == 2) {
                enqueue_target(params, x, y, false);
            } else {
                printf("Invalid command format. Usage: add_abs x y\n");
            }
        } else if (strncmp(command, "add_rel", 7) == 0) {
            int dx, dy;
            if (sscanf(command + 7, "%d %d", &dx, &dy) == 2) {
                enqueue_target(params, dx, dy, true);
            } else {
                printf("Invalid command format. Usage: add_rel dx dy\n");
            }
        } else if (strcmp(command, "quit") == 0) {
            printf("Quit command received. Exiting...\n");
            log_message("Quit command received. Exiting program.");
            EnterCriticalSection(&params->cs);
            params->active = false;
            LeaveCriticalSection(&params->cs);
            break;
        } else {
            printf("Unknown command. Available commands: add_abs, add_rel, quit\n");
        }
    }

    return 0;
}

// Function to parse command-line arguments and set movement parameters
void parse_arguments(int argc, char* argv[], MovementParams* params) {
    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--target_x") == 0) && i + 1 < argc) {
            int x = atoi(argv[++i]);
            params->target_x = x;
        } else if ((strcmp(argv[i], "-y") == 0 || strcmp(argv[i], "--target_y") == 0) && i + 1 < argc) {
            int y = atoi(argv[++i]);
            params->target_y = y;
        } else if ((strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--easing") == 0) && i + 1 < argc) {
            int e = atoi(argv[++i]);
            if (e >= EASE_LINEAR && e <= EASE_EXPONENTIAL) {
                params->easing_type = (EasingType)e;
            } else {
                printf("Invalid easing type. Using default (EASE_SINUSOIDAL).\n");
            }
        } else if ((strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--strength") == 0) && i + 1 < argc) {
            int t = atoi(argv[++i]);
            if (t >= MIN_STRENGTH && t <= MAX_STRENGTH) {
                params->strength = t;
            } else {
                printf("Strength must be between %d and %d. Using default (50).\n", MIN_STRENGTH, MAX_STRENGTH);
            }
        } else {
            printf("Unknown argument: %s\n", argv[i]);
            printf("Usage: %s [-x target_x] [-y target_y] [-e easing_type] [-t strength]\n", argv[0]);
            printf("Easing Types:\n");
            printf("  1 - Linear\n");
            printf("  2 - Quadratic\n");
            printf("  3 - Sinusoidal\n");
            printf("  4 - Cubic\n");
            printf("  5 - Exponential\n");
            exit(1);
        }
    }
}

int main(int argc, char* argv[]) {
    // Open log file
    logFile = fopen(LOG_FILE, "a");
    if (!logFile) {
        printf("Error: Unable to open log file.\n");
        return 1;
    }
    log_message("Program started.");

    srand((unsigned int)time(NULL)); // Initialize random seed if needed

    printf("Starting enhanced mouse movement program...\n");
    log_message("Starting enhanced mouse movement program.");

    // Define movement parameters with default values
    MovementParams params = {
        .target_x = 800,                // Initial Target X position (not used in queue)
        .target_y = 600,                // Initial Target Y position (not used in queue)
        .easing_type = EASE_SINUSOIDAL, // Easing type for smoothness
        .strength = 50,                 // Strength (maps to steps)
        .active = true,                 // Active state
        .paused = false,                // Paused state
        .queue_head = NULL,
        .queue_tail = NULL
    };

    // Initialize the critical section
    InitializeCriticalSection(&params.cs);

    // Parse command-line arguments to override default parameters
    parse_arguments(argc, argv, &params);

    // Log initial parameters
    char buffer[200];
    snprintf(buffer, sizeof(buffer),
             "Parameters - Initial Target: (%d, %d), Easing Type: %d, Strength: %d",
             params.target_x,
             params.target_y,
             params.easing_type,
             params.strength);
    log_message(buffer);

    // Create a thread to perform smooth movement
    HANDLE move_thread = CreateThread(NULL, 0, smooth_move_thread, &params, 0, NULL);
    if (move_thread == NULL) {
        log_message("Error: Unable to create movement thread.");
        fclose(logFile);
        DeleteCriticalSection(&params.cs);
        return 1;
    }

    // Create a thread to handle user input
    HANDLE in_thread = CreateThread(NULL, 0, input_thread, &params, 0, NULL);
    if (in_thread == NULL) {
        log_message("Error: Unable to create input thread.");
        fclose(logFile);
        DeleteCriticalSection(&params.cs);
        return 1;
    }

    // Wait for the input thread to finish (which will happen when 'quit' is called)
    WaitForSingleObject(in_thread, INFINITE);

    // After quitting, wait for the movement thread to finish
    WaitForSingleObject(move_thread, INFINITE);

    // Clean up
    CloseHandle(move_thread);
    CloseHandle(in_thread);
    log_message("Movement thread has terminated.");
    fclose(logFile);
    DeleteCriticalSection(&params.cs);

    printf("Movement complete. Exiting program.\n");
    return 0;
}
