# GeckoPad

intro

![Finished Version](/Pictures/Finish.jpg)

inspiration

other one handed keyboard

Features:

- Glowing keys with configurable light patterns
- Bluetooth and USB
- Battery powered with USB recharging
- Auto deep sleep after 5 minutes

# Design Goals!

test

# Parts

All parts were ordered from [Little Bird Electronics](https://littlebirdelectronics.com.au/) in Australian. Checkout [Adafruit](https://www.adafruit.com/) for internationally shipped parts.

| Part | Qty | Price | Total |
| --- | --- | --- | --- |
| [Relegendable Plastic Keycaps for MX Compatible Switches 10 pack](https://littlebirdelectronics.com.au/products/relegendable-plastic-keycaps-for-mx-compatible-switches-10-pack) | 3   | AUD$7.41 | AUD$22.23 |
| [NeoKey 5x6 Ortho Snap-Apart Mechanical Key Switches w/ NeoPixel - For MX Compatible Switches](https://littlebirdelectronics.com.au/products/neokey-5x6-ortho-snap-apart-mechanical-key-switches-w-neopixel-for-mx-compatible-switches) | 1   | AUD$48.00 | AUD$48.00 |
| [Kailh Mechanical Key Switches - Tactile Brown - 10 pack - Cherry MX Brown Compatible](https://littlebirdelectronics.com.au/products/kailh-mechanical-key-switches-tactile-brown-10-pack-cherry-mx-brown-compatible) | 3   | AUD$12.00 | AUD$36.00 |
| [40 pin 30CM Jumper wire M/M](https://littlebirdelectronics.com.au/products/40-pin-30cm-jumper-wire-m-m) | 1   | AUD$3.30 | AUD$3.30 |
| [Adafruit Feather M0 Bluefruit LE](https://littlebirdelectronics.com.au/products/adafruit-feather-m0-bluefruit-le) | 1   | AUD$67.58 | AUD$67.58 |
| 1,000mAh lipo battery (already had) | 1   | AUD$0.00 | AUD$0.00 |
|     |     |     | AUD$177.11 |

# Design of plastic body

The original design had mounting clips so that the top and bottom bodies would snap into place. However the tolerances of the groovy to ensure they line up proved to be enough to hold the parts together securely while simplifying the design. All of the designing was done in AutoDesk Fusion 360 and the design files are included along with STL exports so new versions can be 3d printed.

The bottom case was design so the Feather M0 would simply snap into place with extra reinforcement to handle the load from the USB plug being inserted and removed regularly. The battery is held in place securely with 4 walls and is held down by the bottom of the NeoKey PCB. The NeoKey Screws into place tightly with M3 8mm screws. There is extra length allowed for longer screws if required.

![Case Bottom](/Pictures/CaseBottom.png)

The top of the 3d printed casing was designed to simply snap onto the top of the bottom body and hide the PCB shown between the keys. The top edges were filleted to ensure the is no rough edges near the users hand. Since all of the components are attached to the bottom body, there is very little load to the top face plate.

![Case Top](/Pictures/CaseTop.png)

# Keyboard Layout Design

This was heavily based on the FrogPad keyboard layout which is no longer available.

![Keyboard Layout](/Pictures/KeyboardLayout.png)

# Firmware

The Arduino firmware files are included in the ArduinoCode directory and contain a fully working keyboard firmware. The firmware is designed to work with a Adafruit Feather M0 Bluefruit LE chip.

Borrowed chuncks of code from https://github.com/clc/chorder

# Build

test

# Testing

Battery was found to last a few days with the LEDs rotating colours.

# Typing Speed / WPM

It was said with the FrogPad that some one could learn to type at 40WPM in 8 to 15 hours. I created a HTML webpage to learn the keys originally then quickly moved to using typing.com to improve my typing speed once the keys were learnt. The KeyLearner can be run from any PC by double clicking on the index.html file or uploaded to a webserver. Learning was done on an iPhone, iPad, Mac and Windows PC. My average WPM on a standard keyboard is approximately 75 WPM when starting to learn to type on the GeckoPad.

| Day | Where | Time | WPM |
| --- | --- | --- | --- |
| Day 1 | KeyLearner | 15min |     |
| Day 2 |     |     |     |
| Day 3 |     |     |     |
| Day 4 |     |     |     |

# Future Improvements

The keyboard high is 35mm which is 5mm over the ISO standard for keyboards. While I could probably reduce the height another 5mm, in a future design I would move the battery and Arduino out from under the keyboard PCB to drastically reduce the key height. Additionally, in a future version, I would replace the lip battery with an easily replaceable 3,000mAh 18650 cell common used in vapes. This would triple the battery life and make it easy to replace when the battery degrades.