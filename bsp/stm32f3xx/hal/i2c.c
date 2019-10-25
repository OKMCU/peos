/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/
#include "M051Series.h"
#include "spl_config.h"
#include "spl_i2c.h"
#include "spl_mcu.h"

#include "bufmgr.h"

#if (SPL_I2C_EN > 0)

#if (SPL_I2C0_SLAVE_EN > 0)
extern void     SPL_I2C0_CALLBACK_ADDRW( void );
extern void     SPL_I2C0_CALLBACK_ADDRR( void );
extern void     SPL_I2C0_CALLBACK_RXD( uint8_t rx_byte );
extern uint8_t  SPL_I2C0_CALLBACK_TXD( void );
extern void     SPL_I2C0_CALLBACK_FRMEND( void );
#endif

#if (SPL_I2C1_SLAVE_EN > 0)
extern void     SPL_I2C1_CALLBACK_ADDRW( void );
extern void     SPL_I2C1_CALLBACK_ADDRR( void );
extern void     SPL_I2C1_CALLBACK_RXD( uint8_t rx_byte );
extern uint8_t  SPL_I2C1_CALLBACK_TXD( void );
extern void     SPL_I2C1_CALLBACK_FRMEND( void );
#endif

extern void     spl_i2c_init( uint8_t port )
{
#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C0_SLAVE_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
        /* Enable I2C0 */
        CLK_EnableModuleClock(I2C0_MODULE);
        /* Set P3 multi-function pins as the SDA0 & SCL0 of I2C0 pins */
        SYS->P3_MFP &= ~(SYS_MFP_P34_Msk | SYS_MFP_P35_Msk);
        SYS->P3_MFP |= SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0;
        return;
    }
#endif

#if (SPL_I2C1_MASTER_EN > 0 || SPL_I2C1_SLAVE_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
        /* Enable I2C1 */
        CLK_EnableModuleClock(I2C1_MODULE);
        /* Set P2 multi-function pins as the SDA1 & SCL1 of I2C1 pins */
        SYS->P2_MFP &= ~(SYS_MFP_P24_Msk | SYS_MFP_P25_Msk);
        SYS->P2_MFP |= SYS_MFP_P24_SCL1| SYS_MFP_P25_SDA1;
        return;
    }
#endif
}

extern void     spl_i2c_open( uint8_t port )
{
    uint32_t u32Div;
    
    (void)u32Div;
    
#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C0_SLAVE_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
#if (SPL_I2C0_MASTER_EN > 0)
        u32Div = (uint32_t)(((SystemCoreClock * 10) / (SPL_I2C0_CLOCK * 4) + 5) / 10 - 1); /* Compute proper divider for I2C clock */
        I2C0->I2CLK = u32Div;
#endif /* (SPL_I2C0_MASTER_EN > 0) */
        I2C0->I2CON |= I2C_I2CON_ENS1_Msk;
#if (SPL_I2C0_SLAVE_EN > 0)
        I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );

        I2C_SetSlaveAddr(I2C0, 0, SPL_I2C0_SLAVE_ADDR0, 0);   /* Slave Address 0 */
        I2C_SetSlaveAddr(I2C0, 1, SPL_I2C0_SLAVE_ADDR1, 0);   /* Slave Address 1 */
        I2C_SetSlaveAddr(I2C0, 2, SPL_I2C0_SLAVE_ADDR2, 0);   /* Slave Address 2 */
        I2C_SetSlaveAddr(I2C0, 3, SPL_I2C0_SLAVE_ADDR3, 0);   /* Slave Address 3 */

        /* Set I2C 4 Slave Addresses Mask */
        I2C_SetSlaveAddrMask(I2C0, 0, 0x00);
        I2C_SetSlaveAddrMask(I2C0, 1, 0x00);
        I2C_SetSlaveAddrMask(I2C0, 2, 0x00);
        I2C_SetSlaveAddrMask(I2C0, 3, 0x00);

        I2C_EnableInt(I2C0);
        NVIC_EnableIRQ(I2C0_IRQn);
#endif /* (SPL_I2C0_SLAVE_EN > 0) */
    }
#endif /* (SPL_I2C0_MASTER_EN > 0 || SPL_I2C0_SLAVE_EN > 0) */

#if (SPL_I2C1_MASTER_EN > 0 || SPL_I2C1_SLAVE_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
#if (SPL_I2C1_MASTER_EN > 0)
        u32Div = (uint32_t)(((SystemCoreClock * 10) / (SPL_I2C1_CLOCK * 4) + 5) / 10 - 1); /* Compute proper divider for I2C clock */
        I2C1->I2CLK = u32Div;
#endif /* (SPL_I2C1_MASTER_EN > 0) */
        I2C1->I2CON |= I2C_I2CON_ENS1_Msk;
