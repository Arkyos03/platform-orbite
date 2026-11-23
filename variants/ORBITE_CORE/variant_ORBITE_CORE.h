/*
 * ORBITE-I CORE-I (STM32F405VGT6, LQFP100) - Arduino board definition
 * Copyright (c) 2026 Arkyos03 - MIT License
 *
 * Based on the STM32duino generic F405VGTx variant (Copyright (c) 2020-2021,
 * STMicroelectronics, BSD 3-Clause license).
 *
 * Every pin of the MCU keeps its STM32duino name (PA0..PH1). The names of the
 * ORBITE-I schematic are defined below (FIRE_1, CS_BMP390, SERVO_1...).
 */
#pragma once

/*----------------------------------------------------------------------------
 *        STM32 pins number
 *----------------------------------------------------------------------------*/
#define PA0                     PIN_A0
#define PA1                     PIN_A1
#define PA2                     PIN_A2
#define PA3                     PIN_A3
#define PA4                     PIN_A4
#define PA5                     PIN_A5
#define PA6                     PIN_A6
#define PA7                     PIN_A7
#define PA8                     8
#define PA9                     9
#define PA10                    10
#define PA11                    11
#define PA12                    12
#define PA13                    13
#define PA14                    14
#define PA15                    15
#define PB0                     PIN_A8
#define PB1                     PIN_A9
#define PB2                     18
#define PB3                     19
#define PB4                     20
#define PB5                     21
#define PB6                     22
#define PB7                     23
#define PB8                     24
#define PB9                     25
#define PB10                    26
#define PB11                    27
#define PB12                    28
#define PB13                    29
#define PB14                    30
#define PB15                    31
#define PC0                     PIN_A10
#define PC1                     PIN_A11
#define PC2                     PIN_A12
#define PC3                     PIN_A13
#define PC4                     PIN_A14
#define PC5                     PIN_A15
#define PC6                     38
#define PC7                     39
#define PC8                     40
#define PC9                     41
#define PC10                    42
#define PC11                    43
#define PC12                    44
#define PC13                    45
#define PC14                    46
#define PC15                    47
#define PD0                     48
#define PD1                     49
#define PD2                     50
#define PD3                     51
#define PD4                     52
#define PD5                     53
#define PD6                     54
#define PD7                     55
#define PD8                     56
#define PD9                     57
#define PD10                    58
#define PD11                    59
#define PD12                    60
#define PD13                    61
#define PD14                    62
#define PD15                    63
#define PE0                     64
#define PE1                     65
#define PE2                     66
#define PE3                     67
#define PE4                     68
#define PE5                     69
#define PE6                     70
#define PE7                     71
#define PE8                     72
#define PE9                     73
#define PE10                    74
#define PE11                    75
#define PE12                    76
#define PE13                    77
#define PE14                    78
#define PE15                    79
#define PH0                     80
#define PH1                     81

// Alternate pins number
#define PA0_ALT1                (PA0  | ALT1)
#define PA0_ALT2                (PA0  | ALT2)
#define PA1_ALT1                (PA1  | ALT1)
#define PA1_ALT2                (PA1  | ALT2)
#define PA2_ALT1                (PA2  | ALT1)
#define PA2_ALT2                (PA2  | ALT2)
#define PA3_ALT1                (PA3  | ALT1)
#define PA3_ALT2                (PA3  | ALT2)
#define PA4_ALT1                (PA4  | ALT1)
#define PA5_ALT1                (PA5  | ALT1)
#define PA6_ALT1                (PA6  | ALT1)
#define PA7_ALT1                (PA7  | ALT1)
#define PA7_ALT2                (PA7  | ALT2)
#define PA7_ALT3                (PA7  | ALT3)
#define PA15_ALT1               (PA15 | ALT1)
#define PB0_ALT1                (PB0  | ALT1)
#define PB0_ALT2                (PB0  | ALT2)
#define PB1_ALT1                (PB1  | ALT1)
#define PB1_ALT2                (PB1  | ALT2)
#define PB3_ALT1                (PB3  | ALT1)
#define PB4_ALT1                (PB4  | ALT1)
#define PB5_ALT1                (PB5  | ALT1)
#define PB8_ALT1                (PB8  | ALT1)
#define PB9_ALT1                (PB9  | ALT1)
#define PB14_ALT1               (PB14 | ALT1)
#define PB14_ALT2               (PB14 | ALT2)
#define PB15_ALT1               (PB15 | ALT1)
#define PB15_ALT2               (PB15 | ALT2)
#define PC0_ALT1                (PC0  | ALT1)
#define PC0_ALT2                (PC0  | ALT2)
#define PC1_ALT1                (PC1  | ALT1)
#define PC1_ALT2                (PC1  | ALT2)
#define PC2_ALT1                (PC2  | ALT1)
#define PC2_ALT2                (PC2  | ALT2)
#define PC3_ALT1                (PC3  | ALT1)
#define PC3_ALT2                (PC3  | ALT2)
#define PC4_ALT1                (PC4  | ALT1)
#define PC5_ALT1                (PC5  | ALT1)
#define PC6_ALT1                (PC6  | ALT1)
#define PC7_ALT1                (PC7  | ALT1)
#define PC8_ALT1                (PC8  | ALT1)
#define PC9_ALT1                (PC9  | ALT1)
#define PC10_ALT1               (PC10 | ALT1)
#define PC11_ALT1               (PC11 | ALT1)

