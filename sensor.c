#include <xc.h>
#include "configBits.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "lcd.h"
#include "global_variable.h"
#include "I2C.h"
#include "prototypes.h"

int hole_drop_bool_function(void){
    if (last_dropped == false){
        last_dropped = true;return 1;
    }
    else{
    float x = last_problem_bool[0];
    float y = last_problem_bool[1];
    //bool last_droped = false;or true
    //if false, drop 
    //if true, decide. 
    
    if (x + y ==0){
        last_dropped = true;return 1;
    }
    else if (last_problem_bool[0] == 1) {//hole 
        if (accum_straight_distance - y >= 0.55){ // 15//unit in cm
        last_dropped = true;return 1;}
        else{
            last_dropped = false;
            return 0;}
    }
    else{
        if (accum_straight_distance - y >= 0.75){//20 ~ 0.75 cycles ish 
            last_dropped = true;return 1;}
        else{last_dropped = false;return 0;}
    }
}
}
int crack_drop_bool_function(void){
    if (last_dropped == false){
        last_dropped = true;
        return 1;
    }
    else{
    
    float x = last_problem_bool[0];
    float y = last_problem_bool[1];
    if (x+y ==0){
        last_dropped = true;
        return 1;
    }
    else if (last_problem_bool[0] == 1) {//last is hole
        if (accum_straight_distance - y >= 0.75){  //unit in cm
            last_dropped = true;
            return 1;}
        else{
            last_dropped = false;
            return 0;}
    }
    else{
        if (accum_straight_distance - y >= 0.36){//10 ~ 0.5
            last_dropped = true;
            return 1;
        }
        else{last_dropped = false;return 0;
        }
    }
}
}

void drop_record(int aa){
    drop_identity[add_index] = aa;
    drop_position[add_index] = accum_straight_distance + car_length;
    add_index +=1;
    /*printf("hhhhh,%d,%.2f",aa,accum_straight_distance + car_length);
    __delay_ms(2000);*/
}
void sensed_function_3(void){
    read_encoder();
    if (a+c == 2){
        int sth = crack_drop_bool_function();
        if (sth){
            drop_record(2);
            //middle_crack_drop();
            last_problem_bool[0] = 0;
            last_problem_bool[1] = accum_straight_distance;
        }
        record('c');
    }
    else if (a == 1){
        /*printf("left crack");
        __delay_ms(2000);*/
        int sth = crack_drop_bool_function();
        if (sth){
            drop_record(1);
            //left_crack_drop();
            last_problem_bool[0] = 0;
            last_problem_bool[1] = accum_straight_distance;
        }
        record('c');
    }
    else if (c ==1){
        /*printf("right crack");
        __delay_ms(2000);*/
        int sth = crack_drop_bool_function();
        if (sth){
            drop_record(3);
            //right_crack_drop();
            last_problem_bool[0] = 0;
            last_problem_bool[1] = accum_straight_distance;
        }
        record('c');
    }
    else{
        /*printf("hole");
        __delay_ms(2000);*/
        int sth = hole_drop_bool_function();
        if (sth ==1){
            drop_record(4);
            //hole_dispense_function();
            last_problem_bool[0] = 1;
            last_problem_bool[1] = accum_straight_distance;
        }
        record('h');
    }
    while(PORTBbits.RB3 + PORTAbits.RA1 + PORTBbits.RB2 > 0){
        moving();
    }
    Stop();
}
void record(char corh){
    if (corh == 'c'){
        array_cracks_distance[crack_counter] = (int) (accum_straight_distance*28.3);//27.5
        crack_counter +=1;
        if (last_sensed == 2){
            LATDbits.LATD0 = 1;
            LATDbits.LATD1 = 0;
            __delay_ms(wety_deg);//180 deg
        }
        else if(last_sensed == 0){
            LATDbits.LATD0 = 1;
            LATDbits.LATD1 = 0;
            __delay_ms(ninty_deg);//90 deg
        }
        else{
            LATDbits.LATD0 = 1;
            LATDbits.LATD1 = 0;
            __delay_ms(50);//shake
            LATDbits.LATD0 = 0;
            LATDbits.LATD1 = 0;
            __delay_ms(50);//shake
            LATDbits.LATD0 = 0;//
            LATDbits.LATD1 = 1;
            __delay_ms(50);//shake
        }
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 0;
        
        last_sensed = 1;
    
    }
    else if (corh == 'h'){
        array_holes_distance[hole_counter] = (int) (accum_straight_distance*28.3);
        hole_counter +=1;
        if (last_sensed == 1){
            LATDbits.LATD0 = 1;
            LATDbits.LATD1 = 0;
            __delay_ms(wety_deg);//180 deg
        }
        else if (last_sensed == 0){
            LATDbits.LATD0 = 0;
            LATDbits.LATD1 = 1;
            __delay_ms(ninty_deg);//90 deg
        }
        else{
            LATDbits.LATD0 = 1;
            LATDbits.LATD1 = 0;
            __delay_ms(50);//shake
            LATDbits.LATD0 = 0;
            LATDbits.LATD1 = 0;
            __delay_ms(50);//shake
            LATDbits.LATD0 = 0;
            LATDbits.LATD1 = 1;
            __delay_ms(50);//shake
        }
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 0;
        last_sensed = 2;
    
    }
    else{
        printf("wrong");
        __delay_ms(10000);
    }
    
}

