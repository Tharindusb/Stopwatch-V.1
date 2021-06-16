#define SSD_DIGI_DDR DDRA    // digit selector DDR
#define SSD_DIGI_PORT PORTA  // digit selector PORT
#define SSD_DATA_DDR DDRC
#define SSD_DATA_PORT PORTC
#define DIGIT0 4
#define DIGIT1 5
#define DIGIT2 6
#define DIGIT3 7


#include <avr/io.h>
#include <util/delay.h>

unsigned char display[] ={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned char ssd_dig[]={0,0,0,0};
volatile unsigned int sec=0, min=0, h=0, mil_sec =0;
void ssd_digit(unsigned int n);
void split(void);
void init_SSD(void);
	
void init_SSD(){
	
	SSD_DATA_DDR = 0xff;
	SSD_DIGI_DDR |=1<<DIGIT0 |1<<DIGIT1 |1<<DIGIT2 |1<<DIGIT3 ;
	SSD_DATA_PORT =0x00;
	SSD_DIGI_PORT &=~(1<<DIGIT0 |1<<DIGIT1 |1<<DIGIT2 |1<<DIGIT3);
}

void ssd_digit(unsigned int n){
	for(int i=0;i<n;i++){
		SSD_DATA_PORT = display[ssd_dig[0]] | 1<<DIGIT3;
		SSD_DIGI_PORT =1<< DIGIT0;
		_delay_us(100);
		SSD_DIGI_PORT &=~(1<<DIGIT0);
		
		SSD_DATA_PORT = display[ssd_dig[1]];
		SSD_DIGI_PORT =1<< DIGIT1;
		_delay_us(100);
		SSD_DIGI_PORT &=~(1<<DIGIT1);
		
		SSD_DATA_PORT = display[ssd_dig[2]] | 1<<DIGIT3;
		SSD_DIGI_PORT =1<< DIGIT2;
		_delay_us(100);
		SSD_DIGI_PORT &=~(1<<DIGIT2);
		
		SSD_DATA_PORT = display[ssd_dig[3]];
		SSD_DIGI_PORT =1<< DIGIT3;
		_delay_us(100);
		SSD_DIGI_PORT &=~(1<<DIGIT3);
	}
}
void split(){
	if (min<=9){
		ssd_dig[0]=min;
		ssd_dig[1]=sec/10;
		ssd_dig[2]=sec%10;
		ssd_dig[3]=mil_sec/100;
	}
	if(min >=10){
		ssd_dig[0] = h;
		ssd_dig[1] = min/10;
		ssd_dig[2] = min%10;
		ssd_dig[3] = sec/10;
	}
}


