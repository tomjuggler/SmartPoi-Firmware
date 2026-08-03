---
id: task-2
title: "Unify LittleFS path addressing (leading slash)"
column: done
priority: high
tags:
  - littlefs
  - refactor
  - backport
createdAt: "2026-08-03T06:56:58.201Z"
completedAt: "2026-08-03T07:00:51.388Z"
contract:
  status: done
  deliverables:
    - type: file
      path: main/main.ino
    - type: file
      path: main/showLittleFSImage.ino
    - type: file
      path: main/webServerSetupLogic.ino
    - type: docs
      path: .brainfile/logs/ledger.jsonl
      description: Completed task ledger entry
  validation:
    commands:
      - bash tools/compile_check.sh
  constraints:
    - "Keep LittleFS mounting working"
    - "No functional regression in file loading"
---

## Unify LittleFS path addressing (leading slash)

ESP32 version uses '/a.bin' (leading slash) consistently. ESP8266 currently uses 'a.bin' with a binIndex hack in updateCurrentImagesForPattern. Standardize on leading slash everywhere (bin string, exists()/open() calls, upload handler), remove the hack.

## Files changed
- `main/main.ino`
- `main/showLittleFSImage.ino`
- `main/webServerSetupLogic.ino`

## Completed
- Completed at: 2026-08-03T07:00:51.388Z
- Validated with: `bash tools/compile_check.sh` (COMPILE OK)
