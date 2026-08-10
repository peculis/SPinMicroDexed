# SPinSynth-T-LCD Hardware

## Validated configuration — 5 August 2026

The current SPinSynth-T-LCD hardware baseline is:

- Teensy 4.0 running at 600 MHz.
- PJRC Audio Shield Rev D2, stacked directly beneath the Teensy.
- KEYES DC 3.3 V LCD1602 I2C module at address `0x27`.
- Two rotary encoders with push-buttons for the local HMI.
- MIDI-DIN input.
- USB configured as Serial + MIDI + Audio.
- Simultaneous USB Audio and Audio Shield output.
- Audio Shield headphone volume initialized to `0.85`.

The KEYES LCD is powered from 3.3 V and connects directly to the Teensy I2C bus:

| Signal | Teensy 4.0 | Electrical level |
| --- | --- | --- |
| SDA | Pin 18 | 3.3 V |
| SCL | Pin 19 | 3.3 V |
| VCC | 3.3 V | 3.3 V |
| GND | GND | 0 V |

The LCD module has no I2C pull-up resistors. The Audio Shield supplies 2.2 kΩ pull-ups on SDA and SCL to 3.3 V, so no additional pull-ups or logic-level shifter are required. With SPinSynth and the HMI operating, both lines measured approximately 3.25 V in their idle-high state and the 3.3 V supply remained steady.

This configuration completed a continuous test exceeding five hours. MIDI-DIN, LCD/HMI, heartbeat, USB Audio, and Audio Shield audio remained operational. CPU temperature was 54.3 °C.

## LCD and level-shifter history

The Teensy 4.0 I/O pins are not 5 V tolerant. The earlier LCD1602 assembly was a 5 V I2C device, so a bidirectional level shifter was introduced between the Teensy and LCD.

An HW-024 V0.1 two-channel I2C level-shifter module was initially wired according to incomplete and conflicting identification information. Investigation established that this particular board's printed A/B arrangement was opposite the assumption used during assembly: its A side was the high-voltage side and its B side was the low-voltage side. The system sometimes appeared to work even with the sides reversed, but this was not a valid or safe operating condition. Back-powering through SDA and SCL could leave the nominal 5 V LCD rail near 4.5 V even when its explicit 5 V connection was removed.

After the HW-024 wiring was corrected, intermittent audio stoppages, elevated Teensy temperatures, and unreliable LCD operation continued. The HW-024 was therefore removed from the validated configuration. A conventional BSS138 bidirectional level-shifter module was installed and worked correctly when wired with LV at 3.3 V and HV at 5 V. This established that the Teensy, firmware, and LCD I2C traffic could operate reliably with a correctly implemented level translator.

The final solution was to replace the 5 V LCD with a KEYES LCD1602 module specified for native 3.3 V operation. This eliminated the voltage-domain crossing entirely. Removing the level shifter simplified the wiring, removed the back-powering path, and produced the cleanest electrical configuration tested.

## Audio Shield history

Several earlier Audio Shield boards produced no analog output even though the Teensy generated the expected I2S clocks and data. In one investigated board, the 3.3 V rail collapsed to approximately 1.62 V when the Shield was installed, while both Teensy 4.0 boards operated normally without it. That board had not been used on the SPinSynth protoboard and was treated as defective rather than evidence that the project had damaged it.

The current PJRC Audio Shield Rev D2 was installed only after checking the assembly and supply behavior. The SGTL5000 responds at I2C address `0x0A`; its enable, headphone routing, volume, and unmute operations succeed. The synth is routed simultaneously to USB Audio and the Shield's stereo I2S output. Headphone volume was progressively validated at 0.25, 0.50, 0.75, and finally 0.85.

## USB stability history

During early tests, Logic Pro intermittently lost USB Audio while Shield audio, MIDI-DIN, the LCD/HMI, and heartbeat continued. macOS Audio MIDI Setup also temporarily lost the Teensy device, proving that the event was a complete USB data disconnection rather than a synth crash or Logic-only failure.

Replacing the micro-USB cable and cleaning the connectors with isopropyl alcohol eliminated the dropouts during the subsequent five-hour test. This result identifies the previous instability as a cable/contact problem. The USB cable should be mechanically supported near the Teensy so its weight does not stress the micro-USB connector.

## Software and test settings

Compile for Teensy 4.0 with an Arduino/Teensyduino USB type that includes Serial, MIDI, and Audio. The production build sends the same synth stream to both USB Audio and the Audio Shield. Temporary continuous-tone and conditional USB-only test paths were removed after hardware validation.

The following lightweight operational diagnostics remain intentionally enabled:

- Teensy `CrashReport` at startup.
- Built-in LED heartbeat.
- Uptime and CPU temperature output every five seconds.
- LCD I2C status at startup; status `0` indicates success.
- SGTL5000 I2C status at startup; status `0` indicates success.
# SPinSynth-T-LCD Hardware

