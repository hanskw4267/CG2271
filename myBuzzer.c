#include "pitches.h"
#include "MKL25Z4.h"  
#include "time.h"
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2


// Despacito tone
int start_melody[] = {
  NOTE_D5, NOTE_CS5, NOTE_B4, NOTE_FS4,
  NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_G4,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_CS5, NOTE_D5, NOTE_A4,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_CS4,
  0
};


int start_durations[] = {
  2, 2, 4, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 2,
  8
};


// Take on me tone
int end_melody[] = {
  NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5, 
  NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5, 
  NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_FS5, 
  NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5
};

int end_durations[] = {
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8, 
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8
};

// Darth vader theme
int song_melody[] = {
   NOTE_G4, NOTE_G4,NOTE_G4, NOTE_DS4, NOTE_AS4, NOTE_G4, NOTE_DS4, NOTE_AS4, NOTE_G4, NOTE_D5, NOTE_D5, NOTE_D5,
	 NOTE_DS5, NOTE_AS4, NOTE_FS4, NOTE_DS4, NOTE_AS4, NOTE_G4, NOTE_G5, NOTE_G4, NOTE_G4, NOTE_G5, NOTE_FS5, NOTE_F5, 
	 NOTE_E5, NOTE_DS5, NOTE_E5, 0, NOTE_GS4, NOTE_CS5, NOTE_C5, NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_AS4, 0, NOTE_DS4, NOTE_FS4, 
	 NOTE_DS4, NOTE_FS4, NOTE_AS4, NOTE_G4, NOTE_AS4, NOTE_D5, NOTE_G5, NOTE_G4, NOTE_G4, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_E5, 
   NOTE_DS5, NOTE_E5, 0, NOTE_GS4, NOTE_CS5, NOTE_C5, NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_AS4, 0, NOTE_DS4, 
	 NOTE_FS4, NOTE_DS4, NOTE_AS4, NOTE_G4, NOTE_DS4, NOTE_AS4, NOTE_G4 
};

 int song_durations[] = {
   4, 4, 4, 6, 16, 4, 6, 16, 2, 4, 4, 4, 6, 16, 4, 6, 16, 2, 4, 6, 16, 4, 6, 16, 16, 16, 8, 8, 8, 4, 
	 6, 16, 16, 16, 8, 8, 8, 4, 6, 16, 4, 6, 16, 2, 4, 6, 16, 4, 6, 16, 16, 16, 8, 8, 8, 4, 6, 16, 16, 
   16, 8, 8, 8, 4, 6, 16, 4, 6, 16, 2,
 };


int start_length = sizeof(start_melody)/sizeof(start_melody[0]);
int end_length = sizeof(end_melody)/sizeof(end_melody[0]);
int song_length = sizeof(song_melody)/sizeof(song_melody[0]);
 

void initBuzzer(){
  SIM_SCGC5 |= ((SIM_SCGC5_PORTE_MASK));
	PORTE->PCR[29] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[29] |= PORT_PCR_MUX(3);
	
	//Enable clock to TPM0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	//Sets the clock source to internal clock
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	TPM0->MOD = 0;
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
}

void setFreq(int freq){
	if (freq != 0){
			double period = 1.0/(double)freq;
			double period_clk = 1.0/(DEFAULT_SYSTEM_CLOCK / (double)128);
			int mod = (period/period_clk) - 1;
			TPM0->MOD = mod;
			TPM0_C2V = (mod + 1)/2;
		 }
		 else{
		   TPM0->MOD = 0;
			 TPM0_C2V = 0;
		 }
}


void playEndTone(){
  
	for (int i = 0; i < end_length; i += 1){
		 int ms_duration = 1000/end_durations[i];
			setFreq(end_melody[i]);
			osDelay(ms_duration/1.5);
			setFreq(0);
			osDelay(ms_duration);
		 
	}
}

void playSong(){
	
 for (int i = 0; i < song_length; i += 1){
		 int duration = 1000/song_durations[i];
		 setFreq(song_melody[i]);
	   osDelay(duration);
	   setFreq(0);
		 osDelay(duration);
	}
}

void playStart(){
	
for (int i = 0; i < start_length; i += 1){
		 int duration = 1000/start_durations[i];
		 setFreq(start_melody[i]);
	   osDelay(duration);
	   setFreq(0);
		 osDelay(duration/1.5);
	}
	
}


