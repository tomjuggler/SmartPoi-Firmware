---
id: task-3
title: "Add pattern file existence cache (backport from ESP32)"
column: done
priority: high
tags:
  - performance
  - backport
  - cache
createdAt: "2026-08-03T06:56:58.205Z"
completedAt: "2026-08-03T07:02:43.738Z"
contract:
  status: done
  deliverables:
    - type: file
      path: main/main.ino
    - type: docs
      path: .brainfile/logs/ledger.jsonl
      description: Completed task ledger entry
  validation:
    commands:
      - bash tools/compile_check.sh
  constraints:
    - "Cache size 62 (26+26+10)"
    - "Refresh cache on file upload/delete"
---

## Add pattern file existence cache (backport from ESP32)

Back-port the 62-entry patternFileExistsCache from ESP32: initializePatternFileCache(), refreshPatternFileCacheEntry(), refreshPatternFileCache(), checkPatternFileExists(). Use it in updateCurrentImagesForPattern() to avoid repeated LittleFS.exists() calls which spam framework errors and slow pattern switching.

## Files changed
- `main/main.ino`

## Completed
- Completed at: 2026-08-03T07:02:43.738Z
- Validated with: `bash tools/compile_check.sh` (COMPILE OK)
