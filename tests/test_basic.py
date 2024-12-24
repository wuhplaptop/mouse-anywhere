# tests/test_basic.py
import pytest
from unittest import mock
import mouse_anywhere
import ctypes

# Mock the ctypes.CDLL to prevent actual DLL loading during tests
@mock.patch('ctypes.CDLL')
def test_package_import(mock_cdll):
    # Simulate successful DLL loading
    mock_cdll.return_value = mock.Mock()
    import mouse_anywhere
    assert mouse_anywhere is not None

def test_initialize_shutdown():
    with mock.patch('ctypes.CDLL') as mock_cdll, \
         mock.patch('mouse_anywhere.mouse_anywhere.mouse_anywhere_dll.initialize') as mock_init, \
         mock.patch('mouse_anywhere.mouse_anywhere.mouse_anywhere_dll.mouse_shutdown') as mock_shutdown:
        mock_cdll.return_value = mock.Mock()
        mouse_anywhere.initialize()
        mock_init.assert_called_once()

        mouse_anywhere.shutdown()
        mock_shutdown.assert_called_once()

def test_set_cursor_functions():
    with mock.patch('mouse_anywhere.mouse_anywhere.mouse_anywhere_dll.set_cursor_abs') as mock_set_abs, \
         mock.patch('mouse_anywhere.mouse_anywhere.mouse_anywhere_dll.set_cursor_rel') as mock_set_rel:
        mouse_anywhere.set_cursor_abs(100, 100)
        mock_set_abs.assert_called_once_with(100, 100)

        mouse_anywhere.set_cursor_rel(50, -50)
        mock_set_rel.assert_called_once_with(50, -50)

def test_enqueue_functions():
    with mock.patch('mouse_anywhere.mouse_anywhere.mouse_anywhere_dll.enqueue_target_abs') as mock_enqueue_abs, \
         mock.patch('mouse_anywhere.mouse_anywhere.mouse_anywhere_dll.enqueue_target_rel') as mock_enqueue_rel:
        mouse_anywhere.enqueue_target_abs(800, 600)
        mock_enqueue_abs.assert_called_once_with(800, 600)

        mouse_anywhere.enqueue_target_rel(-50, 50)
        mock_enqueue_rel.assert_called_once_with(-50, 50)