#define NUM_DIGITAL_PINS        82
#define NUM_ANALOG_INPUTS       16


/*----------------------------------------------------------------------------
 *        ORBITE-I CORE-I : names of the schematic
 *----------------------------------------------------------------------------*/
/* Pyro (SPARK-I) : outputs, LOW at reset (10k pull-down on the board) */
#define FIRE_1                  PE7
#define FIRE_2                  PE8
#define FIRE_3                  PE9
#define FIRE_4                  PE10
/* Pyro continuity (analog, 33k/10k divider of the e-match voltage) */
#define CONT_1                  PC3
#define CONT_2                  PC2
#define CONT_3                  PC1
#define CONT_4                  PC0
#define VPYRO_SENSE             PC4   /* pyro rail, 33k/10k divider */
#define VBAT_SENSE              PC5   /* battery, 33k/10k divider   */
#define VBAT_DIVIDER_RATIO      4.3f  /* (33k + 10k) / 10k          */
#define VPYRO_DIVIDER_RATIO     4.3f
#define CONT_DIVIDER_RATIO      4.3f

/* Switch inputs (SPARK-I terminal blocks, 10k pull-up, closed = LOW) */
#define SW_LAUNCH               PE11
#define SW_PIN                  PE12
#define SW_AUX                  PE13

/* Servos (REACTOR-I headers, TIM4 CH1..CH4) */
#define SERVO_1                 PD12
#define SERVO_2                 PD13
#define SERVO_3                 PD14
#define SERVO_4                 PD15

/* Buzzer (SPARK-I, piezo, TIM3 CH1) : use tone(BUZZER, 4000) */
#define BUZZER                  PC6

/* RGB LED SK6805 (BEACON-I) through a 74AHCT1G125 buffer (TIM1 CH1) */
#define LED_RGB                 PA8

/* SPI1 : MS5611 + BMI088 (CORE-I) */
#define SPI1_SCLK               PA5
#define SPI1_MISO               PA6
#define SPI1_MOSI               PA7
#define CS_MS5611               PA4
#define CS_BMI088_ACC           PE0
#define CS_BMI088_GYR           PE1
#define BMI088_INT_ACC          PE2
#define BMI088_INT_GYR          PE3

/* SPI2 : BMP390 + LSM6DSOX (CORE-I), MMC5983MA (BEACON-I) */
#define SPI2_SCLK               PB13
#define SPI2_MISO               PB14
#define SPI2_MOSI               PB15
#define CS_BMP390               PD8
#define CS_LSM6DSOX             PD9
#define LSM6DSOX_INT1           PD10
#define CS_MAG                  PB12

/* SPI3 : LoRa E22-900M22S (BEACON-I) + flash W25Q128 (CORE-I) */
#define SPI3_SCLK               PB3
#define SPI3_MISO               PB4
#define SPI3_MOSI               PB5
#define CS_LORA                 PD1
#define LORA_RST                PD3
#define LORA_BUSY               PD4
#define LORA_DIO1               PD7
#define CS_FLASH                PD0

/* GPS MAX-M10S (BEACON-I) on Serial2 (USART2) */
#define GPS_TX                  PA2   /* MCU TX -> GPS RXD */
#define GPS_RX                  PA3   /* MCU RX <- GPS TXD */
#define GPS_PPS                 PA15
#define GPS_BAUD                38400

