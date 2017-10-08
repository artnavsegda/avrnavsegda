#include <asf.h>
#include <string.h>

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 9600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

/** Size of the file to write/read. */
#define DATA_SIZE 2048

/** Test settings: Number of bytes to test */
#define TEST_SIZE   (4 * 1024)

/** Example header */
#define STRING_EOL    "\r"
#ifndef BOARD_NAME
# define BOARD_NAME "Undefined"
#endif
#define STRING_HEADER "-- FatFS Example --\r\n" \
"-- "BOARD_NAME " --\r\n" \
"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL
#define MENU_HEADER "\n\r" \
"---------------------------------------------------------\n\r"

/* Read/write buffer */
static uint8_t data_buffer[DATA_SIZE];

/**
 * \brief Scan files under a certain path.
 *
 * \param path Folder path.
 *
 * \return Scan result, 1: success.
 */
static FRESULT scan_files(char *path)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int32_t i;
	char *pc_fn;
#if _USE_LFN
	char c_lfn[_MAX_LFN + 1];
	fno.lfname = c_lfn;
	fno.lfsize = sizeof(c_lfn);
#endif

	/* Open the directory */
	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		i = strlen(path);
		for (;;) {
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) {
				break;
			}

#if _USE_LFN
			pc_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			pc_fn = fno.fname;
#endif
			if (*pc_fn == '.') {
				continue;
			}

			/* Check if it is a directory type */
			if (fno.fattrib & AM_DIR) {
				sprintf(&path[i], "/%s", pc_fn);
				res = scan_files(path);
				if (res != FR_OK) {
					break;
				}

				path[i] = 0;
			} else {
				printf("%s/%s\n\r", path, pc_fn);
			}
		}
	}

	return res;
}

/**
 * \brief Do file system tests.
 *
 * \param disk_dev_num disk number
 *
 * \return Test result, 1: success.
 */
static uint8_t run_fatfs_test(uint32_t disk_dev_num)
{
	uint32_t i;
	UINT byte_to_read;
	UINT byte_read;
#if _FS_READONLY == 0
	UINT byte_written;
#endif

	FRESULT res;
	DIR dirs;
	TCHAR root_directory[3] = "0:";
	/* File name to be validated */
	TCHAR file_name[12] = "0:Basic.bin";

	/* Declare these as static to avoid stack usage.
	 * They each contain an array of maximum sector size.
	 */
	static FATFS fs;
	static FIL file_object;

	root_directory[0] = '0' + disk_dev_num;
	file_name[0] = '0' + disk_dev_num;

	/* Mount disk*/
	printf("-I- Mount disk %d\n\r", (int)disk_dev_num);
	/* Clear file system object */
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(disk_dev_num, &fs);
	if (res != FR_OK) {
		printf("-E- f_mount pb: 0x%X\n\r", res);
		return 0;
	}

	/* Test if the disk is formatted */
	res = f_opendir(&dirs, root_directory);
	if (res == FR_OK) {
		/* Erase sd card to reformat it ? */
		puts("-I- The disk is already formatted.\r");

		/* Display the file tree */
		puts("-I- Display files contained in the memory :\r");
		strcpy((char *)data_buffer, root_directory);
		scan_files((char *)data_buffer);

#if _FS_TINY == 0
		puts("-I- The disk will be re-formatted.\r");

		res = FR_NO_FILESYSTEM;
#endif
	}

	if (res == FR_NO_FILESYSTEM) {
#if _FS_READONLY == 0
		/* Format disk */
		printf("-I- Format disk %d\n\r", (int)disk_dev_num);
		puts("-I- Please wait a moment during formatting...\r");
		res = f_mkfs(disk_dev_num, /* Drv */
				0, /* FDISK partition */
				512); /* AllocSize */
		puts("-I- Disk format finished !\r");
		if (res != FR_OK) {
			printf("-E- f_mkfs pb: 0x%X\n\r", res);
			return 0;
		}

#else
		puts("-I- Please run Full version FAT FS test first\r");
		return 0;
#endif
	} else if (FR_OK != res) {
		printf("-E- f_opendir pb: 0x%X\n\r", res);
		return 0;
	}

#if _FS_READONLY == 0
	/* Create a new file */
	printf("-I- Create a file : \"%s\"\n\r", file_name);
	res = f_open(&file_object, (char const *)file_name,
			FA_CREATE_ALWAYS | FA_WRITE);
	if (res != FR_OK) {
		printf("-E- f_open create pb: 0x%X\n\r", res);
		return 0;
	}

	/* Write a checkerboard pattern in the buffer */
	for (i = 0; i < sizeof(data_buffer); i++) {
		if ((i & 1) == 0) {
			data_buffer[i] = (i & 0x55);
		} else {
			data_buffer[i] = (i & 0xAA);
		}
	}
	puts("-I- Write file\r");
	for (i = 0; i < TEST_SIZE; i += DATA_SIZE) {
		res = f_write(&file_object, data_buffer, DATA_SIZE,
				&byte_written);

		if (res != FR_OK) {
			printf("-E- f_write pb: 0x%X\n\r", res);
			return 0;
		}
	}

	/* Close the file */
	puts("-I- Close file\r");
	res = f_close(&file_object);
	if (res != FR_OK) {
		printf("-E- f_close pb: 0x%X\n\r", res);
		return 0;
	}
#endif

	/* Open the file */
	printf("-I- Open the same file : \"%s\"\n\r", file_name);
	res = f_open(&file_object, (char const *)file_name,
			FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK) {
		printf("-E- f_open read pb: 0x%X\n\r", res);
		return 0;
	}

	/* Read file */
	puts("-I- Read file\r");
	memset(data_buffer, 0, DATA_SIZE);
	byte_to_read = file_object.fsize;

	for (i = 0; i < byte_to_read; i += DATA_SIZE) {
		res = f_read(&file_object, data_buffer, DATA_SIZE, &byte_read);
		if (res != FR_OK) {
			printf("-E- f_read pb: 0x%X\n\r", res);
			return 0;
		}
	}

	/* Close the file*/
	puts("-I- Close file\r");
	res = f_close(&file_object);
	if (res != FR_OK) {
		printf("-E- f_close pb: 0x%X\n\r", res);
		return 0;
	}

	/* Compare the read data with the expected data */
	for (i = 0; i < sizeof(data_buffer); i++) {
		if (!((((i & 1) == 0) && (data_buffer[i] == (i & 0x55))) ||
				(data_buffer[i] == (i & 0xAA)))) {
			printf(
					"Invalid data at data[%u] (expected 0x%02X, read 0x%02X)\n\r",
					(unsigned int)i,
					(unsigned int)(((i & 1) == 0) ? (i & 0x55) : (i & 0xAA)),
					(unsigned int)data_buffer[i]);
		}
	}
	puts("-I- File data Ok !\r");

	return 1;
}

int main (void)
{
	uint32_t disk_dev_num;
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	board_init();
	rtc_init();
	stdio_serial_init(&USARTC0, &usart_serial_options);
	printf("serial start\n\r");
	sd_mmc_init();
	printf("sd mmc start\n\r");

	/* Insert application code here, after the board has been initialized. */
	for (disk_dev_num = 0; disk_dev_num < get_nb_lun(); disk_dev_num++) {
		if (run_fatfs_test(disk_dev_num)) {
			printf("-I- DISK %d Test passed !\n\r\n\r", (int)disk_dev_num);
			} else {
			printf("-F- DISK %d Test Failed !\n\r\n\r", (int)disk_dev_num);
		}
	}

	while (1) {
		/* Do nothing */
	}
}
