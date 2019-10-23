/*
@author Hongyu Chen
*/

#include <xc.h>
#include "configBits.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "I2C.h"
#include "lcd.h"
#include "global_variable.h"
#include "prototypes.h"


void main(void){
    configureports();
    initLCD();
    lcd_display_control(true, false, false);
    unsigned long ticks = 0;
    total_op = read_total();
    while(1){
        if(!key){
            a = PORTBbits.RB2;
            b = PORTBbits.RB3;
            c = PORTAbits.RA1;
            read_encoder();
            if ((completion_bool == false)&&((accum_straight_distance >= whole_distance)||(operation_sec >= 170))){//continue to adjust time limit
                Stop();
                completion_return();
            }
            else if((no_cone == false)&&(completion_bool == false)&&(drop_identity[drop_index]!=0)&&(accum_straight_distance >= drop_position[drop_index])){
                Stop();
                drop_function();
            }
            else if((completion_bool==false)&&(a+b+c > 0)){
                Stop();
                sensed_function_3();
                initmoving_disp();
            }
            else if(completion_bool == false){
                moving();
                if (ticks%1000 == 0){
                    normal_updater();
                }
            }
            else if ((completion_bool == true)&&(ticks%3000 == 0)) {
                lcd_clear();
                if (reg == 0){
                    standby_rotating();
                }
                else if ((reg == 10) ||(reg == 20)||(reg == 30)||(reg ==40)){
                    data_select();
                    if ((ddp ==0)&&(reg%10==0)){
                        display_repeat +=1;
                    }
                    if (display_repeat > 1){
                        lcd_clear();
                        printf("Returning to");
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("the Main Menu...");
                        __delay_ms(500);
                        display_repeat = 0;
                        reg = 0;}
                }
                else if ((reg/10 > 0)&&(reg/10 < 5)&&(reg%10 > 0)){
                    data_disp();
                    reg = (reg/10)*10;
                    display_repeat = 0;
                }
                else if (reg ==50){
                    clear_select();
                    lcd_set_ddram_addr(LCD_LINE3_ADDR);
                    printf("exit in %d sec",clear_waiter);
                    clear_waiter = clear_waiter - 1;
                    if (clear_waiter ==0){
                        clear_waiter = 3;
                        reg = 0;
                    }   
                }
                else if(reg ==51){
                    clear_finish();
                    reg =0;
                    clear_waiter =3;
                }
            }
        ticks++;
        }
        else{//set up: adjust indicator flag position
            LATDbits.LATD0 = 1;
            LATDbits.LATD1 = 0;
            __delay_ms(1);
            LATDbits.LATD0 = 0;
            __delay_ms(0.8);
        }
    }
}

void configureports(void){
    //configure I/O pins
    TRISD = 0;//D0-D7 LCD display
    //D0 D1 = flag motor
    TRISBbits.RB4 = 1;//keypad 
    TRISBbits.RB5 = 1;
    TRISBbits.RB6 = 1;
    TRISBbits.RB7 = 1;
    TRISBbits.RB1 = 1;
    
    TRISCbits.RC3 = 1;//RTC 
    TRISCbits.RC4 = 1;
    
    TRISBbits.RB0 = 1;//encoder of wheels
    
    TRISBbits.RB2 = 1;//middle sensor 1
    TRISBbits.RB3 = 1;//middle sensor 2
    TRISAbits.RA1 = 1;//middle sensor 3
    
    TRISAbits.RA3 = 1;//cone weight sensor
    
    //outputs 
    TRISAbits.RA4 = 0;//gate 1 servo
    TRISAbits.RA5 = 0;//gate 2 servo
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
    
    
    TRISEbits.RE0 = 1;//1st sensor //original sensor motor1 //change in the sub function if confirm resting sensor design
    TRISEbits.RE1 = 1;//2nd sensor //original sensor motor2
    
    TRISCbits.RC1 = 0;//wheel w/o encoder
    TRISCbits.RC2 = 0;//wheel w/o encoder 
    TRISCbits.RC5 = 0;//wheel connected
    TRISCbits.RC6 = 0;//wheel connected
    TRISCbits.RC7 = 0;//wheel w/ encoder
    TRISCbits.RC0 = 0;//wheel w/ encoder
    LATC =0;
    LATD =0;
    // Set all A/D ports to digital (pg. 222)
    ADCON1 = 0b00001111;
    //enable all interrupts
    INT0IE = 1;
    INTEDG0 = 1;
    //INT2IE = 1;
    INT1IE = 1;
    ei();
    
}

