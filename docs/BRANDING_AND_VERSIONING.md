# SPinMicroDexed branding and version display

Date: 11 August 2026

The LCD boot splash displays `SPinMicroDexed` and `Initializing...`. It remains
on screen throughout setup and is replaced only when the initial voice screen
has been completely prepared immediately before the firmware becomes playable.

The System Info function has three pages selected by rotating the right
encoder:

1. SPinMicroDexed semantic version and build number.
2. The retained upstream MicroDexed base version and build configuration.
3. SD-card/storage status.

The initial development identity is `v0.1.0-dev B001`. The SPinMicroDexed
version/build constants are deliberately separate from upstream MicroDexed's
`VERSION` so attribution and diagnostic provenance remain unambiguous.
