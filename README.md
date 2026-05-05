# Autonomous Sumobot – Arduino Robotics Competition

<p align="center">
  <img src="https://github.com/user-attachments/assets/4bf4b6a0-1e34-4247-b4bc-b45c3e4e0b55" height="250" />
  <img src="https://github.com/user-attachments/assets/6033bd5d-2038-4195-90b1-81573d5dc97d" height="250" />
  <img src="https://github.com/user-attachments/assets/f71f6928-9bdf-462c-8186-7073631e7c85" height="250" />
</p>

**[Watch the robot in competition](https://youtu.be/aZ-ZAUZNrYw)**

## Overview
This project is an autonomous sumobot built using an Arduino-based platform for a competitive robotics tournament. The robot detects opponents using ultrasonic sensing and was originally designed with IR-based border detection for ring safety.

During competition testing and iteration, the border detection system was removed in the final version due to interference with offensive performance and match effectiveness. This resulted in a revised behaviour hierarchy prioritising opponent detection and search strategy.

The system is implemented in C++ and focuses on fast reactive decision-making under strict timing constraints.

---

## System Architecture

The robot operates using a **priority-based control loop**:

1. Opponent detection (attack mode)
2. Search mode (default behaviour)

> Note: The original design included IR-based border detection for safety. This was removed in the final competition version after testing showed it reduced offensive effectiveness and disrupted engagement consistency.

Each cycle continuously reads sensor inputs and executes the highest priority action.

---

## Key Parameters (Tuning-Focused Design)

### Movement Speeds
- Attack speed: `255` (max power)
- Search speed: `150` (controlled scanning rotation)
- Reverse speed: `200`

---

### Behaviour Timing
- Pivot duration: `350 ms`
  - Tuned experimentally for ~90° turn accuracy
- Evade cooldown: `1500 ms`
  - Prevents repeated false-trigger evasion loops

---

### Sensor Thresholds
- Opponent detection range: `< 55 cm`
- IR border detection used for ring boundary safety

---

## Behaviour Logic

The final competition version uses a reduced priority system:

### Priority 1: Attack Mode
- If opponent detected within range (< 55 cm):
  - Drive forward at full speed (255 PWM)
- Designed for direct engagement strategy

---

### Priority 2: Search Mode (Default)
- Continuous spin using differential motor control
- Acts as scanning behaviour to locate opponent

---

## Hardware Control

### Motor Driver Interface
Each motor controlled using:
- Direction pins (forward/reverse)
- PWM speed control pin

Left and right motors independently controlled for:
- Forward movement
- Reverse movement
- Pivot turns
- In-place rotation (spin/search)

---

## Sensors Used

- **Ultrasonic Sensor (HC-SR04)**
  - Measures opponent distance
  - Used for attack decision logic

> Note: The IR line sensor was used in an earlier prototype for border detection. This feature was removed in the final competition version due to interference with offensive performance and reduced match effectiveness.

---

## Key Engineering Concepts

- Priority-based behaviour architecture
- Real-time embedded decision-making
- Sensor fusion (IR + ultrasonic)
- PWM motor control tuning
- Timing-based cooldown systems
- Non-blocking loop-based control logic
- Physical system calibration (pivot timing tuning)

---
