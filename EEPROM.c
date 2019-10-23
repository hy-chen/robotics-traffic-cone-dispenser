#@author Hongyu Chen

#include <xc.h>
#include "configBits.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "lcd.h"
#include "global_variable.h"
#include "I2C.h"
#include "prototypes.h"

void clear_mem(void){
//set first two data to 02.
    read_pointer();
    
    while (1){
        EEADRH = addrh;
        EEADR = addr;
        EEDATA = 0;
        EECON1bits.EEPGD = 0;
        EECON1bits.CFGS = 0;
        EECON1bits.WREN = 1;
        GIE =0;

        EECON2 = 0x55;
        EECON2 = 0xAA;

        EECON1bits.WR =1;

        while(EECON1bits.WR==1){
        }//wait writing to complete
        
        GIE =1;
        EECON1bits.WREN = 0;
    
        addr = addr-1;
        if (addr == 0xFF){
            addrh = addrh -1;
        }
        if (addrh == 0xFF){
            break;
        }
    }
    EEADRH = 0;
    EEADR = 0;
    EEDATA = 0;
    
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    GIE =0;

    EECON2 = 0x55;
    EECON2 = 0xAA;

    EECON1bits.WR =1;

    while(EECON1bits.WR==1){
    }//wait writing to complete
    GIE =1;
    EECON1bits.WREN = 0;

    EEADRH = 0;
    EEADR = 1;
    EEDATA = 2;
    
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    GIE =0;

    EECON2 = 0x55;
    EECON2 = 0xAA;

    EECON1bits.WR =1;

    while(EECON1bits.WR==1){
    }//wait writing to complete

    GIE =1;
    EECON1bits.WREN = 0;

}

void completion_write(void){
    read_pointer();
    
    int value = 0;
    while(value <56){
        
        EEADRH = addrh;
        EEADR = addr;
        
        if (value <2){
            EEDATA = start_time[1-value];}
        else if (value <5){
            EEDATA = operation_time[4-value];}
        else if (value == 5){
            EEDATA = cones_deployed;}
        else if (value == 6){
            EEDATA = hole_counter;}
        else if ((value <31)&&(value%2 == 1)){
            EEDATA = array_holes_distance[(value-7)/2]/256;}
        else if ((value <31)&&(value%2 == 0)){
            EEDATA = array_holes_distance[(value-8)/2]%256;}
        else if (value == 31){
            EEDATA = crack_counter;}
        else if ((value <56)&&(value%2 == 0)){
            EEDATA = array_cracks_distance[(value-32)/2]/256;
        }
        else if ((value <56)&&(value%2 == 1)){
            EEDATA = array_cracks_distance[(value-33)/2]%256;
        }
                
        EECON1bits.EEPGD = 0;
        EECON1bits.CFGS = 0;
        EECON1bits.WREN = 1;
        GIE =0;

        EECON2 = 0x55;
        EECON2 = 0xAA;

        EECON1bits.WR =1;

        while(EECON1bits.WR==1){
        }
        GIE =1;
        EECON1bits.WREN = 0;
        
        addr +=1;
        if (addr == 0){
            addrh+=1;
        }
        if (addrh == 0x03){//actually not complete since it will not be here
            //EECON1bits.WREN =0;
            printf("memory full");
            break;
        }
        value +=1;
    }
    update_pointer();
}

void update_pointer(void){
    
    EEADRH = 0;
    EEADR = 0;

    EEDATA = addrh;

    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    GIE = 0;

    EECON2 = 0x55;
    EECON2 = 0xAA;

    EECON1bits.WR =1;

    while(EECON1bits.WR==1){
    }
    
    GIE =1;
    EECON1bits.WREN = 0;
    ///////////////////////////////////
    EEADRH = 0;
    EEADR = 1;

    EEDATA = addr;

    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    GIE = 0;

    EECON2 = 0x55;
    EECON2 = 0xAA;

    EECON1bits.WR =1;

    while(EECON1bits.WR==1){
    }//wait writing to complete
    
    GIE =1;
    EECON1bits.WREN = 0;
}

void read_pointer(void){
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;

    EEADRH = 0;
    EEADR = 0;

    EECON1bits.RD = 1;
    addrh = EEDATA;

    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;

    EEADRH = 0;
    EEADR = 1;

    EECON1bits.RD = 1;
    addr = EEDATA;

}

void subtract(void){
    if (addr >= read_pntr){
        addr = addr - read_pntr;}
    else{
        addrh = addrh - 1;
        addr = addr - read_pntr;}
}
char one_byte_reader(char r, char rh){
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EEADRH = rh;
    EEADR = r;
    EECON1bits.RD = 1;
    char out= EEDATA;
    return out;
}

int read_total(void){
    read_pointer();
    /*printf(",%d,%d,",addrh,addr);
    __delay_ms(1000);*/
    int total = ((addrh*256 + addr)-2)/56;
    /*printf("%d",total);
    __delay_ms(1000);*/
    if (total > 4){
        total = 4;
    }
    return total;
}
