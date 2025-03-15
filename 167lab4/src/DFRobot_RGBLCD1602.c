// /*!
//  * @file DFRobot_RGBLCD1602.cpp
//  * @brief DFRobot_RGBLCD1602 class infrastructure, the implementation of basic methods
//  * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
//  * @licence     The MIT License (MIT)
//  * @maintainer [yangfeng](feng.yang@dfrobot.com)
//  * @version  V1.0
//  * @date  2021-09-24
//  * @url https://github.com/DFRobot/DFRobot_RGBLCD1602
//  */

 #include <stdio.h>


 #include <stdlib.h>
 #include <Board.h>
 #include <timers.h>
#include "DFRobot_RGBLCD1602.h"


uint8_t _showFunction;
uint8_t _showControl;
uint8_t _showMode;
uint8_t _initialized;
uint8_t _numLines,_currLine;
uint8_t _lcdAddr;
uint8_t _RGBAddr;
uint8_t _cols;
uint8_t _rows;

uint8_t REG_RED      =   0 ;       // pwm2
uint8_t REG_GREEN    =   0 ;       // pwm1
uint8_t REG_BLUE     =   0 ;       // pwm0
uint8_t REG_ONLY     =   0 ;       // pwm0


const uint8_t color_define[4][3] = 
{
    {255, 255, 255},            // white
    {255, 0, 0},                // red
    {0, 255, 0},                // green
    {0, 0, 255},                // blue
};

// /*******************************public*******************************/

void DFRobot_Init()
{
    printf("Initializing driver\n");
    printf("Initializing timers\n");
    TIMER_Init(); // init timer module for delay functions
    //HAL_Delay(5000);
    printf("Initializing I2C\n");
    I2C_Init();  // init I2C module
    //Init();  // init I2C module

    uint8_t RGBAddr=RGB_ADDRESS, lcdCols=16, lcdRows=2,lcdAddr=LCD_ADDRESS;

    _lcdAddr = lcdAddr;
    _RGBAddr = RGBAddr;
    _cols = lcdCols;
    _rows = lcdRows;
    // Initialize I2C 
   // configure_i2c((uint16_t) LCD_ADDRESS, (uint32_t) 100000, &_dev_handle, &_bus_handle, _i2cPort);  // Call your I2C initialization function


printf("Driver installed\n");
printf("_RGBAddr: %x\n",_RGBAddr);
    if (_RGBAddr == 0x60) {
        REG_RED   = 0x04;
        REG_GREEN = 0x03;
        REG_BLUE  = 0x02;
        REG_ONLY  = 0x02;
    } else if (_RGBAddr == (0x60 >> 1)) {
        REG_RED   = 0x06;  // pwm2
        REG_GREEN = 0x07;  // pwm1
        REG_BLUE  = 0x08;  // pwm0
        REG_ONLY  = 0x08;
    } else if (_RGBAddr == 0x6B) {
        REG_RED   = 0x06;  // pwm2
        REG_GREEN = 0x05;  // pwm1
        REG_BLUE  = 0x04;  // pwm0
        REG_ONLY  = 0x04;
    } else if (_RGBAddr == 0x2D) {
        REG_RED   = 0x01;  // pwm2
        REG_GREEN = 0x02;  // pwm1
        REG_BLUE  = 0x03;  // pwm0
        REG_ONLY  = 0x01;
    }


    // Setup LCD function
    _showFunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    //HAL_Delay(5000);
    printf("entering begin\n");
    begin(_rows);  // Call your LCD initialization function
}

/**
 * Block the processor for the desired number of milliseconds.
 * @param ms The number of milliseconds to block for.
 */
void DelayMsDFR(uint32_t ms) {
    uint32_t curr_ms = TIMERS_GetMilliSeconds();
    while ((TIMERS_GetMilliSeconds() - curr_ms) < ms);
}


void clear()
{
    command(LCD_CLEARDISPLAY);        // Clear display, set cursor position to zero
    DelayMsDFR(2);     // Delay for 2000 microseconds (2 milliseconds)
    //vTaskDelay(pdMS_TO_TICKS(2));     // Delay for 2000 microseconds (2 milliseconds)
}