//original idea of moving sensor 
/*void sensed_function_1(void){
        lcd_clear();
        printf("sensed");
        //update straight distance,then stop counting 
        accum_straight_distance+=read_encoder_function(Perimeter_wheels);
        LATC = Stop;
        INT0IF = 0;
        INT0IE = 0;
        //TRISBbits.RB0 = 0;
        
        shape_distance=0;
        sensor_return_distance=0;
        turns_counter =0;
        rotary_counter = 0;
        //enable sensor encoder
        TRISBbits.RB2 = 1;
        INT2IF = 0;
        INT2IE = 1;
        INTEDG2 = 1;
        while (PORTAbits.RA0 == 0){
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 1;//left
        }    
            printf("moving left");
            __delay_ms(1000);
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 0;//Stop
        sensor_return_distance+= read_encoder_function(Perimeter_sensor_mover);
        printf("moved %0.1f",sensor_return_distance);
        __delay_ms(1000);
        while(PORTAbits.RA0 !=0){// adjust sensor position to enter the tape area again
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 0;//Right
            
        }
            printf("moving right null");
            __delay_ms(1000);
        turns_counter =0;
        rotary_counter = 0;
                
        while(PORTAbits.RA0 == 0){
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 0;//Right
            }
        printf("moving right");
            __delay_ms(1000);
        
        LATEbits.LATE0 = 0;
        LATEbits.LATE1 = 0;//Stop
        shape_distance+= read_encoder_function(Perimeter_sensor_mover);
        printf("shape distance %0.1f",shape_distance);
        __delay_ms(1000);
        sensor_return_distance =shape_distance-sensor_return_distance;
        printf("return %0.1f cm",sensor_return_distance);
        __delay_ms(1000);
        turns_counter =0;
        rotary_counter = 0;
        while(move_encoder_function(Perimeter_sensor_mover,sensor_return_distance)){
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 1; //left
            
        }
            printf("sensor returning");
            __delay_ms(1000);
        //INT2IF = 0;
        //INT2IE = 0;
        TRISBbits.RB2 = 0;
        
        LATEbits.LATE0 = 0;
        LATEbits.LATE1 = 0;//Stop
        if (distinguish_H_C_function() == 1){
            sensingH();
            TRISCbits.RC0=0;
            LATCbits.LATC0 = 1;
            if (hole_drop_bool_function() ==1){
                hole_dispense_function();
                cones_deployed+=1;
            }
            hole_counter +=1;
            //last_problem_bool = 1;
            array_holes_distance[hole_counter] = (int)(accum_straight_distance);
            while(PORTAbits.RA0 ==0){
            LATC = forw;}
            LATC = Stop;}
        else if (distinguish_H_C_function()==0){
            sensingC();
            TRISCbits.RC0=0;
            LATCbits.LATC0 = 0;
            if (crack_drop_bool_function() ==1){
                crack_dispense_function();
                cones_deployed+=1;}
            crack_counter+=1;
            //last_problem_bool =0;
            array_cracks_distance[crack_counter] =(int) (accum_straight_distance);
            while(PORTAbits.RA0 ==0){
                LATC = forw;} //moved over the solved problem. distance count 
            LATC = Stop;
        }
        //end of the external interrupt handler
        
}
int distinguish_H_C_function(void){
    if ((shape_distance < 6) && (shape_distance >2)){
        return 1;}//hole
    else if ((shape_distance >13)&&(shape_distance <17)){
        return 0;}//crack
    else{
        return 3;}
}*/