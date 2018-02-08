/*
 * i2c.c - I2C interface for T-962 reflow controller
 *
 * Copyright (C) 2014 Werner Johansson, wj@unifiedengineering.se
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <string.h>
/* custom includes */
#include "common_defs.h"
#include "i2c.h"
#include "timers.h"




// Limit to i2c speed 100kHz because of the relatively weak 4k7 pullups
#define I2CSPEED (100E6)



// local function declarations
bool i2c_start(uint8_t deviceaddr);
bool i2c_stop();
bool i2c_restart();
bool i2c_reset();
void send_ack();
void send_nack();
unsigned short int ack_received();
bool i2c_transmit_data(unsigned char data);
bool i2c_receive_data(unsigned char* pData, uint8_t bLastByte);




/**
**  @brief I2C_Xfer the function for doing I2C sends/receives
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return 
*/
void I2C_Init(void) 
{
	uint8_t dummybyte = 0x55;
	
    // initialize I2C#5
    I2C5BRG = 0x1E7;                    // 100Khz (200Khz (0xED))
    //I2C5CONbits.DISSLW = 1;           // disable slew control
    I2C5CONbits.ON = 1;                 // enable I2C module
    Nop();
    Nop();        
	
    // dummy condition to reset
    // and prime bus and slaves...
    i2c_reset();
    
    // return
    return;   
}
/**/
/*******************************************************************/


/**
**  @brief I2C_BusOperation the function for doing I2C sends/receives
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return 
*/
int32_t I2C_BusOperation(uint16_t slaveaddr, uint8_t* theBuffer, uint32_t theLength, uint8_t readwrite)
{
	int32_t retval = 0;
    uint8_t addr_hi = 0;
    uint8_t addr_lo = 0;
    
    // calculate hi & lo addresses
    addr_hi = ((slaveaddr >> 8) & 0x1F);    // top 3-bits not used
    addr_lo = (slaveaddr & 0xFF);           // lower 8-bits all used
    
    // start the I2C operation    
    // always start in 'write' mode for now, 
    // since we will do 'select' address
    if (i2c_start(I2C_EEPROM_ADDRESS | 0) == 0)
    {        
        retval = -1;
        goto exit;
    }
    
    // transmit the 'HI' address
    if (i2c_transmit_data(addr_hi) == 0)
    {        
        retval = -1;
        goto exit;
    }    
    // transmit the 'HI' address
    if (i2c_transmit_data(addr_lo) == 0)
    {        
        retval = -1;
        goto exit;
    }
    
    // if 'bit0' of addr is 1, this
    // is a read operation...
    if (readwrite & I2C_READ)
    {
        // ------------------------------------------- //
        //           I2C READ OPERATION                //
        // ------------------------------------------- //
        //        
        // now re-issue a start, to flip the direct to 'read'
        if (i2c_start(I2C_EEPROM_ADDRESS | I2C_READ) == 0)
        {        
            retval = -1;
            goto exit;
        }   
            
        // read until length is 0...
        while (theLength > 0)
        {            
            // if there is data to receive...
            if ( i2c_receive_data(theBuffer, (theLength == 1) ) == 0)
            {
                // receive failed..
                retval = -1;
                goto exit;
            }            
            // inc buffer, dec len
            theBuffer++;
            theLength--;            
        }      
    }
    else
    {
        // ------------------------------------------- //
        //           I2C WRITE OPERATION               //
        // ------------------------------------------- //
        //
        // write until length is 0...
        while (theLength > 0)
        {
            // if there is data to send...
            if (i2c_transmit_data(*theBuffer) == 0)
            {
                // send failed..
                retval = -1;
                goto exit;
            }   
            // must delay '5ms' for write cycle
            //delay_ms(5);
            theBuffer++;
            theLength--;            
        }      
        // send i2c stop
        i2c_stop();  
        delay_ms(5);
    }    
        
exit:
    // check for any error conditions...
     if ( (I2C5STAT & I2C_COLL_BIT) || (I2C5STAT & I2C_WRCOLL_BIT) || (I2C5STAT & I2C_RCVOVFW_BIT) )
     {
        // i2c error condition, reset the bus
        // and return an error...         
        i2c_reset();
        retval = -1;         
     }

    // return status
    return retval;
}
/**/
/************************************************************************/


