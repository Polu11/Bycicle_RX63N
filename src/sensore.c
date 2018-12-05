/**********************************************************************************
 * File name	: sensore.c
 * Created on	: 11/12/2017
 * H/W Platform : YRDKRX63N
 * Description  : Driver for the current sensor.
 * 				  Implementazione per la calibrazione del sensore di corrente
 * 				  e per la lettura dei dati dal sensore di corrente.
 **********************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "platform.h"
#include "ADb10.h"
#include "sensore.h"


/*******************************************************************************
* Function name: sens_calibration_init
* Description  : Current sensors calibration
* Arguments    : none
* Return value : none
*******************************************************************************/
void sens_in(SensParam* SENS){
	SENS->cont_calibration=0;

}
void sens_calibration_init (SensParam* SENS)
{

	if(Gestisci_ADb10() == 1)
	{
		SENS->Vout = ADb10_read(1);

		SENS->sum_Vout += SENS->Vout;

		SENS->cont_calibration=SENS->cont_calibration+1;
	}
} /* Fine della funzione calibration_init */

/*******************************************************************************
* Function name: sens_calibration_bias
* Description  : Calculation function for current sensors' bias
* Arguments    : none
* Return value : none
*******************************************************************************/
void sens_calibration_bias (SensParam *SENS)
{
	SENS->bias1 = (SENS->sum_Vout/calibration_length) - offset_vout;

	SENS->cont_calibration++;
} /* Fine della funzione calibration_bias */

/*******************************************************************************
* Function name: sens_read
* Description  : Acquisition function for voltage values proportional to the current
*				 from sensors
* Arguments    : none
* Return value : none
*******************************************************************************/
void sens_read (SensParam *SENS_P)
{
	if (Gestisci_ADb10() == 1)
	{
		// Lettura dei valori di tensione legati alla corrente inviata ai motori
		SENS_P->Vout = ADb10_read(1) - SENS_P->bias1;


		// Conversione dei valori di tensione letti dal sensore in valori di corrente
		SENS_P->sens_curr_1 = (SENS_P->Vout - offset_vout)/sensitivity;

	}
} /* Fine della funzione sens_read */

/* End of file sensore.c */
