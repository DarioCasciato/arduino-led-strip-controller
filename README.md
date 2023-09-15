# Arduino LED Strip Controller

## Overview

This Arduino LED Strip Controller is a versatile project that allows you to control addressable LED strips with different lighting modes and effects. The controller is equipped with a button for changing modes, a potentiometer for adjusting brightness, and a function potentiometer that adds unique functionality to each lighting mode.

## Features

- **Multi-mode Lighting:** The controller offers various lighting modes, including but not limited to color cycling, static colors, and dynamic effects.
- **Brightness Control:** Adjust the brightness of the LED strip using the built-in potentiometer.
- **Functionality Potentiometer:** The function potentiometer adds an extra layer of customization to each lighting mode, enabling different behaviors depending on the selected mode.

## File Structure

Here's a brief overview of the project's file structure:

- src/

  - configurations.h
  - hardware.cpp
  - hardware.h
  - main.cpp
  - state.cpp
  - state.h
  - ColorUtilities/
    - color_utilities.cpp
    - color_utilities.h
  - modes/
    - mode_audio.cpp
    - mode_colorpick.cpp
    - mode_cycle.cpp
    - mode_fire.cpp
    - mode_rainbow.cpp
    - mode_white.cpp
    - strip_modes.h


## Getting Started

To use this LED strip controller, follow these steps:

1. **Setup:** Connect your addressable LED strip to the controller and ensure that the power supply is adequate.
2. **Upload Code:** Upload the provided Arduino code to your Arduino board.
3. **Adjust Brightness:** Use the brightness potentiometer to set the desired brightness level for your LED strip.
4. **Explore Modes:** Press the button to cycle through different lighting modes and explore the unique functionalities enabled by the function potentiometer.
5. **Enjoy:** Sit back and enjoy the dynamic lighting effects created by your Arduino LED Strip Controller!

## Contributing

If you want to contribute to this project or have suggestions for improvements, please feel free to submit issues or pull requests.

## License

This project is open-source and available under the [MIT License](LICENSE).
