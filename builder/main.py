# Copyright 2014-present PlatformIO <contact@platformio.org>
# Copyright 2026 Arkyos03 (ORBITE-I adaptation)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Derived from platform-ststm32 (builder/main.py), reduced to the ORBITE-I boards:
# upload "uf2" (TinyUF2 bootloader, ORBITEBOOT drive), ST-Link / J-Link / Black Magic,
# and the "bootloader" target (burn TinyUF2 with an ST-Link).

import sys
from os import makedirs
from os.path import isdir, join
from platform import system

from SCons.Script import (ARGUMENTS, COMMAND_LINE_TARGETS, AlwaysBuild,
                          Builder, Default, DefaultEnvironment)

env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()
PLATFORM_DIR = platform.get_dir()

env.Replace(
    AR="arm-none-eabi-gcc-ar",
    AS="arm-none-eabi-as",
    CC="arm-none-eabi-gcc",
    CXX="arm-none-eabi-g++",
    GDB="arm-none-eabi-gdb",
    OBJCOPY="arm-none-eabi-objcopy",
    RANLIB="arm-none-eabi-gcc-ranlib",
    SIZETOOL="arm-none-eabi-size",
    ARFLAGS=["rc"],
    SIZEPROGREGEXP=r"^(?:\.text|\.data|\.rodata|\.text.align|\.ARM.exidx)\s+(\d+).*",
    SIZEDATAREGEXP=r"^(?:\.data|\.bss|\.noinit)\s+(\d+).*",
    SIZECHECKCMD="$SIZETOOL -A -d $SOURCES",
    SIZEPRINTCMD="$SIZETOOL -B -d $SOURCES",
    PROGSUFFIX=".elf"
)

if env.get("PROGNAME", "program") == "program":
    env.Replace(PROGNAME="firmware")

env.Append(
    BUILDERS=dict(
        ElfToBin=Builder(
            action=env.VerboseAction("$OBJCOPY -O binary $SOURCES $TARGET", "Building $TARGET"),
            suffix=".bin"
        ),
        ElfToHex=Builder(
            action=env.VerboseAction("$OBJCOPY -O ihex -R .eeprom $SOURCES $TARGET", "Building $TARGET"),
            suffix=".hex"
        )
    )
)

#
# Target: build
#
if "nobuild" in COMMAND_LINE_TARGETS:
    target_elf = join("$BUILD_DIR", "${PROGNAME}.elf")
    target_firm = join("$BUILD_DIR", "${PROGNAME}.bin")
else:
    target_elf = env.BuildProgram()
    target_firm = env.ElfToBin(join("$BUILD_DIR", "${PROGNAME}"), target_elf)
    env.Depends(target_firm, "checkprogsize")

AlwaysBuild(env.Alias("nobuild", target_firm))
target_buildprog = env.Alias("buildprog", target_firm, target_firm)

target_size = env.Alias("size", target_elf, env.VerboseAction("$SIZEPRINTCMD", "Calculating size $SOURCE"))
AlwaysBuild(target_size)


def openocd_cmd(commands):
    scripts = join(platform.get_package_dir("tool-openocd") or "", "openocd", "scripts")
    return [
        "openocd", "-d%d" % (2 if int(ARGUMENTS.get("PIOVERBOSE", 0)) else 1),
        "-s", scripts, "-f", "interface/stlink.cfg", "-c", "transport select swd",
        "-f", "target/%s.cfg" % board.get("debug.openocd_target", "stm32f4x"),
        "-c", commands
    ]


#
# Target: upload
#
upload_protocol = env.subst("$UPLOAD_PROTOCOL")
debug_tools = board.get("debug.tools", {})
upload_source = target_firm
upload_actions = []

if upload_protocol == "uf2":
    # USB : appui 1200 bauds sur le port de la carte (redémarrage dans TinyUF2), puis
    # conversion du .bin en UF2 et copie sur le lecteur ORBITEBOOT (outil orbite-uf2).
    tool_dir = join(PLATFORM_DIR, "tools", "orbite-uf2")
    if system() == "Windows":
        uploader = '"%s"' % join(tool_dir, "orbite-uf2-windows-amd64.exe")
    else:
        uploader = '/bin/sh "%s"' % join(tool_dir, "orbite-uf2.sh")

    def before_uf2_upload(target, source, env):  # pylint: disable=unused-argument
        port = env.subst("$UPLOAD_PORT")
        if not port:
            try:
                from serial.tools import list_ports
                hwids = [(int(v, 16), int(p, 16)) for v, p in board.get("build.hwids", [])]
                for p in list_ports.comports():
                    if (p.vid, p.pid) in hwids:
                        port = p.device
                        break
            except Exception:  # pylint: disable=broad-except
                port = None
        if port:
            print("Redemarrage de la carte dans le bootloader (%s, 1200 bauds)" % port)
            try:
                env.TouchSerialPort(port, 1200)
            except Exception as e:  # pylint: disable=broad-except
                print("Appui 1200 bauds impossible : %s" % e)
        else:
            print("Pas de port serie ORBITE-I : carte deja en bootloader ? (sinon double appui sur RESET)")

    env.Replace(
        UPLOADER=uploader,
        UPLOADERFLAGS=[
            "-base", board.get("upload.offset_address", "0x08010000"),
            "-family", "0x57755a57",
            "-board", "STM32F405VG-ORBITE-I-CORE-revA",
            "-timeout", "20"
        ],
        UPLOADCMD='$UPLOADER -bin "$SOURCE" $UPLOADERFLAGS'
    )
    upload_actions = [
        env.VerboseAction(before_uf2_upload, "Looking for the ORBITE-I board..."),
        env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")
    ]

