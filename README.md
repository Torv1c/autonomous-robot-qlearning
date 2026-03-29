# Autonomous Edge AI Robot: Q-Learning Navigation

**Published Research Project — 15th CONICT (2024)**

## 📌 Project Overview
This repository contains the C++ firmware for an autonomous line-following robot that uses **Reinforcement Learning (Q-Learning)** to learn navigation directly on edge hardware. 

Instead of relying on hardcoded rules (like traditional PID or Fuzzy Logic controllers), the robot explores its environment, makes mistakes, and dynamically optimizes its trajectory in real-time. This project was officially published and presented at the 15th Congress of Innovation, Science, and Technology (CONICT 2024).

## 🛠️ Tech Stack & Hardware
* **Language:** C / C++ 
* **Hardware:** Arduino Uno (Microcontroller), 3x Infrared Sensors, 2x DC Motors, PWM Motor Driver.
* **AI Concept:** Tabular Q-Learning, Epsilon-Greedy Exploration, Edge AI.

## 🧠 How the AI Works (Architecture)
The AI model was built entirely from scratch in C++, tailored to fit the memory constraints of an Arduino Uno. 

### States & Actions
The system maps the physical environment into a discrete matrix:
* **State Space (s):** 32 distinct states. The state is determined by a combination of 5 variables: 3 IR sensor readings (line detected/not detected) and 2 motor speed variables (low/high).
* **Action Space (a):** 4 possible actions, corresponding to the speed combinations (low/high) applied to the left and right motors.

### The Q-Learning Algorithm
The core of the navigation relies on continuously updating the Q-Table using the Bellman equation:

<img src="https://latex.codecogs.com/svg.image?Q^{novo}(s_t,a_t)&space;\leftarrow&space;(1-\alpha)&space;\cdot&space;Q^{anterior}(s_t,a_t)&space;&plus;&space;\alpha&space;\cdot&space;(r_{t&plus;1}&space;&plus;&space;\gamma&space;\cdot&space;\max_a&space;Q(s_{t&plus;1},a))" title="Bellman Equation" />

* **Dynamic Hyperparameters:** The learning rate (α) starts at 1.0 and decays to 0.5 to stabilize learning over time, while the discount factor (γ) increases from 0.5 to 0.8, making the agent prioritize long-term rewards as it gains experience.
* **Reward System:** A custom reward array dictates the learning path. Positive rewards (+10) are granted for advancing smoothly while centered, whereas severe penalties (-10) are applied for deviating from the track.
* **Exploration vs. Exploitation:** The system periodically forces a random action to discover better routes, ensuring continuous adaptation to the environment.

## 📊 Results
After initiating with a completely zeroed Q-Table, the robot engages in an active learning phase. Initially losing the track frequently, the agent adjusts its policy matrix and achieves autonomous, error-free navigation within approximately 10 minutes of continuous environmental interaction.

---
**Author:** Victor Cesar de Mecê Prando
