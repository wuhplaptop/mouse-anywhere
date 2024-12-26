# Mouse Anywhere

Mouse Anywhere is a powerful and flexible DLL library designed to provide advanced mouse control functionalities on Windows platforms. Whether you're developing automation scripts, creating custom user interfaces, or enhancing accessibility features, Mouse Anywhere offers a comprehensive set of tools to manipulate mouse movements and actions with precision and ease.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
  - [Prerequisites](#prerequisites)
  - [Installation via pip](#installation-via-pip)
  - [Building the DLL Manually](#building-the-dll-manually)
  - [Using the Python Wrapper](#using-the-python-wrapper)
- [Usage](#usage)
  - [Initialization and Shutdown](#initialization-and-shutdown)
  - [Cursor Control](#cursor-control)
  - [Mouse Clicks](#mouse-clicks)
  - [Hold and Move](#hold-and-move)
  - [Configuration](#configuration)
  - [Presets](#presets)
  - [Logging](#logging)
- [API Reference](#api-reference)
  - [Exported Functions](#exported-functions)
  - [Configuration Parameters](#configuration-parameters)
  - [Easing Types](#easing-types)
  - [Preset Types](#preset-types)
  - [Log Levels](#log-levels)
- [How It Works](#how-it-works)
- [Logging](#logging)
- [Error Handling](#error-handling)
- [Contributing](#contributing)
- [License](#license)
- [Support](#support)
- [Example Usage](#example-usage)
- [Frequently Asked Questions (FAQ)](#frequently-asked-questions-faq)
- [Contact](#contact)

---

## Features

- **Absolute Cursor Positioning:** Move the mouse cursor to any position on the screen, with optional smooth transitions.
- **Mouse Clicks:** Perform left, right, and middle mouse clicks with configurable hold times.
- **Hold and Move:** Hold down a mouse button while moving the cursor, useful for drag-and-drop operations.
- **Configuration Options:** Customize strength, hold time, mouse speed, easing types, and smooth movement.
- **Presets:** Apply predefined configurations for default, fast, or smooth mouse behavior.
- **Logging:** Detailed logging with configurable log levels to monitor actions and debug issues.
- **Thread-Safe:** Utilizes critical sections to manage concurrent access to global parameters.

---

## Installation

Mouse Anywhere can be installed either via `pip` for ease of use or built manually from the source for customization. Choose the method that best fits your needs.

### Prerequisites

- **Operating System:** Windows (tested on Windows 10 and later)
- **Development Tools:** Microsoft Visual Studio or any C/C++ compiler supporting Windows DLLs (only if building manually)
- **Python:** Python 3.6 or later (for using the Python wrapper)
- **Dependencies:** None beyond standard C libraries and Python's `ctypes` module

### Installation via pip

For a straightforward installation, you can install Mouse Anywhere directly from PyPI using `pip`. This method is recommended for most users as it simplifies the setup process.

1. **Install via pip:**

   Open your command prompt or terminal and run:

   ```bash
   pip install mouse-anywhere
   ```

2. **Verify Installation:**

   After installation, you can verify by importing the module in Python:

   ```python
   import mouse_anywhere as mouse
   ```

3. **Note:**

   The `pip` installation includes the pre-built `mouse_anywhere.dll`. Ensure that your system meets the necessary prerequisites, and you have the appropriate permissions to install Python packages.

### Building the DLL Manually

If you prefer to build the DLL from the source, perhaps to customize functionalities or verify the source code, follow these steps:

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/wuhplaptop/mouse-anywhere.git
   ```

2. **Open the Project:**

   Open the project in your preferred C/C++ development environment. For example, using Visual Studio:

   - Open Visual Studio.
   - Select **File > Open > Project/Solution**.
   - Navigate to the cloned repository and open the `.sln` file if available, or create a new DLL project and add the provided `mouse_anywhere.c` file.

3. **Build the DLL:**

   - Configure the build settings as needed (e.g., Release mode).
   - Build the project to generate `mouse_anywhere.dll`.

4. **Locate the DLL:**

   After a successful build, locate the `mouse_anywhere.dll` in the project's output directory (e.g., `Debug` or `Release`).

### Using the Python Wrapper

Whether you installed via `pip` or built the DLL manually, you can utilize the Python wrapper to interact with Mouse Anywhere seamlessly.

1. **Ensure the DLL is Accessible:**

   - **If Installed via pip:** The DLL is included and should be accessible automatically.
   - **If Built Manually:** Place the `mouse_anywhere.dll` in the same directory as your Python script or in a directory that's part of your system's PATH.

2. **Install Python (if not already installed):**

   Download and install Python from the [official website](https://www.python.org/downloads/).

3. **Using the Wrapper:**

   Here's a basic example of how to use the Python wrapper:

   ```python
   import mouse_anywhere as mouse

   # Initialize the library
   mouse.initialize()

   # Set cursor to position (500, 500)
   mouse.set_cursor_abs(500, 500)

   # Perform a left click
   mouse.click(1)

   # Shutdown the library
   mouse.mouse_shutdown()
   ```

   Replace `mouse_anywhere` with the actual name of your Python wrapper file if different.

---

## Usage

### Initialization and Shutdown

Before performing any mouse operations, initialize the library. After completing your tasks, ensure you properly shut down the library to release resources.

#### C Example

```c
#include "mouse_anywhere.h"

int main() {
    initialize();

    // Perform mouse operations here

    mouse_shutdown();
    return 0;
}
```

#### Python Example

```python
import mouse_anywhere as mouse

mouse.initialize()

# Perform mouse operations here

mouse.mouse_shutdown()
```

### Cursor Control

#### Set Cursor to Absolute Position

Move the cursor to a specific (x, y) coordinate on the screen.

##### C Example

```c
set_cursor_abs(1000, 500);
```

##### Python Example

```python
mouse.set_cursor_abs(1000, 500)
```

#### Smooth Movement

Enable or disable smooth cursor transitions by configuring the `smooth_movement` parameter.

### Mouse Clicks

#### Perform a Click

Simulate a mouse button click. Supported buttons are:

- `1` - Left Click
- `2` - Right Click
- `3` - Middle Click

##### C Example

```c
click(1); // Left Click
```

##### Python Example

```python
mouse.click(1)  # Left Click
```

#### Configurable Hold Time

Set how long the mouse button is held down during a click.

```c
// C Example
set_config(50, 300, 5, EASE_LINEAR, true); // strength, hold_time_ms, mouse_speed, easing_type, smooth_movement
```

### Hold and Move

Hold down a mouse button while moving the cursor to perform drag operations.

##### C Example

```c
hold_and_move(1500, 1000, 1, 2000); // x, y, button, duration_ms
```

##### Python Example

```python
mouse.hold_and_move(1500, 1000, 1, 2000)  # x, y, button, duration_ms
```

### Configuration

Customize the mouse behavior by setting configuration parameters:

- **Strength:** Determines the number of steps in movement. Range: `1` to `100`.
- **Hold Time (ms):** Duration to hold mouse clicks.
- **Mouse Speed (ms per step):** Speed of cursor movement.
- **Easing Type:** Type of easing function for smooth transitions.
- **Smooth Movement:** Enable or disable smooth cursor movement.

##### C Example

```c
set_config(70, 200, 3, EASE_QUADRATIC, false);
```

##### Python Example

```python
mouse.set_config(70, 200, 3, 2, False)  # strength, hold_time_ms, mouse_speed, easing_type, smooth_movement
```

### Presets

Apply predefined configurations for common use cases:

- `1` - Default
- `2` - Fast
- `3` - Smooth

##### C Example

```c
apply_preset(PRESET_FAST);
```

##### Python Example

```python
mouse.apply_preset(2)  # Fast preset
```

### Logging

Enable and configure logging to monitor actions and debug issues. Set the logging level:

- `0` - None
- `1` - Error
- `2` - Info
- `3` - Debug

##### C Example

```c
set_logging_level(LOG_LEVEL_DEBUG);
```

##### Python Example

```python
mouse.set_logging_level(3)  # Debug level
```

Logs are written to `mouse_movement.log` in the application's directory.

---

## API Reference

### Exported Functions

#### `initialize()`

**Description:**  
Initializes the Mouse Anywhere library. Must be called before any other functions.

**Usage:**

```c
initialize();
```

```python
mouse.initialize()
```

#### `mouse_shutdown()`

**Description:**  
Shuts down the Mouse Anywhere library and releases resources.

**Usage:**

```c
mouse_shutdown();
```

```python
mouse.mouse_shutdown()
```

#### `set_cursor_abs(int x, int y)`

**Description:**  
Moves the cursor to the specified absolute (x, y) screen coordinates.

**Parameters:**

- `x` (`int`): The x-coordinate on the screen.
- `y` (`int`): The y-coordinate on the screen.

**Usage:**

```c
set_cursor_abs(800, 600);
```

```python
mouse.set_cursor_abs(800, 600)
```

#### `click(int button)`

**Description:**  
Performs a mouse button click.

**Parameters:**

- `button` (`int`): The mouse button to click.
  - `1` - Left Click
  - `2` - Right Click
  - `3` - Middle Click

**Usage:**

```c
click(1); // Left Click
```

```python
mouse.click(1)  # Left Click
```

#### `hold_and_move(int x, int y, int button, int duration_ms)`

**Description:**  
Holds down a specified mouse button and moves the cursor to the target (x, y) position over the specified duration.

**Parameters:**

- `x` (`int`): The target x-coordinate.
- `y` (`int`): The target y-coordinate.
- `button` (`int`): The mouse button to hold.
  - `1` - Left Click
  - `2` - Right Click
  - `3` - Middle Click
- `duration_ms` (`int`): Duration in milliseconds for the movement.

**Usage:**

```c
hold_and_move(1200, 800, 1, 1500); // Hold left button and move
```

```python
mouse.hold_and_move(1200, 800, 1, 1500)  # Hold left button and move
```

#### `set_mouse_speed(int speed)`

**Description:**  
Sets the speed of mouse movement in milliseconds per step.

**Parameters:**

- `speed` (`int`): Mouse speed. Must be greater than `0`.

**Usage:**

```c
set_mouse_speed(10);
```

```python
mouse.set_mouse_speed(10)
```

#### `set_config(int strength, int hold_time_ms, int mouse_speed, int easing_type, bool smooth_movement)`

**Description:**  
Configures the mouse behavior with custom settings.

**Parameters:**

- `strength` (`int`): Movement strength (1-100).
- `hold_time_ms` (`int`): Hold time for clicks in milliseconds.
- `mouse_speed` (`int`): Speed of mouse movement in ms per step.
- `easing_type` (`int`): Type of easing function.
  - `1` - Linear
  - `2` - Quadratic
  - `3` - Sinusoidal
  - `4` - Cubic
  - `5` - Exponential
- `smooth_movement` (`bool`): Enable (`true`) or disable (`false`) smooth movement.

**Usage:**

```c
set_config(60, 250, 5, 3, true);
```

```python
mouse.set_config(60, 250, 5, 3, True)
```

#### `apply_preset(int preset_type)`

**Description:**  
Applies a preset configuration.

**Parameters:**

- `preset_type` (`int`):
  - `1` - Default
  - `2` - Fast
  - `3` - Smooth

**Usage:**

```c
apply_preset(3); // Apply Smooth preset
```

```python
mouse.apply_preset(3)  # Apply Smooth preset
```

#### `set_logging_level(int level)`

**Description:**  
Sets the logging level for the library.

**Parameters:**

- `level` (`int`):
  - `0` - None
  - `1` - Error
  - `2` - Info
  - `3` - Debug

**Usage:**

```c
set_logging_level(2); // Set to Info level
```

```python
mouse.set_logging_level(2)  # Set to Info level
```

### Configuration Parameters

- **Strength (`strength`):**  
  Determines the number of steps in mouse movement. Higher values result in smoother and longer transitions.

- **Hold Time (`hold_time_ms`):**  
  Duration in milliseconds to hold a mouse button down during a click.

- **Mouse Speed (`mouse_speed`):**  
  Speed of mouse movement in milliseconds per step. Lower values result in faster movement.

- **Easing Type (`easing_type`):**  
  Defines the easing function for smooth cursor transitions. Available types:
  
  | Value | Type          |
  |-------|---------------|
  | 1     | Linear        |
  | 2     | Quadratic     |
  | 3     | Sinusoidal    |
  | 4     | Cubic         |
  | 5     | Exponential   |

- **Smooth Movement (`smooth_movement`):**  
  Enables (`true`) or disables (`false`) smooth cursor movement.

### Easing Types

Easing functions control the acceleration and deceleration of cursor movements. They provide more natural and visually appealing transitions.

1. **Linear (`EASE_LINEAR`):**  
   Constant speed from start to finish.

2. **Quadratic (`EASE_QUADRATIC`):**  
   Accelerates or decelerates based on a quadratic curve.

3. **Sinusoidal (`EASE_SINUSOIDAL`):**  
   Smooth, wave-like transitions.

4. **Cubic (`EASE_CUBIC`):**  
   Accelerates or decelerates based on a cubic curve.

5. **Exponential (`EASE_EXPONENTIAL`):**  
   Rapid acceleration or deceleration, creating an exponential curve.

### Preset Types

Presets allow users to quickly apply common configurations without manually setting each parameter.

1. **Default (`PRESET_DEFAULT`):**  
   - Strength: 50  
   - Hold Time: 300 ms  
   - Mouse Speed: 5 ms/step  
   - Easing Type: Sinusoidal  
   - Smooth Movement: Enabled

2. **Fast (`PRESET_FAST`):**  
   - Strength: 80  
   - Hold Time: 100 ms  
   - Mouse Speed: 2 ms/step  
   - Easing Type: Exponential  
   - Smooth Movement: Disabled

3. **Smooth (`PRESET_SMOOTH`):**  
   - Strength: 30  
   - Hold Time: 500 ms  
   - Mouse Speed: 10 ms/step  
   - Easing Type: Sinusoidal  
   - Smooth Movement: Enabled

### Log Levels

Control the verbosity of the library's logging output.

- **None (`LOG_LEVEL_NONE`):**  
  No logging.

- **Error (`LOG_LEVEL_ERROR`):**  
  Logs only error messages.

- **Info (`LOG_LEVEL_INFO`):**  
  Logs informational messages and errors.

- **Debug (`LOG_LEVEL_DEBUG`):**  
  Logs detailed debug information, including all messages.

---

## How It Works

Mouse Anywhere leverages the Windows API to control mouse actions programmatically. Here's an overview of its internal workings:

1. **Initialization:**
   - Opens a log file (`mouse_movement.log`) for recording actions based on the logging level.
   - Allocates and initializes global parameters with default or preset configurations.
   - Sets up critical sections to ensure thread-safe operations when accessing or modifying global parameters.

2. **Cursor Control:**
   - **Absolute Positioning:**  
     Uses `SetCursorPos` to move the cursor to specified screen coordinates.
   - **Smooth Movement:**  
     Implements smooth transitions by calculating intermediate positions using easing functions and incrementally updating the cursor position with delays (`Sleep`) based on the configured mouse speed.

3. **Mouse Clicks:**
   - Simulates mouse button presses and releases using the `SendInput` function.
   - Allows configuring the duration for which a mouse button is held down during a click.

4. **Hold and Move:**
   - Combines holding a mouse button with cursor movement to perform drag operations.
   - Ensures that the cursor movement stays within the screen bounds to prevent errors.

5. **Configuration Management:**
   - Provides functions to update configuration parameters dynamically or apply predefined presets.
   - Ensures that all configuration changes are thread-safe using critical sections.

6. **Logging:**
   - Records actions, errors, and debug information to `mouse_movement.log` based on the set logging level.
   - Includes timestamps for each log entry for better traceability.

7. **Shutdown:**
   - Cleans up resources by deleting critical sections, freeing allocated memory, and closing the log file.

---

## Logging

Mouse Anywhere provides a robust logging system to help you monitor its operations and debug issues effectively.

### Log File

All log entries are written to `mouse_movement.log` located in the application's directory.

### Log Levels

You can control the verbosity of the logs by setting the desired log level:

- **None (`LOG_LEVEL_NONE`):**  
  No logs will be recorded.

- **Error (`LOG_LEVEL_ERROR`):**  
  Only error messages are logged. Useful for identifying issues without the noise of informational messages.

- **Info (`LOG_LEVEL_INFO`):**  
  Logs general information about actions performed by the library, including successful operations and errors.

- **Debug (`LOG_LEVEL_DEBUG`):**  
  Provides detailed logs, including debug information. Ideal for development and troubleshooting.

### Setting the Log Level

You can set the log level using the `set_logging_level` function.

```c
// C Example
set_logging_level(LOG_LEVEL_DEBUG);
```

```python
# Python Example
mouse.set_logging_level(3)  # Debug level
```

### Log Entry Format

Each log entry follows the format:

```
[YYYY-MM-DD HH:MM:SS] [LOG_LEVEL] Message
```

**Example:**

```
[2024-04-27 14:35:22] [2] Mouse click performed.
[2024-04-27 14:35:25] [3] Mouse teleported to absolute position.
```

---

## Error Handling

Mouse Anywhere includes basic error handling to ensure robust operation:

- **Invalid Parameters:**  
  Functions validate input parameters and log errors if invalid values are provided. For example, attempting to click an unsupported mouse button will result in an error log.

- **Resource Allocation:**  
  If the library fails to allocate necessary resources (e.g., memory for global parameters), it logs an error message.

- **File Operations:**  
  If the log file cannot be opened, the library continues to operate but without logging capabilities.

- **Thread Safety:**  
  Uses critical sections to manage concurrent access, preventing race conditions and ensuring data integrity.

**Best Practices:**

- **Check Log Files:**  
  Regularly inspect `mouse_movement.log` to monitor the library's actions and identify any potential issues.

- **Validate Inputs:**  
  Ensure that all input parameters passed to the library functions are within the expected ranges and types to prevent errors.

---

## Contributing

Contributions are welcome! Whether you're reporting a bug, suggesting a feature, or submitting a pull request, your input helps improve Mouse Anywhere.

### How to Contribute

1. **Fork the Repository:**

   Click the **Fork** button on the [GitHub repository](https://github.com/wuhplaptop/mouse-anywhere) to create your own copy.

2. **Clone Your Fork:**

   ```bash
   git clone https://github.com/your-username/mouse-anywhere.git
   ```

3. **Create a Branch:**

   ```bash
   git checkout -b feature/your-feature-name
   ```

4. **Make Your Changes:**

   Implement your changes or additions.

5. **Commit Your Changes:**

   ```bash
   git commit -m "Add your descriptive commit message"
   ```

6. **Push to Your Fork:**

   ```bash
   git push origin feature/your-feature-name
   ```

7. **Create a Pull Request:**

   Navigate to your fork on GitHub and click **Compare & pull request**. Provide a clear description of your changes.

### Reporting Issues

If you encounter any bugs or have suggestions for improvements, please open an issue on the [GitHub Issues](https://github.com/wuhplaptop/mouse-anywhere/issues) page.

---

## License

Mouse Anywhere is licensed under the [MIT License](LICENSE).

---

## Support

For any questions, issues, or support requests, please visit our [GitHub repository](https://github.com/wuhplaptop/mouse-anywhere/tree/main) and open an issue. We're here to help!

---

## Example Usage

### Python Wrapper Example

Here's a comprehensive example demonstrating how to use the Python wrapper to perform various mouse operations:

```python
import mouse_anywhere as mouse
import time

def main():
    # Initialize the library
    mouse.initialize()

    # Set logging level to Debug
    mouse.set_logging_level(3)

    # Apply the Smooth preset
    mouse.apply_preset(3)

    # Move cursor to (500, 500) smoothly
    mouse.set_cursor_abs(500, 500)
    time.sleep(1)

    # Perform a left click
    mouse.click(1)
    time.sleep(0.5)

    # Hold left button and move to (800, 800) over 2 seconds
    mouse.hold_and_move(800, 800, 1, 2000)
    time.sleep(1)

    # Update configuration dynamically
    mouse.set_config(strength=70, hold_time_ms=200, mouse_speed=4, easing_type=2, smooth_movement=False)

    # Move cursor to (300, 300) without smooth movement
    mouse.set_cursor_abs(300, 300)
    time.sleep(1)

    # Shutdown the library
    mouse.mouse_shutdown()

if __name__ == "__main__":
    main()
```

**Explanation:**

1. **Initialization:**
   - Initializes the Mouse Anywhere library.

2. **Logging:**
   - Sets the logging level to Debug for detailed logs.

3. **Preset:**
   - Applies the Smooth preset for smoother cursor movements.

4. **Cursor Movement:**
   - Moves the cursor to `(500, 500)` smoothly.

5. **Click:**
   - Performs a left mouse click.

6. **Hold and Move:**
   - Holds the left mouse button and moves the cursor to `(800, 800)` over 2 seconds, simulating a drag operation.

7. **Dynamic Configuration:**
   - Updates the configuration to increase strength, reduce hold time, adjust mouse speed, change easing type, and disable smooth movement.

8. **Cursor Movement Without Smoothness:**
   - Moves the cursor to `(300, 300)` without smooth transitions based on the updated configuration.

9. **Shutdown:**
   - Shuts down the library, ensuring all resources are properly released.

---

## Frequently Asked Questions (FAQ)

### 1. **Can I use Mouse Anywhere on multiple monitors?**

Yes, Mouse Anywhere supports multi-monitor setups. It automatically enforces screen bounds based on the virtual screen dimensions to ensure the cursor stays within available display areas.

### 2. **How do I change the log file location?**

By default, the log file `mouse_movement.log` is created in the application's directory. To change the location, modify the `LOG_FILE` macro in the C source code and rebuild the DLL.

### 3. **What easing function should I use for the smoothest movement?**

The `EASE_SINUSOIDAL` easing type provides smooth, natural transitions that are visually appealing for most use cases.

### 4. **Is the library thread-safe?**

Yes, Mouse Anywhere uses critical sections to manage concurrent access to global parameters, ensuring thread safety.

### 5. **Can I extend the library to support more features?**

Absolutely! The source code is open for customization. Feel free to fork the repository and add new functionalities as needed.

### 6. **Do I need to have administrator privileges to use Mouse Anywhere?**

No, Mouse Anywhere does not require administrator privileges for standard operations. However, certain actions or system configurations might necessitate elevated permissions.

### 7. **How can I troubleshoot issues with Mouse Anywhere?**

Enable detailed logging by setting the log level to Debug (`3`). Check the `mouse_movement.log` file for detailed information about the library's operations and any errors encountered.

### 8. **Is Mouse Anywhere compatible with Python 3.x?**

Yes, Mouse Anywhere is compatible with Python 3.6 and later versions.

### 9. **Can I use Mouse Anywhere in a virtual environment?**

Yes, as long as the virtual environment has access to the necessary DLL files and dependencies, Mouse Anywhere can be used within a Python virtual environment.

### 10. **How do I update Mouse Anywhere to the latest version?**

If installed via `pip`, you can update using:

```bash
pip install --upgrade mouse-anywhere
```

If built manually, pull the latest changes from the repository and rebuild the DLL.

---

## Contact

For more information, support, or to contribute, visit our [GitHub repository](https://github.com/wuhplaptop/mouse-anywhere/tree/main).
