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
# Derived from platform-ststm32 (platform.py), reduced to the ORBITE-I boards.

import sys

from platformio.public import PlatformBase

IS_WINDOWS = sys.platform.startswith("win")


class OrbitePlatform(PlatformBase):

    def configure_default_packages(self, variables, targets):
        board = variables.get("board")
        board_config = self.board_config(board) if board else None

        # OpenOCD : téléversement / débogage ST-Link, gravure du bootloader
        protocol = variables.get("upload_protocol",
                                 board_config.get("upload.protocol", "") if board_config else "")
        if (protocol in ("stlink", "cmsis-dap") or "bootloader" in targets
                or "debug" in variables.get("build_type", "")
                or "stlink" in variables.get("debug_tool", "stlink")):
            self.packages["tool-openocd"]["optional"] = False

        # J-Link seulement s'il est demandé
        if not any("jlink" in variables.get(o, "") for o in ("upload_protocol", "debug_tool")):
            self.packages.pop("tool-jlink", None)

        return super().configure_default_packages(variables, targets)

    def get_boards(self, id_=None):
        result = super().get_boards(id_)
        if not result:
            return result
        if id_:
            return self._add_default_debug_tools(result)
        for key, value in result.items():
            result[key] = self._add_default_debug_tools(value)
        return result

    def _add_default_debug_tools(self, board):
        debug = board.manifest.get("debug", {})
        upload_protocols = board.manifest.get("upload", {}).get("protocols", [])
        if "tools" not in debug:
            debug["tools"] = {}

        for link in ("blackmagic", "jlink", "stlink"):
            if link not in upload_protocols or link in debug["tools"]:
                continue
            if link == "blackmagic":
                debug["tools"]["blackmagic"] = {
                    "hwids": [["0x1d50", "0x6018"]],
                    "require_debug_port": True
                }
            elif link == "jlink":
                debug["tools"][link] = {
                    "server": {
                        "package": "tool-jlink",
                        "arguments": [
                            "-singlerun", "-if", "SWD", "-select", "USB",
                            "-device", debug.get("jlink_device"), "-port", "2331"
                        ],
                        "executable": ("JLinkGDBServerCL.exe" if IS_WINDOWS else "JLinkGDBServer")
                    }
                }
            else:
                debug["tools"][link] = {
                    "server": {
                        "package": "tool-openocd",
                        "executable": "bin/openocd",
                        "arguments": [
                            "-s", "$PACKAGE_DIR/openocd/scripts",
                            "-f", "interface/%s.cfg" % link,
                            "-c", "transport select swd",
                            "-f", "target/%s.cfg" % debug.get("openocd_target")
                        ]
                    }
                }
            debug["tools"][link]["onboard"] = link in debug.get("onboard_tools", [])
            debug["tools"][link]["default"] = link in debug.get("default_tools", [])

        board.manifest["debug"] = debug
        return board

    def configure_debug_session(self, debug_config):
        if debug_config.speed:
            server_executable = (debug_config.server or {}).get("executable", "").lower()
            if "openocd" in server_executable:
                debug_config.server["arguments"].extend(["-c", "adapter speed %s" % debug_config.speed])
            elif "jlink" in server_executable:
                debug_config.server["arguments"].extend(["-speed", debug_config.speed])
