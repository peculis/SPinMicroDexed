# CAT 12 — configuration serialization integrity

Date: 5 October 2026. Status: compiled and uploaded; host tests pass; bench acceptance pending.

## Isolated change

Fix configuration serialization before broader save-path work. The inherited
writer produces three header bytes, a payload, checksum and end byte: payload
length plus five bytes. The reader used file length minus six, omitting the last
payload byte in both checksum calculation and copying. It also lacked a bound
on the target configuration and callers ignored validation failure.

The corrected reader requires the exact expected struct size, checks the
complete payload checksum/framing before copying, and returns failure to callers.
The writer checks every write, seeks to the start, writes the checksum once even
in DEBUG builds, and verifies final file size. Longer existing files are rejected;
this does not yet implement safe replacement or power-loss recovery.

## Host verification

`python3 tools/test_config_storage.py` compiles the actual serializer functions
with an in-memory File adapter. It checks all payload bytes round trip (including
high-bit data and the final byte), corrupted headers/payload/checksum/end rejection,
no target mutation on validation failure, wrong size/type, and failed writes.
This supports but does not replace physical SD acceptance.

## Apply and test

Use a backed-up/spare card. Upload only after the CAT 12 build compiles.
Save a new voice-configuration slot with a distinctive MIDI channel, transpose,
and level. Change those settings, reload, and confirm restoration. Save a new
performance, change selection/settings, reload and confirm the saved state.
Power cycle and repeat. Check both DIN and USB MIDI and both audio destinations.
Do not yet accept overwrite, malformed-file, full-card, bank reception or
power-interruption behavior. The remaining save/UI warnings and bank-write
paths still require a separate bounded change.

CAT 12 compiled successfully for the documented Teensy 4.0 configuration and
was uploaded on 5 October 2026. Hardware save/reload acceptance is pending.

Ricardo subsequently confirmed successful USB MIDI and USB Audio testing on
5 October 2026. Configuration save/reload and power-cycle restoration have not
yet been reported. USB functional success does not itself accept CAT 12 storage.

## Voice configuration reload result — 5 October 2026

Ricardo saved voice configuration slot 0 and received Done. Transpose was
subsequently confirmed at +0, changed to +12, and slot 0 was loaded with Done.
Returning to Voice > Setup > Transpose showed +0. This confirms restoration of
that saved parameter in the running CAT 12 firmware. Power-cycle restoration,
other configuration fields and performance save/reload remain pending.

## Voice configuration reload after power cycle — 5 October 2026

Ricardo set Transpose to +12, exited the parameter page, powered the synth off
and on, then loaded Voice Config slot 0 with Done. Checking Transpose showed +0.
The saved slot therefore remained readable after the power cycle and restored
the tested parameter. This does not establish automatic startup restoration of
a performance. Other configuration fields, performance save/reload, overwrite
safety and endurance remain pending.

## Performance reload result — 5 October 2026

Ricardo saved performance slot 0 with Done, selected a different voice and
confirmed a different sound, then loaded performance slot 0. He confirmed the
original voice name and sound returned. This accepts that running-session
performance save/reload case. Performance reload after a power cycle, overwrite
and failure handling remain pending.

## Performance reload after power cycle — 5 October 2026

Ricardo selected a different voice, powered off and on, and loaded performance
slot 0. After a follow-up clarification, he confirmed that both the original
voice name and sound returned. Normal new-slot voice-configuration and
performance save/reload, including manual reload after restart, have therefore
passed these bench cases. This does not accept overwrite/failure handling,
all saved fields, automatic performance restoration, or endurance testing.
