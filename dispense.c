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


void drop_function(void){
    if (planB <2){
        if (drop_identity[drop_index] == 1){
            left_crack_drop();
        }
        else if(drop_identity[drop_index] == 2){
            middle_crack_drop();
        }
        else if(drop_identity[drop_index] == 3){
            right_crack_drop();
        }
        else if(drop_identity[drop_index] == 4){//or just else
            hole_dispense_function();
        }
    }
    else{
        hole_dispense_function();
    }
    drop_index+=1;
}
void hole_dispense_function(void){
    unsigned long tick = 0;
    if (PORTAbits.RA3 == 1){
        lcd_clear();
        printf("cone used up");
        while(tick<70){//gate 1 close
            LATAbits.LATA4 = 1;
            __delay_ms(1.2);

            LATAbits.LATA4 = 0;
            __delay_ms(17.8); 
            //}    
            tick ++;
            __delay_ms(1);
        }
        tick = 0;
        while(tick<60){//175//1.5//55  gate 2 open 
            LATAbits.LATA5 = 1;
            __delay_ms(0.5);//1

            LATAbits.LATA5 = 0;
            __delay_ms(18.5);  
            //}
            tick ++;
            __delay_ms(1);
        }
        
        tick =0;
        while(tick<70){//gate 1 open 
            LATAbits.LATA4 = 1;
            __delay_ms(1.85);//1.9 = 85

            LATAbits.LATA4 = 0;
            __delay_ms(17.15); //17.1
            //}
            tick ++;
            __delay_ms(1);
        }
        tick = 0;
        __delay_ms(100);
        cones_deployed +=1;
        no_cone = true;
    }
    
    else{
        tick = 0;
    while(tick<70){//gate 1 close
        
        LATAbits.LATA4 = 1;
        __delay_ms(1.2);
        
        LATAbits.LATA4 = 0;
        __delay_ms(17.8); 
        //}    
        tick ++;
        __delay_ms(1);
    }
    
    tick = 0;
    /*lcd_clear();
    printf("gate 2 open");*/
        
    while(tick<60){
        LATAbits.LATA5 = 1;
        __delay_ms(0.5);//1
        
        LATAbits.LATA5 = 0;
        __delay_ms(18.5);  
        //}
        tick ++;
        __delay_ms(1);
    }
    __delay_ms(100);
    tick = 0;
    /*lcd_clear();
    printf("gate 2 close");*/
        
    while(tick<60){ 
        LATAbits.LATA5 = 1;
        __delay_ms(2.5);
        
        LATAbits.LATA5 = 0;
        __delay_ms(16.5);
        //}    
        tick ++;
        __delay_ms(1);
    }
    __delay_ms(100);
    tick = 0;
    /*lcd_clear();
    printf("gate 1 open");*/
        
    while(tick<70){
        LATAbits.LATA4 = 1;
        __delay_ms(1.85);
        LATAbits.LATA4 = 0;
        __delay_ms(17.15);
        tick ++;
        __delay_ms(1);
    }
    tick = 0;
    __delay_ms(100);
    cones_deployed +=1;
    }
}
void middle_crack_drop(void){
        turns_counter =0;
        rotary_counter = 0;
        float A = turns_counter;
        float B = turns_counter;
        while((B - A) < mc_L){
            L_I();
            B = turns_counter;
        }
        Stop();
        hole_dispense_function();
        //
        
        turns_counter =0;
        rotary_counter = 0;
        A = turns_counter;
        B = turns_counter;
        while((B - A) < car_width){
            R_I();
            B = turns_counter;
        }
        Stop();
        if(no_cone == false){
        hole_dispense_function();}
        //
        turns_counter =0;
        rotary_counter = 0;
        A = turns_counter;
        B = turns_counter;
        while((B - A) < car_width - mc_L){
            L_I();
            B = turns_counter;
        }
        Stop();
        turns_counter =0;
        rotary_counter = 0;
}


void left_crack_drop(void){
        turns_counter =0;
        rotary_counter = 0;
        float A = turns_counter;
        float B = turns_counter;
        while((B - A) < lc_R-0.05){
            R_I();
            B = turns_counter;
        }
        Stop();
        hole_dispense_function();
        //
        turns_counter =0;
        rotary_counter = 0;
        A = turns_counter;
        B = turns_counter;
        while((B - A) < car_width-0.05){
            L_I();
            B = turns_counter;
        }
        Stop();
        if(no_cone == false){
        hole_dispense_function();}
        //
        turns_counter =0;
        rotary_counter = 0;
        A = turns_counter;
        B = turns_counter;
        while((B - A) < car_width - lc_R){
            R_I();
            B = turns_counter;
        }
        Stop();
        turns_counter =0;
        rotary_counter = 0;
}

void right_crack_drop(void){
        turns_counter =0;
        rotary_counter = 0;
        float A = turns_counter;
        float B = turns_counter;
        while((B - A) < lc_R){
            L_I();
            B = turns_counter;
        }
        Stop();
        hole_dispense_function();
        //
        turns_counter =0;
        rotary_counter = 0;
        A = turns_counter;
        B = turns_counter;
        while((B - A) < car_width){
            R_I();
            B = turns_counter;
        }
        Stop();
        if(no_cone == false){
        hole_dispense_function();}
        //
        turns_counter =0;
        rotary_counter = 0;
        A = turns_counter;
        B = turns_counter;
        while((B - A) < car_width - lc_R){
            L_I();
            B = turns_counter;
        }
        Stop();
        turns_counter =0;
        rotary_counter = 0;
        
        
}
