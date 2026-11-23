#!/usr/bin/env python3
"""Recopie dans la plateforme ce qui vient du dépôt ORBITE-I-Arduino.

    python3 scripts/sync_from_arduino.py <chemin du dépôt ORBITE-I-Arduino>

- variante ORBITE_CORE, bootloader TinyUF2, outil orbite-uf2
- bibliothèque Orbite (sans ses exemples)
- STM32SD et FatFs aux commits figés ci-dessous (ceux du paquet Arduino)
- exemples : chaque sketch de Orbite/examples/Tests devient un projet PlatformIO
"""
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

LIBS = {
    "STM32SD": ("https://github.com/stm32duino/STM32SD.git", "9b9bf7053c1d5b61758bd2fa415669233a7f1229"),   # 1.5.0
    "FatFs": ("https://github.com/stm32duino/FatFs.git", "cef1acad37d563e6b7dfd1c4fd3d48e7ff348947"),       # 4.0.4
}
IGNORE = shutil.ignore_patterns(".git", ".github", ".DS_Store", ".pio", "platformio.ini")

EXAMPLE_INI = """; Exemple de la plateforme ORBITE-I.
;   pio run -t upload        téléversement USB (bootloader UF2)
;   pio device monitor
[env:orbite_core]
platform = orbite
board = orbite_core
framework = arduino
monitor_speed = 115200

; ST-Link SWD (connecteur P3) : garde le bootloader, permet le débogage
[env:orbite_core_stlink]
platform = orbite
board = orbite_core
framework = arduino
upload_protocol = stlink
monitor_speed = 115200
"""


def copytree(src, dst):
    if dst.exists():
        shutil.rmtree(dst)
    shutil.copytree(src, dst, ignore=IGNORE)


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    src = Path(sys.argv[1]).resolve() / "platform"
    root = Path(__file__).resolve().parents[1]

    copytree(src / "variants" / "ORBITE_CORE", root / "variants" / "ORBITE_CORE")
    copytree(src / "bootloaders" / "orbite_core", root / "bootloaders" / "orbite_core")
    copytree(src / "tools" / "orbite-uf2", root / "tools" / "orbite-uf2")

    lib = root / "libraries" / "Orbite"
    copytree(src / "libraries" / "Orbite", lib)
    shutil.rmtree(lib / "examples", ignore_errors=True)

    for name, (url, commit) in LIBS.items():
        with tempfile.TemporaryDirectory() as tmp:
            subprocess.run(["git", "clone", "-q", url, tmp], check=True)
            subprocess.run(["git", "-C", tmp, "checkout", "-q", commit], check=True)
            copytree(Path(tmp), root / "libraries" / name)

    examples = root / "examples"
    if examples.exists():
        shutil.rmtree(examples)
    for ino in sorted((src / "libraries" / "Orbite" / "examples").rglob("*.ino")):
        project = examples / ino.parent.name
        copytree(ino.parent, project / "src")
        (project / "platformio.ini").write_text(EXAMPLE_INI)
        print("exemple", ino.parent.name)
    print("Synchronisation terminée depuis", src.parent)


if __name__ == "__main__":
    main()
