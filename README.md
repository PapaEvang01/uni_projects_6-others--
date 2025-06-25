# Introductory Projects – Arduino, NS-3, and RISC-V Assembly

These projects were developed during my early university years while exploring different domains: **Embedded Systems**, **Network Simulation**, and **Computer Architecture**.  
Though introductory in nature, they helped me understand the fundamentals of hardware control, real-time simulation, and low-level logic
— and sparked ideas for future, more advanced work.

Each project reflects a stepping stone toward deeper knowledge in **IoT**, **network behavior**, and **assembly-level computation**.

---

## i) Arduino Projects – IoT & Sensor Control

These two projects were built on the **ESP32 microcontroller** and developed during my **Erasmus program at the University of Deusto, Spain**.  
They involve **sensor fusion**, **actuator control**, **data visualization**, and **Wi-Fi + API integration**.

---

###  Project 1: CropCheck – Smart Agriculture Monitoring Device

A modular portable system designed for **smart farming**, with multiple modes of sensing and response.

####  Features:
- **T1:** Environmental data (light, temperature, humidity) to Serial Monitor
- **T2:** RGB LED feedback with color thresholds
- **T3:** Touch button to toggle servo; vibration simulates seed sorting
- **T4:** Ultrasonic alert system with servo disabling and red LED flash

📁 Files:
- `CropCheck_T1_T2.ino`  
- `CropCheck_T3_T4.ino`

---

###  Project 2: Weather Condition Display System (Wi-Fi + API)

Fetches **live weather data** from OpenWeatherMap and controls multiple actuators based on environmental values.

####  Mapped Output:
- **Temperature** → RGB Strip (color gradient + blinking)
- **Wind Speed** → Servo sweep
- **Humidity** → Vibration motor
- **LCD** → Displays temperature + humidity

📁 File:
- `WeatherDisplaySystem.ino`

> 🧠 Skills: Wi-Fi connection, HTTP GET, JSON parsing, sensor-actuator loop

---

## ii) NS-3 Network Simulation Projects

Two simple simulations built with **ns-3** to explore **network topology design** and **queue behavior in congestion**.

---

###  Project 1: P2P with CSMA – Hybrid Topology Simulation

Simulates a small network with a **point-to-point** backbone and **CSMA Ethernet LAN**.

####  Setup:
- Nodes `n0` and `n1` linked via P2P
- Node `n1` bridges to a CSMA LAN (`n2` to `n21`)
- **UDP Echo Server** at `n0`, **UDP Client** at `n21`

📌 Goal: Visualize traffic flow and hybrid topology interaction

---

###  Project 2: TCP Queuing – DropTail vs RED

A 4-node TCP simulation that compares basic vs. active queuing strategies.

#### Setup:
- TCP bulk traffic from A & B → Router C → Destination D
- Compare:
  - `original.cc` → DropTail queue  
  - `red.cc` → RED (Random Early Detection) AQM

📌 Goal: Observe how queue choice impacts congestion and throughput

---

## iii) RISC-V Assembly Projects – Low-Level System Exploration

Early programming exercises using the **RISC-V instruction set** via the **RARS simulator**.  
These projects introduced me to register manipulation, memory addressing, and the fundamentals of assembly programming.

---

###  Project 1: Fibonacci Sequence

Prints the first `n` terms of the Fibonacci sequence using control flow and loops.

🧠 Concepts:
- Integer registers
- `ecall` input/output
- Loop-based recursion

💻 Tool: RARS (RISC-V simulator)

---

### Project 2: Row Sum of 2D Array

Computes the sum of each row in a 2D array using nested loops and memory traversal.

🧠 Concepts:
- Address arithmetic
- Multi-dimensional array indexing
- Translation from C to Assembly

 Tool: RARS (RISC-V simulator)

---

## ! Summary

These projects laid the groundwork for future work across three key areas:

- **Arduino (ESP32):** real-world interfacing, sensor control, and responsive embedded systems  
- **NS-3:** basic network simulation, congestion modeling, queue discipline understanding  
- **RISC-V Assembly:** low-level computation, memory access logic, hardware-level thinking

> Even as early explorations, they gave me important insight into system behavior and inspired ideas for more complex future development.

---

## 👨‍💻 Author

**Vaggelis Papaioannou**  
Department of Electrical & Computer Engineering  
Democritus University of Thrace
