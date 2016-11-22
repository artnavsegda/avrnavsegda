#include <asf.h>
#include <string.h>
#include "settings.h"
#include "i2c.h"

struct mysettingsstruct default_settings = {
	.cell = {
		.left_out = { .address = 0x18, .pin_number = 7 },
		.right_out = { .address = 0x18, .pin_number = 6 },
		.left_in = { .address = 0x18, .pin_number = 5 },
		.right_in = { .address = 0x18, .pin_number = 4 }
	},
	.length_table = {
		.startlevel = 10,
		.celldelay = 10,
		.celllevel = 10,
		.zerodelay = 10,
		.zerotest = 10,
		.totalmercurydelay = 10,
		.totalmercury = 10
	},
	.jump_table = {
		.startlevel = ZERODELAY,
		.zerodelay = ZEROTEST,
		.zerotest = CELLDELAY,
		.celldelay = CELLLEVEL,
		.celllevel = TOTALMERCURYDELAY,
		.totalmercurydelay = TOTALMERCURY,
		.totalmercury = TOTALMERCURY,
	}
};

void sync_array(uint8_t memory, uint8_t *desination, uint8_t *defaults)
{
	void *temporary = malloc(sizeof(desination));
	if (i2c_read_array(&TWIE,0x08,memory,sizeof(desination),temporary) == STATUS_OK)
		memcpy(desination,temporary,sizeof(desination));
	else
		memcpy(desination,defaults,sizeof(desination));
}

void process_settings(struct mysettingsstruct *settings)
{
	/*struct mysettingsstruct temporary_settings;
	if (i2c_read_array(&TWIE,0x08,I2C_IPADDRESS,4,temporary_settings.ip) == STATUS_OK)
		memcpy(settings->ip,&temporary_settings.ip,sizeof(temporary_settings.ip));
	else
		memcpy(settings->ip,&default_settings.ip,sizeof(temporary_settings.ip));*/
	sync_array(I2C_IPADDRESS,settings->ip,&default_settings.ip);
	sync_array(I2C_MACADDRESS,settings->ip,&default_settings.ip);
	//sync_array(I2C_LENGTHTABLE,(uint8_t *)settings->length_table,(uint8_t *)&default_settings.length_table);
	//sync_array(I2C_JUMPTABLE,(uint8_t *)settings->jump_table,(uint8_t *)&default_settings.jump_table);
	//sync_array(I2C_AD7705_SETUP_REGISTER,settings->ad7705_setup_register,&default_settings.ad7705_setup_register);
	//sync_array(I2C_AD7705_CLOCK_REGISTER,settings->ad7705_clock_register,&default_settings.ad7705_clock_register);
}