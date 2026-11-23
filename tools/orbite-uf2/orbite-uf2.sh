#!/bin/sh
# ORBITE-I upload tool launcher (macOS / Linux) : picks the binary for this computer
DIR=$(cd "$(dirname "$0")" && pwd)
case "$(uname -s)" in
  Darwin*) OS=darwin ;;
  Linux*)  OS=linux ;;
  *) echo "orbite-uf2: unsupported system $(uname -s)" >&2; exit 1 ;;
esac
case "$(uname -m)" in
  arm64|aarch64) ARCH=arm64 ;;
  x86_64|amd64)  ARCH=amd64 ;;
  *) echo "orbite-uf2: unsupported processor $(uname -m)" >&2; exit 1 ;;
esac
BIN="$DIR/orbite-uf2-$OS-$ARCH"
chmod +x "$BIN" 2>/dev/null
exec "$BIN" "$@"
