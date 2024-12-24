# tests/test_basic.py
import pytest
import mouse_anywhere

def test_package_import():
    assert mouse_anywhere is not None

def test_cursor_function():
    # Example test: Replace with actual functionality
    try:
        mouse_anywhere.set_cursor_abs(100, 100)
    except Exception as e:
        pytest.fail(f"set_cursor_abs raised an exception: {e}")
