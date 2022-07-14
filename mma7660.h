#ifndef __MMC7660_H__
#define __MMC7660_H__



/* TWI instance ID. */
#if TWI0_ENABLED
#define TWI_INSTANCE_ID     0
#elif TWI1_ENABLED
#define TWI_INSTANCE_ID     1
#endif

 /* Number of possible TWI addresses. */
 #define TWI_ADDRESSES      127

#define TWI_SCL_M           27        //I2C SCL Pin
#define TWI_SDA_M           26        //I2C SDA Pin

#define MMA7660_ADDR  0x4c
#define MMA7660_ADDRESS_LEN 1


#define MMA7660_X        0x00
#define MMA7660_Y        0x01
#define MMA7660_Z        0x02
#define MMA7660_MODE     0x07
#define MMA7660_STAND_BY 0x00
#define MMA7660_ACTIVE   0x01
#define MMA7660_SR       0x08      //sample rate register
#define AUTO_SLEEP_120   0X00      //120 sample per second
#define AUTO_SLEEP_64    0X01
#define AUTO_SLEEP_32    0X02
#define AUTO_SLEEP_16    0X03
#define AUTO_SLEEP_8     0X04
#define AUTO_SLEEP_4     0X05
#define AUTO_SLEEP_2     0X06
#define AUTO_SLEEP_1     0X07
  
  
void twi_init(void); // initialize the twi communication
bool mma7660_init(void);   

bool mma7660_register_write(uint8_t register_address, const uint8_t value);

bool mma7660_register_read(uint8_t register_address, uint8_t *destination, uint8_t number_of_bytes);


bool mma7660_verify_product_id(void);
bool MMA7660_ReadCord( int8_t *value );

#endif

