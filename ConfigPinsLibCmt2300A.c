#include    <xc.h>
#include    "ConfigPinsLibCmt2300A.h"


void InitPinsCMT2300(void){
// конфигурация ножек АЦП только для PIC18...    
    ANSEL = 0;          // цифровые вхорды/выходы
    ANSELH = 0;         // цифровые вхорды/выходы
    
// конфигурация ножек связанных с CMT2300A    
    CSB_CMT2300 = 1;            // селект регистров управления и конфигурации выключен
    TRIS_CSB_CMT2300 = out;
    FCSB_CMT2300 = 1;           // селект регистров FIFO выключен
    TRIS_FCSB_CMT2300 = out;
    SCK_CMT2300 = 0;
    TRIS_SCK_CMT2300 = out;
    DOUT_CMT2300 = 0;
    TRIS_D_OUT_CMT2300();
    TRIS_GPIO1_CMT2300 = in;
// конец конфигурации ножек связанных с CMT2300A

// конфигурация ножек для ножек отладочной платы не связанных с работой CMT2300A
    LED_A1 = 0;
    TRIS_LED_A1 = out;
    LED_A2 = 0;
    TRIS_LED_A2 = out;
    LED_B2 = 0;
    TRIS_LED_B2 = out;
    LED_B4 = 0;
    TRIS_LED_B4 = out;
    LED_B5 = 0;
    TRIS_LED_B5 = out;
    TRIS_KEY_1 = in;
    TRIS_KEY_2 = in;
    TRIS_KEY_3 = in;
    
    TRIS_RX_PIC = in;
    TX_PIC = 1;
    TRIS_TX_PIC = out;
}