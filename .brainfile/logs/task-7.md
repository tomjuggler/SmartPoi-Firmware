---
id: task-7
title: "Web handler parity with ESP32 (patterns 0-70, captive portal, elegant route)"
column: done
priority: medium
tags:
  - webserver
  - backport
  - routes
createdAt: "2026-08-03T06:56:58.218Z"
completedAt: "2026-08-03T07:12:23.008Z"
contract:
  status: done
  deliverables:
    - type: file
      path: main/webServerSetupLogic.ino
    - type: docs
      path: .brainfile/logs/ledger.jsonl
      description: Completed task ledger entry
  validation:
    commands:
      - bash tools/compile_check.sh
  constraints:
    - "Sync ESP8266WebServer API (server.on/onNotFound)"
    - "Keep CORS headers on all routes"
---

## Web handler parity with ESP32 (patterns 0-70, captive portal, elegant route)

Bring /pattern route validation to 0-70 (currently limited to 62), add captive portal redirects (/generate_204, /hotspot-detect.html, /connectivity-check.html), /elegant route, /poi-available route, and efficient /returnsettings with snprintf buffer (from ESP32 tasks.cpp).

## Files changed
- `main/webServerSetupLogic.ino`

## Completed
- Completed at: 2026-08-03T07:12:23.008Z
- Validated with: `bash tools/compile_check.sh` (COMPILE OK)
