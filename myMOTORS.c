#include "MKL25Z4.h"                    // Device header
#include "myMOTORS.h"

int const FREQ_REQ = 50;
int motorpins[] = {20, 21, 22, 23};
int MOD = 0;

int calcMOD()
{
	int mod = (DEFAULT_SYSTEM_CLOCK / (128 * FREQ_REQ)) - 1;
	return mod;
}

int setPwmVal(float percentage)
{
	int result = (MOD + 1) * (percentage / 100.0);
	return result;
}

void initMotors()
{
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	for(int i = 0; i < sizeof(motorpins)/sizeof(int); i++)
	{
		PORTE->PCR[motorpins[i]] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[motorpins[i]] |= PORT_PCR_MUX(3);
	}
	//Enable clock to TPM1 & TPM2
	SIM_SCGC6 |= (SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK);
	
	//Sets the clock source to internal clock
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	//sets the MOD number for desired frequency
	MOD = calcMOD();
	TPM1->MOD = MOD;
	TPM2->MOD = MOD;
	
	TPM1_C0V = TPM1_C1V = 0;
	TPM2_C0V = TPM2_C1V = 0;
	
	TPM1->SC &= ((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM2->SC &= ((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM2->SC &= ~(TPM_SC_CPWMS_MASK);
	
	//Sets the PWM to the Edge-aligned PWM High-true pulses (clear Output on match, set Output on reload) mode.
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void move(direction_t dir, float power)
{
	switch(dir)
	{
		case FORWARD :	TPM1_C0V = TPM1_C1V = setPwmVal(power);
										TPM2_C0V = TPM2_C1V = 0;
										break;
		case REVERSE :	TPM1_C0V = setPwmVal(0.0);
		                TPM1_C1V = setPwmVal(0.0);
										TPM2_C0V = setPwmVal(power);
                 		TPM2_C1V = setPwmVal(power);
										break;
		case RIGHT :		TPM1_C1V = TPM2_C0V = setPwmVal(power);
										TPM1_C0V = TPM2_C1V = 0;
										break;
		case LEFT : 		TPM1_C1V = TPM2_C0V = 0;
										TPM1_C0V = TPM2_C1V = setPwmVal(power);
										break;
		case FORWARDRIGHT :	TPM1_C1V = setPwmVal(power);
												TPM1_C0V = setPwmVal(power)/2;
												TPM2_C0V = TPM2_C1V = 0;
												break;
		case FORWARDLEFT :	TPM1_C1V = setPwmVal(power)/2;
												TPM1_C0V = setPwmVal(power);
												TPM2_C0V = TPM2_C1V = 0;
												break;
		case REVERSERIGHT : TPM1_C1V = TPM1_C0V = 0;
												TPM2_C1V = setPwmVal(power);
												TPM2_C0V = setPwmVal(power)/2;
												break;
		case REVERSELEFT :	TPM1_C1V = setPwmVal(power)/2;
												TPM1_C0V = setPwmVal(power);
												TPM2_C0V = TPM2_C1V = 0;
												break;
		case STOP : TPM1_C1V = TPM2_C0V = 0;
								TPM1_C0V = TPM2_C1V = 0;
								break;
		default: 	TPM1_C1V = TPM2_C0V = 0;
							TPM1_C0V = TPM2_C1V = 0;
	}
}