#if (SPL_I2C1_SLAVE_EN > 0)
        I2C_SET_CONTROL_REG( I2C1, I2C_I2CON_SI_AA );

        I2C_SetSlaveAddr(I2C1, 0, SPL_I2C1_SLAVE_ADDR0, 0);   /* Slave Address 0 */
        I2C_SetSlaveAddr(I2C1, 1, SPL_I2C1_SLAVE_ADDR1, 0);   /* Slave Address 1 */
        I2C_SetSlaveAddr(I2C1, 2, SPL_I2C1_SLAVE_ADDR2, 0);   /* Slave Address 2 */
        I2C_SetSlaveAddr(I2C1, 3, SPL_I2C1_SLAVE_ADDR3, 0);   /* Slave Address 3 */

        /* Set I2C 4 Slave Addresses Mask */
        I2C_SetSlaveAddrMask(I2C1, 0, 0x00);
        I2C_SetSlaveAddrMask(I2C1, 1, 0x00);
        I2C_SetSlaveAddrMask(I2C1, 2, 0x00);
        I2C_SetSlaveAddrMask(I2C1, 3, 0x00);
        
        I2C_EnableInt(I2C1);
        NVIC_EnableIRQ(I2C1_IRQn);
#endif /* (SPL_I2C1_SLAVE_EN > 0) */
    }
#endif /* (SPL_I2C1_MASTER_EN > 0 || SPL_I2C1_SLAVE_EN > 0) */

}

extern uint8_t  spl_i2c_busy( uint8_t port )
{
    uint32_t status = 0;

#if (SPL_I2C0_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
        status = I2C_GET_STATUS(I2C0);
        return !(status == 0xF8);
    }
#endif

#if (SPL_I2C1_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
        status = I2C_GET_STATUS(I2C1);
        return !(status == 0xF8);
    }
#endif

    return 1;
}

#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C1_MASTER_EN > 0)
extern uint8_t  spl_i2c_start( uint8_t port )
{
    uint32_t status;

#if (SPL_I2C0_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
        I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI | I2C_I2CON_STA);
        I2C_WAIT_READY(I2C0);
        status = I2C_GET_STATUS(I2C0);
        if(status == 0x08)
            return SPL_I2C_ERR_NONE;
    }
#endif

#if (SPL_I2C1_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI | I2C_I2CON_STA);
        I2C_WAIT_READY(I2C1);
        status = I2C_GET_STATUS(I2C1);
        if(status == 0x08)
            return SPL_I2C_ERR_NONE;
    }
#endif

    return SPL_I2C_ERR_BE;
}
#endif

#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C1_MASTER_EN > 0)
extern uint8_t  spl_i2c_restart( uint8_t port )
{
    uint32_t status;

#if (SPL_I2C0_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
        I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI | I2C_I2CON_STA);
        I2C_WAIT_READY(I2C0);
        status = I2C_GET_STATUS(I2C0);
        if(status == 0x10) 
            return SPL_I2C_ERR_NONE;
    }
#endif

#if (SPL_I2C1_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI | I2C_I2CON_STA);
        I2C_WAIT_READY(I2C1);
        status = I2C_GET_STATUS(I2C1);
        if(status == 0x10) 
            return SPL_I2C_ERR_NONE;
    }
#endif

    return SPL_I2C_ERR_BE; 
}
#endif

#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C1_MASTER_EN > 0)
extern uint8_t  spl_i2c_wr_addr( uint8_t port, uint8_t addr )
{
    uint32_t status;

#if (SPL_I2C0_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
        I2C_SET_DATA(I2C0, addr);
        I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI);
        I2C_WAIT_READY(I2C0);
        status = I2C_GET_STATUS(I2C0);
        goto spl_i2c_wr_addr_error_code;
    }
#endif

#if (SPL_I2C1_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
        I2C_SET_DATA(I2C1, addr);
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
        I2C_WAIT_READY(I2C1);
        status = I2C_GET_STATUS(I2C1);
        goto spl_i2c_wr_addr_error_code;
    }
#endif

    spl_i2c_wr_addr_error_code:
    if(addr & 0x01)
    {
        if(status == 0x40)
            return SPL_I2C_ERR_NONE;
        if(status == 0x48)
            return SPL_I2C_ERR_NACK;
        
    }
    else
    {
        if(status == 0x18)
            return SPL_I2C_ERR_NONE;
        if(status == 0x20)
            return SPL_I2C_ERR_NACK;
    }

    if(status == 0x38 ||
       status == 0x68 ||
       status == 0x78 ||
       status == 0xB0)
            return SPL_I2C_ERR_ARB;

    return SPL_I2C_ERR_BE;
}
#endif