void home()
{
    command(LCD_RETURNHOME);          // Set cursor position to zero
    DelayMsDFR(2);    // Delay for 2000 microseconds (2 milliseconds)
}
// void DFRobot_RGBLCD1602::noDisplay()
// {
//     _showControl &= ~LCD_DISPLAYON;
//     command(LCD_DISPLAYCONTROL | _showControl);
// }

void display() {
    _showControl |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _showControl);
}

// void DFRobot_RGBLCD1602::stopBlink()
// {
//     _showControl &= ~LCD_BLINKON;
//     command(LCD_DISPLAYCONTROL | _showControl);
// }
// void DFRobot_RGBLCD1602::blink()
// {
//     _showControl |= LCD_BLINKON;
//     command(LCD_DISPLAYCONTROL | _showControl);
// }

// void DFRobot_RGBLCD1602::noCursor()
// {
//     _showControl &= ~LCD_CURSORON;
//     command(LCD_DISPLAYCONTROL | _showControl);
// }

// void DFRobot_RGBLCD1602::cursor() {
//     _showControl |= LCD_CURSORON;
//     command(LCD_DISPLAYCONTROL | _showControl);
// }

void scrollDisplayLeft(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void scrollDisplayRight(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// void DFRobot_RGBLCD1602::leftToRight(void)
// {
//     _showMode |= LCD_ENTRYLEFT;
//     command(LCD_ENTRYMODESET | _showMode);
// }

// void DFRobot_RGBLCD1602::rightToLeft(void)
// {
//     _showMode &= ~LCD_ENTRYLEFT;
//     command(LCD_ENTRYMODESET | _showMode);
// }

void noAutoscroll(void)
{
    _showMode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _showMode);
}

void autoscroll(void)
{
    _showMode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _showMode);
}

// void DFRobot_RGBLCD1602::customSymbol(uint8_t location, uint8_t charmap[])
// {

//     location &= 0x7; // we only have 8 locations 0-7
//     command(LCD_SETCGRAMADDR | (location << 3));
    
    
//     uint8_t data[9];
//     data[0] = 0x40;
//     for(int i=0; i<8; i++)
//     {
//         data[i+1] = charmap[i];
//     }
//     send(data, 9);
// }

void setCursor(uint8_t col, uint8_t row)
{

    col = (row == 0 ? col|0x80 : col|0xc0);
    uint8_t data[3] = {0x80, col};

    send(0x80, col);

}

void setRGB(uint8_t r, uint8_t g, uint8_t b)
{
  uint16_t temp_r,temp_g,temp_b;
  if(_RGBAddr == 0x60 >> 1){
    temp_r = (uint16_t)r*192/255;
    temp_g = (uint16_t)g*192/255;
    temp_b = (uint16_t)b*192/255;
    setReg(REG_RED, temp_r);
    setReg(REG_GREEN, temp_g);
    setReg(REG_BLUE, temp_b);
  } else{
    setReg(REG_RED, r);
    setReg(REG_GREEN, g);
    setReg(REG_BLUE, b);
    if(_RGBAddr == 0x6B){
      setReg(0x07, 0xFF);
    }
  }

}

// void DFRobot_RGBLCD1602::setColor(uint8_t color)
// {
//     if(color > 3)return ;
//     setRGB(color_define[color][0], color_define[color][1], color_define[color][2]);
// }


int write(uint8_t value)
{

    //uint8_t data[2] = {0x40, value};
    send(0x40, value);
    return 1; // assume sucess
}

inline void command(uint8_t value)
{
    printf("sending command \n");
   // uint8_t data_u, data_l;
  //  data_u = value & 0xf0;
  //  data_l = (value<<4) & 0xf0;
// Allocate 2 bytes on the heap
    uint8_t reg_addr = 0x80;

    
    
    // uint8_t* data = new uint8_t[4];  
    // data[0] = data_u | 0x0D;
    // data[1] = data_u | 0x09;
    // data[2] = data_l | 0x0D;
    // data[3] = data_l | 0x09;
    send(reg_addr, value);
}



