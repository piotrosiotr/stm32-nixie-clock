# stm32-nixie-clock

This is code for nice minimalistic nixie clock running on stm32f103c8t6 bluepill board.

# What this clock can do?

This clock doesn't have any annoying alarms, LED backlights, brightness control, etc. But it can show time quite nice, correct the time with one button in very intuitive way if needed and sometimes it can glitch (added just for fun).

# Features:

- powered from usb;
- uses embedded stm32 RTC;
- uses one touch button for time editing.

# How to build?

It's not as easy as Arduino sketch, but you can find nice instructions [here](https://github.com/piotrosiotr/stm32-vscode-blinky). Also if you don't want to build it yourself, you can use prebuilt firmware from here.

# Pinout:

- **PA4** - nixie anode 0;
- **PA5** - nixie anode 1;
- **PA6** - nixie anode 2;
- **PA7** - nixie anode 3;
- **PB0** - decoder input 0;
- **PB1** - decoder input 1;
- **PB10** - decoder input 2;
- **PB11** - decoder input 3;
- **PA11** - PWM for dcdc converter;
- **PA2** - touch button.

# Hardware:
Coming soon :)