#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C1_MASTER_EN > 0)
extern uint8_t  spl_i2c_wr_data( uint8_t port, uint8_t byte )
{
    uint32_t status;

#if (SPL_I2C0_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
        I2C_SET_DATA(I2C0, byte);
        I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI);
        I2C_WAIT_READY(I2C0);
        status = I2C_GET_STATUS(I2C0);
        goto spl_i2c_wr_data_error_code;
    }
#endif

#if (SPL_I2C1_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
        I2C_SET_DATA(I2C1, byte);
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
        I2C_WAIT_READY(I2C1);
        status = I2C_GET_STATUS(I2C1);
        goto spl_i2c_wr_data_error_code;
    }
#endif
    
    spl_i2c_wr_data_error_code:
    if(status == 0x28)
        return SPL_I2C_ERR_NONE;
    if(status == 0x30)
        return SPL_I2C_ERR_NACK;
    if(status == 0x38)
        return SPL_I2C_ERR_ARB;

    return SPL_I2C_ERR_BE;
    
}
#endif

#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C1_MASTER_EN > 0)
extern uint8_t  spl_i2c_rd_data( uint8_t port, uint8_t ack, uint8_t *p_data )
{
    uint32_t status;
    uint8_t byte;
#if (SPL_I2C0_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
        if(ack)
            I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI | I2C_I2CON_AA);
        else
            I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI);
        I2C_WAIT_READY(I2C0);
        status = I2C_GET_STATUS(I2C0);
        byte = I2C_GET_DATA(I2C0);
        goto spl_i2c_rd_data_error_code;
    }
#endif

#if (SPL_I2C1_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
        if(ack)
            I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI | I2C_I2CON_AA);
        else
            I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
        I2C_WAIT_READY(I2C1);
        status = I2C_GET_STATUS(I2C1);
        byte = I2C_GET_DATA(I2C1);
        goto spl_i2c_rd_data_error_code;
    }
#endif

    spl_i2c_rd_data_error_code:
    if(p_data)
        *p_data = byte;
    
    if(ack)
    {
        if(status == 0x50)
            return SPL_I2C_ERR_NONE;
    }
    else
    {
        if(status == 0x58)
            return SPL_I2C_ERR_NONE;
    }
    if(status == 0x38)
        return SPL_I2C_ERR_ARB;

    return SPL_I2C_ERR_BE;
}
#endif

#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C1_MASTER_EN > 0)
extern uint8_t  spl_i2c_stop( uint8_t port )
{
    uint32_t status;

#if (SPL_I2C0_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
        //P35 = 0;
        //P34 = 0;
        //SYS->P3_MFP &= ~(SYS_MFP_P35_Msk | SYS_MFP_P34_Msk);
        //I2C_Close( I2Cx );
        //P35 = 1;
        //CLK_SysTickDelay( 1 );
        //SYS->P3_MFP |= SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0;
        I2C_STOP(I2C0);
        status = I2C_GET_STATUS(I2C0);
        goto spl_i2c_stop_error_code;
    }
#endif

#if (SPL_I2C1_MASTER_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
        //P25 = 0;
        //P24 = 0;
        //SYS->P2_MFP &= ~(SYS_MFP_P24_Msk | SYS_MFP_P25_Msk);
        //I2C_Close( I2Cx );
        //P24 = 1;
        //CLK_SysTickDelay( 1 );
        //SYS->P2_MFP |= SYS_MFP_P25_SDA0 | SYS_MFP_P24_SCL0;
        I2C_STOP(I2C1);
        status = I2C_GET_STATUS(I2C1);
        goto spl_i2c_stop_error_code;
    }
#endif
    spl_i2c_stop_error_code:
    if(status == 0xF8)
        return SPL_I2C_ERR_NONE;

     return SPL_I2C_ERR_BE;
}
#endif

extern void     spl_i2c_close( uint8_t port )
{
#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C0_SLAVE_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
        I2C_Close(I2C0);
#if (SPL_I2C0_SLAVE_EN > 0)
        I2C_DisableInt(I2C0);
        NVIC_DisableIRQ(I2C0_IRQn);
#endif
        return;
    }
#endif


#if (SPL_I2C1_MASTER_EN > 0 || SPL_I2C1_SLAVE_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
        I2C_Close(I2C1);
#if (SPL_I2C1_SLAVE_EN > 0)
        I2C_DisableInt(I2C1);
        NVIC_DisableIRQ(I2C1_IRQn);
#endif
        return;
    }
#endif
}

