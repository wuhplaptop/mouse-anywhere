import pytest
from mouse_anywhere import (
    initialize,
    shutdown,
    set_cursor_abs,
    set_cursor_rel,
    enqueue_target_abs,
    enqueue_target_rel,
    perform_click,
)

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

def test_perform_click():
    """Test performing a mouse click."""
    try:
        mouse_control.perform_click(1)  # Left click
        assert True
    except Exception as e:
        pytest.fail(f"perform_click failed: {e}")

def test_shutdown():
    """Test shutting down the DLL."""
    try:
        mouse_control.shutdown()
        assert True
    except Exception as e:
        pytest.fail(f"Shutdown failed: {e}")
