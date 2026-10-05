# CAT 7: Simultaneous USB Audio and Audio Shield output

Date: 10 August 2026

## Status

**PASS on target hardware.** USB MIDI triggered the built-in voice while audio
remained audible from the Audio Shield and was simultaneously received,
monitored through the selected audio interface/speakers, and recorded by Logic.
The initially tiny waveform was a low master-volume setting, not a USB Audio
failure.

With the master volume set to 100, repeated strong-chord peaks measured
0.394--0.418 on both channels (approximately -8.1 to -7.6 dBFS), providing a
healthy recording level with headroom and confirming balanced stereo output.
Peak Audio CPU remained 7.1%, Audio Memory peaked at 13 blocks, temperature was
approximately 54 degrees C, and the system remained stable beyond 41 minutes.

No additional firmware gain is required. The inherited `pow(value, 2.2)` master
volume curve explains the large level change at lower settings; its feel may be
reviewed later as a UI/gain-law improvement rather than an audio-path defect.

## Objective

Validate that synthesized audio is delivered simultaneously to the PJRC Audio
Shield over I2S and to the Mac through the Teensy USB Audio device, while USB
MIDI continues to trigger the synth reliably.

CAT 6 per-transport counters are disabled. Normal DIN and USB MIDI support
remain active. CAT 2 health reporting remains enabled.

## Signal-path verification

The existing audio graph sends the same post-volume stereo signal to both
`AudioOutputI2S` and `AudioOutputUSB`. CAT 7 reports the maximum output level
seen on each channel during every five-second interval:

```text
OutPeak(ch0/ch1)=0.123/0.123
```

Silence may read zero. Notes should produce nonzero values. This proves signal
exists at the branch feeding both outputs, but host selection, monitoring and
speaker routing must still be verified in Logic.

An upstream wiring error connected both peak analyzers to output channel 0.
CAT 7 corrects the second analyzer to output channel 1. This changes monitoring
only; it does not change either audio output.

## Test

1. Keep the DIN cable disconnected. Compile and upload using USB type `Serial +
   MIDI + Audio` and the established Teensy 4.0 settings.
2. Confirm `[CAT7-AUDIO]` appears and the Mac sees both the Teensy USB MIDI and
   two-channel USB Audio device.
3. Trigger notes over USB MIDI. Confirm the Audio Shield headphones continue to
   sound and both `OutPeak` values become nonzero.
4. In Logic, select the Teensy/MicroDexed USB Audio device as the audio input and
   the desired Mac interface or built-in output as Logic's output device.
5. Create a stereo audio track using input 1--2, enable input monitoring, and
   route the track to Stereo Out. Start with monitoring volume low.
6. Confirm the same performance is heard through the Audio Shield and through
   Logic's selected output. Record 20--30 seconds and confirm the waveform and
   playback are clean in both channels.
7. Play chords and repeated notes for at least five minutes. Confirm no USB
   disappearance, gaps, distortion, crash, or steadily increasing CPU/memory.

If Logic records a waveform but produces no speaker sound, the Teensy USB Audio
input is already working; inspect Logic input monitoring, track output, Stereo
Out and selected output device rather than changing firmware.

CAT 7 passes when Shield and USB audio operate concurrently, the recording is
clean and stable, and MIDI triggering causes no USB or processor fault.
