# mouse_anywhere/mouse_anywhere.py
import ctypes
import os
from ctypes import wintypes

# Determine the path to the DLL
current_dir = os.path.dirname(os.path.abspath(__file__))
dll_path = os.path.join(current_dir, 'mouse-anywhere.dll')

# Load the DLL
try:
    mouse_anywhere_dll = ctypes.CDLL(dll_path)
except OSError as e:
    raise RuntimeError(f"Failed to load mouse-anywhere.dll: {e}")

# Define function prototypes

# void initialize()
mouse_anywhere_dll.initialize.argtypes = []
mouse_anywhere_dll.initialize.restype = None

# void mouse_shutdown()
mouse_anywhere_dll.mouse_shutdown.argtypes = []
mouse_anywhere_dll.mouse_shutdown.restype = None

# void set_cursor_abs(int x, int y)
mouse_anywhere_dll.set_cursor_abs.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_anywhere_dll.set_cursor_abs.restype = None

# void set_cursor_rel(int delta_x, int delta_y)
mouse_anywhere_dll.set_cursor_rel.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_anywhere_dll.set_cursor_rel.restype = None

# void enqueue_target_abs(int x, int y)
mouse_anywhere_dll.enqueue_target_abs.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_anywhere_dll.enqueue_target_abs.restype = None

# void enqueue_target_rel(int delta_x, int delta_y)
mouse_anywhere_dll.enqueue_target_rel.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_anywhere_dll.enqueue_target_rel.restype = None

# Python wrappers

def initialize():
    """Initialize the mouse-anywhere DLL."""
    mouse_anywhere_dll.initialize()

def shutdown():
    """Shutdown the mouse-anywhere DLL."""
    mouse_anywhere_dll.mouse_shutdown()

def set_cursor_abs(x, y):
    """Move the cursor to an absolute position (x, y)."""
    mouse_anywhere_dll.set_cursor_abs(x, y)

def set_cursor_rel(delta_x, delta_y):
    """Move the cursor relative to its current position by (delta_x, delta_y)."""
    mouse_anywhere_dll.set_cursor_rel(delta_x, delta_y)

def enqueue_target_abs(x, y):
    """Enqueue an absolute target at (x, y) for smooth movement."""
    mouse_anywhere_dll.enqueue_target_abs(x, y)

def enqueue_target_rel(delta_x, delta_y):
    """Enqueue a relative movement by (delta_x, delta_y) for smooth movement."""
    mouse_anywhere_dll.enqueue_target_rel(delta_x, delta_y)

# Optional: Context manager for automatic initialization and shutdown
class MouseAnywhere:
    def __enter__(self):
        initialize()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        shutdown()

    def set_cursor_abs(self, x, y):
        set_cursor_abs(x, y)

    def set_cursor_rel(self, delta_x, delta_y):
        set_cursor_rel(delta_x, delta_y)

    def enqueue_target_abs(self, x, y):
        enqueue_target_abs(x, y)

    def enqueue_target_rel(self, delta_x, delta_y):
        enqueue_target_rel(delta_x, delta_y)
