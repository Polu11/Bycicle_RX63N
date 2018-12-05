
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 5.12.2018     1.00        First release
*         Author:
*******************************************************************************/
/*                                                                     */
/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <machine.h>
#include "platform.h"
#include "ADb10.h"
#include "cmt.h"
#include "pwm.h"
#include "sensore.h"
#include "PID.h"

/*  FILE        : Bycicle_RX63N.c                                   */
/*  DATE        :Nov,  15, 2018                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

//void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif


/*
void main(void)
{

}
*/


#ifdef __cplusplus
void abort(void)
{

}
#endif
//Variabili esterne
//extern unsigned int general_timer_mS;
extern int timer_2mS;
float vout; //variabili dichiarate per la stampa a video
float volt_signal_1;

 //vettore prova sensore
const int vettP[10]={3, -3, 3, 3};

int ptvettP=0;
int oldtimer=0;
int timerCont=0;

/* Creazione variabile di tipo struttura PID_Struct che verra'  passata alle funzioni di gestione del PID */
PIDSt_Type PID_P_curr_1;
//PIDSt_Type PID_P_curr_2;
//PIDSt_Type PID_P_curr_3;

SensParam SENS_P;

//Variabili dichiarate per vedere la durata di un ciclo
unsigned int timer_init=0;
	unsigned int tempo=0;

/* Declare display buffer */
uint8_t  lcd_buffer[13];
void main(void)
{
    /* Initialize LCD */
    lcd_initialize();

    /* Clear LCD */
    lcd_clear();

    /* Display message on LCD */
    lcd_display(LCD_LINE1, "Bici");
    lcd_display(LCD_LINE2, "TEST PID COPPIA ");

	CMT_init(); // Settaggio del tempo di clock

	// Inizializzazione della MTU2 per generare la PWM sul primo canale (MTIOC3A)

	PWM_Init(1);
	PWM_Init(2);
	PWM_Init(3);
	PWM_Init(4); // PWM motor 1 JN2-PIN 22

	/*Inizializzazione dei parametri per il controllo PID in coppia*/
	init_pid(&PID_P_curr_1,SET_PARAM_PID_TORQUE);


	//valori per la prova espressi in mA
	/*PID_P_curr_1.uc=-100;
	PID_P_curr_2.uc=200;
	PID_P_curr_3.uc=150;*/

	// Inizializzazione del convertitore A/D a 10-bit
	ADb10_init();
	Init_Port_Dir(); // Inizializzazione delle porte di Direzione

	vettore_vout_init(); // Inizializzazione a 0 dei vettori per le tensioni lette dai sensori di corrente

	//Variabili resettate per vedere la durata di un ciclo
	timer_init=0;
	tempo=0;

	sens_in(&SENS_P);
    //This is the main loop.
	while (1)
    {
		timer_init=general_timer_mS;

    	// Ciclo di acquisizione dati e controllo dei motori .-.
    	// E' stato implementato un timer da 2ms per l'acquisizione della corrente assorbita dai motori
		if(timer_2mS!=oldtimer){
			oldtimer = timer_2mS;
			if (++timerCont>1){
				timerCont=0;
				if(++ptvettP>2)
					ptvettP=0;
				PID_P_curr_1.uc=vettP[ptvettP];

			}
		}

    	// Ciclo che scandisce l'acquisizione dei dati dai sensori di corrente
    	if(timer_2mS)
    		{
    			timer_2mS = 0;

    			if(SENS_P.cont_calibration < calibration_length)
    			{
    					// Stampa a video dell'avviso di calibrazione dei sensori di corrente
    					sprintf((char *)lcd_buffer, "Calibrating");
    					lcd_display(LCD_LINE4, lcd_buffer);
    					sprintf((char *)lcd_buffer, "Sensors: %d",SENS_P.cont_calibration);
    					lcd_display(LCD_LINE5, lcd_buffer);


    				sens_calibration_init(&SENS_P);	// Funzione di calibrazione dei sensori di corrente
    			}
    			else if (SENS_P.cont_calibration == calibration_length)
    				sens_calibration_bias(&SENS_P);   	/*Calcolo del bias calcolato su 1000 elementi da sottrare
    											  al valore letto dai sensori*/
    			else
    			{
    				//Lettura dei valori di tensione legati alla corrente inviata ai motori e relativa conversione in corrente
    				sens_read(&SENS_P);

    				sprintf((char *)lcd_buffer, "C1=%f",SENS_P.sens_curr_1);
    				lcd_display(LCD_LINE4, lcd_buffer);

    				sprintf((char *)lcd_buffer, "RIF=%d",vettP[ptvettP]);
    				lcd_display(LCD_LINE5, lcd_buffer);

    				/*Assegnazione dei valori letti dal sensore di corrente alla struttura attuale del PID
    			  	  per il controllo in corrente*/
    				PID_P_curr_1.y=SENS_P.sens_curr_1;


    				// Calcolo del segnale di controllo da inviare ai motori
    				// Inviamo il segnale di riferimento curr_ref al PID in corrente che provvedera' a generare
    				// il segnale di controllo che invieremo ai motori.
    				calcPID(&PID_P_curr_1);

    				volt_signal_1 = PID_P_curr_1.output;



    				/* Controllo del verso di rotazione */
    				motor_direction(volt_signal_1);

    				/* Calcolo del Duty-Cycle da inviare ai motori */
    				DutyCycle_to_Motor(volt_signal_1);
    				sprintf((char *)lcd_buffer, "OUT=%f",volt_signal_1);
    				    				lcd_display(LCD_LINE6, lcd_buffer);
    			}
    		}/* Fine ciclo acquisizione dati e controllo (2ms)*/

    	 	 tempo =general_timer_mS  - timer_init;
    	     sprintf((char *)lcd_buffer, "tempo=%d", tempo);
    	     lcd_display(LCD_LINE7, lcd_buffer);
   }

} /* End function main() */

