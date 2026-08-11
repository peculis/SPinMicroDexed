# SPinMicroDexed Compile And Test process

## Definition

**CAT** means **Compile And Test**.

A CAT is a controlled development stage in which a limited set of changes is
compiled, deployed to the target hardware, tested against stated acceptance
criteria, and recorded before further changes are introduced.

## Workflow

Each CAT follows the same basic sequence:

1. Define one narrow objective and its expected result.
2. Inspect the relevant existing implementation before changing it.
3. Make limited, reviewable changes.
4. Compile using the documented Teensy and USB configuration.
5. Resolve compilation errors before hardware deployment.
6. Upload to the target SPinSynth-T-LCD hardware.
7. Perform the stated electrical, control, MIDI, audio or HMI tests.
8. Collect objective evidence such as Serial telemetry, event counts, crash
   reports, CPU/memory measurements and audio observations.
9. Classify the CAT as passed, failed or requiring refinement.
10. Record the result and preserve the accepted Git baseline before proceeding.

## Principles

- Change one meaningful variable at a time whenever practical.
- Treat compilation as necessary but not sufficient evidence.
- Do not hide failed or ambiguous tests; use them to design smaller isolation
  tests.
- Keep diagnostic activity out of real-time callbacks unless its timing impact
  is explicitly part of the test.
- Compare subjective musical observations with objective MIDI and audio
  telemetry.
- Preserve a known-working commit before beginning the next CAT.
- Defer unrelated improvements so each result remains attributable to the
  intended change.

## Acceptance records

Numbered CAT records are stored in `docs/CAT*.md`. Git commits preserve the
corresponding source changes and accepted baselines. The developing narrative
is maintained in `docs/PROJECT_HISTORY_DRAFT.md`.

