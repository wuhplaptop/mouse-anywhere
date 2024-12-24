
# Mouse Anywhere

[![PyPI Version](https://img.shields.io/pypi/v/mouse-anywhere.svg)](https://pypi.org/project/mouse-anywhere/)
[![Build Status](https://github.com/wuhplaptop/mouse-anywhere/actions/workflows/workflow.yaml/badge.svg)](https://github.com/wuhplaptop/mouse-anywhere/actions)
[![License](https://img.shields.io/pypi/l/mouse-anywhere.svg)](https://github.com/wuhplaptop/mouse-anywhere/blob/main/LICENSE)
[![Python Versions](https://img.shields.io/pypi/pyversions/mouse-anywhere.svg)](https://www.python.org/downloads/)

![Mouse Anywhere Banner](https://github.com/wuhplaptop/mouse-anywhere/blob/main/banner.png)

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
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

**Mouse Anywhere** is a Python package providing smooth and programmable mouse movements for Windows systems by leveraging a high-performance C library. Designed for automation, gaming, and demonstrations, this package offers precise and customizable cursor control.

---

## Features

- Smooth and natural movements with multiple easing types.
- **Absolute** and **Relative Targeting** for flexibility.
- Multithreaded operation to run seamlessly in the background.
- Queue-based system for sequential movements.
- Adjustable strength and easing for custom movement profiles.
- Logging support for debugging and monitoring.
- Low-level DLL access for advanced use cases.

---

## Installation

Install Mouse Anywhere from [PyPI](https://pypi.org/project/mouse-anywhere/):

```bash
pip install mouse-anywhere
```

**Note**: Mouse Anywhere is **Windows-only** due to platform-specific dependencies.

---

## Quick Start

Below is a quick example to get you started:

```python
from mouse_anywhere import MouseMovement, EASE_SINUSOIDAL
import time

# Initialize MouseMovement
mouse = MouseMovement(easing_type=EASE_SINUSOIDAL, strength=50)

# Start the movement
mouse.start()

# Add targets
mouse.add_absolute_target(1000, 800)
mouse.add_relative_target(-50, 50)

# Allow some time for movement
time.sleep(5)

# Stop the movement
mouse.stop()
```

---

## Detailed Usage

### Initialization

```python
from mouse_anywhere import MouseMovement, EASE_LINEAR

# Create a MouseMovement instance
mouse = MouseMovement(easing_type=EASE_LINEAR, strength=75)
```

### Adding Targets

#### Absolute Target
```python
mouse.add_absolute_target(500, 300)
```

#### Relative Target
```python
mouse.add_relative_target(100, 50)
```

### Starting and Stopping Movement

```python
mouse.start()
mouse.stop()
```

---

## Easing Types

| Easing Type      | Description                             |
| ---------------- | --------------------------------------- |
| `EASE_LINEAR` (1) | Constant speed throughout the movement. |
| `EASE_QUADRATIC` (2) | Quadratic acceleration.               |
| `EASE_SINUSOIDAL` (3) | Smooth sinusoidal easing.            |
| `EASE_CUBIC` (4)    | Cubic acceleration.                   |
| `EASE_EXPONENTIAL` (5) | Exponential easing.                 |

---

## Low-Level API (DLL Integration)

Advanced users can directly interact with the underlying DLL for additional control. Below is an example:

### Python Integration

```python
from ctypes import CDLL, c_int

# Load the DLL
mouse_dll = CDLL('mouse-anywhere.dll')

# Initialize
mouse_dll.initialize()

# Absolute cursor movement
mouse_dll.set_cursor_abs(c_int(500), c_int(300))

# Shutdown
mouse_dll.mouse_shutdown()
```

### C DLL Functions

The DLL includes the following exported functions:

- `initialize()` - Initialize the DLL.
- `mouse_shutdown()` - Shut down the DLL.
- `set_cursor_abs(int x, int y)` - Move the cursor to an absolute position.
- `set_cursor_rel(int dx, int dy)` - Move the cursor relatively.
- `enqueue_target_abs(int x, int y)` - Add absolute target to the queue.
- `enqueue_target_rel(int dx, int dy)` - Add relative target to the queue.

Refer to the C source for more details.

---

## Logging

Mouse Anywhere logs all actions to `mouse_movement.log`:

**Sample Log:**
```
[2024-12-23 14:35:22] Movement initialized.
[2024-12-23 14:35:25] Enqueued new target: (1000, 800) [Absolute]
[2024-12-23 14:35:27] Moved to (1000, 800)
[2024-12-23 14:35:30] Stopped movement.
```

---

## Contributing

Contributions are welcome! Follow these steps:

1. Fork the repo.
2. Create a feature branch.
3. Submit a pull request.

---

## License

This project is licensed under the MIT License. See the [LICENSE](https://github.com/wuhplaptop/mouse-anywhere/blob/main/LICENSE) file for details.

---

## Acknowledgements

Special thanks to the contributors and users who made this project possible.

---

**Mouse Anywhere** — Smooth, customizable, and powerful mouse control for Windows!
