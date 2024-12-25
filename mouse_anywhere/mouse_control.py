import ctypes
import os

# Get the path to the DLL
DLL_PATH = os.path.join(os.path.dirname(__file__), "mouse-anywhere.dll")
if not os.path.exists(DLL_PATH):
    raise FileNotFoundError(f"Required DLL not found at {DLL_PATH}")

# Load the DLL
mouse_anywhere = ctypes.WinDLL(DLL_PATH)

# Define function prototypes
mouse_anywhere.initialize.argtypes = []
mouse_anywhere.initialize.restype = None

mouse_anywhere.mouse_shutdown.argtypes = []
mouse_anywhere.mouse_shutdown.restype = None

mouse_anywhere.set_cursor_abs.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_anywhere.set_cursor_abs.restype = None

mouse_anywhere.set_cursor_rel.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_anywhere.set_cursor_rel.restype = None

mouse_anywhere.perform_click.argtypes = [ctypes.c_int]
mouse_anywhere.perform_click.restype = None

mouse_anywhere.enqueue_target_abs.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_anywhere.enqueue_target_abs.restype = None

mouse_anywhere.enqueue_target_rel.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_anywhere.enqueue_target_rel.restype = None

# Initialize the DLL
def initialize():
    mouse_anywhere.initialize()

# Shutdown the DLL
def shutdown():
    mouse_anywhere.mouse_shutdown()

# Move the cursor to an absolute position
def set_cursor_abs(x, y):
    mouse_anywhere.set_cursor_abs(x, y)

# Move the cursor relative to the current position
def set_cursor_rel(delta_x, delta_y):
    mouse_anywhere.set_cursor_rel(delta_x, delta_y)

# Perform a mouse click (1: Left, 2: Right, 3: Middle)
def perform_click(button):
    mouse_anywhere.perform_click(button)

# Enqueue an absolute movement target
def enqueue_target_abs(x, y):
    mouse_anywhere.enqueue_target_abs(x, y)

# Enqueue a relative movement target
def enqueue_target_rel(delta_x, delta_y):
    mouse_anywhere.enqueue_target_rel(delta_x, delta_y)
