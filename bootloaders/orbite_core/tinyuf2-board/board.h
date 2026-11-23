/*
 * ORBITE-I CORE-I (STM32F405VGT6) - TinyUF2 board definition
 * Copyright (c) 2026 Arkyos03 - MIT License
 *
 * Bootloader : 64 KB at 0x08000000, application at 0x08010000.
 * Double-tap RESET (or 1200 bps touch from the Arduino IDE) -> drive ORBITEBOOT.
 * The double-tap flag lives in the RTC backup register BKP0R (survives a reset,
 * cleared at power-on because VBAT is tied to VCC).
 */
#ifndef BOARD_H_
#define BOARD_H_

//--------------------------------------------------------------------+
// Double tap reset
//--------------------------------------------------------------------+
#define TINYUF2_DBL_TAP_DFU   1
#define TINYUF2_DBL_TAP_REG   (RTC->BKP0R)

//--------------------------------------------------------------------+
// LED : SK6805 RGB (LED_RGB, PA8) through a 74AHCT1G125 buffer
//--------------------------------------------------------------------+
#define NEOPIXEL_NUMBER       1
#define NEOPIXEL_BRIGHTNESS   0x30
#define NEOPIXEL_PORT         GPIOA
#define NEOPIXEL_PIN          GPIO_PIN_8
#define NEOPIXEL_PIN_MODE     GPIO_MODE_OUTPUT_PP

//--------------------------------------------------------------------+
// Flash
//--------------------------------------------------------------------+
#define BOARD_FLASH_SIZE      (1024 * 1024)

//--------------------------------------------------------------------+
// USB UF2 (pid.codes test VID/PID)
//--------------------------------------------------------------------+
#define USB_NO_VBUS_PIN       1     /* PA9 is the debug UART TX, VBUS is not routed */
#define USB_VID               0x1209
#define USB_PID               0x0002
#define USB_MANUFACTURER      "Arkyos03"
#define USB_PRODUCT           "ORBITE-I CORE"

#define UF2_PRODUCT_NAME      USB_MANUFACTURER " " USB_PRODUCT
#define UF2_BOARD_ID          "STM32F405VG-ORBITE-I-CORE-revA"
#define UF2_VOLUME_LABEL      "ORBITEBOOT"
#define UF2_INDEX_URL         "https://github.com/Arkyos03/ORBITE-I-Arduino"

//--------------------------------------------------------------------+
// UART (debug header P3) : USART1 PA9 TX / PA10 RX, only with LOG=1
//--------------------------------------------------------------------+
#define UART_DEV              USART1
#define UART_CLOCK_ENABLE     __HAL_RCC_USART1_CLK_ENABLE
#define UART_CLOCK_DISABLE    __HAL_RCC_USART1_CLK_DISABLE
#define UART_GPIO_PORT        GPIOA
#define UART_GPIO_AF          GPIO_AF7_USART1
#define UART_TX_PIN           GPIO_PIN_9
#define UART_RX_PIN           GPIO_PIN_10

//--------------------------------------------------------------------+
// RCC Clock : HSE 8 MHz -> 168 MHz, 48 MHz USB
//--------------------------------------------------------------------+
static inline void clock_init(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* access to the RTC backup registers (double tap flag) */
  PWR->CR |= PWR_CR_DBP;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = HSE_VALUE/1000000;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

#endif
