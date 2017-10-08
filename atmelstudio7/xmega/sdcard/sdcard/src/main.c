#include <asf.h>
#include <string.h>
#include "sd_mmc_protocol.h"

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 9600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

//! \name Read/write access configurations
//! @{

//! Offset before the end of memory card to start the test
#define TEST_MEM_START_OFFSET  (1024lu * 1024lu * 4lu) // 4MB

//! Memory area size dedicated for the read/write test
#define TEST_MEM_AREA_SIZE (1024lu * 1024lu * 1lu) // 1MB
/**
 * Size of each read or write access.
 * Increasing this number can get higher R/W performance.
 */
#define TEST_MEM_ACCESS_SIZE  (8lu * SD_MMC_BLOCK_SIZE)

#if TEST_MEM_START_OFFSET < TEST_MEM_AREA_SIZE
#  error TEST_MEM_START_OFFSET must be higher than TEST_MEM_AREA_SIZE
#endif
#if TEST_MEM_ACCESS_SIZE > TEST_MEM_AREA_SIZE
#  error TEST_MEM_AREA_SIZE must be higher than TEST_MEM_ACCESS_SIZE
#endif

//! The value used to generate test data
#define TEST_FILL_VALUE_U32     (0x5500AAFFU)

//! Buffer used by read/write tests
COMPILER_WORD_ALIGNED
static uint8_t buf_test[TEST_MEM_ACCESS_SIZE];

//! Read and write test length of CIA in bytes
#define TEST_CIA_SIZE           (0x16)

//! Buffer for test SDIO data
static uint8_t buf_cia[TEST_CIA_SIZE];
//! @}

static void main_display_info_card(uint8_t slot);
static void main_test_memory(uint8_t slot);
static void main_test_sdio(uint8_t slot);
static void main_dump_buffer(uint8_t *data_buffer, uint32_t length);

int main (void)
{
	uint8_t slot = 0;
	sd_mmc_err_t err;
	/* Insert system clock initialization code here (sysclk_init()). */

	irq_initialize_vectors();
	cpu_irq_enable();
	sysclk_init();
	board_init();
	rtc_init();
	stdio_serial_init(&USARTC0, &usart_serial_options);
	printf("serial start\n\r");
	sd_mmc_init();
	printf("sd mmc start\n\r");

	/* Insert application code here, after the board has been initialized. */
	printf("\x0C\n\r-- SD/MMC/SDIO Card Example --\n\r");
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
	while (1) {
		if (slot == sd_mmc_nb_slot()) {
			slot = 0;
		}
		printf("Please plug an SD, MMC or SDIO card in slot %d.\n\r", slot+1);

		// Wait for a card and ready
		do {
			err = sd_mmc_check(slot);
			if ((SD_MMC_ERR_NO_CARD != err)
			&& (SD_MMC_INIT_ONGOING != err)
			&& (SD_MMC_OK != err)) {
				printf("Card install FAILED\n\r");
				printf("Please unplug and re-plug the card.\n\r");
				while (SD_MMC_ERR_NO_CARD != sd_mmc_check(slot)) {
				}
			}
		} while (SD_MMC_OK != err);
		
		// Display basic card information
		main_display_info_card(slot);
		
		/* Test the card */
		if (sd_mmc_get_type(slot) & CARD_TYPE_SDIO) {
			// Test CIA of SDIO card
			main_test_sdio(slot);
		}
		if (sd_mmc_get_type(slot) & (CARD_TYPE_SD | CARD_TYPE_MMC)) {
			// SD/MMC Card R/W
			main_test_memory(slot);
		}

		printf("Test finished, please unplugged the card.\n\r");
		while (SD_MMC_OK == sd_mmc_check(slot)) {
		}
		slot++;
	}
}

/**
 * \brief Display basic information of the card.
 * \note This function should be called only after the card has been
 *       initialized successfully.
 *
 * \param slot   SD/MMC slot to test
 */
static void main_display_info_card(uint8_t slot)
{
	printf("Card information:\n\r");

	printf("    ");
	switch (sd_mmc_get_type(slot)) {
	case CARD_TYPE_SD | CARD_TYPE_HC:
		printf("SDHC");
		break;
	case CARD_TYPE_SD:
		printf("SD");
		break;
	case CARD_TYPE_MMC | CARD_TYPE_HC:
		printf("MMC High Density");
		break;
	case CARD_TYPE_MMC:
		printf("MMC");
		break;
	case CARD_TYPE_SDIO:
		printf("SDIO\n\r");
		return;
	case CARD_TYPE_SD_COMBO:
		printf("SD COMBO");
		break;
	case CARD_TYPE_UNKNOWN:
	default:
		printf("Unknow\n\r");
		return;
	}
	printf("\n\r    %d MB\n\r", (uint16_t)(sd_mmc_get_capacity(slot)/1024));
}