elif upload_protocol.startswith("blackmagic"):
    env.Replace(
        UPLOADER="$GDB",
        UPLOADERFLAGS=[
            "-nx", "--batch",
            "-ex", "target extended-remote $UPLOAD_PORT",
            "-ex", "monitor swdp_scan",
            "-ex", "attach 1", "-ex", "load", "-ex", "compare-sections", "-ex", "kill"
        ],
        UPLOADCMD="$UPLOADER $UPLOADERFLAGS $SOURCE"
    )
    upload_source = target_elf
    upload_actions = [
        env.VerboseAction(env.AutodetectUploadPort, "Looking for BlackMagic port..."),
        env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")
    ]

elif upload_protocol.startswith("jlink"):

    def _jlink_cmd_script(env, source):
        build_dir = env.subst("$BUILD_DIR")
        if not isdir(build_dir):
            makedirs(build_dir)
        script_path = join(build_dir, "upload.jlink")
        commands = ["h", 'loadbin "%s", %s' % (source, board.get("upload.offset_address", "0x08010000")), "r", "q"]
        with open(script_path, "w") as fp:
            fp.write("\n".join(commands))
        return script_path

    env.Replace(
        __jlink_cmd_script=_jlink_cmd_script,
        UPLOADER="JLink.exe" if system() == "Windows" else "JLinkExe",
        UPLOADERFLAGS=[
            "-device", board.get("debug", {}).get("jlink_device"),
            "-speed", env.GetProjectOption("debug_speed", "4000"),
            "-if", "swd", "-autoconnect", "1", "-NoGui", "1"
        ],
        UPLOADCMD='$UPLOADER $UPLOADERFLAGS -CommanderScript "${__jlink_cmd_script(__env__, SOURCE)}"'
    )
    upload_actions = [env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")]

elif upload_protocol in debug_tools:
    # ST-Link (OpenOCD) : programme le .bin à 0x08010000, le bootloader est conservé
    openocd_args = ["-d%d" % (2 if int(ARGUMENTS.get("PIOVERBOSE", 0)) else 1)]
    openocd_args.extend(debug_tools.get(upload_protocol).get("server").get("arguments", []))
    if env.GetProjectOption("debug_speed", ""):
        openocd_args.extend(["-c", "adapter speed %s" % env.GetProjectOption("debug_speed")])
    openocd_args.extend([
        "-c", "program {$SOURCE} %s verify reset; shutdown;" % board.get("upload.offset_address", "")
    ])
    openocd_args = [f.replace("$PACKAGE_DIR", platform.get_package_dir("tool-openocd") or "")
                    for f in openocd_args]
    env.Replace(UPLOADER="openocd", UPLOADERFLAGS=openocd_args, UPLOADCMD="$UPLOADER $UPLOADERFLAGS")
    upload_actions = [env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")]

elif upload_protocol == "custom":
    upload_actions = [env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")]

else:
    sys.stderr.write("Warning! Unknown upload protocol %s\n" % upload_protocol)

AlwaysBuild(env.Alias("upload", upload_source, upload_actions))

#
# Target: bootloader — grave TinyUF2 à 0x08000000 avec un ST-Link (connecteur SWD P3).
# Équivaut à « Graver la séquence d'initialisation » de l'IDE Arduino. Efface le programme.
#
bootloader_bin = join(PLATFORM_DIR, "bootloaders", "orbite_core", "tinyuf2-orbite_core.bin")
env.AddPlatformTarget(
    "bootloader",
    None,
    [env.VerboseAction(
        " ".join('"%s"' % a if " " in a or ";" in a else a for a in openocd_cmd(
            "init; reset halt; flash protect 0 0 3 off; reset halt; "
            "program {%s} 0x08000000 verify reset exit" % bootloader_bin.replace("\\", "/"))),
        "Burning the TinyUF2 bootloader (ST-Link)")],
    "Burn Bootloader",
    "Grave le bootloader TinyUF2 avec un ST-Link (efface le programme)",
)

Default([target_buildprog, target_size])
