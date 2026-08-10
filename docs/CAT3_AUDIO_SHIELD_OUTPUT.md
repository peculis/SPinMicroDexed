# CAT 3: Audio Shield output path

Date: 10 August 2026

## Objective

Validate the complete PJRC Audio Library and Audio Shield output path without
depending on MIDI, a DX7 voice, SysEx or a microSD card.

## Test signal

CAT 3 temporarily injects two sine oscillators into the otherwise-unused input
2 of the existing left/right master mixers. The normal volume, stereo routing,
I2S output and SGTL5000 codec remain in the path. USB Audio receives the same
signal but is not an acceptance requirement until CAT 7.

The nonblocking pattern repeats continuously:

1. I2S channel 0: 440 Hz for one second.
2. Silence for 250 ms.
3. I2S channel 1: 880 Hz for one second.
4. Silence for 250 ms.

The firmware fixes the CAT 3 master volume at 50% in RAM and uses oscillator
amplitude 0.12. It does not write that temporary setting to EEPROM. Begin with
headphones off your ears and external amplification turned down, then increase
carefully.

## Hardware test

1. Compile and upload with the established Teensy 4.0 CAT settings.
2. Confirm Serial Monitor alternates `[CAT3-AUDIO]` messages matching the four
   states above while `[CAT2]` health reports continue.
3. Connect stereo headphones to the Audio Shield headphone jack. Confirm a low
   tone in exactly one side, silence, then a higher tone in exactly the other
   side. Record which physical side corresponds to I2S channel 0 and channel 1.
4. If suitable powered speakers, mixer, or an oscilloscope are available, test
   the shield line-out pads separately. Do not connect passive headphones to
   line out and do not short either line output to ground.
5. Listen for clicks beyond the expected tone transitions, distortion, hum,
   channel bleed, or resets. Confirm diagnostics remain stable for one minute.

CAT 3 passes when both headphone channels are clean and distinct, codec status
remains OK, and no crash or resource growth occurs. Line-out observation is
recorded separately and may be deferred if no safe line-level receiver exists.
