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
#define MIN_STRENGTH 1
#define MAX_STRENGTH 100
#define DEFAULT_MOUSE_SPEED 5 // Default movement speed (ms per step)
#define LOG_FILE "mouse_movement.log"

// Easing Types
typedef enum {
    EASE_LINEAR = 1,
    EASE_QUADRATIC,
    EASE_SINUSOIDAL,
    EASE_CUBIC,
    EASE_EXPONENTIAL
} EasingType;

// Preset Types
typedef enum {
    PRESET_DEFAULT = 1,
    PRESET_FAST,
    PRESET_SMOOTH
} PresetType;

// Log Levels
typedef enum {
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel;

// Configurable parameters structure
typedef struct {
    int strength;
    int hold_time_ms; // Time to hold mouse clicks
    int mouse_speed; // Speed of mouse movement in ms per step
    EasingType easing_type;
    bool smooth_movement;
} Config;

// Structure to manage global parameters
typedef struct {
    Config config;
    CRITICAL_SECTION cs;
} GlobalParams;

// Global variables
FILE* logFile = NULL;
GlobalParams* global_params = NULL;
LogLevel current_log_level = LOG_LEVEL_INFO;

// Function prototypes
double ease(double t, EasingType easing_type);
void enforce_screen_bounds(int* x, int* y);
void set_cursor_abs_internal(int x, int y, bool smooth);
void perform_click(int button, int hold_time_ms);
void hold_and_move_internal(int x, int y, int button, int duration_ms, bool smooth);
void log_message(LogLevel level, const char* message);

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) void initialize();
__declspec(dllexport) void mouse_shutdown();
__declspec(dllexport) void set_cursor_abs(int x, int y);
__declspec(dllexport) void click(int button);
__declspec(dllexport) void hold_and_move(int x, int y, int button, int duration_ms);
__declspec(dllexport) void set_mouse_speed(int speed);
__declspec(dllexport) void set_config(int strength, int hold_time_ms, int mouse_speed, int easing_type, bool smooth_movement);
__declspec(dllexport) void apply_preset(int preset_type);
__declspec(dllexport) void set_logging_level(int level);

#ifdef __cplusplus
}
#endif

// Easing function
double ease(double t, EasingType easing_type) {
    switch (easing_type) {
        case EASE_LINEAR: return t;
        case EASE_QUADRATIC: return t * t;
        case EASE_SINUSOIDAL: return sin(t * (M_PI / 2));
        case EASE_CUBIC: return t * t * t;
        case EASE_EXPONENTIAL: return (t == 0.0) ? 0.0 : pow(2, 10 * (t - 1));
        default: return t;
    }
}

// Ensure cursor stays within screen bounds
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

// Move cursor while holding click
void hold_and_move_internal(int x, int y, int button, int duration_ms, bool smooth) {
    enforce_screen_bounds(&x, &y);

    INPUT input = {0};
    input.type = INPUT_MOUSE;

    DWORD button_down_flag, button_up_flag;

    switch (button) {
        case 1: // Left click
            button_down_flag = MOUSEEVENTF_LEFTDOWN;
            button_up_flag = MOUSEEVENTF_LEFTUP;
            break;
        case 2: // Right click
            button_down_flag = MOUSEEVENTF_RIGHTDOWN;
            button_up_flag = MOUSEEVENTF_RIGHTUP;
            break;
        case 3: // Middle click
            button_down_flag = MOUSEEVENTF_MIDDLEDOWN;
            button_up_flag = MOUSEEVENTF_MIDDLEUP;
            break;
        default:
            log_message(LOG_LEVEL_ERROR, "Invalid button for hold and move.");
            return;
    }

    // Press the button
    input.mi.dwFlags = button_down_flag;
    SendInput(1, &input, sizeof(INPUT));

    // Smoothly move the cursor
    POINT start_pos;
    if (GetCursorPos(&start_pos)) {
        double delta_x = x - start_pos.x;
        double delta_y = y - start_pos.y;
        int steps = MAX_STRENGTH - global_params->config.strength + 1;

        for (int i = 1; i <= steps; i++) {
            double progress = ease((double)i / steps, global_params->config.easing_type);
            int current_x = start_pos.x + (int)(delta_x * progress);
            int current_y = start_pos.y + (int)(delta_y * progress);
            enforce_screen_bounds(&current_x, &current_y);
            SetCursorPos(current_x, current_y);
            Sleep(global_params->config.mouse_speed);
        }
    }

    // Release the button
    input.mi.dwFlags = button_up_flag;
    SendInput(1, &input, sizeof(INPUT));

    log_message(LOG_LEVEL_INFO, "Mouse moved and button held.");
}

// Move cursor to an absolute position
void set_cursor_abs_internal(int x, int y, bool smooth) {
    enforce_screen_bounds(&x, &y);

    if (!smooth) {
        SetCursorPos(x, y);
        log_message(LOG_LEVEL_DEBUG, "Mouse teleported to absolute position.");
        return;
    }

    POINT start_pos;
    if (GetCursorPos(&start_pos)) {
        double delta_x = x - start_pos.x;
        double delta_y = y - start_pos.y;
        int steps = MAX_STRENGTH - global_params->config.strength + 1;

        for (int i = 1; i <= steps; i++) {
            double progress = ease((double)i / steps, global_params->config.easing_type);
            int current_x = start_pos.x + (int)(delta_x * progress);
            int current_y = start_pos.y + (int)(delta_y * progress);
            enforce_screen_bounds(&current_x, &current_y);
            SetCursorPos(current_x, current_y);
            Sleep(global_params->config.mouse_speed); // Use configurable mouse speed
        }
    }
}

