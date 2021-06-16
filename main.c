#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ssd_lib.h"

#define in0 0
#define in1 1
#define in2 2
#define in3 3

volatile unsigned int start_stop_flag=1;
unsigned int cliflag =0;
void timer0_interrupt(void);

int main(void){
	timer0_interrupt();
	init_SSD();
	DDRD &=~(1<<in2);
    while (1){

		split();
		ssd_digit(1);
    }
	return 0;
}

void timer0_interrupt(){
	TCNT0 = 0x00;
	TIMSK |=(1<<OCIE0);
	OCR0 = 31;
	GICR |= 1<<INT0 | 1<<INT1;   
	MCUCR |= 1<<ISC01| 1<<ISC11;
	if(cliflag ==0){
		sei();
	}else{
		cli();
		_delay_ms(2000);
		cliflag =0;
	}
}

ISR(TIMER0_COMP_vect){
	mil_sec++;
	if(mil_sec >= 1000){
		mil_sec = 0;
		sec++;
		if(sec >= 60){
			sec = 0;
			min++;
			if(min >= 60){
				min = 0;
				h++;
			}
		}
	}
	cliflag =1;
}

ISR(INT0_vect){
	if(start_stop_flag == 1){
		TCCR0 |= 1<<WGM01 | 1<<CS02;
		start_stop_flag = 0;
		}else{
			start_stop_flag = 1;
			TCCR0 &= ~(1<<CS02);
		}
	while((PIND & (1<<in2))==1);
	cliflag =1;
}

ISR(INT1_vect){
	TCCR0 &= ~(1<<CS02);
	mil_sec = 0;
	sec = 0;
	min = 0;
	h = 0;	
	cliflag =1;
}




