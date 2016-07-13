/**
 * \file
 *
 * \brief Example application for GFX Monochrome spinner widget
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <stdio.h>
#include <conf_spinctrl.h>
#include <conf_sysfont.h>

/**
 * \mainpage
 *
 * \section intro Introduction
 * This simple example shows how to set up a collection of spinners and display
 * them on the screen.
 *
 * \section files Main files:
 * - \ref example_spinctrl.c: the example itself.
 * - \ref conf_st7565r.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_sysfont.h
 * - \ref conf_usart_spi.h
 * - \ref conf_spinctrl.h
 *
 * \section deviceinfo Device Info
 * All AVR devices can be used.
 * This example has been tested with the following setup:
 * - XMEGA-A3BU Xplained
 *
 * \section description Description of the example application
 * This example application will initialize string spinner and two integer
 * spinners, set up a spincollection with the spinners and display them on
 * the screen.
 *
 * \section dependencies Dependencies
 * Relevant module dependencies for this application are:
 * - \ref gfx_mono
 * - \ref gfx_mono_font
 * - \ref gfx_mono_spinctrl
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for AVR.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 */

PROGMEM_DECLARE(char const, spinnertitle[]) = "Strings:";
PROGMEM_DECLARE(char const, spinnertitle2[]) = "Numbers:";
PROGMEM_DECLARE(char const, spinnertitle3[]) = "Numbers2:";
PROGMEM_DECLARE(char const, spinner_choice1[]) = "String";
PROGMEM_DECLARE(char const, spinner_choice2[]) = "String 2";
PROGMEM_DECLARE(char const, spinner_choice3[]) = "String 3";
PROGMEM_DECLARE(char const, spinner_choice4[]) = "String 4";

PROGMEM_STRING_T spinner_choicestrings[] = {
	spinner_choice1,
	spinner_choice2,
	spinner_choice3,
	spinner_choice4,
};

PROGMEM_DECLARE(char const, menu_title[]) = "Menu";
PROGMEM_DECLARE(char const, menu_1[]) = "One";
PROGMEM_DECLARE(char const, menu_2[]) = "Two";
PROGMEM_DECLARE(char const, menu_3[]) = "Three";
PROGMEM_DECLARE(char const, menu_4[]) = "Four";

PROGMEM_STRING_T menu_strings[] = {
	menu_1,
	menu_2,
	menu_3,
	menu_4,
};

struct gfx_mono_menu mymenu = {
	// Title
	menu_title,
	// Array with menu strings
	menu_strings,
	// Number of menu elements
	4,
	// Initial selection
	0
};

#define GPIO_PUSH_BUTTON_3              IOPORT_CREATE_PIN(PORTF, 3)

typedef uint8_t keycode_t;

//! Definition of the different keyboard event types
enum keyboard_event_type {
	KEYBOARD_NO_EVENT,
	KEYBOARD_PRESS,
	KEYBOARD_RELEASE,
	/* KEYBOARD_NEAR // for a possible proximity state */
};

//! Struct to hold a keycode and the related event
struct keyboard_event {
	keycode_t keycode;
	enum keyboard_event_type type;
};

void keyboard_get_key_state(struct keyboard_event *key);

/**
 * \brief Check the status of the keyboard
 *
 * \param keybuffer  a keyboard_event struct to store keyboard status
 */
