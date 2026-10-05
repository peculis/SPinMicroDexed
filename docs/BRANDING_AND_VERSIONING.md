# SPinMicroDexed branding and version display

Date: 11 August 2026

The LCD boot splash displays `SPinMicroDexed` and `Initializing...`. It remains
on screen throughout setup and is replaced only when the initial voice screen
has been completely prepared immediately before the firmware becomes playable.
Its configured minimum visible duration is 2500 ms, measured from the moment
the text is drawn. Initialization proceeds during that interval, and firmware
adds only any remaining time before switching screens.

The System Info function has three pages selected by rotating the right
encoder:

1. SPinMicroDexed semantic version and build number.
2. The retained upstream MicroDexed base version and build configuration.
3. SD-card/storage status.

The initial development identity is `v0.1.0-dev B001`. The SPinMicroDexed
version/build constants are deliberately separate from upstream MicroDexed's
`VERSION` so attribution and diagnostic provenance remain unambiguous.

## USB identity

The project-level Teensy USB product descriptor is `SPinMicroDexed`. This is
the device name presented by the composite Serial + MIDI + Audio firmware to
macOS applications such as Audio MIDI Setup and Logic Pro. The descriptor is
defined in `name.c`; the Teensy core's vendor ID, product ID and unique serial
number are not changed.

After installing firmware with a changed descriptor, disconnect and reconnect
the Teensy and restart applications that were already using the old device
name. macOS or Logic may retain the former name until the USB device is
re-enumerated.

## Hardware acceptance

The branded splash, clean transition to the voice screen and 2500 ms minimum
duration passed on the target LCD1602 hardware. The System Info implementation
remains the basis for the next combined HMI/storage test.

On 5 October 2026, the next candidate identity is `0.1.0-rc1`, build 2. It retains
one engine and disabled optional effects. A final public release tag is pending
CAT 14 bench acceptance and the chosen GitHub destination.
