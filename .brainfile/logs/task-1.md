---
id: task-1
title: "Fix baseline compile error in udpHandler.ino"
column: done
priority: critical
tags:
  - bugfix
  - udp
  - compile
createdAt: "2026-08-03T06:56:58.174Z"
completedAt: "2026-08-03T06:59:05.591Z"
contract:
  status: done
  deliverables:
    - type: file
      path: main/udpHandler.ino
    - type: docs
      path: .brainfile/logs/ledger.jsonl
      description: Completed task ledger entry
  validation:
    commands:
      - bash tools/compile_check.sh
  constraints:
    - "Do not change UDP decompression logic"
    - "Only fix the compile error"
---

## Fix baseline compile error in udpHandler.ino

The current sketch does not compile: main/udpHandler.ino:128 uses LEDS.show() which is undeclared. Replace with FastLED.show(). This is a pre-existing bug blocking all further work.

## Files changed
- `main/udpHandler.ino`

## Completed
- Completed at: 2026-08-03T06:59:05.591Z
- Validated with: `bash tools/compile_check.sh` (COMPILE OK)
