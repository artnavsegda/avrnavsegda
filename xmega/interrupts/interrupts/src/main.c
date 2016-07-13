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

PROGMEM_DECLARE(char const, spinnertitle[]) = "Start time:";
PROGMEM_DECLARE(char const, spinnertitle2[]) = "Cell delay:";
PROGMEM_DECLARE(char const, spinnertitle3[]) = "Cell time:";
PROGMEM_DECLARE(char const, spinnertitle4[]) = "Zero delay:";
PROGMEM_DECLARE(char const, spinnertitle5[]) = "Zero time:";
PROGMEM_DECLARE(char const, spinnertitle6[]) = "Total delay:";
PROGMEM_DECLARE(char const, spinnertitle7[]) = "Total time:";
PROGMEM_DECLARE(char const, spinnertitle8[]) = "Ele delay:";
PROGMEM_DECLARE(char const, spinnertitle9[]) = "Ele time:";
PROGMEM_DECLARE(char const, spinnertitle10[]) = "Precal del:";
PROGMEM_DECLARE(char const, spinnertitle11[]) = "Cal time:";
PROGMEM_DECLARE(char const, spinnertitle12[]) = "Postcal del:";
PROGMEM_DECLARE(char const, spinnertitle13[]) = "Purge tim:";
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
struct gfx_mono_spinctrl spinner4;
struct gfx_mono_spinctrl spinner5;
struct gfx_mono_spinctrl spinner6;
struct gfx_mono_spinctrl spinner7;
struct gfx_mono_spinctrl spinner8;
struct gfx_mono_spinctrl spinner9;
struct gfx_mono_spinctrl spinner10;
struct gfx_mono_spinctrl spinner11;
struct gfx_mono_spinctrl spinner12;
struct gfx_mono_spinctrl spinner13;
struct gfx_mono_spinctrl_spincollection spinners;
struct gfx_mono_spinctrl_spincollection spinners2;
struct gfx_mono_spinctrl_spincollection spinners3;
struct gfx_mono_spinctrl_spincollection spinners4;
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
		case 1:
			mode = 2;
			gfx_mono_spinctrl_spincollection_show(&spinners2);
			break;
		case 2:
			mode = 3;
			gfx_mono_spinctrl_spincollection_show(&spinners3);
			break;
		case 3:
			mode = 4;
			gfx_mono_spinctrl_spincollection_show(&spinners4);
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
	case 2:
		spinner_status = gfx_mono_spinctrl_spincollection_process_key(&spinners2, keycode, spinner_results);
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
	case 3:
		spinner_status = gfx_mono_spinctrl_spincollection_process_key(&spinners3, keycode, spinner_results);
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
	case 4:
		spinner_status = gfx_mono_spinctrl_spincollection_process_key(&spinners4, keycode, spinner_results);
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
	gfx_mono_spinctrl_init(&spinner4, SPINTYPE_INTEGER,	spinnertitle4, NULL, -60, -41, 0);
	gfx_mono_spinctrl_init(&spinner5, SPINTYPE_INTEGER,	spinnertitle5, NULL, -60, -41, 0);
	gfx_mono_spinctrl_init(&spinner6, SPINTYPE_INTEGER,	spinnertitle6, NULL, -60, -41, 0);
	gfx_mono_spinctrl_init(&spinner7, SPINTYPE_INTEGER,	spinnertitle7, NULL, -60, -41, 0);
	gfx_mono_spinctrl_init(&spinner8, SPINTYPE_INTEGER,	spinnertitle8, NULL, -60, -41, 0);
	gfx_mono_spinctrl_init(&spinner9, SPINTYPE_INTEGER,	spinnertitle9, NULL, -60, -41, 0);
	gfx_mono_spinctrl_init(&spinner10, SPINTYPE_INTEGER, spinnertitle10, NULL, -60, -41, 0);
	gfx_mono_spinctrl_init(&spinner11, SPINTYPE_INTEGER, spinnertitle11, NULL, -60, -41, 0);
	gfx_mono_spinctrl_init(&spinner12, SPINTYPE_INTEGER, spinnertitle12, NULL, -60, -41, 0);
	gfx_mono_spinctrl_init(&spinner13, SPINTYPE_INTEGER, spinnertitle13, NULL, -60, -41, 0);

	// Initialize spincollection
	gfx_mono_spinctrl_spincollection_init(&spinners);
	gfx_mono_spinctrl_spincollection_init(&spinners2);
	gfx_mono_spinctrl_spincollection_init(&spinners3);
	gfx_mono_spinctrl_spincollection_init(&spinners4);

	// Add spinners to spincollection
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner1, &spinners);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner2, &spinners);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner3, &spinners);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner4, &spinners2);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner5, &spinners2);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner6, &spinners2);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner7, &spinners3);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner8, &spinners3);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner9, &spinners3);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner10, &spinners4);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner11, &spinners4);
	gfx_mono_spinctrl_spincollection_add_spinner(&spinner12, &spinners4);

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		delay_s(1);
	}
}
