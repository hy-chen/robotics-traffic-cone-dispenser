#include <xc.h>
#include "configBits.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "lcd.h"
#include "global_variable.h"
#include "I2C.h"
#include "prototypes.h"


void read_encoder(void){
    rotary_accum+=rotary_counter;
    accum_straight_distance= rotary_accum / 334;
    turns_counter = 0;
    rotary_counter = 0;
}

void moving(void){
    if ((PORTEbits.RE0 && PORTEbits.RE1) ==1){
        straight();
    }
    if(PORTEbits.RE1 ==0){
        turn_left();
    }
    else if (PORTEbits.RE0 ==0){
        turn_right();
    }
}
void straight(void){
    LATCbits.LATC5 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC7 = 0;
    
    LATCbits.LATC2 = 1;
    LATCbits.LATC6 = 1;
    LATCbits.LATC0 = 1;
    __delay_ms(7);
    LATCbits.LATC2 = 0;
    LATCbits.LATC6 = 1;
    LATCbits.LATC0 = 1;
    __delay_ms(1);
    LATCbits.LATC2 = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC0 = 0;
    __delay_ms(2.5);
}
void turn_left(void){//10ms
    LATCbits.LATC5 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC7 = 0;
    
    LATCbits.LATC2 = 1;
    LATCbits.LATC6 = 1;
    LATCbits.LATC0 = 1;
    __delay_ms(8);
    LATCbits.LATC2 = 1;
    LATCbits.LATC6 = 0;
    LATCbits.LATC0 = 0;
    __delay_ms(1);
    LATCbits.LATC2 = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC0 = 0;
    __delay_ms(1.5);
}
void turn_right(void){//10ms 8 1 1 
    LATCbits.LATC5 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC7 = 0;
    
    LATCbits.LATC2 = 1;
    LATCbits.LATC6 = 1;
    LATCbits.LATC0 = 1;
    __delay_ms(5);
    LATCbits.LATC2 = 0;
    LATCbits.LATC6 = 1;
    LATCbits.LATC0 = 1;
    __delay_ms(4);
    LATCbits.LATC2 = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC0 = 0;
    __delay_ms(1.5);
}

void L_I(void){
    LATCbits.LATC6 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC7 = 0;
    
    LATCbits.LATC0 = 1;
    LATCbits.LATC5 = 1;
    LATCbits.LATC2 = 1;
    __delay_ms(4.5);
    LATCbits.LATC5 = 0;
    __delay_ms(1);
    LATCbits.LATC0 = 1;
    LATCbits.LATC2 = 0;
    __delay_us(700);
}
void Stop(void){
    LATCbits.LATC6 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC7 = 0;
    LATCbits.LATC0 = 0;
    LATCbits.LATC5 = 0;
    LATCbits.LATC2 = 0;
}
void R_I(void){
    LATCbits.LATC0 = 0;
    LATCbits.LATC5 = 0;
    LATCbits.LATC2 = 0;
    
    LATCbits.LATC6 = 1;
    LATCbits.LATC1 = 1;
    LATCbits.LATC7 = 1;
    __delay_ms(5);
    LATCbits.LATC6 = 0;
    LATCbits.LATC1 = 0;
    __delay_ms(1.2);
    LATCbits.LATC1 = 1;
    __delay_ms(0.5);
    LATCbits.LATC6 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC7 = 1;
    __delay_us(700);
}
void backw(void){
    LATCbits.LATC6 = 0;
    LATCbits.LATC2 = 0;
    LATCbits.LATC0 = 0;
    
    LATCbits.LATC1 = 1;
    LATCbits.LATC5 = 1;
    LATCbits.LATC7 = 1;
    __delay_us(100);
}