#include <asf.h>
#include "conf_usb.h"

static volatile bool main_b_vendor_enable = false;

#define  MAIN_LOOPBACK_SIZE    12
COMPILER_WORD_ALIGNED static uint8_t main_buf_loopback[MAIN_LOOPBACK_SIZE];
COMPILER_WORD_ALIGNED static uint8_t main_buf_answer[MAIN_LOOPBACK_SIZE] = "hEllo world";

void main_vendor_int_in_received(udd_ep_status_t status, iram_size_t nb_transfered, udd_ep_id_t ep);
void main_vendor_int_out_received(udd_ep_status_t status, iram_size_t nb_transfered, udd_ep_id_t ep);

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 9600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

int main(void)
{
	uint16_t adcdata;
	irq_initialize_vectors();
	cpu_irq_enable();

	sysclk_init();
	board_init();
	// replace everything with simple port assignment pls
    ioport_configure_port_pin(&PORTC, PIN4_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
    ioport_configure_port_pin(&PORTC, PIN5_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
    //ioport_configure_port_pin(&PORTC, PIN6_bm, IOPORT_DIR_INPUT);
    ioport_configure_port_pin(&PORTC, PIN7_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	// okay ?
	stdio_serial_init(&USARTC0, &usart_serial_options);
	spi_master_init(&SPIC);
	spi_enable(&SPIC);
	udc_start();

	printf("\n\rMCU started\n\r");
	spi_write_packet(&SPIC, "\xFF\xFF\xFF\xFF\xFF", 5);
	printf("Reset complete\n\r");
	spi_write_packet(&SPIC, "\x20\x0C\x10\x40", 4);
	
	while (true)
	{
		delay_ms(100);
		spi_write_packet(&SPIC, "\x38", 1);
		spi_read_packet(&SPIC, (uint8_t *)&adcdata, 2);
		printf("%04X\r\n",adcdata);
	}
}

void main_suspend_action(void)
{
	;
}

void main_resume_action(void)
{
	;
}

void main_sof_action(void)
{
	if (!main_b_vendor_enable)
		return;
}

bool main_vendor_enable(void)
{
	main_b_vendor_enable = true;
	main_vendor_int_in_received(UDD_EP_TRANSFER_OK, 0, 0);
	return true;
}

void main_vendor_disable(void)
{
	main_b_vendor_enable = false;
}

bool main_setup_out_received(void)
{
	udd_g_ctrlreq.payload = main_buf_loopback;
	udd_g_ctrlreq.payload_size = min(udd_g_ctrlreq.req.wLength,	sizeof(main_buf_loopback));
	return true;
}

bool main_setup_in_received(void)
{
	udd_g_ctrlreq.payload = main_buf_loopback;
	udd_g_ctrlreq.payload_size = min( udd_g_ctrlreq.req.wLength, sizeof(main_buf_loopback) );
	return true;
}

void main_vendor_int_in_received(udd_ep_status_t status,
		iram_size_t nb_transfered, udd_ep_id_t ep)
{
	UNUSED(nb_transfered);
	UNUSED(ep);
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Transfer aborted, then stop loopback
	}
	// Wait a full buffer
	udi_vendor_interrupt_out_run(main_buf_loopback,	sizeof(main_buf_loopback), main_vendor_int_out_received);
}

void main_vendor_int_out_received(udd_ep_status_t status,
		iram_size_t nb_transfered, udd_ep_id_t ep)
{
	UNUSED(ep);
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Transfer aborted, then stop loopback
	}
	// Send on IN endpoint the data received on endpoint OUT
	udi_vendor_interrupt_in_run(main_buf_answer, nb_transfered,	main_vendor_int_in_received);
}