// Perform mouse click with hold time
void perform_click(int button, int hold_time_ms) {
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
            log_message(LOG_LEVEL_ERROR, "Invalid button for click.");
            return;
    }

    SendInput(1, &inputs[0], sizeof(INPUT));
    Sleep(hold_time_ms);
    SendInput(1, &inputs[1], sizeof(INPUT));

    log_message(LOG_LEVEL_INFO, "Mouse click performed.");
}

// Exported function to dynamically update configuration
__declspec(dllexport) void set_config(int strength, int hold_time_ms, int mouse_speed, int easing_type, bool smooth_movement) {
    if (global_params) {
        EnterCriticalSection(&global_params->cs);
        global_params->config.strength = (strength >= MIN_STRENGTH && strength <= MAX_STRENGTH) ? strength : global_params->config.strength;
        global_params->config.hold_time_ms = (hold_time_ms > 0) ? hold_time_ms : global_params->config.hold_time_ms;
        global_params->config.mouse_speed = (mouse_speed > 0) ? mouse_speed : global_params->config.mouse_speed;
        global_params->config.easing_type = (easing_type >= EASE_LINEAR && easing_type <= EASE_EXPONENTIAL) ? (EasingType)easing_type : global_params->config.easing_type;
        global_params->config.smooth_movement = smooth_movement;
        LeaveCriticalSection(&global_params->cs);
        log_message(LOG_LEVEL_INFO, "Configuration updated dynamically.");
    }
}

// Exported function to apply preset configurations
__declspec(dllexport) void apply_preset(int preset_type) {
    if (global_params) {
        EnterCriticalSection(&global_params->cs);
        switch (preset_type) {
            case PRESET_DEFAULT:
                global_params->config = (Config){50, 300, DEFAULT_MOUSE_SPEED, EASE_LINEAR, true};
                break;
            case PRESET_FAST:
                global_params->config = (Config){80, 100, 2, EASE_EXPONENTIAL, false};
                break;
            case PRESET_SMOOTH:
                global_params->config = (Config){30, 500, 10, EASE_SINUSOIDAL, true};
                break;
            default:
                log_message(LOG_LEVEL_ERROR, "Invalid preset type.");
                LeaveCriticalSection(&global_params->cs);
                return;
        }
        LeaveCriticalSection(&global_params->cs);
        log_message(LOG_LEVEL_INFO, "Preset applied successfully.");
    }
}

// Exported function to set logging level
__declspec(dllexport) void set_logging_level(int level) {
    current_log_level = (level >= LOG_LEVEL_NONE && level <= LOG_LEVEL_DEBUG) ? (LogLevel)level : current_log_level;
    log_message(LOG_LEVEL_INFO, "Logging level updated.");
}

// Logging function with levels
void log_message(LogLevel level, const char* message) {
    if (logFile && level <= current_log_level) {
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", t);

        fprintf(logFile, "[%s] [%d] %s\n", timeStr, level, message);
        fflush(logFile);
    }
}

// Initialization
__declspec(dllexport) void initialize() {
    logFile = fopen(LOG_FILE, "a");
    if (!logFile) return;

    log_message(LOG_LEVEL_INFO, "DLL initialized.");

    global_params = (GlobalParams*)malloc(sizeof(GlobalParams));
    if (!global_params) {
        log_message(LOG_LEVEL_ERROR, "Error: Unable to allocate GlobalParams.");
        return;
    }

    global_params->config = (Config){50, 300, DEFAULT_MOUSE_SPEED, EASE_SINUSOIDAL, true};

    InitializeCriticalSection(&global_params->cs);

    log_message(LOG_LEVEL_INFO, "Global parameters initialized.");
}

// Shutdown
__declspec(dllexport) void mouse_shutdown() {
    if (global_params) {
        DeleteCriticalSection(&global_params->cs);
        free(global_params);
        global_params = NULL;
    }

    if (logFile) {
        log_message(LOG_LEVEL_INFO, "DLL shutting down.");
        fclose(logFile);
    }
}

// Exported function to set cursor absolute position
__declspec(dllexport) void set_cursor_abs(int x, int y) {
    if (global_params) {
        set_cursor_abs_internal(x, y, global_params->config.smooth_movement);
    }
}

// Exported function to perform a click
__declspec(dllexport) void click(int button) {
    if (global_params) {
        perform_click(button, global_params->config.hold_time_ms);
    }
}

// Exported function to hold click and move
__declspec(dllexport) void hold_and_move(int x, int y, int button, int duration_ms) {
    if (global_params) {
        hold_and_move_internal(x, y, button, duration_ms, global_params->config.smooth_movement);
    }
}

// Exported function to set mouse speed
__declspec(dllexport) void set_mouse_speed(int speed) {
    if (global_params) {
        EnterCriticalSection(&global_params->cs);
        global_params->config.mouse_speed = (speed > 0) ? speed : DEFAULT_MOUSE_SPEED;
        LeaveCriticalSection(&global_params->cs);
        log_message(LOG_LEVEL_INFO, "Mouse speed updated.");
    }
}
