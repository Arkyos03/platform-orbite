# ORBITE-I pour PlatformIO

Plateforme [PlatformIO](https://platformio.org) de l'ordinateur de bord **ORBITE-I** (carte **CORE-I**, STM32F405VGT6). Une fois installée, la carte **ORBITE-I CORE (STM32F405)** apparaît dans PlatformIO comme n'importe quelle carte : assistant *New Project*, liste des cartes, exemples.

Elle contient tout ce qu'il faut :
- la carte et sa variante `ORBITE_CORE` (horloge 168 MHz sur quartz 8 MHz, programme à 0x08010000 derrière le bootloader) ;
- la bibliothèque **Orbite** et les bibliothèques de la carte micro SD (STM32SD 1.5.0, FatFs 4.0.4) ;
- le téléversement **USB** par le bootloader UF2 (lecteur ORBITEBOOT), comme dans l'IDE Arduino ;
- le téléversement et le **débogage** par ST-Link, ainsi que la **gravure du bootloader** ;
- les 12 tests de composants en exemples.

Version Arduino IDE et documentation de la carte : [ORBITE-I-Arduino](https://github.com/Arkyos03/ORBITE-I-Arduino).

## Installer (une seule fois)

Dans VS Code avec l'extension PlatformIO : **PIO Home › Platforms › Advanced Installation**, colle l'adresse suivante puis clique sur **Install** :

```
https://github.com/Arkyos03/platform-orbite.git
```

En ligne de commande :

```sh
pio pkg install -g -p https://github.com/Arkyos03/platform-orbite.git
```

Pour mettre à jour : **PIO Home › Platforms › Updates**, ou `pio pkg update -g -p orbite`.

## Nouveau projet

**PIO Home › New Project**, carte **ORBITE-I CORE (STM32F405)**, framework **Arduino**. Le `platformio.ini` créé :

```ini
[env:orbite_core]
platform = orbite
board = orbite_core
framework = arduino
monitor_speed = 115200
```

Puis dans `src/main.cpp` :

```cpp
#include <Orbite.h>

void setup() {
  Serial.begin(115200);
  Orbite.begin();
  Orbite.selfTest();          // 3 bips = tout va bien
}

void loop() {
  Orbite.update();
}
```

**Upload** (→) téléverse par USB, **Serial Monitor** ouvre le port série. Pour qu'un projet installe lui-même la plateforme sur un autre ordinateur, en version figée :

```ini
platform = https://github.com/Arkyos03/platform-orbite.git#v1.0.4
```

## Exemples

**PIO Home › Platforms › ORBITE-I › Examples** : les tests `T01_Buzzer_LED` à `T12_Tir_Pyro`, à importer en un clic.

## Téléverser

| `upload_protocol` | Comment |
|---|---|
| `uf2` (par défaut) | USB. La carte redémarre seule dans le bootloader (appui 1200 bauds), puis le programme est copié sur le lecteur **ORBITEBOOT**. Si rien ne se passe : double appui sur **RESET**, puis relancer dans les 20 s. |
| `stlink` | ST-Link sur le connecteur SWD P3. Programme à 0x08010000 : le bootloader est conservé. Débogage avec `pio debug` ou le bouton *Debug* de VS Code. |
| `jlink`, `blackmagic` | Sondes SWD équivalentes. |

## Carte neuve : graver le bootloader

Avec un ST-Link branché sur le connecteur SWD P3 : **Project Tasks › orbite_core › Platform › Burn Bootloader**, ou :

```sh
pio run -t bootloader
```

Cette commande **efface le programme** et grave TinyUF2 à 0x08000000. Ensuite, tout se fait par USB.

## Réglages

Ce sont les mêmes que dans l'IDE Arduino par défaut : `Serial` sur l'USB, `-Os`, Newlib Nano avec printf des float. Pour les changer, dans `build_flags` :

| Drapeau | Effet |
|---|---|
| `-D ORBITE_USB_DISABLED` | Sans USB (le téléversement USB demande alors un double appui sur RESET) |
| `-D PIO_FRAMEWORK_ARDUINO_ENABLE_CDC_WITHOUT_SERIAL` | `SerialUSB` sur l'USB, `Serial` sur l'UART de debug |
| `-D ORBITE_NO_FLOAT_PRINTF` | Sans printf des float (binaire plus petit) |
| `-D PIO_FRAMEWORK_ARDUINO_STANDARD_LIB` | Newlib complète |
| `build_unflags = -Os` + `build_flags = -O2` | Optimisation en vitesse |

Versions figées : cœur STM32duino 3.0.0 (`framework-arduinoststm32@4.30000.0`, requis par la variante qui utilise `pin_size_t`), GCC ARM 12.3.

## Mettre à jour depuis ORBITE-I-Arduino

La variante, la bibliothèque Orbite, le bootloader, l'outil UF2 et les exemples viennent du dépôt ORBITE-I-Arduino :

```sh
python3 scripts/sync_from_arduino.py ../ORBITE-I-Arduino
```

Ensuite, changer `version` dans `platform.json`, committer, puis taguer `v<version>`.

## Licences

Code ORBITE-I : MIT (voir `LICENSE`). `platform.py` et `builder/main.py` sont dérivés de [platform-ststm32](https://github.com/platformio/platform-ststm32) et restent sous licence Apache-2.0 (en-têtes des fichiers). STM32SD et FatFs : licences de STMicroelectronics et de ChaN, dans leurs dossiers.
