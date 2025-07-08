# AirControlX ✈️

**AirControlX** is a C++ simulation of an air traffic control system that manages aircraft landings and takeoffs using advanced scheduling and resource allocation logic. Designed as an Operating Systems project, it emphasizes process synchronization and CPU scheduling algorithms to simulate realistic air control behavior.

## 🧠 Key Concepts

- CPU Scheduling Algorithms (SJF, Round Robin, etc.)
- Thread Synchronization (using atomic variables and condition logic)
- Resource Allocation (e.g., runway management)
- Process Lifecycle Simulation

## ✨ Features

- Simulates arrival and departure of flights
- Allocates runways based on direction and priority
- Uses real-time constraints to schedule takeoffs/landings
- Avoids busy waiting and ensures smooth synchronization
- Terminal-style output with logs of flight activity

## 🛠 Tech Stack

- C++ (Standard Library, STL)
- Threading (using `std::thread`, `std::atomic`, etc.)
- Command Line / Console Application

## 🚀 How to Run

1. **Clone the repo**
   ```bash
   git clone https://github.com/MishalFatima09/AirControlX.git
   cd AirControlX
2. Compile the code:
   g++ -std=c++17 main.cpp -o AirControlX -pthread
3. Sample Output:
   [RUNWAY 1] Flight PK303 is cleared to land.
   [RUNWAY 2] Flight EK121 is cleared for takeoff.
   ...

## 🤝 Collaborators

This project was developed as a team effort. Special thanks to:

- [Mishal Fatima](https://github.com/MishalFatima09)  
- [Aleena Babar](https://github.com/aleenababar04)  

