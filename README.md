# Arduino Firmware Template Repository

This is a template repository for developing firmware using Object-Oriented Programming (OOP) principles with Arduino and PlatformIO. It provides a structured starting point for creating Arduino projects that follow best practices for modularity, maintainability, and scalability.

## Features

- Object-Oriented Programming approach for organizing code into reusable and modular classes.
- PlatformIO integration for easy project management, dependency management, and build system configuration.
- Basic directory structure to organize your code effectively.

## Prerequisites

To use this template repository, you need to have the following installed:

- [Arduino IDE](https://www.arduino.cc/en/software)
- [PlatformIO Core](https://platformio.org/platformio-ide)
- [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation.html)
- [Git](https://git-scm.com/)

## Getting Started

To create a new firmware project using this template repository, follow these steps:

1. Click on the "Use this template" button at the top of the repository page to create a new repository based on this template.
2. Clone the newly created repository to your local machine using Git.
   ```
   git clone https://github.com/your-username/your-project.git
   ```
3. Change into the project directory.
   ```
   cd your-project
   ```
4. Open the project in your preferred integrated development environment (i personally use VS-Code).
5. Build and upload the firmware to your Arduino board using PlatformIO.

## Directory Structure

The directory structure of this template repository is as follows:

```
│
├───src
│       configurations.h
│       hardware.cpp
│       hardware.h
│       main.cpp
│       state.cpp
│       state.h
│
└───utils
    ├───Arduino
    ├───Buzzer
    ├───EdgeDetection
    ├───LED
    ├───LED_RGB
    ├───Potentiometer
    ├───Security
    └───Timer
```

- **`src/`**: This directory contains the main firmware code. You can create additional source files as needed, such as configuration files (`configurations.h`) or modules (`hardware.cpp`, `hardware.h`, etc.).
- **`utils/`**: contains useful libraries
- **`README.md`**: This file provides an overview and instructions for the template repository.

## Removing Example Code

If you prefer to start with a clean slate and remove the example code provided in this template, you can use the "remove-template" branch. This branch has already removed the example code and is ready for you to begin your project without any pre-existing code.

### Instructions:

1. Switch to the "remove-template" branch using the following command:

   ```
   git checkout remove-template
   ```
2. Merge the "remove-template" branch into the "main" branch:

   ```
   git merge remove-template
   ```

   **Note:** Before merging, make sure you have committed or stashed any changes you made to the "main" branch to avoid conflicts.
3. Once the merge is complete, the "main" branch will now contain only the directory structure and the README.md file.
4. You can now start building your firmware project from this clean template.

### Additional Information:

The "remove-template" branch was created to provide users with a minimalist starting point, eliminating any example code that might not be relevant to your specific project requirements.

## Contributing

Contributions to improve this template repository are always welcome. If you find any issues or have suggestions for enhancements, please open an issue or submit a pull request.

## License

This template repository is licensed under the [MIT License](LICENSE). Feel free to use it as a starting point for your own firmware projects.

## Resources

- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [Arduino Forum](https://forum.arduino.cc/)
- [PlatformIO Community](https://community.platformio.org/)
