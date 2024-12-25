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
  - [Clicking the Mouse](#clicking-the-mouse)
  - [Customizing Movement](#customizing-movement)
- [Easing Types](#easing-types)
- [Low-Level API (DLL Integration)](#low-level-api-dll-integration)
- [Logging](#logging)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

---

## Introduction

**Mouse Anywhere** is a Python package designed for precise and programmable mouse movements on Windows. Built on a high-performance C-based DLL, it offers features like advanced easing types, smooth cursor transitions, and target queueing, making it perfect for automation, testing, or gaming tasks.

---

## Features

- **Smooth Movement Profiles**: Sinusoidal, quadratic, cubic, exponential, and linear easing.
- **Absolute and Relative Movement**: Move the cursor by screen coordinates or relative offsets.
- **Multi-Target Queue**: Queue multiple movements for seamless transitions.
- **Click Functionality**: Simulate left, right, or middle mouse clicks.
- **High-Performance C Backend**: Leverages native Windows API for efficiency.
- **Cross-Monitor Precision**: Ensures movements respect all screen boundaries.
- **Detailed Logs**: Track mouse movements and actions.

---

## Why Mouse Anywhere?

Mouse Anywhere surpasses other Python libraries like `pyautogui` by offering:
- **Advanced Easing**: Human-like smoothness for mouse movements.
- **Queue Management**: Stack and execute multiple cursor actions in sequence.
- **Performance**: Built with a robust C library for minimal latency.
- **Versatility**: Suitable for both high-speed automation and interactive applications.

---

## Installation

Install Mouse Anywhere via PyPI:

```bash
pip install mouse-anywhere
```

### Requirements
- Windows OS
- Python 3.6+
- The `mouse-anywhere.dll` file is included in the package.

---

## Quick Start

Here's a minimal example to get started:

```python
from mouse_anywhere import initialize, shutdown, set_cursor_abs, perform_click

# Initialize the DLL
initialize()

# Move the cursor to an absolute position
set_cursor_abs(500, 500)

# Perform a left click
perform_click(1)

# Shutdown the DLL
shutdown()
```

---

## Detailed Usage

### Initialization

Start by initializing the DLL:

```python
from mouse_anywhere import initialize, shutdown

# Initialize
initialize()

# Perform cursor actions here

# Shutdown
shutdown()
```

### Adding Targets

#### Absolute Targeting
Queue a movement to a specific position:
```python
from mouse_anywhere import enqueue_target_abs

# Queue a movement to position (600, 400)
enqueue_target_abs(600, 400)
```

#### Relative Targeting
Queue a movement relative to the current position:
```python
from mouse_anywhere import enqueue_target_rel

# Move 50 pixels right and 30 pixels down
enqueue_target_rel(50, 30)
```

### Clicking the Mouse

Simulate mouse clicks with `perform_click`. Supported buttons:
- `1`: Left click
- `2`: Right click
- `3`: Middle click

```python
from mouse_anywhere import perform_click

# Perform a left click
perform_click(1)

# Perform a right click
perform_click(2)
```

### Customizing Movement

Adjust easing type and movement strength for smoother animations.

```python
from mouse_anywhere import set_cursor_abs

# Set cursor with easing and smooth movement
set_cursor_abs(800, 300)
```

---

## Easing Types

| **Type**            | **Description**                             |
|----------------------|---------------------------------------------|
| `EASE_LINEAR` (1)    | Constant speed.                            |
| `EASE_QUADRATIC` (2) | Smooth acceleration using a quadratic curve.|
| `EASE_SINUSOIDAL` (3)| Sinusoidal easing for natural movement.     |
| `EASE_CUBIC` (4)     | Faster easing with a cubic curve.           |
| `EASE_EXPONENTIAL` (5)| Exponential acceleration for rapid movement.|

---

## Low-Level API (DLL Integration)

For advanced use cases, interact directly with the DLL:

```python
from ctypes import CDLL

# Load the DLL
dll = CDLL('mouse-anywhere.dll')

# Initialize the DLL
dll.initialize()

# Move the cursor
dll.set_cursor_abs(500, 500)

# Shutdown the DLL
dll.mouse_shutdown()
```

---

## Logging

Mouse Anywhere automatically logs all actions to `mouse_movement.log`.

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

1. Fork the repository: [Mouse Anywhere](https://github.com/wuhplaptop/mouse-anywhere)
2. Create a feature branch: `git checkout -b feature-name`
3. Implement your changes.
4. Submit a pull request.

