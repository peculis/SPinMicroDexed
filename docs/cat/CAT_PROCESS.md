# SPinMicroDexed Compile, Apply, Test process

## Definition

**CAT** means **Compile, Apply, Test**.

A CAT is the mandatory acceptance activity performed after a limited change has
been designed and coded. The resulting firmware is **compiled**, **applied**
(uploaded and deployed) to the real target hardware, and **tested** against
stated acceptance criteria before that change is accepted or further changes
are introduced.

The complete engineering cycle therefore contains four practical activities:
change definition, coding, application and test. Names such as CCAT or C2AT
would enumerate more of those activities, but CAT is retained because it is
concise and because **Compile, Apply, Test** accurately names the acceptance
gate that follows implementation. A successful compile alone never constitutes
a passed CAT.

## Strategy and negotiated scope

CAT planning is part of the engineering work, not administrative preparation.
Before coding, Ricardo and Codex define and negotiate the number, order, content
and expected evidence of the proposed changes. Each change should be limited,
self-contained and attributable enough that its effect can be distinguished
from the previous accepted baseline.

The CAT plan is deliberately revisable. A bench result, crash report, timing
measurement or unexpected interaction may show that a planned CAT is too broad,
premature or based on an incorrect assumption. In that case the work can be
split into smaller isolation CATs, reordered, repeated with different
instrumentation, or deferred. Conversely, closely related checks may be
combined when doing so does not weaken attribution or safety.

This negotiated sequence of changes is fundamental to project success. It
controls risk, preserves a working baseline, prevents several hypotheses from
being confused in one build and gives Ricardo explicit authority to accept,
reject or request refinement of each result on the physical instrument.

## Workflow

Each CAT follows the same basic sequence:

1. Jointly define and agree one narrow change, its reason, boundaries, expected
   result and acceptance evidence.
2. Inspect the relevant existing implementation before coding it.
3. Make limited, reviewable source changes.
4. **Compile** using the documented Teensy and USB configuration.
5. Resolve compilation errors before hardware deployment.
6. **Apply** the build by uploading it to the target SPinSynth-T-LCD hardware.
7. **Test** the stated electrical, control, MIDI, audio or HMI behavior.
8. Collect objective evidence such as Serial telemetry, event counts, crash
   reports, CPU/memory measurements and audio observations.
9. Classify the CAT as passed, failed or requiring refinement.
10. Reassess the planned CAT sequence in light of the result; split, reorder,
    repeat, defer or refine later CATs when the evidence requires it.
11. Record the result and preserve the accepted Git baseline before proceeding.

## Principles

- Change one meaningful variable at a time whenever practical.
- Treat compilation as necessary but not sufficient evidence.
- Require a CAT after every material implemented change before considering that
  change accepted.
- Test the deployed firmware on the actual target; simulation, inspection and
  successful compilation support a CAT but do not replace bench evidence.
- Do not hide failed or ambiguous tests; use them to design smaller isolation
  tests.
- Keep diagnostic activity out of real-time callbacks unless its timing impact
  is explicitly part of the test.
- Compare subjective musical observations with objective MIDI and audio
  telemetry.
- Preserve a known-working commit before beginning the next CAT.
- Defer unrelated improvements so each result remains attributable to the
  intended change.
- Treat the CAT roadmap as controlled and negotiated, but not rigid: revise it
  when evidence demonstrates a safer or more informative sequence.

## Acceptance records

Numbered CAT records are stored in `docs/cat/CAT*.md`. Git commits preserve the
corresponding source changes and accepted baselines. The developing narrative
is maintained in `docs/PROJECT_HISTORY_DRAFT.md`.
