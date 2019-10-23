/* 
 * File:   prototypes.h
 * Author: Hongyu Chen
 *
 * Created on February 25, 2019, 2:04 AM
 */

#ifndef PROTOTYPES_H
#define	PROTOTYPES_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

//functions

void configureports(void);
void readRTC(void);
void rtc_set_time(void);

unsigned int digit0(unsigned int f);
unsigned char nextLine(unsigned char g);
void printintarray2(unsigned int* a,int b);
void ot(unsigned int* print_data);
void Cones(unsigned int* print_data);
void Holes2(unsigned int* a,int b);
void Cracks(unsigned int* a,int b);
void sensingH(void);
void sensingC(void);
void page1(void);
void page2(void); 
void page3(void); 
void page4(void); 
void page5(void); 
void page6(void); 
void data1(void);
void data2(void);
void data3(void);
void data4(void);
void initialize_func(void);
void initmoving_disp(void);
void high_priority interrupt interruptHandler(void);
void read_encoder(void);
int distinguish_H_C_function(void);
int hole_drop_bool_function(void);
int crack_drop_bool_function(void);
void hole_dispense_function(void);
void crack_dispense_function(void);
void sensed_function_3(void);
void normal_updater(void);

void standby_rotating(void);
void middle_crack_drop(void);
void left_crack_drop(void);
void right_crack_drop(void);
void record(char corh);

void L_I(void);
void R_I(void);
void backw(void);

void Stop(void);
void straight(void);
void turn_left(void);
void turn_right(void);

void moving(void);
void move_to_hole(void);

void drop_record(int a);
void drop_function(void);
void completion_return(void);

void clear_mem(void);
void update_pointer(void);
void completion_write(void);
void read_pointer(void);
void pntr_head_read(void);
void subtract(void);
char one_byte_reader(char r, char rh);

void data_disp(void);
void clear_select(void);
void clear_finish(void);
void data_select(void);
int read_total(void);
#endif	/* PROTOTYPES_H */

