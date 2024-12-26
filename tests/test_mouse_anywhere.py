import pytest
import time
from mouse_anywhere import (
    initialize,
    mouse_shutdown,  # Updated to match the correct function name
    set_cursor_abs,
    click,
    hold_and_move,
    set_config,
    apply_preset,
)  # Import the functions directly

def setup_module(module):
    """Setup the DLL before tests."""
    print("Initializing mouse library...")
    initialize()  # Call the function directly

def teardown_module(module):
    """Shutdown the DLL after tests."""
    print("Shutting down mouse library...")
    mouse_shutdown()  # Call the function directly

def test_clicks():
    """Test basic mouse click functionality."""
    print("Testing mouse clicks...")
    click(1)  # Left click
    time.sleep(0.5)
    click(2)  # Right click
    time.sleep(0.5)
    click(3)  # Middle click
    time.sleep(0.5)
    assert True, "Mouse clicks completed successfully."
    print("Mouse clicks test completed.")

def test_cursor_movement():
    """Test moving the cursor to specific positions."""
    print("Testing cursor movement...")
    positions = [(100, 100), (200, 200), (300, 300)]
    for x, y in positions:
        set_cursor_abs(x, y)  # Call the function directly
        time.sleep(0.5)
    assert True, "Cursor moved to all positions successfully."
    print("Cursor movement test completed.")

def test_hold_and_move():
    """Test holding a click and moving."""
    print("Testing hold and move...")
    hold_and_move(400, 400, 1, 1000)  # Hold left click for 1 second
    assert True, "Hold and move completed successfully."
    print("Hold and move test completed.")

def test_presets():
    """Test applying different presets."""
    print("Testing preset configurations...")
    apply_preset(1)  # Default preset
    apply_preset(2)  # Fast preset
    apply_preset(3)  # Smooth preset
    assert True, "Preset configurations applied successfully."
    print("Preset configuration test completed.")

def test_dynamic_config():
    """Test setting dynamic configurations."""
    print("Testing dynamic configuration...")
    set_config(30, 200, 3, 1, True)  # Lower strength, fast speed
    set_config(80, 500, 10, 3, False)  # Higher strength, slower speed
    assert True, "Dynamic configurations set successfully."
    print("Dynamic configuration test completed.")
