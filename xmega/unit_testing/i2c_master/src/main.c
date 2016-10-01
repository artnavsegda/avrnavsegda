#include <asf.h>
#include "i2c.h"
#include "setup.h"

//holding register addressing
#define ELEMENTALMERCURYROW 128
#define TOTALMERCURYROW 129
#define OXIDIZEDMERCURYROW 130
#define MONITORFLOW 131
#define VACUUM 132
#define DILUTIONPRESSURE 133
#define BYPASSPRESSURE 134
#define TEMPERATUREOFSPECTROMETER 135
#define CODEOFACURRENTMODE 136
#define ERRORSANDWARNINGS 137
#define TOTALMERCURYCOEFFICENT 138

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	i2c_send_double(&TWIE, 0x08, TOTALMERCURYROW, 0.4554);
	i2c_send_double(&TWIE, 0x08, MONITORFLOW, 0.656375);
}
