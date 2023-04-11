// Critical Section
#include "cmsis_compiler.h"
#define UTILS_ENTER_CRITICAL_SECTION()      \
    uint32_t primask_bit = __get_PRIMASK(); \
    __disable_irq()
#define UTILS_EXIT_CRITICAL_SECTION() __set_PRIMASK(primask_bit)

// Debugging in Low Power Stop Mode

void HAL_DBGMCU_EnableDBGStopMode(void) {
    LL_DBGMCU_EnableDBGStopMode();
}

void HAL_DBGMCU_DisableDBGStopMode(void) {
    LL_DBGMCU_DisableDBGStopMode();
}


#include "stm32_systime.h" // time functions - to check if they use the RTC


HAL_SUBGHZ_Init(SUBGHZ_HandleTypeDef *hsubghz) // this is the init of the SX126 and its interface...

void              SUBGHZSPI_Init(uint32_t BaudratePrescaler);
void              SUBGHZSPI_DeInit(void);
HAL_StatusTypeDef SUBGHZSPI_Transmit(SUBGHZ_HandleTypeDef *hsubghz, uint8_t Data);
HAL_StatusTypeDef SUBGHZSPI_Receive(SUBGHZ_HandleTypeDef *hsubghz, uint8_t *pData);
HAL_StatusTypeDef SUBGHZ_WaitOnBusy(SUBGHZ_HandleTypeDef *hsubghz);
HAL_StatusTypeDef SUBGHZ_CheckDeviceReady(SUBGHZ_HandleTypeDef *hsubghz);


https://github.com/DuncanAitken/OSBMS/tree/c25ba68eccbb28b95b95f3d3b01bac5ae3d5f12d/BatteryLookup

/***********************************************************
	A lookup table where the first column is the cell oc mv,
	and the second column is the SoC; 0 - 10,000
	These are not evenly spaced but located at inflexion points
	where the slope of the graph changes.
	This gives a better fit with fewer data points.
***********************************************************/
const uint16_t	u16a_lifepo4_soc[OCV_NUM_ENTRIES][2]	= {
#if (DATASET == LFP_A123)				// Based on data from A123
		//	mV,		SoC
		{	2900,	0	 	},
		{	3205,	1000 	},
		{	3268,	2500	},
		{	3300,	4000	},	// start of flat region 1
		{	3315,	6500	},	// end of flat region 1
		{	3340,	7500	},	// start of flat region 2
		{	3349,	8900	},	// end of flat region 2
		{	3390,	9800	},
		{	3400,	10000	}
#elif (DATASET == LFP_TI)			// based on some TI data but unsure of cell manufacturer
		//	mV,		SoC
		{	2650,	0	 	},
		{	3130,	600 	},
		{	3270,	2600	},
		{	3290,	4000	},	// start of flat region 1
		{	3300,	6000	},	// end of flat region 1
		{	3325,	7500	},	// start of flat region 2
		{	3345,	9400	},	// end of flat region 2
		{	3390,	9800	},
		{	3500,	10000	}
#elif (DATASET == LFP_SP)			// Based on Thundersky data which became SinoPoly
		//	mV,		SoC
		{	2800,	0	 	},
		{	3184,	1000 	},
		{	3235,	2500	},
		{	3271,	4000	},	// start of flat region 1
		{	3294,	6500	},	// end of flat region 1
		{	3307,	7500	},	// start of flat region 2
		{	3324,	8900	},	// end of flat region 2
		{	3385,	9800	},
		{	3400,	10000	}
#else
#error Dataset not defined
#endif
};
