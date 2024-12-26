import os
import ctypes

# Get the path to the current package directory
package_dir = os.path.dirname(__file__)
dll_path = os.path.join(package_dir, "mouse_anywhere2.dll")

# Load the DLL dynamically
try:
    mouse_library = ctypes.WinDLL(dll_path)
except FileNotFoundError as e:
    raise FileNotFoundError(
        f"Could not find the DLL at {dll_path}. Ensure the DLL is properly packaged and installed."
    ) from e

# Define argument and return types for the exported functions
mouse_library.initialize.argtypes = []
mouse_library.initialize.restype = None

mouse_library.mouse_shutdown.argtypes = []
mouse_library.mouse_shutdown.restype = None

mouse_library.set_cursor_abs.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_library.set_cursor_abs.restype = None

mouse_library.click.argtypes = [ctypes.c_int]
mouse_library.click.restype = None

mouse_library.hold_and_move.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int]
mouse_library.hold_and_move.restype = None

mouse_library.set_mouse_speed.argtypes = [ctypes.c_int]
mouse_library.set_mouse_speed.restype = None

mouse_library.set_config.argtypes = [
    ctypes.c_int,  # strength
    ctypes.c_int,  # hold_time_ms
    ctypes.c_int,  # mouse_speed
    ctypes.c_int,  # easing_type
    ctypes.c_bool,  # smooth_movement
]
mouse_library.set_config.restype = None

mouse_library.apply_preset.argtypes = [ctypes.c_int]
mouse_library.apply_preset.restype = None

mouse_library.set_logging_level.argtypes = [ctypes.c_int]
mouse_library.set_logging_level.restype = None

# Wrapper functions
def initialize():
    """Initialize the mouse library."""
    mouse_library.initialize()

def mouse_shutdown():
    """Shutdown the mouse library."""
    mouse_library.mouse_shutdown()

def set_cursor_abs(x, y):
    """Set the cursor to an absolute position."""
    mouse_library.set_cursor_abs(x, y)

def click(button):
    """Perform a mouse click."""
    if button not in {1, 2, 3}:
        raise ValueError("Invalid button. Use 1 (left), 2 (right), or 3 (middle).")
    mouse_library.click(button)

def hold_and_move(x, y, button, duration_ms):
    """Hold a mouse button and move the cursor."""
    if button not in {1, 2, 3}:
        raise ValueError("Invalid button. Use 1 (left), 2 (right), or 3 (middle).")
    mouse_library.hold_and_move(x, y, button, duration_ms)

def set_mouse_speed(speed):
    """Set the mouse movement speed."""
    if speed <= 0:
        raise ValueError("Mouse speed must be greater than 0.")
    mouse_library.set_mouse_speed(speed)

def set_config(strength, hold_time_ms, mouse_speed, easing_type, smooth_movement):
    """Set configuration for the mouse library."""
    if not (1 <= strength <= 100):
        raise ValueError("Strength must be between 1 and 100.")
    if hold_time_ms <= 0:
        raise ValueError("Hold time must be greater than 0.")
    if mouse_speed <= 0:
        raise ValueError("Mouse speed must be greater than 0.")
    if easing_type not in {1, 2, 3, 4, 5}:
        raise ValueError("Easing type must be between 1 (linear) and 5 (exponential).")
    mouse_library.set_config(strength, hold_time_ms, mouse_speed, easing_type, smooth_movement)

def apply_preset(preset_type):
    """Apply a preset configuration."""
    if preset_type not in {1, 2, 3}:
        raise ValueError("Invalid preset type. Use 1 (default), 2 (fast), or 3 (smooth).")
    mouse_library.apply_preset(preset_type)

def set_logging_level(level):
    """Set the logging level."""
    if level not in {0, 1, 2, 3}:
        raise ValueError("Invalid logging level. Use 0 (none), 1 (error), 2 (info), or 3 (debug).")
    mouse_library.set_logging_level(level)
