import ctypes
import time

# Import the DLL functions via the package
from mouse_anywhere import mouse_library

def test_clicks():
    print("Testing mouse clicks...")

    # Left click
    print("Performing Left click...")
    mouse_library.click(1)
    time.sleep(1)

    # Right click
    print("Performing Right click...")
    mouse_library.click(2)
    time.sleep(1)

    # Middle click
    print("Performing Middle click...")
    mouse_library.click(3)
    time.sleep(1)

    print("Mouse click tests completed.")

def test_movement_and_click():
    print("Testing movement combined with mouse clicks...")
    for x in range(100, 800, 200):
        print(f"Moving to ({x}, 300) and performing left click...")
        mouse_library.set_cursor_abs(x, 300)
        mouse_library.click(1)
        time.sleep(0.5)
    print("Movement and click test completed.")

def test_hold_and_move():
    print("Testing hold and move...")
    for x in range(100, 800, 200):
        print(f"Holding click and moving to ({x}, 300)...")
        mouse_library.hold_and_move(x, 300, 1, 1000)
        time.sleep(0.5)
    print("Hold and move test completed.")

if __name__ == "__main__":
    mouse_library.initialize()
    try:
        test_clicks()
        test_movement_and_click()
        test_hold_and_move()
    finally:
        mouse_library.mouse_shutdown()
