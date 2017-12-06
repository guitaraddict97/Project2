#include <REG932.h>
#include <Simon2.h>
#include <MusicLib.h>

unsigned char TH0_saved;
unsigned char TH1_saved;
unsigned char TL0_saved;
unsigned char TL1_saved;

void init_shortdelay(unsigned int us, unsigned char timer);
void init_longdelay(unsigned int us, unsigned char timer);
void cont_longdelay(unsigned char timer);
void sound(unsigned short* song, unsigned short bpm);

void timer0() interrupt 1 {
	Speaker = ~Speaker;
	TR0 = 0;
	TF0 = 0;
	TL0 = TL0_saved;
	TH0 = TH0_saved;
	TR0 = 1;
}
	
void main() {
	unsigned char mode = 0;
	unsigned short bpm = 200;
	
	while(1) {
		if(SW7 == 0) {
			mode = 1;
			sound(POKEMON, bpm);
		}
		else if(SW8 == 0)
			mode = 2;
		else if(SW9 == 0)
			mode = 3;
	}
}


void sound(unsigned short* song, unsigned short bpm) {
	//Both timers are in mode 1
	//Timer 0 controls tones, while 1 controls spacing
	//Timer 0 done with interrupts, 1 done manually
	unsigned int eighth = (30/bpm)*1000000;
	unsigned short index = 0;
	unsigned short length = sizeof(song)/sizeof(short);
	unsigned short i;
	unsigned char j;
	
	TMOD = 0x11;
	EA = 1;
	ET0 = 1;
	
	init_longdelay(eighth,1);
	
	for(i=0; i+=2; i<length) {
		init_longdelay(song[i],0);
		TR0 = 0;

		for(j=0; j+=1; j<song[i+1]) {
			cont_longdelay(1);
		}
	}
}

void init_longdelay(unsigned int us, unsigned char timer) { 
	//Works up to 71000 us
	short cycles;
	short complement;
	cycles = us/1.085;
	complement = 0xFFFF-cycles+1;
	
	if(timer == 0) {
		TR0 = 0;
		TF0 = 0;
		TH0_saved = complement >> 4;
		TL0_saved = complement && 0x0F;
	}
	else {
		TR1 = 0;
		TF1 = 0;
		TH1_saved = complement >> 4;
		TL1_saved = complement && 0x0F;
	}
}

void cont_longdelay(unsigned char timer) {
	if(timer == 0) {
		TR0 = 0;
		TF0 = 0;
		TH0 = TH0_saved;
		TL0 = TL0_saved;
		TR0 = 1;
		while(TF0 == 0)
		TR0 = 0;
		TF0 = 0;
	}
	else {
		TR1 = 0;
		TF1 = 0;
		TH1 = TH1_saved;
		TL1 = TL1_saved;
		TR1 = 1;
		while(TF1 == 0)
		TR1 = 0;
		TF1 = 0;
	}
}

void init_shortdelay(unsigned int us, unsigned char timer) {
	//Works up to 138 us
	short cycles;
	cycles = us/1.085;
	
	if(timer == 0) {
		TL0 = 0;
		TH0 = -cycles;
	}
	else {
		TL1 = 0;
		TH1 = -cycles;
	}
}