/*
@author
*/

#include <xc.h>
#include "configBits.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lcd.h"
#include "global_variable.h"
#include "I2C.h"
#include "prototypes.h"

void data_disp(void){ 
    read_pointer();
    //compute the start of the requested data
    int datalength = 0;
    if(reg%10 == 1){//ot
        read_pntr = (total_op +1 - reg/10)*56 - 4 + 2;
        datalength = 3;}
    else if (reg%10 == 2){//cones
        read_pntr = (total_op +1- reg/10)*56 - 7 + 2;
        datalength = 1;}
    else if (reg%10 == 3){//hole
        read_pntr = (total_op +1- reg/10)*56 - 8 + 2;
        datalength = 25;}
    else if (reg%10 == 4){//crack
        read_pntr = (total_op +1- reg/10)*56 - 33 + 2;
        datalength = 25;}
    subtract();
    
    if (datalength == 25){
        print_data[0]= one_byte_reader(addr,addrh);
        addr +=1;
        if (addr == 0){
            addrh+=1;
        }
        int temp_read = 0;
        for (int length_i = 1;length_i<datalength;length_i++){
            if (length_i%2 == 0){
                temp_read += one_byte_reader(addr,addrh);
                print_data[(length_i / 2)] = temp_read;
                temp_read = 0;
            }
            else{
                temp_read += 256 * one_byte_reader(addr,addrh);
            }
            addr +=1;
            if (addr == 0){
                addrh+=1;
            }
        }
    }
    else{
        for (int length_i = 0;length_i<datalength;length_i++){
            print_data[length_i]= one_byte_reader(addr,addrh);
            addr +=1;
            if (addr == 0){
                addrh+=1;
            }
        }
    }
        if(reg%10 == 1){//ot
            ot(print_data);}
        else if (reg%10 == 2){//cones
            Cones(print_data);}
        else if (reg%10 == 3){//hole
            Holes2(print_data,print_data[0]);}
        else if (reg%10 == 4){//crack
            Cracks(print_data,print_data[0]);}
}
void ot(unsigned int* print_data){   
    printf("Operation Time:");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("%02d hour",print_data[0]);
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("%02d min",print_data[1]);
    lcd_set_ddram_addr(LCD_LINE4_ADDR);
    printf("%02d sec",print_data[2]);
    __delay_ms(2000);
}

void Cones(unsigned int* print_data){
    printf("Number of Cones");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Deployed:");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("%d",print_data[0]);
    __delay_ms(2000);
}


void Holes2(unsigned int* a,int b){
    printf("Number of Holes");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Detected:");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("%d",a[0]); 
    __delay_ms(2000);
    //Distance from Start Line: (cm)
    printintarray2(a,b);
    
}

void Cracks(unsigned int* a,int b){
    printf("Number of Cracks");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Detected:");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("%d",a[0]);
    __delay_ms(2000);
    //Distance from Start Line: (cm)
    printintarray2(a,b);
}
void page5(void){
    unsigned char curr_line = LCD_LINE1_ADDR;
    for (int op_counter = 0;op_counter < total_op;op_counter++){
        printf("#%d.",op_counter+1);
        read_pntr = (total_op - op_counter)*56;//latest is the first
        read_pointer();
        subtract();//correct 
        times[op_counter*2] = one_byte_reader(addr,addrh);
        addr +=1;
        if (addr == 0){
            addrh+=1;
        }
        times[op_counter*2+1] = one_byte_reader(addr,addrh);
        if (times[op_counter*2] >= 12){
            times[op_counter*2] = times[op_counter*2] - 12;
            printf("%d:%02d pm",times[op_counter*2],times[op_counter*2+1]);
        }
        else{
            printf("%d:%02d am",times[op_counter*2],times[op_counter*2+1]);
        }
        curr_line = nextLine(curr_line);
        lcd_set_ddram_addr(curr_line);
    }
    
}
/////////////////////////////////////////
void clear_select(void){
    printf("Clear?");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("C: confirm");
    
}
void clear_finish(void){
    clear_mem();
    printf("History Cleared");
    
}


void data_select(void){
    if (ddp == 0){
        data1();
    }
    else if (ddp ==1){
        data2();
    }
    else if (ddp ==2){
        data3();
    }
    else if (ddp ==3){
        data4();
    }
    ddp = (ddp+1)%4;
}
void standby_rotating(void){
    
    if (disp_standby_page == 0){
        page1();
    }
    else if (disp_standby_page ==1){
        page2();
    }
    else if (disp_standby_page ==2){
        page3();
    }
    else if (disp_standby_page ==3){
        page4();
    }
    else if (disp_standby_page ==4){
        if (total_op == 0){
            page3();
        }
        else{
            page5();}
    }else if (disp_standby_page ==5){
        page6();
    }
    
    disp_standby_page = (disp_standby_page+1)%6;
}
unsigned int digit0(unsigned int h){
	if (h<10){
		return 1;}
	else if (h<100){
		return 2;}
	else if (h<1000){
		return 3;}
    else if (h<10000){
        return 4;
    }
	else{
		return 0;}
}
unsigned char nextLine(unsigned char g){
	if (g == LCD_LINE1_ADDR){
		return LCD_LINE2_ADDR;
    }
	else if (g ==LCD_LINE2_ADDR){
		return LCD_LINE3_ADDR;
    }
	else if (g == LCD_LINE3_ADDR){
		return LCD_LINE4_ADDR;
    }
	else if (g == LCD_LINE4_ADDR){
		return LCD_LINE1_ADDR;
    }
}
//display in necessary digits, display one page one time. 

