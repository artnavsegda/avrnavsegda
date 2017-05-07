/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

// External oscillator settings.
// Uncomment and set correct values if external oscillator is used.

// External oscillator frequency
//#define BOARD_XOSC_HZ          8000000

// External oscillator type.
//!< External clock signal
//#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
//!< 32.768 kHz resonator on TOSC
//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
//!< 0.4 to 16 MHz resonator on XTALS
//#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL

// External oscillator startup time
//#define BOARD_XOSC_STARTUP_US  500000

#define ARDUINO0                       IOPORT_CREATE_PIN(PORTD, 0)
#define ARDUINO1                       IOPORT_CREATE_PIN(PORTD, 1)
#define ARDUINO2                       IOPORT_CREATE_PIN(PORTD, 2)
#define ARDUINO3                       IOPORT_CREATE_PIN(PORTD, 3)
#define ARDUINO4                       IOPORT_CREATE_PIN(PORTD, 4)
#define ARDUINO5                       IOPORT_CREATE_PIN(PORTD, 5)
#define ARDUINO6                       IOPORT_CREATE_PIN(PORTD, 6)
#define ARDUINO7                       IOPORT_CREATE_PIN(PORTD, 7)
#define ARDUINO8                       IOPORT_CREATE_PIN(PORTB, 0)
#define ARDUINO9                       IOPORT_CREATE_PIN(PORTB, 1)
#define ARDUINO10                      IOPORT_CREATE_PIN(PORTB, 2)
#define ARDUINO11                      IOPORT_CREATE_PIN(PORTB, 3)
#define ARDUINO12                      IOPORT_CREATE_PIN(PORTB, 4)
#define ARDUINO13                      IOPORT_CREATE_PIN(PORTB, 5)

#endif // USER_BOARD_H
