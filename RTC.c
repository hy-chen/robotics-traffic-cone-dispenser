/*
@author Hongyu Chen
*/

#include <xc.h>
#include "configBits.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "lcd.h"
#include "global_variable.h"
#include "I2C.h"
#include "prototypes.h"


void readRTC(void){
    I2C_Master_Init(100000);
    // Reset RTC memory pointer
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
        I2C_Master_Write(0x00); // Set memory pointer to seconds
        I2C_Master_Stop(); // Stop condition
    // Read current time
        char t_data = 0;
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
        for(unsigned char i = 0; i < 6; i++){
            t_data = I2C_Master_Read(ACK);
            time[i] = (t_data>>4)*10+(t_data&0x0F);
        }
        t_data = I2C_Master_Read(NACK);
        time[6] = (t_data>>4)*10+(t_data&0x0F); // Final Read with NACK
        I2C_Master_Stop(); // Stop condition       
}

void rtc_set_time(void){
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    
    // Write array
    for(char i=0; i < 7; i++){
        I2C_Master_Write(happynewyear[i]);
    }
    
    I2C_Master_Stop(); //Stop condition
}
