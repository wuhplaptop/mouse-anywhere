# abexample.py
from mouse_anywhere import (
    initialize,
    shutdown,
    set_cursor_abs,
    set_cursor_rel,
    enqueue_target_abs,
    enqueue_target_rel
)
import time

# Define easing constants
EASE_LINEAR = 1
EASE_QUADRATIC = 2
EASE_SINUSOIDAL = 3
EASE_CUBIC = 4
EASE_EXPONENTIAL = 5

def main():
    # Initialize the DLL
    initialize()
    print("Initialized mouse_anywhere DLL.")

    try:
        # Move the cursor to absolute position (500, 500)
        print("Moving cursor to (500, 500)...")
        set_cursor_abs(500, 500)
        time.sleep(2)

        # Move the cursor relative by (100, 100)
        print("Moving cursor relative by (100, 100)...")
        set_cursor_rel(100, 100)
        time.sleep(2)

        # Enqueue a target absolute position for smooth movement
        print("Enqueueing target position (800, 600) for smooth movement...")
        enqueue_target_abs(800, 600)
        time.sleep(5)  # Wait for movement to complete

        # Enqueue a target relative movement
        print("Enqueueing relative movement by (-200, -200)...")
        enqueue_target_rel(-200, -200)
        time.sleep(5)

    finally:
        # Shutdown the DLL
        shutdown()
        print("Shutdown mouse_anywhere DLL.")

if __name__ == "__main__":
    main()
