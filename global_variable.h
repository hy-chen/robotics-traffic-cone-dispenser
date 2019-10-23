/* 
 * File:   global_variable.h
 * Author: Hongyu Chen
 *
 * Created on February 24, 2019, 9:51 PM
 */
#include <xc.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef GLOBAL_VARIABLE_H
#define	GLOBAL_VARIABLE_H

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif

//RTC variables/////
unsigned char happynewyear[7] = {//current local time, for initializing RTC
    0x20, // 20 Seconds
    0x20, // 20 Minutes
    0x20, // 24 hour mode, set to 20:00
    0x03, // Wednesday
    0x03, // 03st
    0x19,  // 2019
    0x04, // April
};
unsigned char time[7] = {//declare the data type of real-time array
    0x45, // 45 Seconds 
    0x59, // 59 Minutes
    0x23, // 24 hour mode, set to 23:00
    0x00, // Sunday
    0x31, // 31st
    0x12, // December
    0x18  // 2018
};

//"transition variables" that will be computed and then used as inputs in the robot main operating routine///////
float rotary_counter=0;//instant number of ticks traveled
float turns_counter=0;// instant number of cycles traveled
float rotary_accum = 0;// total number of ticks traveled in the straight line
float accum_straight_distance=0;// total number of cycles traveled in the straight line
long start_sec=0;// real start time converted to second form 
long end_sec=0;// real current time converted to second form 
long operation_sec=0;// time difference between start_sec and end_sec in the unit of seconds
unsigned int a = 0;//left detection sensor reading
unsigned int b = 0;//middle detection sensor reading
unsigned int c = 0;//right detection sensor reading
float last_problem_bool[2] = {0,0};//last dispense information: {'crack' -> 0 or 'hole' -> 1, distance from the start line}
bool completion_bool = true;// switch of standby or running 
int last_sensed = 0; //1-crack, 2-hole, 0-none identified yet
float drop_position[12] = {0,0,0,0,0,0,0,0,0,0,0,0};//a queue of next positions that the robot supposes to stop and dispense cones
int drop_identity[12] = {0,0,0,0,0,0,0,0,0,0,0,0};//identity of the obstacle the cone(s) should cover: 0-none, 1-left crack,2-middle crack,3-right crack,4-hole
int drop_index = 0;//pointer to the next drop task in the drop queue
int add_index = 0;//where to add the new drop task in the drop queue
bool last_dropped = false;//last obstacle has/will have cone(s) deployed
bool no_cone = false;//switch of dispense functions
char times[8] = "00000000";//temporary list of start time(hours and minutes) of four latest operations

//"record variables" that will be sent to permanent log/////
int hole_counter =0;//number of holes detected
int crack_counter=0;//number of cracks detected
int array_holes_distance[12] = {0,0,0,0,0,0,0,0,0,0,0,0};//distance between each hole and the start line
int array_cracks_distance[12] = {0,0,0,0,0,0,0,0,0,0,0,0};//distance between each crack and the start line
char start_time[2] = "00";//minute, hour(24)
unsigned char operation_time[3]={1,2,3};//sec, min, hour(24)
unsigned int cones_deployed = 0;

//LCD display state variables/////
int disp_standby_page = 0;//level 1 rotating counter
int ddp = 0;//level 2 rotating counter
int reg =0;//level variable
int display_repeat = 0;//data selection page counter (rotates twice)
int clear_waiter = 3;//wait 3 iterations for user command to confirm clearing history

//constants and parameters/////
const char keys[] = "123A456B789C*0#D";//keypad array
const float car_length = 1.45;
const float mc_L = 0.2;//see usage 
const float car_width = 0.45;
const float half_lane_width = 1.5;
const float lc_R = 0.1;//see usage 
const float whole_distance = 15;//distance between destination and the start line
int wety_deg = 600;//delay time for indicator flag to rotate 180 degrees
int ninty_deg = 260;//delay time for indicator flag to rotate 90 degrees

//high-level program state switches//////
bool key = false;//switch of setup mode
int planB = 0;//switch between deployment strategies. 0-1: at plan A, 2-3 at plan B %4 every RESTART make it plan A. 

//EEPROM variables//////
char addr =0;//lower 8 bits of the address to write
char addrh = 0;//higher 8 bits of the address to write
char read_pntr = 0;//temporary variable for computing the right address to read a specific type of data
int total_op =0;//total number of operations recorded in EEPROM
int print_data[13];//temporary display content
#endif	/* GLOBAL_VARIABLE_H */