/**
**  @brief i2c_start the function for 
**	@return 
*/
bool i2c_start(uint8_t deviceaddr)
{
    bool bRetVal = FALSE;
    uint8_t temp = 0;   
    

    
    // initiate I2C start
    I2C5CONbits.SEN = 1;	   
	
    // wait for the start to clear   
    temp = 0;
    while (I2C5CONbits.SEN == 1)
    {
        delay_us(100);
        temp++;
        if (temp > 10)
        {
            bRetVal = 0;
            goto exit;
        }
    } 
    
    // transmit the device address
    if (i2c_transmit_data(deviceaddr) == 0)
    {
        bRetVal = 0;
        goto exit;        
    }

    // success
    bRetVal = TRUE;
    
exit:
    // return status
    return bRetVal;
}
/**/
//---------------------------------------------------------------------------------------------------------------------------


/**
**  @brief i2c_stop the function for 
**	@return 
*/
bool i2c_stop()
{
    bool bRetVal = FALSE;
    uint8_t temp = 0;    
    
    // maybe should check 'least 5' bits here first??
    /*
    if (I2C5STAT & I2C_PEN_REN_CHECK)
    {
       delay_us(100);
        temp++;
        if (temp > 10)
        {
            bRetVal = 0;
            goto exit;
        }         
    }
    */
    
    // initiate I2C stop
    I2C5CONbits.PEN = 1;	   
	
    // wait for the stop to clear 
    temp = 0;
    while (I2C5CONbits.PEN == 1)
    {
        delay_us(100);
        temp++;
        if (temp > 10)
        {
            bRetVal = 0;
            goto exit;
        }
    }    

    // success
    bRetVal = TRUE;
    
exit:
    // return status
    return bRetVal;
}
/**/
//---------------------------------------------------------------------------------------------------------------------------



/**
**  @brief i2c_restart the function for 
**	@return 
*/
bool i2c_restart()
{
    bool bRetVal = FALSE;
    uint8_t temp = 0;    
    
    // maybe should check 'least 5' bits here first??
    /*
    if (I2C5STAT & I2C_PEN_REN_CHECK)
    {
        delay_us(100);
        temp++;
        if (temp > 10)
        {
            bRetVal = 0;
            goto exit;
        }         
    }
    */
    
    // initiate I2C restart
    I2C5CONbits.RSEN = 1;	
    Nop();
	
    // wait for the restart to clear 
    temp = 0;
    while (I2C5CONbits.RSEN == 1)
    {
        delay_us(100);
        temp++;
        if (temp > 10)
        {
            bRetVal = 0;
            goto exit;
        }
    }    

    // success
    bRetVal = TRUE;
    
exit:
    // return status
    return bRetVal;
}

//---------------------------------------------------------------------------------------------------------------------------


/**
**  @brief i2c_restart the function for 
**	@return 
*/
bool i2c_reset()
{
    bool bRetVal = FALSE;
    uint8_t temp = 0;    
    
       
    
    // initiate I2C start
    I2C5CONbits.SEN = 1;	
    Nop();
	
    // wait for the restart to clear 
    temp = 0;
    while (I2C5CONbits.SEN == 1)
    {
        delay_us(100);
        temp++;
        if (temp > 10)
        {
            bRetVal = 0;
            goto exit;
        }
    } 
    
    // transmit bogus data
    i2c_transmit_data(0xff);
    
    // stop the bus
    i2c_stop(); 
    delay_ms(10);
    
    // turn the module off and back on...
    I2C5CONbits.ON = 0;                 // disable I2c
    Nop();
    Nop();
    I2C5CONbits.ON = 1;                 // enable I2C module
    Nop();   
    Nop();
    
    // success
    bRetVal = TRUE;
    
exit:
    // return status
    return bRetVal;
}

//---------------------------------------------------------------------------------------------------------------------------



