

Mouse Anywhere Library
=

Overview
--------
Mouse Anywhere is a Python library powered by a DLL for dynamic mouse control on Windows. This library allows you to:
- Move the cursor to any absolute position on the screen.
- Simulate mouse clicks (left, right, middle).
- Perform smooth cursor movements with customizable easing types.
- Dynamically update configuration settings.
- Log mouse interactions at different levels (error, info, debug).

This package is suitable for applications requiring precise mouse control, such as automated testing, custom accessibility solutions, or macro creation.

====================================

Features
--------
- **Cursor Control:** Move the cursor to specified screen coordinates.
- **Mouse Clicks:** Simulate mouse clicks with configurable hold times.
- **Smooth Movements:** Enable smooth cursor movements with easing types like linear, quadratic, sinusoidal, cubic, and exponential.
- **Presets:** Apply predefined configurations for fast, smooth, or default behavior.
- **Logging:** Monitor mouse interactions with adjustable log levels.

====================================

Installation
------------
Install the library using pip:

    pip install mouse-anywhere

====================================

Getting Started
---------------
### Initialization
To use the library, initialize it first:

    from mouse_anywhere import initialize, mouse_shutdown

    # Initialize the library
    initialize()

    # Shutdown the library (when done)
    mouse_shutdown()

### Example Usage

#### Moving the Cursor
    from mouse_anywhere import set_cursor_abs

    # Move the cursor to screen coordinates (100, 200)
    set_cursor_abs(100, 200)

#### Simulating Mouse Clicks
    from mouse_anywhere import click

    # Perform a left mouse click
    click(1)

    # Perform a right mouse click
    click(2)

#### Smooth Movement
    from mouse_anywhere import hold_and_move

    # Hold the left mouse button and move to (300, 400) over 2 seconds
    hold_and_move(300, 400, 1, 2000)

#### Applying Presets
    from mouse_anywhere import apply_preset

    # Apply the "smooth" preset
    apply_preset(3)

#### Custom Configurations
    from mouse_anywhere import set_config

    # Set custom configurations
    set_config(
        strength=70,        # Strength (1-100)
        hold_time_ms=500,   # Hold time in milliseconds
        mouse_speed=10,     # Movement speed in ms per step
        easing_type=2,      # Easing type (1-5)
        smooth_movement=True
    )

#### Adjust Logging Level
    from mouse_anywhere import set_logging_level

    # Set logging to debug level
    set_logging_level(3)

====================================

Configuration Details
----------------------
### Easing Types
Easing types control the smoothness of cursor movement:
- **Linear:** 1
- **Quadratic:** 2
- **Sinusoidal:** 3
- **Cubic:** 4
- **Exponential:** 5

### Presets
Presets offer predefined configurations:
- **Default:** Strength=50, Hold Time=300ms, Speed=5, Easing=Linear
- **Fast:** Strength=80, Hold Time=100ms, Speed=2, Easing=Exponential
- **Smooth:** Strength=30, Hold Time=500ms, Speed=10, Easing=Sinusoidal

### Logging Levels
Control the verbosity of logs:
- **None:** 0 (No logging)
- **Error:** 1 (Log errors only)
- **Info:** 2 (Log informational messages)
- **Debug:** 3 (Log detailed debug messages)

====================================

DLL Integration
---------------
The library uses `mouse_anywhere2.dll` for low-level mouse control. Ensure the DLL is packaged with the library and accessible in the Python module directory.

If you encounter issues:
1. Verify the DLL is present in the installed package directory.
2. Confirm your system is running a compatible version of Windows.

====================================

Troubleshooting
---------------
### Common Errors
- **FileNotFoundError:** Ensure the DLL is correctly installed and accessible.
- **ValueError:** Check the input parameters for invalid values.

### Debugging Steps
1. Enable debug logging with `set_logging_level(3)`.
2. Check the log file (`mouse_movement.log`) for detailed information.


