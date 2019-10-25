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
#ifndef __SPL_I2C_H__
#define __SPL_I2C_H__


#include <stdint.h>

#define SPL_I2C_PORT_0      0
#define SPL_I2C_PORT_1      1

#define SPL_I2C_ERR_NONE    0
#define SPL_I2C_ERR_NACK    1   //No ACK
#define SPL_I2C_ERR_ARB     2   //Arbitration Lost
#define SPL_I2C_ERR_BE      3   //Bus error


extern void     spl_i2c_init( uint8_t port );
extern void     spl_i2c_open( uint8_t port );
extern uint8_t  spl_i2c_busy( uint8_t port );
extern uint8_t  spl_i2c_start( uint8_t port );
extern uint8_t  spl_i2c_restart( uint8_t port );
extern uint8_t  spl_i2c_wr_addr( uint8_t port, uint8_t addr );
extern uint8_t  spl_i2c_wr_data( uint8_t port, uint8_t byte );
extern uint8_t  spl_i2c_rd_data( uint8_t port, uint8_t ack, uint8_t *p_data );
extern uint8_t  spl_i2c_stop( uint8_t port );
extern void     spl_i2c_close( uint8_t port );
extern void     spl_i2c_deinit( uint8_t port );

#endif /* __SPL_I2C_H__ */

