---
id: task-6
title: "Load-once image caching in showLittleFSImage"
column: done
priority: medium
tags:
  - performance
  - backport
  - littlefs
createdAt: "2026-08-03T06:56:58.215Z"
completedAt: "2026-08-03T07:11:00.972Z"
contract:
  status: done
  deliverables:
    - type: file
      path: main/showLittleFSImage.ino
    - type: docs
      path: .brainfile/logs/ledger.jsonl
      description: Completed task ledger entry
  validation:
    commands:
      - bash tools/compile_check.sh
  constraints:
    - "Keep 3-3-2 bit-packed decompression"
    - "Handle missing-file fallback gracefully"
---

## Load-once image caching in showLittleFSImage

ESP32 version loads the .bin file only when the image char changes (currentLoadedImageChar/fileNeedsReload), instead of reopening every frame like the current ESP8266 code ('Open every time?'). Back-port loadImageData() + markFileForReload() pattern.

## Files changed
- `main/showLittleFSImage.ino`

## Completed
- Completed at: 2026-08-03T07:11:00.972Z
- Validated with: `bash tools/compile_check.sh` (COMPILE OK)
