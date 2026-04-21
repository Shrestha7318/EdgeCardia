/**
 * SSD1306 Configuration — Bare Metal STM32F446RE
 *
 * Adapted from afiskon/stm32-ssd1306 (https://github.com/afiskon/stm32-ssd1306)
 * Original by Olivier Van den Eede (4ilo), refactored by Aleksander Alekseev (afiskon)
 *
 * HAL dependencies removed — uses register-level I2C on STM32F446RE.
 */

#ifndef __SSD1306_CONF_H__
#define __SSD1306_CONF_H__

/* We use bare-metal register access — no STM32 family define needed */
#define SSD1306_BAREMETAL

/* Bus selection: I2C only for this build */
#define SSD1306_USE_I2C

/* I2C address (7-bit 0x3C, shifted to 8-bit write address) */
#define SSD1306_I2C_ADDR    (0x3C << 1)

/* Display dimensions */
/* #define SSD1306_WIDTH    128 */  /* default in ssd1306.h */
/* #define SSD1306_HEIGHT   64  */  /* default in ssd1306.h */

/* Uncomment to mirror the display */
/* #define SSD1306_MIRROR_VERT */
/* #define SSD1306_MIRROR_HORIZ */

/* Uncomment for inverse (white on black becomes black on white) */
/* #define SSD1306_INVERSE_COLOR */

/* Include the fonts you need (comment out to save flash) */
#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_16x26

#endif // __SSD1306_CONF_H__