void keyboard_get_key_state(struct keyboard_event *keybuffer)
{
	// Variable to hold the key mask
	static uint8_t key_state;
	char key;

	if ((gpio_pin_is_low(GPIO_PUSH_BUTTON_0)) &&
			!(key_state & GFX_MONO_SPINCTRL_KEYCODE_ENTER_MASK)) {
		// Key pressed
		key_state |= GFX_MONO_SPINCTRL_KEYCODE_ENTER_MASK;
		keybuffer->keycode = GFX_MONO_SPINCTRL_KEYCODE_ENTER;
		keybuffer->type = KEYBOARD_PRESS;
	} else if ((gpio_pin_is_high(GPIO_PUSH_BUTTON_0))
			&& (key_state & GFX_MONO_SPINCTRL_KEYCODE_ENTER_MASK)) {
		// Key released
		key_state &= ~GFX_MONO_SPINCTRL_KEYCODE_ENTER_MASK;
		keybuffer->keycode = GFX_MONO_SPINCTRL_KEYCODE_ENTER;
		keybuffer->type = KEYBOARD_RELEASE;
	} else if ((gpio_pin_is_low(GPIO_PUSH_BUTTON_1)) &&
			!(key_state & GFX_MONO_SPINCTRL_KEYCODE_UP_MASK)) {
		// Key pressed
		key_state |= GFX_MONO_SPINCTRL_KEYCODE_UP_MASK;
		keybuffer->keycode = GFX_MONO_SPINCTRL_KEYCODE_UP;
		keybuffer->type = KEYBOARD_PRESS;
	} else if ((gpio_pin_is_high(GPIO_PUSH_BUTTON_1))
			&& (key_state & GFX_MONO_SPINCTRL_KEYCODE_UP_MASK)) {
		// Key released
		key_state &= ~GFX_MONO_SPINCTRL_KEYCODE_UP_MASK;
		keybuffer->keycode = GFX_MONO_SPINCTRL_KEYCODE_UP;
		keybuffer->type = KEYBOARD_RELEASE;
	} else if ((gpio_pin_is_low(GPIO_PUSH_BUTTON_2)) &&
			!(key_state & GFX_MONO_SPINCTRL_KEYCODE_DOWN_MASK)) {
		// Key pressed
		key_state |= GFX_MONO_SPINCTRL_KEYCODE_DOWN_MASK;
		keybuffer->keycode = GFX_MONO_SPINCTRL_KEYCODE_DOWN;
		keybuffer->type = KEYBOARD_PRESS;
	} else if ((gpio_pin_is_high(GPIO_PUSH_BUTTON_2))
			&& (key_state & GFX_MONO_SPINCTRL_KEYCODE_DOWN_MASK)) {
		// Key released
		key_state &= ~GFX_MONO_SPINCTRL_KEYCODE_DOWN_MASK;
		keybuffer->keycode = GFX_MONO_SPINCTRL_KEYCODE_DOWN;
		keybuffer->type = KEYBOARD_RELEASE;
	} else if ((gpio_pin_is_low(GPIO_PUSH_BUTTON_3)) &&
			!(key_state & GFX_MONO_SPINCTRL_KEYCODE_BACK_MASK)) {
		// Key pressed
		key_state |= GFX_MONO_SPINCTRL_KEYCODE_BACK_MASK;
		keybuffer->keycode = GFX_MONO_SPINCTRL_KEYCODE_BACK;
		keybuffer->type = KEYBOARD_PRESS;
	} else if ((gpio_pin_is_high(GPIO_PUSH_BUTTON_3)) &&
			(key_state & GFX_MONO_SPINCTRL_KEYCODE_BACK_MASK)) {
		// Touch key released
		key_state &= ~GFX_MONO_SPINCTRL_KEYCODE_BACK_MASK;
		keybuffer->keycode = GFX_MONO_SPINCTRL_KEYCODE_BACK;
		keybuffer->type = KEYBOARD_RELEASE;
	} else {
		keybuffer->keycode = GFX_MONO_SPINCTRL_KEYCODE_NO_KEY;
		keybuffer->type = KEYBOARD_NO_EVENT;
	}
}

/**
 * \brief Main entry of example application
 */
int main(void)
{
	uint8_t spinner_status;
	int16_t spinner_results[3];
	uint8_t menu_status;
	struct keyboard_event input;
	struct gfx_mono_spinctrl spinner1;
	struct gfx_mono_spinctrl spinner2;
	struct gfx_mono_spinctrl spinner3;
	struct gfx_mono_spinctrl_spincollection spinners;
	int16_t results[GFX_MONO_SPINCTRL_MAX_ELEMENTS_IN_SPINCOLLECTION];

	/**
	 * Starts off by initializing the system clock before configuring the
	 * board and the monochrome graphical system.
	 */
	board_init();
	sysclk_init();
	gfx_mono_init();

	gfx_mono_menu_init(&mymenu);

	// Initialize spinners
	gfx_mono_spinctrl_init(&spinner1, SPINTYPE_STRING, spinnertitle, spinner_choicestrings, 0, 3, 0);
	gfx_mono_spinctrl_init(&spinner2, SPINTYPE_INTEGER,	spinnertitle2, NULL, -60, -41, 0);
	gfx_mono_spinctrl_init(&spinner3, SPINTYPE_INTEGER,	spinnertitle3, NULL, 19999, 20200, 0);

	// Initialize spincollection
	gfx_mono_spinctrl_spincollection_init(&spinners);

	// Add spinners to spincollection
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner1, &spinners);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner2, &spinners);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner3, &spinners);

	// Show spincollection on screen
	//gfx_mono_spinctrl_spincollection_show(&spinners);

	// Spincollection is now ready to process input from user

	do {
		// Intentionally left empty.
		do {
			keyboard_get_key_state(&input);
			// Wait for key release
		} while (input.type != KEYBOARD_RELEASE);
		//spinner_status = gfx_mono_spinctrl_spincollection_process_key(&spinners, input.keycode, spinner_results);
		menu_status = gfx_mono_menu_process_key(&mymenu, input.keycode);
	//} while (spinner_status != GFX_MONO_SPINCTRL_EVENT_FINISH);
	} while (menu_status == GFX_MONO_MENU_EVENT_IDLE);

	switch(menu_status) {
		case 0:
			gfx_mono_spinctrl_spincollection_show(&spinners);
			do {
				// Intentionally left empty.
				do {
					keyboard_get_key_state(&input);
					// Wait for key release
				} while (input.type != KEYBOARD_RELEASE);
				spinner_status = gfx_mono_spinctrl_spincollection_process_key(&spinners, input.keycode, spinner_results);
				} while (spinner_status != GFX_MONO_SPINCTRL_EVENT_FINISH);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			// Return to the main menu on unknown element or back key
			return;
		}
}
