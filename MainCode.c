#include <REG932.h>
#include <Simon2.h>

float quarter;
float eighth;
float half;
float whole;
unsigned char bpm;
unsigned char mode;
int cycles;
int complement;

void delay_t0(float ms);
void calc_bpm(unsigned char bpm);	

void main() {
	mode = 0;
	calc_bpm(200);

	while(1) {
		if(SW7 == 0)
			mode = 1;
		else if(SW8 == 0)
			mode = 2;
		else if(SW9 == 0)
			mode = 3;
	}
}


void delay_t0(float ms) { 
	//Works up to 71 ms
	
	cycles = (ms*1000)/1.085;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	
	complement = 0xFFFF-cycles+1;
	TH0 = complement >> 4;
	TL0 = complement && 0x0F;
	TR0 = 1;
	
	while (TF0 == 0)
	TR0 = 0;
	TF0 = 0;
}

void calc_bpm(unsigned char bpm) {
	quarter = 60 / bpm;
	eighth = quarter / 2;
	half = quarter * 2;
	whole = half * 2;
}