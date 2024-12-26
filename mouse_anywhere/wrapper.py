import os
import ctypes

# Get the path to the current package directory
package_dir = os.path.dirname(__file__)
dll_path = os.path.join(package_dir, 'mouse_anywhere2.dll')

# Load the DLL dynamically
try:
    mouse_library = ctypes.WinDLL(dll_path)
except FileNotFoundError as e:
    raise FileNotFoundError(f"Could not find the DLL at {dll_path}. Ensure the DLL is properly packaged and installed.") from e

# Define the argument and return types for the DLL functions
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

mouse_library.set_config.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_bool]
mouse_library.set_config.restype = None

mouse_library.apply_preset.argtypes = [ctypes.c_int]
mouse_library.apply_preset.restype = None