/**
 * \brief Card R/W tests
 *
 * \param slot   SD/MMC slot to test
 */
static void main_test_memory(uint8_t slot)
{
	uint32_t last_blocks_addr, i, nb_trans;
	uint32_t tick_start, time_ms;

	// Compute the last address
	last_blocks_addr = sd_mmc_get_capacity(slot) *
			(1024 / SD_MMC_BLOCK_SIZE);
	if (last_blocks_addr < (TEST_MEM_START_OFFSET / 512lu)) {
		printf("[Memory is too small.]\n\r");
		return;
	}
	last_blocks_addr -= (TEST_MEM_START_OFFSET / SD_MMC_BLOCK_SIZE);

	printf("Card R/W test:\n\r");

	// Read the last block
	printf("    Read... ");
	tick_start = 0;
	if (SD_MMC_OK != sd_mmc_init_read_blocks(slot,
			last_blocks_addr,
			TEST_MEM_AREA_SIZE / SD_MMC_BLOCK_SIZE)) {
		printf("[FAIL]\n\r");
		return;
	}
	for (nb_trans = 0; nb_trans < (TEST_MEM_AREA_SIZE / TEST_MEM_ACCESS_SIZE);
			nb_trans++) {
		if (SD_MMC_OK != sd_mmc_start_read_blocks(buf_test,
					TEST_MEM_ACCESS_SIZE / SD_MMC_BLOCK_SIZE)) {
			printf("[FAIL]\n\r");
			return;
		}
		if (SD_MMC_OK != sd_mmc_wait_end_of_read_blocks(false)) {
			printf("[FAIL]\n\r");
			return;
		}		
	}
	time_ms = 0;
	if (time_ms) { // Valid time_ms
		printf(" %d KBps ", (int)(((TEST_MEM_AREA_SIZE
				* 1000lu) / 1024lu) / time_ms));
	}
	printf("[OK]\n\r");

	if (sd_mmc_is_write_protected(slot)) {
		printf("Card is write protected [WRITE TEST SKIPPED]\n\r");
		return;
	}
	
	// Fill buffer with a pattern
	for (i = 0; i < (TEST_MEM_ACCESS_SIZE / sizeof(uint32_t)); i++) {
		((uint32_t*)buf_test)[i] = TEST_FILL_VALUE_U32;
	}

	printf("    Write pattern... ");
	if (SD_MMC_OK != sd_mmc_init_write_blocks(slot,
			last_blocks_addr,
			TEST_MEM_AREA_SIZE / SD_MMC_BLOCK_SIZE)) {
		printf("[FAIL]\n\r");
		return;
	}
	tick_start = 0;
	for (nb_trans = 0; nb_trans < (TEST_MEM_AREA_SIZE / TEST_MEM_ACCESS_SIZE);
			nb_trans++) {
		((uint32_t*)buf_test)[0] = nb_trans; // Unique value for each area
		if (SD_MMC_OK != sd_mmc_start_write_blocks(buf_test,
				TEST_MEM_ACCESS_SIZE / SD_MMC_BLOCK_SIZE)) {
			printf("[FAIL]\n\r");
			return;
		}
		if (SD_MMC_OK != sd_mmc_wait_end_of_write_blocks(false)) {
			printf("[FAIL]\n\r");
			return;
		}
	}
	time_ms = 0;
	if (time_ms) { // Valid time_ms
		printf(" %d KBps ", (int)(((TEST_MEM_AREA_SIZE
				* 1000lu) / 1024lu) / time_ms));
	}
	printf("[OK]\n\r");

	printf("    Read and check pattern... ");
	if (SD_MMC_OK != sd_mmc_init_read_blocks(slot,
			last_blocks_addr,
			TEST_MEM_AREA_SIZE / SD_MMC_BLOCK_SIZE)) {
		printf("Read [FAIL]\n\r");
		return;
	}
	for (nb_trans = 0; nb_trans < (TEST_MEM_AREA_SIZE / TEST_MEM_ACCESS_SIZE);
			nb_trans++) {
		// Clear buffer
		for (i = 0; i < (TEST_MEM_ACCESS_SIZE / sizeof(uint32_t)); i++) {
			((uint32_t*)buf_test)[i] = 0xFFFFFFFF;
		}
		// Fill buffer
		if (SD_MMC_OK != sd_mmc_start_read_blocks(buf_test,
					TEST_MEM_ACCESS_SIZE / SD_MMC_BLOCK_SIZE)) {
			printf("Read [FAIL]\n\r");
			return;
		}
		if (SD_MMC_OK != sd_mmc_wait_end_of_read_blocks(false)) {
			printf("Read [FAIL]\n\r");
			return;
		}
		// Check the unique value of the area
		if (((uint32_t*)buf_test)[0] != nb_trans) {
			printf("Check [FAIL]\n\r");
			return;
		}
		// Check buffer
		for (i = 1; i < (TEST_MEM_ACCESS_SIZE / sizeof(uint32_t)); i++) {
			if (((uint32_t*)buf_test)[i] != TEST_FILL_VALUE_U32) {
				printf("Check [FAIL]\n\r");
				return;
			}
		}
	}
	printf("[OK]\n\r");
}

