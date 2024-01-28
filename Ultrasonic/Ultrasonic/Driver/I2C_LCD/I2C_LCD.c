#include "I2C_LCD.h"

uint8_t lcdControlData; // E RW RS x x x x x

void LCD_init()
{
	I2C_init();
	_delay_ms(15);
	LCD_cmdMode();
	LCD_writeMode();
	LCD_writeNibble(0x30);
	_delay_ms(5);
	LCD_writeNibble(0x30);
	_delay_ms(1);
	LCD_writeNibble(0x30);
	LCD_writeNibble(0x20);
	LCD_writeCmdData(LCD_4BIT_FUNCTION_SET);
	LCD_writeCmdData(LCD_DISPLAY_OFF);
	LCD_writeCmdData(LCD_DISPLAY_CLEAR);
	LCD_writeCmdData(LCD_ENTRY_MODE_SET);
	LCD_writeCmdData(LCD_DISPLAY_ON);
	LCD_backLightOn();
}
void LCD_backLightOn()
{
	lcdControlData |= (1<<LCD_BACKLIGHT);
	LCD_sendToI2C(LCD_SLA_W, lcdControlData);
}
void LCD_backLightOff()
{
	lcdControlData &= ~(1<<LCD_BACKLIGHT);
	LCD_sendToI2C(LCD_SLA_W, lcdControlData);
}
void LCD_sendToI2C(uint8_t sla_w, uint8_t data)
{
	I2C_txByte(sla_w, data);
}
void LCD_cmdMode()
{
	lcdControlData &= ~(1<<LCD_RS);
	LCD_sendToI2C(LCD_SLA_W, lcdControlData);
}
void LCD_charMode()
{
	lcdControlData |= (1<<LCD_RS);
	LCD_sendToI2C(LCD_SLA_W, lcdControlData);
}
void LCD_writeMode()
{
	lcdControlData &= ~(1<<LCD_RW);
	LCD_sendToI2C(LCD_SLA_W, lcdControlData);
}
void LCD_enableHigh()
{
	lcdControlData |= (1<<LCD_E);
	LCD_sendToI2C(LCD_SLA_W, lcdControlData);
	_delay_ms(1); // 정상적으로 enable이 H에서 L로 되는것을 기다려준다.
}
void LCD_enableLow()
{
	lcdControlData &= ~(1<<LCD_E);
	LCD_sendToI2C(LCD_SLA_W, lcdControlData);
	_delay_ms(1); // 정상적으로 enable이 H에서 L로 되는것을 기다려준다.
}
void LCD_writeNibble(uint8_t data)
{
	LCD_enableHigh();
	lcdControlData = (lcdControlData & 0x0f) | (data & 0xf0); // 상위 4bit data, 하위 4bit control
	LCD_sendToI2C(LCD_SLA_W, lcdControlData);
	LCD_enableLow();
}
void LCD_writeByte(uint8_t data)
{
	LCD_writeNibble(data); // 4bit LCD는 8bit를 두번에 나눠서 출력 상위 먼저
	data <<= 4; // 4칸 shift해서 
	LCD_writeNibble(data); // 나머지 출력
}

void LCD_writeCmdData(uint8_t data) // LCD instruction 쓰기
{   // datasheet의 timing diagram을 setting
	LCD_cmdMode();
	LCD_writeMode();
	LCD_enableHigh();
	LCD_writeByte(data); // 1byte data를 레지스터에 저장
	LCD_enableLow(); // data를 LCD에 출력
}
void LCD_writeCharData(uint8_t data) // LCD character 쓰기
{
	LCD_charMode();
	LCD_writeMode();
	LCD_enableHigh();
	LCD_writeByte(data);
	LCD_enableLow();
}

void LCD_writeString(char *str) // 문자열은 주소다
{
	for (int i = 0; str[i]; i++) // 조건식 str[i]가 NULL이 나오면 거짓
	{
		LCD_writeCharData(str[i]);
	}
}

void LCD_gotoXY(uint8_t row, uint8_t col)
{
	col %= 16;
	row %= 2;
	
	uint8_t lcdregisterAddress = (0x40 * row) + col;
	uint8_t command = 0x80 + lcdregisterAddress;
	LCD_writeCmdData(command);
}

void LCD_displayClear()
{
	LCD_writeCmdData(LCD_DISPLAY_CLEAR);
}
void LCD_writeStringXY(uint8_t row, uint8_t col, char *str)
{
	LCD_gotoXY(row, col);
	LCD_writeString(str);
}