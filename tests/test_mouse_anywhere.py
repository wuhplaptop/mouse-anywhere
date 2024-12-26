import pytest
import time
from mouse_anywhere import mouse_library

def setup_module(module):
    """Setup the DLL before tests."""
    print("Initializing mouse library...")
    mouse_library.initialize()

def teardown_module(module):
    """Shutdown the DLL after tests."""
    print("Shutting down mouse library...")
    mouse_library.mouse_shutdown()

def test_clicks():
    """Test basic mouse click functionality."""
    print("Testing mouse clicks...")
    mouse_library.click(1)  # Left click
    time.sleep(0.5)
    mouse_library.click(2)  # Right click
    time.sleep(0.5)
    mouse_library.click(3)  # Middle click
    time.sleep(0.5)
    print("Mouse clicks test completed.")

def test_cursor_movement():
    """Test moving the cursor to specific positions."""
    print("Testing cursor movement...")
    positions = [(100, 100), (200, 200), (300, 300)]
    for x, y in positions:
        mouse_library.set_cursor_abs(x, y)
        time.sleep(0.5)
    print("Cursor movement test completed.")

def test_hold_and_move():
    """Test holding a click and moving."""
    print("Testing hold and move...")
    mouse_library.hold_and_move(400, 400, 1, 1000)  # Hold left click for 1 second
    print("Hold and move test completed.")

def test_presets():
    """Test applying different presets."""
    print("Testing preset configurations...")
    mouse_library.apply_preset(1)  # Default preset
    mouse_library.apply_preset(2)  # Fast preset
    mouse_library.apply_preset(3)  # Smooth preset
    print("Preset configuration test completed.")

def test_dynamic_config():
    """Test setting dynamic configurations."""
    print("Testing dynamic configuration...")
    mouse_library.set_config(30, 200, 3, 1, True)  # Lower strength, fast speed
    mouse_library.set_config(80, 500, 10, 3, False)  # Higher strength, slower speed
    print("Dynamic configuration test completed.")
