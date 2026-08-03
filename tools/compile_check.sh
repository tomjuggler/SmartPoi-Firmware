#!/usr/bin/env bash
# Compile-check the SmartPoi ESP8266 Arduino sketch (main/) using arduino-cli.
# Target: ESP8266 (LOLIN WEMOS D1 mini clone) - 4MB flash, 3MB LittleFS, 160MHz.
# Used by brainfile contract validation; also runnable manually.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SKETCH_DIR="$(cd "$SCRIPT_DIR/.." && pwd)/main"

# Locate arduino-cli
ARDUINO_CLI=""
for cand in \
  "${ARDUINO_CLI_PATH:-}" \
  "$(command -v arduino-cli || true)" \
  /opt/arduino-ide/resources/app/lib/backend/resources/arduino-cli \
  /mnt/Storage/Documents/PROGRAMMING/Blockly-AI/Blockly-AI-Cecli-API/bin/arduino-cli; do
  if [ -n "$cand" ] && [ -x "$cand" ]; then ARDUINO_CLI="$cand"; break; fi
done
if [ -z "$ARDUINO_CLI" ]; then
  echo "ERROR: arduino-cli not found. Install it or set ARDUINO_CLI_PATH." >&2
  exit 2
fi
echo "Using arduino-cli: $ARDUINO_CLI"

# 4M1M layout: 1MB FS + ~1019KB OTA headroom (ElegantOTA needs the OTA partition).
# Data folder (~216KB) fits comfortably in 1MB FS.
FQBN="esp8266:esp8266:d1_mini:xtal=160,vt=flash,exception=disabled,stacksmash=disabled,ssl=all,mmu=3232,non32xfer=fast,eesz=4M1M,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none,baud=921600"

echo "Compiling sketch: $SKETCH_DIR"
"$ARDUINO_CLI" compile --fqbn "$FQBN" "$SKETCH_DIR"
echo "COMPILE OK"
