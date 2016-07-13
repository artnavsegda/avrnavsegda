/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

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

uint8_t menu_status;
uint8_t spinner_status;
int16_t spinner_results[3];
struct gfx_mono_spinctrl spinner1;
struct gfx_mono_spinctrl spinner2;
struct gfx_mono_spinctrl spinner3;
struct gfx_mono_spinctrl_spincollection spinners;
int16_t results[GFX_MONO_SPINCTRL_MAX_ELEMENTS_IN_SPINCOLLECTION];

int mode = 0;

void process_input(uint8_t keycode)
{
	switch (mode)
	{
	case 0:
		menu_status = gfx_mono_menu_process_key(&mymenu, keycode);
		switch (menu_status)
		{
		case 0:
			mode = 1;
			gfx_mono_spinctrl_spincollection_show(&spinners);
			break;
		default:
			break;
		}
		break;
	case 1:
		spinner_status = gfx_mono_spinctrl_spincollection_process_key(&spinners, keycode, spinner_results);
		switch (spinner_status)
		{
		case GFX_MONO_SPINCTRL_EVENT_FINISH:
			mode = 0;
			gfx_mono_menu_init(&mymenu);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

ISR(PORTE_INT0_vect) // sw0
{
	LED_Toggle(LED0);
	process_input(GFX_MONO_MENU_KEYCODE_ENTER);
}

ISR(PORTF_INT0_vect) // sw1
{
	LED_Toggle(LED1);
	process_input(GFX_MONO_MENU_KEYCODE_UP);
}

ISR(PORTF_INT1_vect) //sw2
{
	LED_Toggle(LED2);
	process_input(GFX_MONO_MENU_KEYCODE_DOWN);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	//sysclk_init();
	//pmic_init();
	irq_initialize_vectors()
	board_init();
	ioport_init();
	ioport_set_pin_dir(LED0, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED3, IOPORT_DIR_OUTPUT);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_0, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_2, IOPORT_SENSE_FALLING);
	PORTE.INT0MASK = PIN5_bm;
	PORTF.INT0MASK = PIN1_bm;
	PORTF.INT1MASK = PIN2_bm;
	PORTE.INTCTRL = PORT_INT0LVL_LO_gc;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc;
	cpu_irq_enable();
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

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		delay_s(1);
	}
}
