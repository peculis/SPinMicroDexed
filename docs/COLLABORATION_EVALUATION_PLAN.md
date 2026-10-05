# Human–AI Engineering Collaboration Evaluation Plan

Living document — 12 August 2026

## Purpose

SPinMicroDexed has a second fundamental objective beyond producing a complete
MicroDexed Teensy 4.0 instrument: it is a practical experiment in modifying
embedded real-time software through collaboration between an experienced
engineer, **Ricardo Peculis**, and a software-engineering AI platform,
**OpenAI Codex**.

The final project report will evaluate both the product and the collaboration.
It will ask not only whether the firmware works, but how the human–AI method
affected feature delivery, engineering quality, effort, traceability and the
ability to diagnose problems on real hardware.

## Roles in the collaboration

Ricardo contributes:

- project intent, priorities and completion criteria;
- engineering experience and critical judgement;
- ownership of the SPinSynth hardware and its history;
- negotiation and acceptance of each bounded change;
- physical assembly, wiring, measurement and instrument operation;
- compilation/upload actions when performed at the bench;
- MIDI, audio, HMI and musical listening tests;
- interpretation of usability and musical behavior; and
- correction of assumptions using observed physical evidence.

Codex contributes:

- repository and upstream-source inspection;
- architecture and dependency analysis;
- change proposals and risk decomposition;
- firmware implementation and source review;
- diagnostic instrumentation and fault localization;
- interpretation of logs, crash reports and measurements;
- Git history, bounded commits and comparison of baselines;
- technical and historical research; and
- continuous preparation and revision of engineering documentation.

The result is not attributable to either role in isolation. Code changes are
guided by Ricardo's objectives and accepted through his bench evidence; bench
tests are made more discriminating by Codex's source analysis and diagnostic
builds.

## Evaluation questions

The final evaluation will address:

1. **Features** — Was the full relevant MicroDexed Teensy 4.0 functionality,
   including two DX7 instances, implemented and accepted on SPinSynth hardware?
2. **Quality** — How stable, responsive, safe, maintainable and reproducible is
   the resulting instrument?
3. **Fidelity** — Were inherited DX7 synthesis semantics preserved, and were
   any intentional behavioral differences documented?
4. **Engineering effort** — How much human bench/design effort, AI-assisted
   analysis/coding effort and shared elapsed collaboration time were required?
5. **Method effectiveness** — Did negotiated, revisable CAT planning isolate
   causes and prevent regressions more effectively than broad changes would
   have?
6. **Documentation and traceability** — Can every important conclusion be
   traced to source, commit, CAT evidence or physical observation?
7. **Human–AI complementarity** — Which results depended primarily on human
   judgement, which on automated source reasoning, and which emerged only from
   the feedback loop between them?
8. **Limitations and rework** — Where did the AI make incorrect assumptions,
   where was human evidence incomplete, and how much work was repeated?
9. **Transferability** — Which practices and changes can benefit future
   SPinSynth projects or upstream MicroDexed?

## Evidence to retain

The evaluation will use:

- Git commits, timestamps and diffs;
- CAT plans, acceptance records and revisions;
- compile results and memory reports;
- Serial diagnostics and Teensy crash reports;
- CPU, loop-gap, MIDI-event and audio measurements;
- hardware measurements and photographs where relevant;
- Ricardo's listening and usability observations;
- rejected hypotheses and isolation builds, not only successful outcomes;
- final feature and regression matrices; and
- long-duration endurance results.

## Provisional retrospective effort estimate

Exact time was not recorded at the beginning of the project, so the figures
below are estimates and must not be presented as timesheet measurements.

The Git record provides a defensible lower-bound view:

- 40 accepted commits were created on 10–11 August 2026.
- The 10 August commit-active window ran from 13:54 to 18:23: approximately
  **4 hours 29 minutes**.
- The 11 August activity appears in two main windows, 08:32–09:20 and
  13:39–15:14: approximately **2 hours 23 minutes**.
- Those windows total approximately **6 hours 52 minutes of shared active
  collaboration elapsed time** through the last committed baseline.

This is a lower bound because it excludes discussion, physical preparation,
tests performed before or after commits, breaks inside or outside the windows,
and the uncommitted historical/documentation work on 12 August.

A cautious retrospective estimate through CAT 10 is:

| Effort category | Provisional estimate | Confidence and interpretation |
| --- | ---: | --- |
| Shared active collaboration elapsed time | 7–10 hours | Medium; anchored by commit windows but includes overlapping human/AI activity |
| Ricardo active engineering/bench effort | 6–9 hours | Low-to-medium; design decisions, uploads, wiring checks, playing, measurement and acceptance |
| Codex active engineering contribution | 5–8 hours equivalent wall-clock engagement | Low-to-medium; inspection, coding, diagnosis, research, Git and documentation, much of it concurrent with Ricardo |
| Combined non-overlapped labor-equivalent total | Not reliably reconstructable | Human and AI work overlap, and AI computation/response time is not directly equivalent to human labor |

The estimates should be refined from Ricardo's recollection, but they should
remain ranges. Claiming precise separate totals would create false accuracy.

## Prospective effort tracking

From the next CAT onward, each work session should record:

- session date and start/end time;
- breaks or long unattended test intervals separately;
- CATs planned, revised, passed or failed;
- Ricardo's approximate active bench/design minutes;
- Codex interaction elapsed time and principal activities;
- compilation/upload/test cycles;
- rework caused by wrong assumptions or failed changes;
- documentation time; and
- resulting commits and accepted evidence.

Long endurance tests should be recorded as **test duration**, not active
engineering labor, unless they require continuous supervision.

## Final deliverable

At project conclusion, a **SPinMicroDexed Project and Human–AI Collaboration
Evaluation Report** should synthesize:

- final functionality and limitations;
- quality, performance, stability and fidelity evidence;
- planned versus actual CAT evolution;
- engineering effort by activity and role;
- defects found, regressions prevented and rework incurred;
- contributions suitable for upstream consideration;
- strengths and weaknesses of the collaboration; and
- recommendations for future human–AI embedded-software projects.

This plan, the Executive Summary and the detailed Project History are living
documents and must be revised together as the project progresses.
