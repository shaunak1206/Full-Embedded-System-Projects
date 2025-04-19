# Embedded Systems Projects Portfolio

This repository contains a collection of embedded systems projects developed using the Texas Instruments MSPM0 microcontroller family. These projects demonstrate proficiency in real-time systems, hardware interfacing, and low-level programming.

## 🚀 Featured Projects

### Space Invaders Game (ECE319K_Lab9H)
- **Description**: A complete implementation of the classic Space Invaders game with multiple language support
- **Key Features**:
  - Real-time game mechanics with collision detection
  - Multi-language support (English, Spanish, French)
  - ST7735 LCD display interface
  - Input handling via buttons and slide switches
  - Sound effects and game state management
- **Technologies**: C++, MSPM0, ST7735 LCD, GPIO, Timer Interrupts

### Waveform Player (WavPlay)
- **Description**: Digital audio player with waveform visualization
- **Key Features**:
  - WAV file playback
  - Real-time waveform display
  - Audio processing and buffering
- **Technologies**: DAC, DMA, File I/O, Audio Processing

### Traffic Light Controller (TrafficLightFSM)
- **Description**: Finite State Machine implementation of a traffic light system
- **Key Features**:
  - State-based control logic
  - Timer-based transitions
  - LED control and timing management
- **Technologies**: FSM, Timer Interrupts, GPIO

### UART Communication (UART_ints, UART_busywait)
- **Description**: Serial communication implementations using both interrupt and polling methods
- **Key Features**:
  - Interrupt-driven UART communication
  - Polling-based UART implementation
  - Data buffering and error handling
- **Technologies**: UART, Interrupts, Serial Communication

### Display Interfaces
- **ST7735 LCD Interface**: Color LCD display driver with graphics capabilities
- **SSD1306 OLED**: Monochrome OLED display interface
- **Crystalfontz LCD**: Character LCD interface implementation

## 🛠️ Technical Skills Demonstrated

- **Microcontroller Programming**: MSPM0 family (ARM Cortex-M0+)
- **Hardware Interfaces**:
  - GPIO
  - UART
  - I2C
  - SPI
  - ADC/DAC
  - Timer Modules
- **Display Technologies**:
  - ST7735 Color LCD
  - SSD1306 OLED
  - Character LCD
- **Real-time Systems**:
  - Interrupt Handling
  - Timer Management
  - State Machines
- **Low-level Programming**:
  - C/C++
  - Assembly Language
  - Register-level Programming
- **Development Tools**:
  - Code Composer Studio
  - TExaS Debugger
  - Version Control (Git)

## 📁 Project Structure

```
├── ECE319K_Lab9H/          # Space Invaders Game
├── WavPlay/                # Audio Player
├── UART_ints/             # Interrupt-based UART
├── UART_busywait/         # Polling-based UART
├── TrafficLightFSM/       # Traffic Light Controller
├── Display Interfaces/    # Various display drivers
└── Documentation/         # Project documentation
```

## 🔧 Development Environment

- **IDE**: Code Composer Studio
- **Debugger**: TExaS
- **Target Hardware**: Texas Instruments MSPM0 Family
- **Version Control**: Git

## 📚 Learning Resources

- Texas Instruments MSPM0 Family Documentation
- Embedded Systems Design Patterns
- Real-time Systems Programming
- Hardware Interface Protocols

## 🤝 Contributing

Feel free to explore the projects and use them as learning resources. If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## 📄 License

This project is licensed under the terms specified in the license.txt file.

---

*Created by Manas Pathak - Embedded Systems Engineer* 