/**
**  @brief i2c_restart the function for 
**	@return 
*/
bool i2c_transmit_data(unsigned char data)
{
    bool bRetVal = FALSE;
    uint8_t temp = 0;
    
    
    // wait for the xmit buffer to be empty
    while (I2C5STATbits.TBF == 1)
    {       
        delay_us(100);
        temp++;
        if (temp > 10)
        {
            bRetVal = 0;
            goto exit;
        }        
    }
    
    // send the output data
    I2C5TRN = (data & 0xFF); // Data transmission is initiated here
    while( I2C5STATbits.TBF != 1)   
    {       
        delay_us(100);
        temp++;
        if (temp > 10)
        {
            // don't bail, it prob just sent fast...
            break;
        }        
    }
    
    // wait for the TRSTAT to complete...
    temp = 0;
    while( I2C5STATbits.TRSTAT == 1)
    {
        delay_us(100);
        temp++;
        if (temp > 10)
        {
            bRetVal = 0;
            goto exit;
        }             
    }        
    
    // check for any error conditions
    if ( (I2C5STATbits.BCL) || (I2C5STATbits.IWCOL) || (I2C5STATbits.I2COV) )
    {
        // we encountered an error...
        bRetVal = 0;
        goto exit;                
    }   
    
    // check for ACK or NACK??
    if ( I2C5STATbits.ACKSTAT == 0)
    {     
        // status success
        bRetVal = TRUE;        
    }   
    
exit:
    // return status
    return bRetVal;
}

//---------------------------------------------------------------------------------------------------------------------------


/**
**  @brief i2c_receive_data the function for 
**	@return 
*/
bool i2c_receive_data(unsigned char* pData, uint8_t bLastByte)
{
    bool bRetVal = FALSE;
    uint8_t temp = 0;        
    unsigned char data;
    
    
    
    // enable I2C 'receive'
    I2C5CONbits.RCEN = 1;	  
    Nop();

    // wait for the receive to finish
    while( (I2C5STATbits.RBF) == 0 )
    {
        delay_us(100);
        temp++;
        if (temp > 10)
        {
            bRetVal = 0;
            goto exit;
        }         
    }
    // extract out the recv data
    data = (I2C5RCV & 0xFF); 
    *pData = data;
    
    // if more bytes to follow,
    // send an 'ACK', else 
    // send a 'NACK'...
    if (bLastByte == TRUE)
        i2c_stop();
    else
        send_ack();        
    
    // status success
    bRetVal = TRUE;
    
exit:
    // disable receive
    I2C5CONbits.RCEN = 0;
    
    // return status
    return bRetVal; 
}

//---------------------------------------------------------------------------------------------------------------------------


/**
**  @brief i2c_restart the function for 
**	@return 
*/
void send_ack()
{
    // send and enable the 'ACK'
    I2C5CONbits.ACKDT = 0;      // This ACKDT bit will be transmitted to the slave when Enabling the ACKEN bit
    I2C5CONbits.ACKEN = 1;      // ACKDT is transmitted at this point and thus ACK is made, after this the slave receives ACK and auto increments its Register address 
    while(I2C5CONbits.ACKEN);   // ACKEN is a Hardware clearable bit
}
/**/
//---------------------------------------------------------------------------------------------------------------------------



/**
**  @brief i2c_restart the function for 
**	@return 
*/
void send_nack()
{
    // send and enable the 'NACK'
    I2C5CONbits.ACKDT = 1;      // This ACKDT bit will be transmitted to the slave when Enabling the ACKEN bit indicating No ACK
    I2C5CONbits.ACKEN = 1;      // ACKDT is transmitted at this point and thus ACK is made, after this the slave receives No ACK 
    while(I2C5CONbits.ACKEN);   // ACKEN is a Hardware clearable bit

}
/**/
//---------------------------------------------------------------------------------------------------------------------------



/**
**  @brief i2c_restart the function for 
**	@return 
*/
unsigned short int ack_received()
{
    if (I2C5STATbits.ACKSTAT == 0)
        return 1;
    else
        return 0;
}
/**/
//---------------------------------------------------------------------------------------------------------------------------