## Validated configuration — 5 August 2026

The current SPinSynth-T-LCD hardware baseline is:

- Teensy 4.0 running at 600 MHz.
- PJRC Audio Shield Rev D2, stacked directly beneath the Teensy.
- KEYES DC 3.3 V LCD1602 I2C module at address `0x27`.
- Two rotary encoders with push-buttons for the local HMI.
- MIDI-DIN input.
- USB configured as Serial + MIDI + Audio.
- Simultaneous USB Audio and Audio Shield output.
- Audio Shield headphone volume initialized to `0.85`.

The KEYES LCD is powered from 3.3 V and connects directly to the Teensy I2C bus:

| Signal | Teensy 4.0 | Electrical level |
| --- | --- | --- |
| SDA | Pin 18 | 3.3 V |
| SCL | Pin 19 | 3.3 V |
| VCC | 3.3 V | 3.3 V |
| GND | GND | 0 V |

The LCD module has no I2C pull-up resistors. The Audio Shield supplies 2.2 kΩ pull-ups on SDA and SCL to 3.3 V, so no additional pull-ups or logic-level shifter are required. With SPinSynth and the HMI operating, both lines measured approximately 3.25 V in their idle-high state and the 3.3 V supply remained steady.

This configuration completed a continuous test exceeding five hours. MIDI-DIN, LCD/HMI, heartbeat, USB Audio, and Audio Shield audio remained operational. CPU temperature was 54.3 °C.

## LCD and level-shifter history

The Teensy 4.0 I/O pins are not 5 V tolerant. The earlier LCD1602 assembly was a 5 V I2C device, so a bidirectional level shifter was introduced between the Teensy and LCD.

An HW-024 V0.1 two-channel I2C level-shifter module was initially wired according to incomplete and conflicting identification information. Investigation established that this particular board's printed A/B arrangement was opposite the assumption used during assembly: its A side was the high-voltage side and its B side was the low-voltage side. The system sometimes appeared to work even with the sides reversed, but this was not a valid or safe operating condition. Back-powering through SDA and SCL could leave the nominal 5 V LCD rail near 4.5 V even when its explicit 5 V connection was removed.

After the HW-024 wiring was corrected, intermittent audio stoppages, elevated Teensy temperatures, and unreliable LCD operation continued. The HW-024 was therefore removed from the validated configuration. A conventional BSS138 bidirectional level-shifter module was installed and worked correctly when wired with LV at 3.3 V and HV at 5 V. This established that the Teensy, firmware, and LCD I2C traffic could operate reliably with a correctly implemented level translator.

The final solution was to replace the 5 V LCD with a KEYES LCD1602 module specified for native 3.3 V operation. This eliminated the voltage-domain crossing entirely. Removing the level shifter simplified the wiring, removed the back-powering path, and produced the cleanest electrical configuration tested.

## Audio Shield history

Several earlier Audio Shield boards produced no analog output even though the Teensy generated the expected I2S clocks and data. In one investigated board, the 3.3 V rail collapsed to approximately 1.62 V when the Shield was installed, while both Teensy 4.0 boards operated normally without it. That board had not been used on the SPinSynth protoboard and was treated as defective rather than evidence that the project had damaged it.

The current PJRC Audio Shield Rev D2 was installed only after checking the assembly and supply behavior. The SGTL5000 responds at I2C address `0x0A`; its enable, headphone routing, volume, and unmute operations succeed. The synth is routed simultaneously to USB Audio and the Shield's stereo I2S output. Headphone volume was progressively validated at 0.25, 0.50, 0.75, and finally 0.85.

## USB stability history

During early tests, Logic Pro intermittently lost USB Audio while Shield audio, MIDI-DIN, the LCD/HMI, and heartbeat continued. macOS Audio MIDI Setup also temporarily lost the Teensy device, proving that the event was a complete USB data disconnection rather than a synth crash or Logic-only failure.

Replacing the micro-USB cable and cleaning the connectors with isopropyl alcohol eliminated the dropouts during the subsequent five-hour test. This result identifies the previous instability as a cable/contact problem. The USB cable should be mechanically supported near the Teensy so its weight does not stress the micro-USB connector.

## Software and test settings

Compile for Teensy 4.0 with an Arduino/Teensyduino USB type that includes Serial, MIDI, and Audio. The production build sends the same synth stream to both USB Audio and the Audio Shield. Temporary continuous-tone and conditional USB-only test paths were removed after hardware validation.

The following lightweight operational diagnostics remain intentionally enabled:

- Teensy `CrashReport` at startup.
- Built-in LED heartbeat.
- Uptime and CPU temperature output every five seconds.
- LCD I2C status at startup; status `0` indicates success.
- SGTL5000 I2C status at startup; status `0` indicates success.
# SPinSynth-T-LCD Hardware

