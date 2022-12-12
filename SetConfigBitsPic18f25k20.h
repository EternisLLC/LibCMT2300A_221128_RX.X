/* 
 * File:   
 * Author: 
 * Comments: файл используется только в проекте для pic18f25k20
 * Revision history: 
 */
/** C O N F I G U R A T I O N   B I T S ******************************/
// CONFIG1H
#include    <xc.h>
#pragma config	FOSC = HSPLL/*INTIO7*/,	FCMEN = OFF,	IESO = ON
//CONFIG2L
#pragma config	PWRT = OFF,	BOREN = OFF,	BORV = 22
//CONFIG2H
#pragma config	WDTEN = OFF,	WDTPS = 1024//1024
//CONFIG3H
#pragma config	CCP2MX = PORTC,	PBADEN = OFF,	LPT1OSC = OFF,	HFOFST = OFF, MCLRE = ON
//CONFIG4L
#pragma config	STVREN = OFF,	LVP = OFF,	XINST = OFF,	DEBUG = ON
//CONFIG5L
#pragma config	CP0 = OFF,	CP1 = OFF,	CP2 = OFF,	CP3 = OFF
//CONFIG5H
#pragma config	CPB = OFF,	CPD = OFF
//CONFIG6L
#pragma config	WRT0 = OFF,	WRT1 = OFF,	WRT2 = OFF,	WRT3 = OFF
//CONFIG6H
#pragma config	WRTC = OFF,	WRTB = OFF,	WRTD = OFF
//CONFIG7L
#pragma config	EBTR0 = OFF,	EBTR1 = OFF,	EBTR2 = OFF,	EBTR3 = OFF
//CONFIG7H
#pragma config	EBTRB = OFF