extern void     spl_i2c_deinit( uint8_t port )
{
#if (SPL_I2C0_MASTER_EN > 0 || SPL_I2C0_SLAVE_EN > 0)
    if(port == SPL_I2C_PORT_0)
    {
        CLK_DisableModuleClock(I2C0_MODULE);
        SYS->P3_MFP &= ~(SYS_MFP_P34_Msk | SYS_MFP_P35_Msk);
        return;
    }
#endif

#if (SPL_I2C1_MASTER_EN > 0 || SPL_I2C1_SLAVE_EN > 0)
    if(port == SPL_I2C_PORT_1)
    {
        CLK_DisableModuleClock(I2C1_MODULE);
        SYS->P3_MFP &= ~(SYS_MFP_P25_Msk | SYS_MFP_P24_Msk);
        return;
    }
#endif
}

#if (SPL_I2C0_SLAVE_EN > 0)
extern void spl_i2c0_isr  ( void )
{
    uint32_t u32Status;
    
    u32Status = I2C_GET_STATUS( I2C0 );

    if( I2C_GET_TIMEOUT_FLAG( I2C0 ) )
    {
        /* Clear I2C0 Timeout Flag */
        I2C_ClearTimeoutFlag( I2C0 );
    }
    else
    {
        if( u32Status == 0x60 )                       /* Own SLA+W has been receive; ACK has been return */
        {
            SPL_I2C0_CALLBACK_ADDRW();
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0x80 )                 /* Previously address with own SLA address
                                                       Data has been received; ACK has been returned*/
        {
            SPL_I2C0_CALLBACK_RXD((uint8_t)I2C_GET_DATA(I2C0));
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xA8 )                  /* Own SLA+R has been receive; ACK has been return */
        {
            SPL_I2C0_CALLBACK_ADDRR();
            I2C_SET_DATA(I2C0, SPL_I2C0_CALLBACK_TXD()); //put the first byte into the buffer
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xB8 )                 /* Data byte or last data in I2CDAT has been transmitted
                                                        ACK has been received */
        {
            I2C_SET_DATA(I2C0, SPL_I2C0_CALLBACK_TXD());//previous byte is transmitted successfully, 
                                                        //put the next byte into the buffer
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xC0 )                 /* Data byte or last data in I2CDAT has been transmitted
                                                       Not ACK has been received */
        {
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0x88 )                 /* Previously addressed with own SLA address; NOT ACK has
                                                       been returned */
        {
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xA0 )                 /* A STOP or repeated START has been received while still
                                                       addressed as Slave/Receiver*/
        {
            SPL_I2C0_CALLBACK_FRMEND();
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else
        {
            /* TO DO */
            //HAL_ASSERT_FORCED();
        }
    }
}
#endif

#if (SPL_I2C1_SLAVE_EN > 0)
extern void spl_i2c1_isr  ( void )
{
    uint32_t u32Status;
    
    u32Status = I2C_GET_STATUS( I2C1 );

    if( I2C_GET_TIMEOUT_FLAG( I2C1 ) )
    {
        /* Clear I2C1 Timeout Flag */
        I2C_ClearTimeoutFlag( I2C1 );
    }
    else
    {
        if( u32Status == 0x60 )                       /* Own SLA+W has been receive; ACK has been return */
        {
            SPL_I2C1_CALLBACK_ADDRW();
            I2C_SET_CONTROL_REG( I2C1, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0x80 )                 /* Previously address with own SLA address
                                                       Data has been received; ACK has been returned*/
        {
            SPL_I2C1_CALLBACK_RXD((uint8_t)I2C_GET_DATA(I2C1));
            I2C_SET_CONTROL_REG( I2C1, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xA8 )                  /* Own SLA+R has been receive; ACK has been return */
        {
            SPL_I2C1_CALLBACK_ADDRR();
            I2C_SET_DATA(I2C1, SPL_I2C1_CALLBACK_TXD()); //put the first byte into the buffer
            I2C_SET_CONTROL_REG( I2C1, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xB8 )                 /* Data byte or last data in I2CDAT has been transmitted
                                                        ACK has been received */
        {
            I2C_SET_DATA(I2C1, SPL_I2C1_CALLBACK_TXD());//previous byte is transmitted successfully, 
                                                        //put the next byte into the buffer
            I2C_SET_CONTROL_REG( I2C1, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xC0 )                 /* Data byte or last data in I2CDAT has been transmitted
                                                       Not ACK has been received */
        {
            I2C_SET_CONTROL_REG( I2C1, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0x88 )                 /* Previously addressed with own SLA address; NOT ACK has
                                                       been returned */
        {
            I2C_SET_CONTROL_REG( I2C1, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xA0 )                 /* A STOP or repeated START has been received while still
                                                       addressed as Slave/Receiver*/
        {
            SPL_I2C1_CALLBACK_FRMEND();
            I2C_SET_CONTROL_REG( I2C1, I2C_I2CON_SI_AA );
        }
        else
        {
            /* TO DO */
            //HAL_ASSERT_FORCED();
        }
    }
}
#endif
#endif /* (SPL_I2C_EN > 0) */

