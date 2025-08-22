# Bare-Metal-Programming
# Bare Metal OS - EEET2490 Group Assignment

A bare metal operating system implementation for Raspberry Pi as part of RMIT University's EEET2490 - Embedded System: OS and Interfacing course.

## 📋 Project Overview

This project implements a complete bare metal operating system from scratch, featuring a command line interface, multimedia display capabilities, and an interactive game application. The OS runs directly on Raspberry Pi hardware without any underlying operating system.

## 🎯 Assignment Objectives

This group project demonstrates advanced embedded systems concepts including:
- Operating system kernel development
- Hardware abstraction and driver implementation  
- Peripheral interfacing and communication protocols
- Real-time graphics and multimedia processing
- User interface and input handling systems

## 🚀 Features

### Task 1: Welcome Message & Command Line Interpreter (~30%)
- **ASCII Art Welcome Screen**: Custom branded boot message
- **Full-Featured CLI** with OS prompt (MyOS>)
- **Command Implementation**:
  - `help` - Command documentation and usage
  - `clear` - Screen clearing functionality
  - `showinfo` - Board revision and MAC address display
  - `baudrate` - UART baud rate configuration (9600-115200)
  - `handshake` - CTS/RTS handshaking control
- **Advanced CLI Features**:
  - TAB auto-completion for commands
  - Command history navigation (_ for UP, + for DOWN)
  - Character deletion with bounds protection
  - User-friendly input handling

### Task 2: Image, Video & Text Display (~20%)
- **Custom Font Rendering**: Self-designed bitmap font system
- **Image Display**: Background images with ARGB32 support
- **Video Playback**: Frame-by-frame video display capability
- **Team Member Display**: Multi-colored text overlay on images
- **Graphics Engine**: Direct framebuffer manipulation

### Task 3: Game Application (~50%)
- **Interactive Gameplay**: Screen and keyboard-based game
- **Multi-Level Design**: At least 2 game stages/levels
- **Colorful Graphics**: Full-color game implementation
- **Terminal Integration**: 
  - Command acknowledgment system (ACK/NAK)
  - Real-time data logging
  - Game control via terminal input
- **Performance Monitoring**: Command counting and system telemetry

## 🛠 Technical Implementation

### Hardware Target
- **Primary**: Raspberry Pi (Real hardware testing required)
- **Development**: QEMU emulation for development phase

### Architecture
```
Bare Metal OS
├── Kernel Layer
│   ├── Boot sequence & initialization
│   ├── Memory management
│   └── Hardware abstraction layer
├── Driver Layer  
│   ├── UART communication
│   ├── Framebuffer graphics
│   ├── GPIO & input handling
│   └── System peripherals
├── Application Layer
│   ├── Command line interface
│   ├── Graphics & multimedia
│   └── Game application
└── Hardware Layer
    └── Raspberry Pi peripherals
```

## 🔧 Development Setup

### Prerequisites
- Cross-compilation toolchain for ARM
- QEMU for Raspberry Pi emulation
- SD card and Raspberry Pi hardware for testing

### Build System
```bash
# Setup and clean build
make setup

# Clean previous builds  
make clean

# Compile the OS kernel
make

# Flash to SD card (replace /dev/sdX with your SD card)
sudo dd if=kernel8.img of=/dev/sdX bs=1M conv=fsync
```

### QEMU Testing
```bash
# Run in QEMU emulator
qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio
```

## 📁 Project Structure
```
.
├── src/
│   ├── kernel/           # Core kernel implementation
│   ├── drivers/          # Hardware drivers
│   │   ├── uart.c       # Serial communication
│   │   ├── framebuffer.c # Graphics display
│   │   └── gpio.c       # General purpose I/O
│   ├── cli/             # Command line interface
│   ├── graphics/        # Image, video, font rendering
│   ├── game/            # Game application
│   └── utils/           # Common utilities
├── include/             # Header files
├── assets/              # Images, fonts, game resources
├── docs/                # Documentation and screenshots
├── boot/                # Boot loader and startup code
├── Makefile            # Build configuration
└── README.md           # This file
```

## 🎮 Usage

### Booting the OS
1. Flash `kernel8.img` to SD card
2. Insert SD card into Raspberry Pi
3. Connect via UART/Serial terminal
4. Power on - welcome message should appear

### CLI Commands
```bash
MyOS> help                    # Show all commands
MyOS> showinfo               # Display board info
MyOS> baudrate 115200        # Set UART speed
MyOS> clear                  # Clear screen
MyOS> game start             # Launch game application
```

### Game Controls
- Use keyboard input for game control
- Terminal displays game telemetry
- Commands acknowledged with ACK/NAK

## 🧪 Testing & Validation

### Development Phase
- QEMU emulation for rapid prototyping
- Unit testing of individual components
- Integration testing of system modules

### Hardware Validation  
- **Real Raspberry Pi testing required for submission**
- Serial communication verification
- Graphics output validation
- Performance benchmarking

## 📊 Assessment Criteria

### Technical Implementation (70%)
- **CLI Functionality**: Command parsing, history, auto-complete
- **Graphics System**: Image, video, text rendering
- **Game Application**: Complexity, user experience, terminal integration

### Code Quality (20%)
- Variable naming and coding style
- Resource usage optimization  
- Error handling and robustness

### Creativity & Innovation (10%)
- Original game concept (inspiration allowed, no direct copying)
- Creative use of graphics and multimedia
- Advanced features beyond basic requirements

## 📚 Supporting Resources

### Course Materials
- **Course**: EEET2490 - Embedded System: OS and Interfacing
- **Institution**: RMIT University
- **Semester**: 2025-2

### Development Tools
- [ASCII Art Generator](https://onlineasciitools.com/convert-text-to-ascii-art)
- [Image to ARGB32 Converter](https://javl.github.io/image2cpp/)
- [Raspberry Pi Board Versions](https://www.raspberrypi-spy.co.uk/2012/09/checking-your-raspberry-pi-board-version/)

### References
- [Bare Metal Programming Tutorial](https://github.com/bztsrc/raspi3-tutorial)
- [RPi4 OS Development](https://github.com/babbleberry/rpi4-osdev)
- [Scratch Game Development](https://scratch.mit.edu/)

## 👥 Team Members
- **billybean93** - [GitHub Profile](https://github.com/billybean93)
- [Add other team members]

## 📝 Documentation

### Report Requirements
- Background and technical explanation
- Implementation details with screenshots
- Success analysis and limitations discussion
- Evidence of successful hardware testing

### Presentation
- Maximum 20-minute video demonstration
- All team members must present
- Live demonstration on real hardware

## 🔗 Academic Integrity

This project is for educational purposes as part of EEET2490 coursework. While learning from existing resources is encouraged, direct copying of code from other students or complete game implementations is prohibited per course guidelines.

## 📄 License

Academic project - RMIT University EEET2490  
© 2025 - For educational use only

---

*Successfully tested on Raspberry Pi hardware - Ready for final submission* ✅