## Validated configuration — 5 August 2026

The current SPinSynth-T-LCD hardware baseline is:

- Teensy 4.0 running at 600 MHz.
- PJRC Audio Shield Rev D2, stacked directly beneath the Teensy.
- KEYES DC 3.3 V LCD1602 I2C module at address `0x27`.
- Two rotary encoders with push-buttons for the local HMI.
- MIDI-DIN input.
- USB configured as Serial + MIDI + Audio.
- Simultaneous USB Audio and Audio Shield output.
- Audio Shield headphone volume initialized to `0.85`.

The KEYES LCD is powered from 3.3 V and connects directly to the Teensy I2C bus:

| Signal | Teensy 4.0 | Electrical level |
| --- | --- | --- |
| SDA | Pin 18 | 3.3 V |
| SCL | Pin 19 | 3.3 V |
| VCC | 3.3 V | 3.3 V |
| GND | GND | 0 V |

The LCD module has no I2C pull-up resistors. The Audio Shield supplies 2.2 kΩ pull-ups on SDA and SCL to 3.3 V, so no additional pull-ups or logic-level shifter are required. With SPinSynth and the HMI operating, both lines measured approximately 3.25 V in their idle-high state and the 3.3 V supply remained steady.

This configuration completed a continuous test exceeding five hours. MIDI-DIN, LCD/HMI, heartbeat, USB Audio, and Audio Shield audio remained operational. CPU temperature was 54.3 °C.

## LCD and level-shifter history

The Teensy 4.0 I/O pins are not 5 V tolerant. The earlier LCD1602 assembly was a 5 V I2C device, so a bidirectional level shifter was introduced between the Teensy and LCD.

An HW-024 V0.1 two-channel I2C level-shifter module was initially wired according to incomplete and conflicting identification information. Investigation established that this particular board's printed A/B arrangement was opposite the assumption used during assembly: its A side was the high-voltage side and its B side was the low-voltage side. The system sometimes appeared to work even with the sides reversed, but this was not a valid or safe operating condition. Back-powering through SDA and SCL could leave the nominal 5 V LCD rail near 4.5 V even when its explicit 5 V connection was removed.

After the HW-024 wiring was corrected, intermittent audio stoppages, elevated Teensy temperatures, and unreliable LCD operation continued. The HW-024 was therefore removed from the validated configuration. A conventional BSS138 bidirectional level-shifter module was installed and worked correctly when wired with LV at 3.3 V and HV at 5 V. This established that the Teensy, firmware, and LCD I2C traffic could operate reliably with a correctly implemented level translator.

The final solution was to replace the 5 V LCD with a KEYES LCD1602 module specified for native 3.3 V operation. This eliminated the voltage-domain crossing entirely. Removing the level shifter simplified the wiring, removed the back-powering path, and produced the cleanest electrical configuration tested.

## Audio Shield history

Several earlier Audio Shield boards produced no analog output even though the Teensy generated the expected I2S clocks and data. In one investigated board, the 3.3 V rail collapsed to approximately 1.62 V when the Shield was installed, while both Teensy 4.0 boards operated normally without it. That board had not been used on the SPinSynth protoboard and was treated as defective rather than evidence that the project had damaged it.

The current PJRC Audio Shield Rev D2 was installed only after checking the assembly and supply behavior. The SGTL5000 responds at I2C address `0x0A`; its enable, headphone routing, volume, and unmute operations succeed. The synth is routed simultaneously to USB Audio and the Shield's stereo I2S output. Headphone volume was progressively validated at 0.25, 0.50, 0.75, and finally 0.85.

## USB stability history

During early tests, Logic Pro intermittently lost USB Audio while Shield audio, MIDI-DIN, the LCD/HMI, and heartbeat continued. macOS Audio MIDI Setup also temporarily lost the Teensy device, proving that the event was a complete USB data disconnection rather than a synth crash or Logic-only failure.

Replacing the micro-USB cable and cleaning the connectors with isopropyl alcohol eliminated the dropouts during the subsequent five-hour test. This result identifies the previous instability as a cable/contact problem. The USB cable should be mechanically supported near the Teensy so its weight does not stress the micro-USB connector.

## Software and test settings

Compile for Teensy 4.0 with an Arduino/Teensyduino USB type that includes Serial, MIDI, and Audio. The production build sends the same synth stream to both USB Audio and the Audio Shield. Temporary continuous-tone and conditional USB-only test paths were removed after hardware validation.

The following lightweight operational diagnostics remain intentionally enabled:

- Teensy `CrashReport` at startup.
- Built-in LED heartbeat.
- Uptime and CPU temperature output every five seconds.
- LCD I2C status at startup; status `0` indicates success.
- SGTL5000 I2C status at startup; status `0` indicates success.