// void DFRobot_RGBLCD1602::setBacklight(bool mode){
// 	if(mode){
// 		setColorWhite();		// turn backlight on
// 	}else{
// 		closeBacklight();		// turn backlight off
// 	}
// }

// /*******************************private*******************************/
void begin(uint8_t rows) 
{
    uint8_t charSize = LCD_5x8DOTS;

    if (rows > 1) {
        _showFunction |= LCD_2LINE;
    }
    _numLines = rows;
    _currLine = 0;

    // For some 1 line displays you can select a 10 pixel high font
    if ((charSize != 0) && (rows == 1)) {
        _showFunction |= LCD_5x10DOTS;
    }

    // Wait for power stabilization (at least 40ms after VCC > 2.7V)
    DelayMsDFR(100);
    //vTaskDelay(50 / portTICK_PERIOD_MS);

    // Send function set command sequence according to the HD44780 datasheet
  //  HAL_Delay(5000);
    printf("sending first command\n");
    command(LCD_FUNCTIONSET | _showFunction);
   // HAL_Delay(5000);
    //vTaskDelay(5 / portTICK_PERIOD_MS);  // wait more than 4.1ms
    DelayMsDFR(5);



    // Second try
    command(LCD_FUNCTIONSET | _showFunction);
    //vTaskDelay(5 / portTICK_PERIOD_MS);
    DelayMsDFR(5);

    // Third try
    command(LCD_FUNCTIONSET | _showFunction);

    // Turn the display on with no cursor or blinking default
    _showControl = LCD_DISPLAYON ;//| LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // Clear the display
    clear();

    // Initialize to default text direction (for romance languages)
    _showMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // Set the entry mode
    command(LCD_ENTRYMODESET | _showMode);
    
    // RGB backlight initialization

    // if (_RGBAddr == (0xc0)) {
    //     // Backlight initialization
    //     printf("1\n");
        setReg(REG_MODE1, 0);
        setReg(REG_OUTPUT, 0xFF);
        setReg(REG_MODE2, 0x20);  // Set MODE2 values
    // } else if (_RGBAddr == (0x60 >> 1)) {
    //             printf("2\n");
    //     setReg(0x01, 0x00);
    //     setReg(0x02, 0xFF);
    //     setReg(0x04, 0x15);
    // } else if (_RGBAddr == 0x6B) {
    //             printf("3\n");
    //     setReg(0x2F, 0x00);
    //     setReg(0x00, 0x20);
    //     setReg(0x01, 0x00);
    //     setReg(0x02, 0x01);
    //     setReg(0x03, 4);
    // } 
    // printf("initialized registers\n");

    // Set color to white
    setColorWhite();
}

void setColorWhite(){setRGB(255, 255, 255);}

void send(uint8_t reg_addr, uint8_t data)
{
    printf("in send\n");
    printf("data %x %x\n",reg_addr,data);
    I2C_WriteRegDFRobot(_lcdAddr, reg_addr, data);
    // ESP_ERROR_CHECK(i2c_master_write_to_device(_i2cPort, _lcdAddr >> 1, data, len, 1000/ portTICK_PERIOD_MS));
   // ESP_ERROR_CHECK(i2c_master_transmit(_dev_handle, data, len, 10000));

   /*
     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    // Start I2C communication
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    
    // Write the slave address with the write bit
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (_lcdAddr << 1) | I2C_MASTER_WRITE, true));
    
    // Write the data bytes
    ESP_ERROR_CHECK(i2c_master_write(cmd, data, len, true));
    
    // Send a stop bit to end the transaction
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    
    // Execute the command link
    ESP_ERROR_CHECK(i2c_master_cmd_begin(_i2cPort, cmd, 1000 / portTICK_PERIOD_MS));
    
    // Delete the command link to free up resources
    i2c_cmd_link_delete(cmd);
*/
               // stop transmitting
}

void setReg(uint8_t addr, uint8_t data)
{
    printf("setting register\n");
    I2C_WriteRegDFRobot(_RGBAddr, addr, data);
}


void printstr(const char* string){
 //   if(strlen(string) > _cols){
 //   }
        for(int i = 0; i < strlen(string); i++){
            write((uint8_t) string[i]);
    }
}



