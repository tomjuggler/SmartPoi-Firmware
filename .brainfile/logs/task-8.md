---
id: task-8
title: "Add cylonMulticolour pattern 70 and brightness ramp"
column: done
priority: low
tags:
  - feature
  - backport
  - patterns
createdAt: "2026-08-03T06:56:58.220Z"
completedAt: "2026-08-03T07:16:38.700Z"
contract:
  status: done
  deliverables:
    - type: file
      path: main/colourPalette_code.ino
    - type: file
      path: main/main.ino
    - type: docs
      path: .brainfile/logs/ledger.jsonl
      description: Completed task ledger entry
  validation:
    commands:
      - bash tools/compile_check.sh
  constraints:
    - "Keep existing palette functions (ESP8266 retains ChangePalettePeriodically2 etc.)"
---

## Add cylonMulticolour pattern 70 and brightness ramp

Back-port cylonMulticolour() + fadeall() from ESP32 ColourPalette.cpp (pattern 70) and the smooth brightness ramp (targetBrightness ramping to newBrightness, BRIGHTNESS_RAMP_INTERVAL/STEP).

## Files changed
- `main/colourPalette_code.ino`
- `main/main.ino`

## Completed
- Completed at: 2026-08-03T07:16:38.700Z
- Validated with: `bash tools/compile_check.sh` (COMPILE OK)
