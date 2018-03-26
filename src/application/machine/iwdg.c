/*
 * iwdg.c
 *
 *  Created on: 28.02.2018
 *      Author: kamil
 */

#include <hal.h>

#ifdef HAL_IWDG_MODULE_ENABLED

#include "machine/iwdg.h"

#include "machine/mx.h" // hiwdg

void HW_IWDG_refresh()
{
	HAL_try( HAL_IWDG_Refresh(&hiwdg) );
}

bool HW_IWDG_test()
{
	return __HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST);
}

/*
 * IWDG_calc_period
 * @return period in seconds
 *
 * LSI_freq may be not calibrated, so it's value may be between -30% to +50% it's nominal value.
 * So LSI_freq is between 32*70% kHz to 32*150%kHz
 * Following code in octave calculates minimum and maximum IWDG intervals depending on prescaler value,
 *   for different minimum, nominal and maximum LSI_freq values when IWDG reload value is equal to 0xFFF.

octave:25> format short g
octave:28> [ 1 , 1 ./ ( 32000 .* [70,100,150] ./ 100) .* 2^12 ] .* [1;2;4;8;16;32;64;128;256]
ans =

           1     0.18286       0.128    0.085333
           2     0.36571       0.256     0.17067
           4     0.73143       0.512     0.34133
           8      1.4629       1.024     0.68267
          16      2.9257       2.048      1.3653
          32      5.8514       4.096      2.7307
          64      11.703       8.192      5.4613
         128      23.406      16.384      10.923
         256      46.811      32.768      21.845
                                        ^^^^^^^^ - minimum IWDG in seconds , LSI_freq = 32*70% kHz
                              ^^^^^^             - nominal IWDG in seconds , LSI_freq = 32 kHz
                 ^^^^^^^                         - maximum IWDG in seconds , LSI_freq = 32*150% kHz
         ^^^                                     - prescaler value
Reload value is equal to 0xfff = 2^12.
 *
 */
double HW_IWDG_calc_period(uint32_t Prescaler, uint32_t Period, double *min, double *max)
{
	assert(IS_IWDG_PRESCALER(Prescaler));
	assert(IS_IWDG_RELOAD(Period));

	double scale = 0;
	switch(Prescaler) {
#ifdef IWDG_PRESCALER_1
	case IWDG_PRESCALER_1: scale = 1; break;
#endif
#ifdef IWDG_PRESCALER_2
	case IWDG_PRESCALER_2: scale = 2; break;
#endif
	case IWDG_PRESCALER_4: scale = 4; break;
	case IWDG_PRESCALER_8: scale = 8; break;
	case IWDG_PRESCALER_16: scale = 16; break;
	case IWDG_PRESCALER_32: scale = 32; break;
	case IWDG_PRESCALER_64: scale = 64; break;
	case IWDG_PRESCALER_128: scale = 128; break;
	case IWDG_PRESCALER_256: scale = 256; break;
	default: assert(0); break;
	}

	const double lsi = LSI_VALUE;
	const double ret = Period * scale / lsi;
	if (min) {
		const double min_modifier = 70./100.;
		const double ret_min = ret * min_modifier;
		*min = ret_min;
	}
	if (max) {
		const double max_modifier = 150./100.;
		const double ret_max = ret * max_modifier;
		*max = ret_max;
	}
	return ret;
}

#else

bool HW_IWDG_test()
{
	return false;
}

void HW_IWDG_refresh()
{

}

#endif