void normal_updater(void){//regular time counter updater
    readRTC();
    end_sec = time[2]*3600+time[1]* 60 + time[0];
    operation_sec = end_sec - start_sec;
    operation_time[2] = operation_sec/3600;
    operation_time[1] = (operation_sec%3600)/60;
    operation_time[0] = (operation_sec%3600)%60;
    }
void completion_return(void){
    //communicate with the user about the state
    lcd_clear();
    printf("returning");
    //put down indicator flag
    if (last_sensed == 1){
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 1;
        __delay_ms(ninty_deg);//90 deg
        LATDbits.LATD1 = 0;
    }
    else if(last_sensed == 2){
        LATDbits.LATD0 = 1;
        LATDbits.LATD1 = 0;
        __delay_ms(ninty_deg);//180 deg
        LATDbits.LATD0 = 0;
    }
    //initiate and prepare distance counter for returning 
    INT0IF = 0;
    INT0IE = 1;//encoder input
    INTEDG0 = 1;
    turns_counter=0;
    rotary_counter=0;
    //Slide to the right
    while (turns_counter < (car_width/2 + half_lane_width)){
        R_I();
    }
    Stop();
    //Move backward
    turns_counter=0;
    rotary_counter=0;
    while (turns_counter < accum_straight_distance + 1.8 ){
        backw();
    }
    Stop();
    //Slide to the left
    turns_counter=0;
    rotary_counter=0;
    while (turns_counter < car_width/2 + half_lane_width){
        L_I();
    }
    Stop();
    //clear the counter 
    turns_counter=0;
    rotary_counter=0;
    //switch off wheel encoder interrupt
    INT0IF = 0;
    INT0IE = 0;
    //sliding door close if it was open
    unsigned long tick = 0;
    if (no_cone == true){
        while(tick<60){//pwm period = 20ms, duty cycle = 2.5ms
            LATAbits.LATA5 = 1;
            __delay_ms(2.5);
            LATAbits.LATA5 = 0;
            __delay_ms(16.5);
            tick ++;
            __delay_ms(1);
        }
        tick = 0;
    }
    //update the final operation time
    readRTC();
    end_sec = time[2]*3600+time[1]* 60 + time[0];//convert to seconds for one-step subtraction
    operation_sec = end_sec - start_sec;
    operation_time[2] = operation_sec/3600;
    operation_time[1] = (operation_sec%3600)/60;
    operation_time[0] = (operation_sec%3600)%60;//convert back to hh,mm,ss format
    //write to permanent memory
    completion_write();
    //set global variables that re-direct the branch for next main loop
    completion_bool = true;
    disp_standby_page =0;
}
void initialize_func(void){
    //Restart time and distance counter
    readRTC();
    start_sec = time[2]*3600 + time[1]* 60 + time[0];
    start_time[1] = time[2];
    start_time[0] = time[1];
    operation_sec = 0;
    rotary_accum = 0;
    accum_straight_distance=0;
    turns_counter=0;
    rotary_counter=0;
    //Initialize record data variables
    cones_deployed=0;
    hole_counter = 0;
    crack_counter =0;
    //Initialize operational global variables
    last_sensed = 0;
    no_cone = false;
    drop_index = 0;
    add_index = 0;
    reg = 0;
    ddp = 0;
    last_dropped = false;
    last_problem_bool[0] = 0;
    last_problem_bool[1] = 0;
    //Clear global variables
    for (unsigned int g = 0;g<12;g++){
        array_holes_distance[g]= 0;//record data
        array_cracks_distance[g]=0;//record data
        drop_identity[g]=0;//operational global variables
        drop_position[g]=0;//operational global variables
    }
    //Enable encoder interrupt
    INT0IE = 1;
    INTEDG0 = 1;
    ei();
    
    initmoving_disp();//display "running"
    normal_updater();//start operation time counter
    completion_bool = false;//toggle case variable 
}
