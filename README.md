# trimControl

**trimControl** is an Arduino based controller for flight simulators. Technically working as a standard joystick, but intended to be used as an auxillary controller for whatever one would like to control with it. In IL2 for instance, the trim axis - and toggling with one of the mode switches, control Gyro sights and/or level bombing settings. The buttons can be programmed in game to whatever as well, with it's own toggle switch to change between bank 0 and 1 for them as well. Rotary encoder to control radios and such, with button press.

## Build

Intended target is a Sparkfun Pro Micro Arduino clone and developed using PlatformIO. Should work fine with standard Arduino IDE as well. Ensure target MCU is set to Sparkfun Pro Micro and that the libraries this project uses is available for your dev environment. 

**Contact:**

location   | name/handle
-----------|-------------
github:    | rDybing
X:         | dybingRoy
Linked In: | Roy Dybing

---

## Releases

- Version format: [major release].[new feature(s)].[bugfix patch-version]: Date

### v.0.0.1: 29th of September 2024

- Day 1 work very much in process...

### v.0.0.2: 11th of October 2024

- Working more or less with the hardware

---

## To do

- Have Button LEDs latch
- Mode switch
  - B0 + B1 for 3 seconds: Toggle jitter to pot bank 1 for IL2 gyro sights purposes
  - B0 + B2 for 3 seconds: Zero out LEDs and toggle button LED latching
  - B0 + B3 for 3 seconds: TBA
- Set as a Joystick device

---

## Known issues

- Toggles not initializing properly to set mode on startup
- Button LEDs not lighting up

## License: MIT

**Copyright © 2019 Roy Dybing** 

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions: The above copyright notice and this 
permission notice shall be included in all copies or substantial portions of 
the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.

---

ʕ◔ϖ◔ʔ