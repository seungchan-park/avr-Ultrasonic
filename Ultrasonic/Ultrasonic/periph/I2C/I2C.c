#include "I2C.h"

void I2C_init()
{
	I2C_DDR |= (1<<I2C_SCL)|(1<<I2C_SDA);
	TWBR = 72; // 100kHz SCL
	TWSR = 0;
}
void I2C_txByte(uint8_t SLA_W, uint8_t data)
{
	I2C_start(); // 1. start bit
	I2C_txDevAddr(SLA_W); // 2. SLA + R/W 8bit + 1bit ACK
	I2C_txData(data); // 3. 1byte data 송신
	I2C_stop(); // 4. stop bit
}
void I2C_start()
{	//Send START condition
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	//Wait for TWINT flag set. This indicates that the
	//START condition has been transmitted
	while (!(TWCR & (1<<TWINT)));
}
void I2C_txDevAddr(uint8_t SLA_W)
{	//Load SLA_W into TWDR Register.
	TWDR = SLA_W;
	//Clear TWINT bit in TWCR to start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	//Wait for TWINT flag set. This indicates that the
	//SLA+W has been transmitted, and
	//ACK/NACK has been received.
	while (!(TWCR & (1<<TWINT)));
}
void I2C_txData(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}
void I2C_stop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}