
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
- [API Reference](#api-reference)
- [Logging](#logging)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Introduction

**Mouse Anywhere** is a Python package that provides smooth and programmable mouse movements on Windows systems by leveraging a high-performance C library. Whether you're automating repetitive tasks, creating demonstrations, or enhancing your gaming experience, Mouse Anywhere offers precise and customizable control over your mouse cursor.

## Features

- **Smooth Movement**: Utilize various easing functions to achieve natural and fluid mouse movements.
- **Absolute and Relative Targeting**: Move the cursor to specific screen coordinates or relative to its current position.
- **Multithreaded Operation**: Perform mouse movements in the background without blocking your main application.
- **Queue System**: Add multiple movement targets that execute sequentially.
- **Customizable Strength and Easing**: Adjust the speed and style of movements to fit your needs.
- **Logging**: Keep track of all movements and actions with detailed logs.

## Installation

Mouse Anywhere is available on [PyPI](https://pypi.org/project/mouse-anywhere/). You can install it using `pip`:

```bash
pip install mouse-anywhere
```

**Note**: This package is **Windows-only** due to its reliance on Windows-specific APIs.

## Quick Start

Here's a simple example to get you started with Mouse Anywhere:

```python
from mouse_anywhere import MouseMovement, EASE_SINUSOIDAL
import time

# Initialize MouseMovement with Sinusoidal easing and medium strength
mouse = MouseMovement(easing_type=EASE_SINUSOIDAL, strength=50)

# Start the movement thread
mouse.start()

# Add an absolute target to move the cursor to (1000, 800)
mouse.add_absolute_target(1000, 800)

# Add a relative target to move the cursor left by 50 pixels and down by 50 pixels
mouse.add_relative_target(-50, 50)

# Allow some time for movements to execute
time.sleep(5)

# Stop the movement thread
mouse.stop()
```

## Detailed Usage

### Initialization

To begin using Mouse Anywhere, create an instance of `MouseMovement` with your desired settings:

```python
from mouse_anywhere import MouseMovement, EASE_LINEAR

# Parameters:
# easing_type: Determines the movement easing function.
# strength: Controls the speed and smoothness (1-100).

mouse = MouseMovement(easing_type=EASE_LINEAR, strength=75)
```

### Adding Targets

You can queue multiple movement targets, both absolute and relative.

#### Add Absolute Target

Moves the cursor to specific screen coordinates.

```python
# Move cursor to (500, 300) on the screen
mouse.add_absolute_target(500, 300)
```

#### Add Relative Target

Moves the cursor relative to its current position.

```python
# Move cursor 100 pixels to the right and 50 pixels down
mouse.add_relative_target(100, 50)
```

### Starting and Stopping Movement

Start the background thread to begin processing queued movements and stop it when done.

```python
# Start processing movements
mouse.start()

# ... Add targets as needed ...

# Stop processing movements
mouse.stop()
```

## Easing Types

Easing functions determine how the cursor accelerates and decelerates during movement. Mouse Anywhere supports the following easing types:

| Easing Type      | Description                             |
| ---------------- | --------------------------------------- |
| `EASE_LINEAR` (1) | Constant speed throughout the movement. |
| `EASE_QUADRATIC` (2) | Accelerates quadratically at the start. |
| `EASE_SINUSOIDAL` (3) | Smooth sinusoidal acceleration and deceleration. |
| `EASE_CUBIC` (4) | Accelerates cubically, offering a more pronounced easing effect. |
| `EASE_EXPONENTIAL` (5) | Starts slowly and accelerates exponentially. |

**Example Usage:**

```python
from mouse_anywhere import MouseMovement, EASE_QUADRATIC

mouse = MouseMovement(easing_type=EASE_QUADRATIC, strength=60)
```

## API Reference

### `MouseMovement` Class

#### Initialization

```python
MouseMovement(easing_type=EASE_TYPE, strength=INT)
```

- **Parameters**:
  - `easing_type` (`int`): The easing function to use (see [Easing Types](#easing-types)).
  - `strength` (`int`): Determines the speed and smoothness (1-100).

#### Methods

- **`start()`**

  Starts the movement processing thread.

  ```python
  mouse.start()
  ```

- **`stop()`**

  Stops the movement processing thread gracefully.

  ```python
  mouse.stop()
  ```

- **`add_absolute_target(x, y)`**

  Adds an absolute movement target to the queue.

  ```python
  mouse.add_absolute_target(800, 600)
  ```

- **`add_relative_target(dx, dy)`**

  Adds a relative movement target to the queue.

  ```python
  mouse.add_relative_target(-100, 100)
  ```

### Easing Type Constants

```python
from mouse_anywhere import (
    EASE_LINEAR,
    EASE_QUADRATIC,
    EASE_SINUSOIDAL,
    EASE_CUBIC,
    EASE_EXPONENTIAL
)
```

## Logging

Mouse Anywhere maintains a log file named `mouse-anywhere.log` in the working directory. This log records all actions, movements, and any encountered errors, aiding in debugging and monitoring.

**Sample Log Entry:**

```
[2024-12-23 14:35:22] MovementParams created.
[2024-12-23 14:35:25] Movement thread started.
[2024-12-23 14:35:26] Enqueued new target: (1000, 800) [Absolute]
[2024-12-23 14:35:27] Moved to (1000, 800)
[2024-12-23 14:35:28] Target reached within radius.
[2024-12-23 14:35:30] Movement stopped.
[2024-12-23 14:35:30] MovementParams destroyed.
```
