# mouse_movement/__init__.py
import os
import ctypes
from ctypes import wintypes
import threading

# Load the DLL
dll_path = os.path.join(os.path.dirname(__file__), 'mouse_movement.dll')
mouse_lib = ctypes.CDLL(dll_path)

# Define Easing Types
EASE_LINEAR = 1
EASE_QUADRATIC = 2
EASE_SINUSOIDAL = 3
EASE_CUBIC = 4
EASE_EXPONENTIAL = 5

# Define MovementParams as a ctypes structure
class MovementParams(ctypes.Structure):
    pass  # Opaque structure

# Define function prototypes
mouse_lib.create_movement_params.argtypes = [ctypes.c_int, ctypes.c_int]
mouse_lib.create_movement_params.restype = ctypes.POINTER(MovementParams)

mouse_lib.destroy_movement_params.argtypes = [ctypes.POINTER(MovementParams)]
mouse_lib.destroy_movement_params.restype = None

mouse_lib.start_movement.argtypes = [ctypes.POINTER(MovementParams)]
mouse_lib.start_movement.restype = None

mouse_lib.add_absolute_target.argtypes = [ctypes.POINTER(MovementParams), ctypes.c_int, ctypes.c_int]
mouse_lib.add_absolute_target.restype = None

mouse_lib.add_relative_target.argtypes = [ctypes.POINTER(MovementParams), ctypes.c_int, ctypes.c_int]
mouse_lib.add_relative_target.restype = None

mouse_lib.stop_movement.argtypes = [ctypes.POINTER(MovementParams)]
mouse_lib.stop_movement.restype = None

class MouseMovement:
    def __init__(self, easing_type=EASE_SINUSOIDAL, strength=50):
        self.params = mouse_lib.create_movement_params(easing_type, strength)
        if not self.params:
            raise RuntimeError("Failed to create MovementParams.")
        self._lock = threading.Lock()
        self._is_running = False

    def start(self):
        with self._lock:
            if not self._is_running:
                mouse_lib.start_movement(self.params)
                self._is_running = True

    def stop(self):
        with self._lock:
            if self._is_running:
                mouse_lib.stop_movement(self.params)
                self._is_running = False

    def add_absolute_target(self, x, y):
        mouse_lib.add_absolute_target(self.params, x, y)

    def add_relative_target(self, dx, dy):
        mouse_lib.add_relative_target(self.params, dx, dy)

    def __del__(self):
        self.stop()
        mouse_lib.destroy_movement_params(self.params)

# Convenience functions
def create_mouse_movement(easing_type=EASE_SINUSOIDAL, strength=50):
    return MouseMovement(easing_type, strength)
