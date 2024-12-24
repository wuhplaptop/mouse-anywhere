# Mouse Anywhere

[![PyPI Version](https://img.shields.io/pypi/v/mouse-anywhere.svg)](https://pypi.org/project/mouse-anywhere/)
[![Build Status](https://github.com/wuhplaptop/mouse-anywhere/actions/workflows/workflow.yaml/badge.svg)](https://github.com/wuhplaptop/mouse-anywhere/actions)
[![License](https://img.shields.io/pypi/l/mouse-anywhere.svg)](https://github.com/wuhplaptop/mouse-anywhere/blob/main/LICENSE)
[![Python Versions](https://img.shields.io/pypi/pyversions/mouse-anywhere.svg)](https://www.python.org/downloads/)

![Mouse Anywhere Banner](https://github.com/wuhplaptop/mouse-anywhere/blob/main/banner.png)

---

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Why Mouse Anywhere?](#why-mouse-anywhere)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Detailed Usage](#detailed-usage)
  - [Initialization](#initialization)
  - [Adding Targets](#adding-targets)
  - [Starting and Stopping Movement](#starting-and-stopping-movement)
- [Easing Types](#easing-types)
- [Low-Level API (DLL Integration)](#low-level-api-dll-integration)
- [Logging](#logging)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

---

## Introduction

**Mouse Anywhere** is a Python library that provides smooth and programmable mouse movements on Windows systems, powered by a high-performance C-based DLL. This library is ideal for tasks requiring precise cursor control, such as automation, gaming, GUI testing, and presentations.

Unlike other Python libraries like `pyautogui`, **Mouse Anywhere** supports:

- **Advanced Easing Types:** Enable natural, human-like mouse movements.
- **Target Queueing:** Queue multiple cursor movements seamlessly.
- **Absolute and Relative Movement:** Move the cursor by screen coordinates or relative offsets.
- **High Precision:** Ensure cursor movements stay within screen bounds.

---

## Features

- **Smooth Movements:** Supports multiple easing types like sinusoidal, quadratic, and exponential.
- **Absolute and Relative Targeting:** Flexibly move the cursor.
- **Queue-Based Targeting:** Chain multiple movements for seamless transitions.
- **Multi-threading:** Perform cursor operations without blocking the main thread.
- **DLL Performance:** Leverages native Windows API for efficiency.
- **Comprehensive Logging:** Tracks every action for debugging and monitoring.

---

## Why Mouse Anywhere?

**Mouse Anywhere** outshines alternatives like `pyautogui` by providing:

- **Advanced Movement Profiles:** Customizable easing types and strengths.
- **Precision Control:** Handles multi-monitor setups and edge cases.
- **Background Operations:** Ensures smooth integration with other applications.
- **Performance and Reliability:** Built with a robust C library for low latency.

---

## Installation

Install Mouse Anywhere via [PyPI](https://pypi.org/project/mouse-anywhere/):

```bash
pip install mouse-anywhere
```

**Requirements:**

- Windows OS
- Python 3.x
- The `mouse-anywhere.dll` file must be in the same directory as your Python script.

---

## Quick Start

Here's a quick example to get you started:

```python
from mouse_anywhere import initialize, shutdown, set_cursor_abs, set_cursor_rel

# Initialize the DLL
initialize()

# Move cursor to an absolute position (100, 200)
set_cursor_abs(100, 200)

# Move cursor relatively by (50, -25)
set_cursor_rel(50, -25)

# Shutdown the DLL
shutdown()
```

---

## Detailed Usage

### Initialization

To initialize the library, create an instance of `MouseAnywhere` or directly call `initialize()`.

```python
from mouse_anywhere import MouseAnywhere

# Using context manager
with MouseAnywhere() as mouse:
    pass  # Cursor operations go here

# Or manually
mouse = MouseAnywhere()
mouse.initialize()
# Perform operations...
mouse.shutdown()
```

### Adding Targets

#### Absolute Target

Queue a movement to a specific screen coordinate:
```python
mouse.enqueue_target_abs(500, 300)
```

#### Relative Target

Queue a movement relative to the current cursor position:
```python
mouse.enqueue_target_rel(100, 50)
```

### Starting and Stopping Movement

Start the movement thread:
```python
mouse.start()
```

Stop the movement when all operations are done:
```python
mouse.stop()
```

---

## Easing Types

Customize cursor movement with easing functions for a natural feel:

| Easing Type         | Description                                |
|---------------------|--------------------------------------------|
| `EASE_LINEAR` (1)   | Constant speed throughout the movement.    |
| `EASE_QUADRATIC` (2)| Accelerated movement with a quadratic curve.|
| `EASE_SINUSOIDAL` (3)| Smooth sinusoidal acceleration and deceleration.|
| `EASE_CUBIC` (4)    | Accelerated movement with a cubic curve.    |
| `EASE_EXPONENTIAL` (5)| Exponential acceleration.                 |

---

## Low-Level API (DLL Integration)

Advanced users can interact directly with the C-based DLL for custom functionality:

### Python Integration

```python
from ctypes import CDLL

# Load the DLL
mouse_dll = CDLL('mouse-anywhere.dll')

# Initialize the DLL
mouse_dll.initialize()

# Move the cursor to an absolute position
mouse_dll.set_cursor_abs(500, 300)

# Shutdown the DLL
mouse_dll.mouse_shutdown()
```

### Exported Functions

The following functions are exposed by the DLL:

- `initialize()` - Initializes the movement system.
- `mouse_shutdown()` - Cleans up resources and stops operations.
- `set_cursor_abs(int x, int y)` - Moves the cursor to an absolute screen coordinate.
- `set_cursor_rel(int dx, int dy)` - Moves the cursor relative to its current position.
- `enqueue_target_abs(int x, int y)` - Adds an absolute target to the queue.
- `enqueue_target_rel(int dx, int dy)` - Adds a relative target to the queue.

---

## Logging

Mouse Anywhere logs all actions to `mouse_movement.log` for debugging and monitoring.

**Sample Log:**
```
[2024-12-23 14:35:22] DLL initialized.
[2024-12-23 14:35:25] Enqueued target: (1000, 800) [Absolute]
[2024-12-23 14:35:27] Cursor moved to (1000, 800).
[2024-12-23 14:35:30] DLL shutdown.
```

---

## Contributing

We welcome contributions! Here's how to get started:

1. Fork the repository on [GitHub](https://github.com/wuhplaptop/mouse-anywhere).
2. Create a feature branch.
3. Implement your feature or fix.
4. Submit a pull request.

See the [contribution guidelines](https://github.com/wuhplaptop/mouse-anywhere/blob/main/CONTRIBUTING.md) for more details.

---

## License

This project is licensed under the MIT License. See the [LICENSE](https://github.com/wuhplaptop/mouse-anywhere/blob/main/LICENSE) file for details.

---

## Acknowledgements

Special thanks to the contributors and community for supporting this project.

---

**Mouse Anywhere** — Smooth, programmable mouse control for Windows.
