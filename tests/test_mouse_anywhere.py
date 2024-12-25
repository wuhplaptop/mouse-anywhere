import pytest
import os
from ctypes import WinDLL
from mouse_anywhere import (
    initialize,
    shutdown,
    set_cursor_abs,
    set_cursor_rel,
    enqueue_target_abs,
    enqueue_target_rel,
    perform_click,
)

# Ensure DLL path is correctly located
dll_path = os.path.join(os.path.dirname(__file__), "..", "mouse_anywhere", "mouse-anywhere.dll")
if not os.path.exists(dll_path):
    raise FileNotFoundError(f"Required DLL not found at {dll_path}")

# Load the DLL explicitly for the tests
mouse_control = WinDLL(dll_path)

@pytest.fixture(scope="module", autouse=True)
def setup_and_teardown():
    """Setup and teardown for tests."""
    initialize()
    yield
    shutdown()

def test_initialize():
    """Test initializing the DLL."""
    try:
        mouse_control.initialize()
        assert True
    except Exception as e:
        pytest.fail(f"Initialization failed: {e}")

def test_set_cursor_abs():
    """Test setting cursor to an absolute position."""
    try:
        mouse_control.set_cursor_abs(100, 100)
        assert True
    except Exception as e:
        pytest.fail(f"set_cursor_abs failed: {e}")

def test_set_cursor_rel():
    """Test setting cursor to a relative position."""
    try:
        mouse_control.set_cursor_rel(10, 10)
        assert True
    except Exception as e:
        pytest.fail(f"set_cursor_rel failed: {e}")

def test_perform_click():
    """Test performing a mouse click."""
    try:
        mouse_control.perform_click(1)  # Left click
        assert True
    except Exception as e:
        pytest.fail(f"perform_click failed: {e}")

def test_enqueue_target_abs():
    """Test enqueueing an absolute target."""
    try:
        enqueue_target_abs(200, 200)
        assert True
    except Exception as e:
        pytest.fail(f"enqueue_target_abs failed: {e}")

def test_enqueue_target_rel():
    """Test enqueueing a relative target."""
    try:
        enqueue_target_rel(20, 20)
        assert True
    except Exception as e:
        pytest.fail(f"enqueue_target_rel failed: {e}")

def test_shutdown():
    """Test shutting down the DLL."""
    try:
        mouse_control.shutdown()
        assert True
    except Exception as e:
        pytest.fail(f"Shutdown failed: {e}")
