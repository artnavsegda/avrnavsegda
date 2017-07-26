#include <asf.h>
#include "conf_usb.h"

static volatile bool main_b_vendor_enable = false;

/**
 * \name Buffer for loopback
 */
//@{
//! Size of buffer used for the loopback
#define  MAIN_LOOPBACK_SIZE    12
COMPILER_WORD_ALIGNED
		static uint8_t main_buf_loopback[MAIN_LOOPBACK_SIZE];
COMPILER_WORD_ALIGNED
		static uint8_t main_buf_answer[MAIN_LOOPBACK_SIZE] = "hEllo world";
//@}

void main_vendor_int_in_received(udd_ep_status_t status,
		iram_size_t nb_transfered, udd_ep_id_t ep);
void main_vendor_int_out_received(udd_ep_status_t status,
		iram_size_t nb_transfered, udd_ep_id_t ep);

/*! \brief Main function. Execution starts here.
 */
int main(void)
{
	irq_initialize_vectors();
	cpu_irq_enable();

	// Initialize the sleep manager
	sleepmgr_init();
	sysclk_init();
	board_init();
	//ui_init();

	// Start USB stack to authorize VBus monitoring
	udc_start();

	// The main loop manages only the power mode
	// because the USB management is done by interrupt
	while (true) {
		sleepmgr_enter_sleep();
	}
}

void main_suspend_action(void)
{
	//ui_powerdown();
}

void main_resume_action(void)
{
	//ui_wakeup();
}

void main_sof_action(void)
{
	if (!main_b_vendor_enable)
		return;
	//ui_process(udd_get_frame_number());
}

bool main_vendor_enable(void)
{
	main_b_vendor_enable = true;
	// Start data reception on OUT endpoints
#if UDI_VENDOR_EPS_SIZE_INT_FS
	main_vendor_int_in_received(UDD_EP_TRANSFER_OK, 0, 0);
#endif
	return true;
}

void main_vendor_disable(void)
{
	main_b_vendor_enable = false;
}

bool main_setup_out_received(void)
{
	//ui_loop_back_state(true);
	udd_g_ctrlreq.payload = main_buf_loopback;
	udd_g_ctrlreq.payload_size = min(
			udd_g_ctrlreq.req.wLength,
			sizeof(main_buf_loopback));
	return true;
}

bool main_setup_in_received(void)
{
	//ui_loop_back_state(false);
	udd_g_ctrlreq.payload = main_buf_loopback;
	udd_g_ctrlreq.payload_size =
			min( udd_g_ctrlreq.req.wLength,
			sizeof(main_buf_loopback) );
	return true;
}

#if UDI_VENDOR_EPS_SIZE_INT_FS
void main_vendor_int_in_received(udd_ep_status_t status,
		iram_size_t nb_transfered, udd_ep_id_t ep)
{
	UNUSED(nb_transfered);
	UNUSED(ep);
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Transfer aborted, then stop loopback
	}
	//ui_loop_back_state(false);
	// Wait a full buffer
	udi_vendor_interrupt_out_run(
			main_buf_loopback,
			sizeof(main_buf_loopback),
			main_vendor_int_out_received);
}

void main_vendor_int_out_received(udd_ep_status_t status,
		iram_size_t nb_transfered, udd_ep_id_t ep)
{
	UNUSED(ep);
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Transfer aborted, then stop loopback
	}
	//ui_loop_back_state(true);
	// Send on IN endpoint the data received on endpoint OUT
	udi_vendor_interrupt_in_run(
			main_buf_answer,
			nb_transfered,
			main_vendor_int_in_received);
}
#endif
