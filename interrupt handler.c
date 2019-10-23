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

void high_priority interrupt interruptHandler(void){
    // Interrupt on change handler for RB1
    if(INT1IF){
        // Notice how we keep the interrupt processing very short by simply
        // setting a "flag" which the main program loop checks
        INT1IF = 0; // Clear interrupt flag bit to signify it's been handled
        char keypress = (PORTB & 0xF0) >> 4;
        char command = keys[keypress];
        //lcd_clear();
        //0-4 :48 49 50 51 52;
        if((command == '1') ||(command == '2')||(command == '3')||(command =='4') ){
            
            if (reg == 0){
                if ((command - 48) > total_op){
                    lcd_clear();
                    printf("Oops. I don't get");
                    lcd_set_ddram_addr(LCD_LINE2_ADDR);      
                    printf("it... Please");
                    lcd_set_ddram_addr(LCD_LINE3_ADDR);
                    printf("Review the Menu.");
                    __delay_ms(500);
                }
                else{
                reg = (command - 48)*10;
                display_repeat = 0;}
            }
            else if ((reg == 10)||(reg == 20)||(reg == 30)||(reg == 40)){
                reg = reg + command - 48;
            }
        }
        else if (command == 48){
            
            if (reg == 0){
                reg =50;
            }
        }
        else if (command == 'C'){
            
            if(reg == 50){
                reg = 51;
                clear_waiter = 3;
            }
        }
        else if(command =='A'){//start command
            
            initialize_func();
        }
        else if(command == 'D'){
            key = !key;
            LATDbits.LATD0 = 0;
            LATDbits.LATD1 = 0;
        }
        else if (command == 'B'){
            lcd_clear();
            planB +=1;
            planB = planB%4;
            if (planB%4 == 1){
                printf("planB?");
                }//pressed once
            else if (planB%4 == 2){
                printf("planB set");
                }
            else if (planB%4 == 3){
                printf("planA?");
            }
            else if (planB%4 == 0){
                printf("planA set");
            }
            __delay_ms(800);
        }

        else{
            
            if (reg == 50){
                reg = 0;
            }
            else{
                lcd_clear();
                printf("Oops. I don't get");
                lcd_set_ddram_addr(LCD_LINE2_ADDR);      
                printf("it... Please");
                lcd_set_ddram_addr(LCD_LINE3_ADDR);
                printf("Review the Menu.");
                __delay_ms(500);
            }
        }
        
    }
    else if(INT0IF){
        int b0 = PORTBbits.RB0;
        if (b0==1){//read high inputs
            rotary_counter +=1;
            turns_counter = rotary_counter / 334;
        }
        INT0IF= 0;
    }
}   