/* Debug UART (P3) on Serial1 (USART1) */
#define DEBUG_TX                PA9
#define DEBUG_RX                PA10

/* micro SD (SDIO 4 bits, Molex 503398-1892) */
#define SD_DETECT               PD11  /* LOW = card inserted */
#define SD_DETECT_PIN           PD11
#define SD_DETECT_LEVEL         LOW

/* Reserved line of the bus */
#define RSV                     PE15

/*----------------------------------------------------------------------------
 *        Arduino defaults
 *----------------------------------------------------------------------------*/
#ifndef LED_BUILTIN
  #define LED_BUILTIN           LED_RGB  /* addressable LED : use the Orbite library */
#endif
#define LED_BUILDIN             LED_RGB  /* name used on the schematic */
#define PIN_NEOPIXEL            LED_RGB

#ifndef USER_BTN
  #define USER_BTN              PNUM_NOT_DEFINED
#endif

/* SPI (default) = SPI1 : sensors of CORE-I */
#ifndef PIN_SPI_SS
  #define PIN_SPI_SS            CS_MS5611
#endif
#ifndef PIN_SPI_SS1
  #define PIN_SPI_SS1           CS_BMI088_ACC
#endif
#ifndef PIN_SPI_SS2
  #define PIN_SPI_SS2           CS_BMI088_GYR
#endif
#ifndef PIN_SPI_SS3
  #define PIN_SPI_SS3           PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_MOSI
  #define PIN_SPI_MOSI          PA7
#endif
#ifndef PIN_SPI_MISO
  #define PIN_SPI_MISO          PA6
#endif
#ifndef PIN_SPI_SCK
  #define PIN_SPI_SCK           PA5
#endif

/* I2C (Wire) : header P3 of BEACON-I, 4k7 pull-ups on CORE-I */
#ifndef PIN_WIRE_SDA
  #define PIN_WIRE_SDA          PB7
#endif
#ifndef PIN_WIRE_SCL
  #define PIN_WIRE_SCL          PB6
#endif

/* Timers used by the core */
#ifndef TIMER_TONE
  #define TIMER_TONE            TIM6
#endif
#ifndef TIMER_SERVO
  #define TIMER_SERVO           TIM7
#endif

/* Hardware serial : Serial1 = debug (USART1), Serial2 = GPS (USART2).
 * "Serial" is the USB port (Tools > USB support > CDC). */
#ifndef SERIAL_UART_INSTANCE
  #define SERIAL_UART_INSTANCE  1
#endif
#ifndef PIN_SERIAL_RX
  #define PIN_SERIAL_RX         PA10
#endif
#ifndef PIN_SERIAL_TX
  #define PIN_SERIAL_TX         PA9
#endif
#define ENABLE_HWSERIAL1
#define ENABLE_HWSERIAL2
#define PIN_SERIAL1_RX          PA10
#define PIN_SERIAL1_TX          PA9
#define PIN_SERIAL2_RX          PA3
#define PIN_SERIAL2_TX          PA2

/* HSE crystal 8 MHz (Y2). LSE crystal 32.768 kHz (Y1, 15 pF) : RTC with STM32RTC, rtc.setClockSource(STM32RTC::LSE_CLOCK) */
#ifndef HSE_VALUE
  #define HSE_VALUE             8000000U
#endif

/* Extra HAL modules */
#if !defined(HAL_DAC_MODULE_DISABLED)
  #define HAL_DAC_MODULE_ENABLED
#endif
#if !defined(HAL_SD_MODULE_DISABLED)
  #define HAL_SD_MODULE_ENABLED
#endif

/* UF2 bootloader (TinyUF2) : application at 0x08010000 */
#define ORBITE_BOOTLOADER_SIZE  0x10000
#define ORBITE_DBL_TAP_MAGIC    0xf01669efUL  /* RTC->BKP0R -> stay in bootloader */

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/
#ifdef __cplusplus
  #ifndef SERIAL_PORT_MONITOR
    #define SERIAL_PORT_MONITOR   Serial
  #endif
  #ifndef SERIAL_PORT_HARDWARE
    #define SERIAL_PORT_HARDWARE  Serial1
  #endif
  #define SERIAL_PORT_HARDWARE_OPEN Serial1
  #define SERIAL_PORT_GPS         Serial2
#endif
