import ctypes
import os

# Locate the DLL file
dll_path = os.path.join(os.path.dirname(__file__), "mouse_anywhere2.dll")
mouse_anywhere = ctypes.WinDLL(dll_path)

# Initialize the DLL
mouse_anywhere.initialize()

# Define function prototypes
mouse_anywhere.set_cursor_abs.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_anywhere.set_cursor_abs.restype = None

mouse_anywhere.click.argtypes = [ctypes.c_int]
mouse_anywhere.click.restype = None

mouse_anywhere.hold_and_move.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int]
mouse_anywhere.hold_and_move.restype = None

mouse_anywhere.set_mouse_speed.argtypes = [ctypes.c_int]
mouse_anywhere.set_mouse_speed.restype = None

mouse_anywhere.set_config.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_bool]
mouse_anywhere.set_config.restype = None

mouse_anywhere.apply_preset.argtypes = [ctypes.c_int]
mouse_anywhere.apply_preset.restype = None

mouse_anywhere.set_logging_level.argtypes = [ctypes.c_int]
mouse_anywhere.set_logging_level.restype = None

# Expose the functions
def initialize():
    mouse_anywhere.initialize()

def shutdown():
    mouse_anywhere.mouse_shutdown()

def set_cursor_abs(x, y):
    mouse_anywhere.set_cursor_abs(x, y)

def click(button):
    mouse_anywhere.click(button)

def hold_and_move(x, y, button, duration_ms):
    mouse_anywhere.hold_and_move(x, y, button, duration_ms)

def set_mouse_speed(speed):
    mouse_anywhere.set_mouse_speed(speed)

def set_config(strength, hold_time, speed, easing_type, smooth):
    mouse_anywhere.set_config(strength, hold_time, speed, easing_type, smooth)

def apply_preset(preset_type):
    mouse_anywhere.apply_preset(preset_type)

def set_logging_level(level):
    mouse_anywhere.set_logging_level(level)