void printintarray2(unsigned int* a,int b){
    unsigned char curr_line = LCD_LINE3_ADDR;
    unsigned int dig =0;
    unsigned int counter =0;
    unsigned int left =16;
    
    lcd_clear();
    
    printf("Distance from");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Start Line: (cm)");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    __delay_ms(200);
    for(unsigned int i = 1;i<b+1;i++){//change 
        dig = digit0(a[i]);
        counter = counter + 1 + dig;
        if (counter <= 16){
            printf(" ");
            printf("%d",a[i]);
            left = 16-counter;
        }
        else if (counter > 16){
            curr_line = nextLine(curr_line);
            if (curr_line == LCD_LINE1_ADDR){
               __delay_ms(3000);
               lcd_clear();}
            lcd_set_ddram_addr(curr_line);
            printf(" ");
            printf("%d",a[i]);
            counter = counter - 16 + left;
            left = 16-counter;
        }
    }
    __delay_ms(3000);
}


void page1(void){
    printf("Thanks for using ");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Traffic Cone");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("Dispenser");
}
void page3(void){
    readRTC();
    printf("Date");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("20");
    printf("%02d-%02d-%02d",time[6],time[5],time[4]);
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("Time");
    lcd_set_ddram_addr(LCD_LINE4_ADDR);
    printf("%02d:%02d:%02d",time[2],time[1],time[0]);
}

void page2(void){
    printf("Press A:");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Start a New");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("Operation"); 
}
void page6(void){
    printf("Press 0:");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Clear History");
}
void page4(void){
    printf("Past Operations:");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    total_op = read_total();
    if (total_op == 0){
        printf("     Empty");
    }
    else if (total_op == 1){
        printf("Choose 1");
    }
    else{
    printf("Choose 1 - %d",total_op);}
    //lcd_set_ddram_addr(LCD_LINE3_ADDR);
    //printf("");       
    
}

void data1(void){   
    printf("Press 1:");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Operation Time");
}

void data2(void){    
    printf("Press 2:");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Number of");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("Cones Deployed");
    
}
void data3(void){
    printf("Press 3:");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Number and ");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("Location of ");
    lcd_set_ddram_addr(LCD_LINE4_ADDR);
    printf("Holes Detected");
}

void data4(void){
    printf("Press 4:");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Number and ");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("Location of ");
    lcd_set_ddram_addr(LCD_LINE4_ADDR);
    printf("Cracks Detected");
}
void initmoving_disp(void){
    lcd_clear();
    printf("Running");
}

//old alternatives methods
//delay all in three digits, one page one time
/*void printintarray1(int* a,int b){
    unsigned char curr_line = LCD_LINE3_ADDR;
    unsigned int counter=0;
    lcd_clear(); 
    printf("Distance from");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Start Line: (cm)");
    
    __delay_ms(500);
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    for(unsigned int i=0;i<b;i++){
        printf("%03d",a[i]);
        printf(" ");
        counter+=1;
        if ((curr_line == LCD_LINE4_ADDR)&&(counter%4 == 0)){
            __delay_ms(3000);
            lcd_clear();
            }
    
        if (counter%4==0){
            curr_line = nextLine(curr_line);
            lcd_set_ddram_addr(curr_line);
        }
    }    
    __delay_ms(3000);
  }*/


/*void Holes(int* a,int b){
    
    //number of Holes Detected
    unsigned char hole[] = "8";
    
    lcd_clear();
     
    printf("Number of Holes");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Detected:");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("%s",hole);
    __delay_ms(2000);
    //Distance from Start Line: (cm)
    printintarray3(a,b);
}


void Holes1(int* a,int b){
    
    //number of Holes Detected
    unsigned char hole[] = "8";
    
    lcd_clear();
     
    printf("Number of Holes");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Detected:");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("%s",hole);
    __delay_ms(2000);
    //Distance from Start Line: (cm)
    printintarray1(a,b);
    
}*/
/*// display in necessary digits, display one by one, clear one line by one line
void printintarray3(int* a,int b){
    unsigned char curr_line = LCD_LINE3_ADDR;
    unsigned int dig =0;
    unsigned int counter =0;
    unsigned int left =16;
    
    lcd_clear();
     
    printf("Distance from");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Start Line: (cm)");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    __delay_ms(500);
    //printf("size of a");
    //printf("%d",sizeof(a)); /fixed by b;
    for(unsigned int i = 0;i<b;i++){
        dig = digit0(a[i]);
        counter = counter + 1 + dig;
        if (counter <=16){
            printf(" ");
            printf("%d",a[i]);
            left = 16-counter;
        }
        else if (counter > 16){
            curr_line = nextLine(curr_line);
            lcd_set_ddram_addr(curr_line);
            printf("                ");
            __delay_ms(500);
            lcd_set_ddram_addr(curr_line);
            printf(" ");
            printf("%d",a[i]);
            counter = counter - 16 + left;
            left = 16-counter;
        }
        //printf("%u",counter);
        //printf("%d",curr_line);
        __delay_ms(1000);
    }
    __delay_ms(2000);
}*/

/*
void sensingH(void){
//sensed, hole
    
    lcd_clear();
     
    //while(1){
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    printf("Detected: Hole");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("   Operating");
    //__delay_ms(500);
    //}
    
}

void sensingC(void){
//sensed, crack
    
    lcd_clear();
     
    //while(1){
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    printf("Detected: Crack");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("   Operating");
   
}
*/