/**
 * \brief Perform test on CIA (Common I/O Area) of SDIO card.
 *
 * \note The Common I/O Area (CIA) shall be implemented on all SDIO cards.
 *
 * \param slot   SD/MMC slot to test
 */
static void main_test_sdio(uint8_t slot)
{
	uint32_t i;
	uint32_t err;

	/*
	 * Test with direct read and write command.
	 */
	printf("\n\r--- Test with direct read and write command of CIA:\n\r");
	/* Read */
	for (i = 0; i < TEST_CIA_SIZE; i++) {
		err = sdio_read_direct(slot, SDIO_CIA, i, &buf_cia[i]);
		if (err) {
			printf("Error: SDIO direct read failed.\n\r");
			return;
		}
	}
	main_dump_buffer(buf_cia, TEST_CIA_SIZE);

	/* Write */
	printf("Write 0x02 to IEN(CIA.4).\n\r");
	err = sdio_write_direct(slot, SDIO_CIA, SDIO_CCCR_IEN, 0x02);
	if (err) {
		printf("Error: SDIO direct write failed.\n\r");
		return;
	}

	/* Check */
	printf("Check IEN after write:");
	err = sdio_read_direct(slot, SDIO_CIA, SDIO_CCCR_IEN,
			&buf_cia[SDIO_CCCR_IEN]);
	if (err) {
		printf("Error: SDIO direct read failed.\n\r");
		return;
	}

	printf("0x%02x\n\r", buf_cia[SDIO_CCCR_IEN]);
	if (0x02 == buf_cia[SDIO_CCCR_IEN]) {
		printf("Test OK.\n\r");
	} else {
		printf("Test failed.\n\r");
	}

	/* Restore data to 0 */
	sdio_write_direct(slot, SDIO_CIA, SDIO_CCCR_IEN, 0);

	/* Clear the buffer. */
	memset(buf_cia, 0x0, sizeof(buf_cia));

	/*
	 *  Test with extended read and write command.
	 */
	printf("\n\r--- Test with extended read and write command of CIA:\n\r");
	/* Read */
	err = sdio_read_extended(slot, SDIO_CIA, 0, 1, &buf_cia[0],
			TEST_CIA_SIZE);
	if (err) {
		printf("Error: SDIO extended read failed.\n\r");
		return;
	}

	main_dump_buffer(buf_cia, TEST_CIA_SIZE);

	/* Write */
	printf("Modify Some R/W bytes for FN0 and write:\n\r");
	buf_cia[SDIO_CCCR_IEN] = 0x3;
	err = sdio_write_extended(slot, SDIO_CIA, SDIO_CCCR_IEN, 1,
			&buf_cia[SDIO_CCCR_IEN], 1);
	if (err) {
		printf("Error: SDIO extended write failed.\n\r");
		return;
	}

	/* Read and check */
	printf("Check CIA after write:\n\r");
	err = sdio_read_extended(slot, SDIO_CIA, 0, 1, &buf_cia[0],
			TEST_CIA_SIZE);
	if (err) {
		printf("Error: SDIO extended read failed.\n\r");
		return;
	}

	main_dump_buffer(buf_cia, TEST_CIA_SIZE);

	if (buf_cia[SDIO_CCCR_IEN] != 0x3) {
		printf("CIA.4 Fail\n\r");
	} else {
		printf("Test OK\n\r");
	}

	/* Restore data to 0 */
	sdio_write_direct(slot, SDIO_CIA, SDIO_CCCR_IEN, 0);

	return;
}

/**
 * \brief Dump and print buffer.
 *
 * \param data_buffer Pointer to data buffer.
 * \param length      Buffer length.
 */
static void main_dump_buffer(uint8_t *data_buffer, uint32_t length)
{
	uint32_t i;

	printf("Dump buffer (length=%d):", (int)length);
	for (i = 0; i < length; i++) {
		if ((i % 16) == 0) { /* Display 16 data per line */
			printf("\n\r%3x:", (int)i);
		}

		printf(" %02x", data_buffer[i]);
	}
	printf("\n\r");
}
