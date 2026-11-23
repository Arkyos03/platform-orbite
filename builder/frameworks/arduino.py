# ORBITE-I — framework Arduino : cœur STM32duino + variante ORBITE_CORE et bibliothèques
# fournies par cette plateforme.
import sys
from os.path import isfile, join

from SCons.Script import DefaultEnvironment, SConscript

env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()
PLATFORM_DIR = platform.get_dir()

# Variante ORBITE_CORE livrée avec la plateforme (absente du cœur STM32duino officiel)
board.update("build.variants_dir", join(PLATFORM_DIR, "variants"))

# Bibliothèques livrées avec la plateforme : Orbite, STM32SD, FatFs
env.Append(LIBSOURCE_DIRS=[join(PLATFORM_DIR, "libraries")])
# FatFs a besoin de ffconf.h, fourni par STM32SD (dépendance circulaire que l'IDE Arduino
# résout en mettant toutes les bibliothèques dans le chemin d'include)
env.Append(CPPPATH=[join(PLATFORM_DIR, "libraries", "STM32SD", "src")])

# Identité USB identique au paquet Arduino. Le builder PlatformIO de STM32duino ne définit
# que USB_VID/USB_PID, alors que le cœur lit USBD_VID/USBD_PID (sinon : VID 0x0483 de ST).
vid, pid = board.get("build.hwids")[0]
env.Append(CPPDEFINES=[
    ("USBD_VID", vid),
    ("USBD_PID", pid),
    ("USB_MANUFACTURER_STRING", '\\"Arkyos03\\"'),
    ("USB_PRODUCT_STRING", '\\"ORBITE-I CORE\\"'),
])

# Réglages par défaut de l'IDE Arduino (menus Outils), modifiables dans build_flags :
#   USB série (Serial = USB)          -> -D ORBITE_USB_DISABLED pour le couper, ou un autre
#                                        PIO_FRAMEWORK_ARDUINO_ENABLE_* (ex. CDC_WITHOUT_SERIAL)
#   Newlib Nano + printf des float    -> -D ORBITE_NO_FLOAT_PRINTF, ou PIO_FRAMEWORK_ARDUINO_STANDARD_LIB
defines = [str(d[0]) if isinstance(d, (list, tuple)) else str(d) for d in env.get("CPPDEFINES", [])]
if not any(d.startswith("PIO_FRAMEWORK_ARDUINO_ENABLE_") for d in defines) and "ORBITE_USB_DISABLED" not in defines:
    env.Append(CPPDEFINES=["PIO_FRAMEWORK_ARDUINO_ENABLE_CDC"])
if not any(d in defines for d in ("PIO_FRAMEWORK_ARDUINO_NANOLIB_FLOAT_PRINTF", "PIO_FRAMEWORK_ARDUINO_STANDARD_LIB",
                                  "ORBITE_NO_FLOAT_PRINTF")):
    env.Append(CPPDEFINES=["PIO_FRAMEWORK_ARDUINO_NANOLIB_FLOAT_PRINTF"])
# En-têtes MAVLink générés de la bibliothèque Orbite
env.Append(CCFLAGS=["-Wno-address-of-packed-member"])

build_script = join(platform.get_package_dir("framework-arduinoststm32"),
                    "tools", "platformio", "platformio-build.py")
if not isfile(build_script):
    sys.stderr.write("Error: Missing PlatformIO build script %s!\n" % build_script)
    env.Exit(1)

SConscript(build_script)
