#include "iodefine.h"
#include "mathf.h"
#include "sci.h"
#include "init.h"
#include "spi.h"
#include "i2c.h"
#include "parameters.h"
#include "glob_var.h"
#include "portdef.h"

extern wait_ms(int wtime);


void LED(short led_num){
	
	short led_pt = 0x00;
	if((led_num & 0x01) == 0){
		led_pt |= 0x02;	
	}
	if((led_num & 0x02) == 0){
		led_pt |= 0x01;	
	}
	if((led_num & 0x04) == 0){
		led_pt |= 0x08;	
	}
	if((led_num & 0x08) == 0){
		led_pt |= 0x04;	
	}
	
	if(led_num == 0){
		led_pt = 0xFF;
	}
	
	IOex_LED(led_pt);
}

void BEEP(void){
	int i,j;
	for(j=0 ; j<100 ; j++){
		PORTB.PODR.BIT.B5 = 1;
		for(i = 0; i < 1000; i++);
		PORTB.PODR.BIT.B5 = 0;
		for(i = 0; i < 1000; i++);
	}
	
}

void BEEP_MULT(short n){
	int i,j,k;
	for(k = 0; k < n+1; k++){
		for(j=0 ; j<1000*0.1 ; j++){
			PORTB.PODR.BIT.B5 = 1;
			for(i = 0; i < 100*10*1; i++);
			PORTB.PODR.BIT.B5 = 0;
			for(i = 0; i < 100*10*1; i++);
		}
		wait_ms(100);
	}
}

void mode_change( char* mode){
	int i;
	//ƒ‚[ƒhØ‚è‘Ö‚¦—pˆ—
	if(speed_r > 0.1){
		if(*mode == 15){
			*mode = 0;
		}else{
			*mode= *mode + 1;
		}
		BEEP(); 
		LED(*mode);
		for(i = 0; i < 100*1000*50; i++);
	}

	if(speed_r < -0.1){
		if(mode == 0){
			*mode = 15;
		}else{
			*mode = *mode -1;
		}
		BEEP(); 
		LED(*mode);
		for(i = 0; i < 100*1000*50; i++);
	}	
}
