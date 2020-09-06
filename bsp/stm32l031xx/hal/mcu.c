/******************************************************************************

 @file  hal.c

 @brief Describe the purpose and contents of the file.

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/


/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_mcu.h"

/* ------------------------------------------------------------------------------------------------
 *                                       Local Prototypes
 * ------------------------------------------------------------------------------------------------
 */

extern void hal_mcu_init( void )
{
    spl_mcu_init();

#if (SPL_UART0_EN > 0)
    spl_uart_init( SPL_UART_PORT_0 );
    spl_uart_open( SPL_UART_PORT_0 );
#endif

#if (SPL_UART1_EN > 0)
    spl_uart_init( SPL_UART_PORT_1 );
    spl_uart_open( SPL_UART_PORT_1 );
#endif

#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C0_SLAVE_EN > 0)
    spl_i2c_init( SPL_I2C_PORT_0 );
    spl_i2c_open( SPL_I2C_PORT_0 );
#endif

#if (SPL_EXTINT_EN > 0)
    spl_extint_init();
#endif

#if (SPL_TIMER_EN > 0)
    spl_timer_init();
#endif
}

#if HAL_MCU_RESET_EN > 0
extern void hal_mcu_reset( void )
{
    spl_mcu_reset();
}
#endif //HAL_MCU_RESET_EN > 0

#if HAL_MCU_DELAYUS_EN > 0
extern void hal_mcu_delayus( uint32_t us)
{
    spl_mcu_delayus(us);
}
#endif //HAL_MCU_DELAYUS_EN > 0


#if SPL_MCU_XTAL_EN > 0
void spl_mcu_init (void)
{   
    /* Unlock protected registers */
    SYS_UnlockReg();
	
	/*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);
	CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);
	
	CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));
	
	CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    CLK_SetCoreClock(50000000);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and cyclesPerUs automatically. */
    SystemCoreClockUpdate();

    /* Enable 12.288MHz Clock output */
    CLK_EnableModuleClock(FDIV_MODULE);
    SYS->P3_MFP &= ~SYS_MFP_P36_Msk;
    SYS->P3_MFP |= SYS_MFP_P36_CKO;
    CLK_EnableCKO(CLK_CLKSEL2_FRQDIV_S_HXT, 0, 1);
    
    /* Enable UART0 */
    CLK_EnableModuleClock(UART0_MODULE);
    /* Set UART0 clock */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_PLL, CLK_CLKDIV_UART(1));
    /* Set P3 multi-function pins for UART0 RXD and TXD  */
	SYS->P3_MFP &= ~(SYS_MFP_P30_Msk | SYS_MFP_P31_Msk);
    SYS->P3_MFP |= (SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0);

    /* Enable UART1 */
    CLK_EnableModuleClock(UART1_MODULE);
    /* Set UART1 clock */
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_PLL, CLK_CLKDIV_UART(1));
    /* Set P1 multi-function pins for UART1 RXD and TXD  */
	SYS->P1_MFP &= ~(SYS_MFP_P12_Msk | SYS_MFP_P13_Msk);
    SYS->P1_MFP |= (SYS_MFP_P12_RXD1 | SYS_MFP_P13_TXD1);

    /* Enable I2C0 */
    CLK_EnableModuleClock(I2C0_MODULE);
    /* Set P3 multi-function pins as the SDA0 & SCL0 of I2C0 pins */
    SYS->P3_MFP &= ~(SYS_MFP_P34_Msk | SYS_MFP_P35_Msk);
    SYS->P3_MFP |= SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0;

    /*  Set PWM01 clock */
    CLK_SetModuleClock(PWM01_MODULE, CLK_CLKSEL1_PWM01_S_HXT, 0);
    /*  Enable PWM01 */
    CLK_EnableModuleClock(PWM01_MODULE);
    GPIO_SetMode(P2, BIT0 , GPIO_PMD_OUTPUT);
    GPIO_SetMode(P2, BIT1 , GPIO_PMD_OUTPUT);
    /* Set P2 multi-function pins for PWM0 and PWM1  */
    SYS->P2_MFP &= ~(SYS_MFP_P20_PWM0 | SYS_MFP_P21_PWM1);
    SYS->P2_MFP |= (SYS_MFP_P20_PWM0 | SYS_MFP_P21_PWM1);
    
    /* Enable PWM0 PWM1 output, output 3.072MHz and 48kHz */
    PWM_EnableOutput(PWMA, 0x3);
    PWM_ConfigOutputChannel(PWMA, PWM_CH0, 3072000, 50);
    PWM_ConfigOutputChannel(PWMA, PWM_CH1, 48000, 50);
    PWM_Start(PWMA, 0x3);

    /* Lock protected registers */
    SYS_LockReg();
}
#else
void spl_mcu_init (void)
{   
    /* Unlock protected registers */
    SYS_UnlockReg();
	
	/*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);
	CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);
	
	CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));
	
    CLK_SetCoreClock(FREQ_50MHZ);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and cyclesPerUs automatically. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();
}
#endif
/**************************************************************************************************
*/
