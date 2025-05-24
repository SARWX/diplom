#include "deca_device_api.h"
#include "deca_regs.h"
#include "sleep.h"
#include "port.h"

/* Example application name and version to display on LCD screen. */
#define APP_NAME "SIMPLE TX v1.2"

/* Default communication configuration. We use here EVK1000's default mode (mode 3). */
static dwt_config_t config = {
    2,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_1024,   /* Preamble length. Used in TX only. */
    DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. */
    9,               /* TX preamble code. Used in TX only. */
    9,               /* RX preamble code. Used in RX only. */
    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_110K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

uint32_t SystemCoreClock = 20000000;		// Без этого не компилится

/* The frame sent in this example is an 802.15.4e standard blink. It is a 12-byte frame composed of the following fields:
 *     - byte 0: frame type (0xC5 for a blink).
 *     - byte 1: sequence number, incremented for each new frame.
 *     - byte 2 -> 9: device ID, see NOTE 1 below.
 *     - byte 10/11: frame check-sum, automatically set by DW1000.  */
static uint8 tx_msg[] = {0xC5, 0, 'D', 'E', 'C', 'A', 'W', 'A', 'V', 'E', 0, 0};
/* Index to access to sequence number of the blink frame in the tx_msg array. */
#define BLINK_FRAME_SN_IDX 1

/* Inter-frame delay period, in milliseconds. */
#define TX_DELAY_MS 1000



#ifdef USE_FULL_ASSERT
	void assert_failed(uint8_t* file, uint32_t line)
	{
	  while (1)
		  ;
	}
#endif


/**
 * Application entry point.
 */
int main(void)
{
    /* Start with board specific hardware init. */
    peripherals_init();


    /* Reset and initialise DW1000. See NOTE 2 below.
     * For initialisation, DW1000 clocks must be temporarily set to crystal speed. After initialisation SPI rate can be increased for optimum
     * performance. */
    reset_DW1000(); /* Target specific drive of RSTn line into DW1000 low for a period. */
    spi_set_rate_low();
    if (dwt_initialise(DWT_LOADNONE) == DWT_ERROR)
     {
        // lcd_display_str("INIT FAILED");
        while (1)
        { };
    }
    spi_set_rate_high();

    /* Configure DW1000. See NOTE 3 below. */
    dwt_configure(&config);

    // Пробуем GPIO
    dwt_enablegpioclocks();
    dwt_setgpiodirection(DWT_GxM0 | DWT_GxM1 | DWT_GxM2, DWT_GxM3);

    /* Loop forever sending frames periodically. */
    while(1)
    {
        // dwt_setgpiovalue(DWT_GxM0 | DWT_GxM1 | DWT_GxM2 | DWT_GxM3, DWT_GxP0 | DWT_GxP1 | DWT_GxP2 | DWT_GxP3); /* set GPIO2 high (LED4 will light up)*/
        dwt_setleds(DWT_LEDS_ENABLE);       // configure tx / rx leds
        /* Write frame data to DW1000 and prepare transmission. See NOTE 4 below.*/
        dwt_writetxdata(sizeof(tx_msg), tx_msg, 0); /* Zero offset in TX buffer. */
        dwt_writetxfctrl(sizeof(tx_msg), 0, 0); /* Zero offset in TX buffer, no ranging. */

        /* Start transmission. */
        dwt_starttx(DWT_START_TX_IMMEDIATE);

        /* Poll DW1000 until TX frame sent event set. See NOTE 5 below.
         * STATUS register is 5 bytes long but, as the event we are looking at is in the first byte of the register, we can use this simplest API
         * function to access it.*/
        while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS))
        { };

        /* Clear TX frame sent event. */
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);

        /* Execute a delay between transmissions. */
        sleep_ms(TX_DELAY_MS/2);
        dwt_setgpiovalue(DWT_GxM0 | DWT_GxM1 | DWT_GxM2 | DWT_GxM3, 0); /* set GPIO2 low (LED4 will be off)*/
        sleep_ms(TX_DELAY_MS/2);

        /* Increment the blink frame sequence number (modulo 256). */
        tx_msg[BLINK_FRAME_SN_IDX]++;
    }
}
