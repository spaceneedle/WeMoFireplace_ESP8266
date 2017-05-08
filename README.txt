WeMoFireplace

Introduction

I have a gas fireplace with an extremely simple remote. You have to hold down a button ("on" or "off"),
which drives a motor forward or back. This eventually will open up the gas valve and light from a pilot light.
Holding down the remote in the "off" position cuts out the gas. A built in millivolt thermocouple prevents
gas from leaking if the pilot light is out. The fireplace has a lot of safeguards, and an unencrypted 315MHz
remote is nothing more than a gloried way to automatically turn the knob so you don't have to put your hands in there.

Instead of all this button holding nonsense, I decided to automate it and add a few features. It appears to
Amazon Echo as a WeMo device, which responds to "on" and "off" commands.

You could easily adapt this project to any sort of thingy that needs to drive a motor. 

Features

- Captive portal allows easy configuration (no need to modify arduino scripts)
- Saves configuration to device non-volitle memory
- Uses FauxMo to appear as a WeMo device to Amazon Echo
- Appears as "fireplace" and responds to "on" and "off" commands
- Automatically turns off fireplace after fire_timeout (default is 2 hours)
- Crude handling of arduino timer rollover (just shuts fire off), happens every several weeks

Requirements

Hardware

ESP8266 with H-Bridge controller board

Commission-free link to Amazon product: https://www.amazon.com/KOOKYE-ESP8266-NodeMcu-ESP-12E-Expansion/dp/B01C6MR62E

(Others will probably work, but its a great cheap option for motor controlling with ESP8266)

Libraries required

fauxmoESP library		https://bitbucket.org/xoseperez/fauxmoesp
WiFiManager			https://github.com/tzapu/WiFiManager
ESP8266 libraries		https://github.com/esp8266/Arduino
Ticker				https://github.com/esp8266/Arduino/tree/master/libraries/Ticker

