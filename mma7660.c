#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf_drv_twi.h"
#include "mma7660.h"




/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

// A flag to indicate the transfer state
static volatile bool m_xfer_done = false;
void twi_init (void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
       .scl                = TWI_SCL_M ,
       .sda                = TWI_SDA_M ,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}



/*
   A function to write a Single Byte to MMA7660's internal Register
*/ 
bool mma7660_register_write(uint8_t register_address, uint8_t value)
{
    ret_code_t err_code;
    uint8_t tx_buf[MMA7660_ADDRESS_LEN+1];
	
    //Write the register address and data into transmit buffer
    tx_buf[0] = register_address;
    tx_buf[1] = value;

    //Set the flag to false to show the transmission is not yet completed
    m_xfer_done = false;
    
    //Transmit the data over TWI Bus
    err_code = nrf_drv_twi_tx(&m_twi, MMA7660_ADDR, tx_buf, MMA7660_ADDRESS_LEN+1, false);
    printf("%x",err_code);
    //Wait until the transmission of the data is finished
  /*  while (m_xfer_done == false)
    {
      }*/

    // if there is no error then return true else return false
    if (NRF_SUCCESS != err_code)
    {
        return false;
    }
    
    return true;	
}




/*
  A Function to read data from the MMA7660
*/ 
bool mma7660_register_read(uint8_t register_address, uint8_t * destination, uint8_t number_of_bytes)
{
    ret_code_t err_code;

    //Set the flag to false to show the receiving is not yet completed
    m_xfer_done = false;
    
    // Send the Register address where we want to write the data
    err_code = nrf_drv_twi_tx(&m_twi, MMA7660_ADDR, &register_address, 1, true);
	  
    //Wait for the transmission to get completed
   // while (m_xfer_done == false){}
    
    // If transmission was not successful, exit the function with false as return value
    if (NRF_SUCCESS != err_code)
    {
        return false;
    }

    //set the flag again so that we can read data from the MMA7660's internal register
    m_xfer_done = false;
	  
    // Receive the data from the MMA7660
    err_code = nrf_drv_twi_rx(&m_twi, MMA7660_ADDR, destination, number_of_bytes);
		
    //wait until the transmission is completed
   // while (m_xfer_done == false){}
	
    // if data was successfully read, return true else return false
    if (NRF_SUCCESS != err_code)
    {
        return false;
    }
    
    return true;
}

//Event Handler
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    //Check the event to see what type of event occurred
    switch (p_event->type)
    {
        //If data transmission or receiving is finished
	case NRF_DRV_TWI_EVT_DONE:
        m_xfer_done = true;//Set the flag
        break;
        
        default:
        // do nothing
          break;
    }
}
bool mma7660_verify_product_id(void)
{
    uint8_t who_am_i = 0x4c;
        if (who_am_i != MMA7660_ADDR)
        {
            return false;
        }
        else
        {
            return true;
        }
}


/*
  Function to initialize the mma7660
*/ 
bool mma7660_init(void)
{   
  bool transfer_succeeded = true;
	
  //Check the id to confirm that we are communicating with the right device
  transfer_succeeded &= mma7660_verify_product_id();
	
  if(mma7660_verify_product_id() == false)
    {
	return false;
      }

  // Set the registers with the required values, see the datasheet to get a good idea of these values
  (void)mma7660_register_write(MMA7660_MODE , 0x00); 
  (void)mma7660_register_write(MMA7660_SR , AUTO_SLEEP_1); 
  (void)mma7660_register_write(MMA7660_MODE , MMA7660_ACTIVE); 	
  
  return transfer_succeeded;
}



/*
  A Function to read accelerometer's values from the internal registers of MMA7660
*/ 
bool MMA7660_ReadCord( int8_t val[] )
{
 
  unsigned char reg[3] = { MMA7660_X,MMA7660_Y,MMA7660_Z };
  uint8_t count=0,sign=0;
  bool ret = false;
  for(count=0;count<3;count++)
  {
     mma7660_register_read(reg[count],&val[count],1);
     sign=val[count]&0x10;
     val[count]=val[count]&0x1f;
     if(sign)
     {
       val[count]=-val[count];
     }
     if(count == 2)
        ret = true;
  }
   //printf("%d-%d-%d",val[0],val[1],val[2]);
      
      
      
    
  return ret;
}

