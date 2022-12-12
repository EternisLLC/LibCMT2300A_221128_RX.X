/*
 * File:   mainLibCmt2300A.c
 * Author: 11
 *
 * Created on 28 но€бр€ 2022 г., 11:58
 */
#include    <xc.h>
#include    "SetConfigBitsPic18f25k20.h"
#include    "ConfigPinsLibCmt2300A.h"
#include    "InitProcessor.h"
//#include    "CheckFunction.h"

#include    "LibCmt2300.h"

unsigned char   TempAddr = 0;
unsigned char   TempData = 0;

unsigned char   CounterMs = 0;

//unsigned char   ReadSettingCmt2300;

// прототипы обработчиков прерываний
void low_priority interrupt myLoIsr(void);
void high_priority interrupt myHiIsr(void);
void main(void) {
    ProcessorInit();
    
//// отладка тактировани€ работы с FIFO
//    while(1){
//        if(Interval._100ms){
//            TxFifo[0] ++;
//            TxFifo[1] --;
////            ReadFifoCmt2300(RxFifo,32);
////            WriteByteFifoCmt2300(TxFifo[0]);
//            WriteFifoCmt2300(TxFifo, 32);
//            Interval._100ms = 0;
//        }
//    }
////     
    while(!Interval._1s);
    Interval._1s = 0;
// программный сброс CMT2300
    SoftResetCmt2300();
    while(!Interval._1s);
    Interval._1s = 0;
    ModeCmt2300 = CheckModeCmt2300();
    while(!Interval._1s);
    Interval._1s = 0;
    if(ModeCmt2300 == 1){
        go_stby(); // переводим в STBY
    }
    while(!Interval._1s);
    Interval._1s = 0;
    ModeCmt2300 = CheckModeCmt2300();
    if(ModeCmt2300 == _STBY){
        // здесь конфигурируем CMT2300
        InitCmt2300();
        ModeCmt2300 = GotoNewModeCmt2300(_SLEEP);// сохран€ем конфигурацию переводом в слип
        ModeCmt2300 = GotoNewModeCmt2300(_STBY);//выводим дл€ работы в стендбай
    }
    
    Nop();
    ModeCmt2300 = GotoNewModeCmt2300(_TFS);//переходим в TFS
    Nop();
//    ModeCmt2300 = GotoNewModeCmt2300(_TX);//переходим в TX
    Nop();
//    ModeCmt2300 = GotoNewModeCmt2300(_STBY);//выводим дл€ работы в стендбай
    Nop();
    // провер€ем флаги заполнени€ tx_fifo
    TempData = ReadRegistrCmt2300(CUS_FIFO_FLAG);
    // загружаем tx_fifo 
    WriteRegistrCmt2300(CUS_FIFO_CTL,0x01);// включеем бит PI_FIFO_RD_WR_SEL
    WriteFifoCmt2300(TxFifoCmt2300, 9);              // записываем TX FIFO
    TempData = ReadRegistrCmt2300(CUS_FIFO_FLAG); // проверка заполнености tx_fifo
    Nop();
    WritePktLength(9);
    WriteNodeId(1);
    // включаем передачу
    ModeCmt2300 = GotoNewModeCmt2300(_TX);//переходим в TX
    
//    TempData = ReadRegistrCmt2300(CUS_INT_CLR1);// провер€ем окончание передачи
    Nop();
    WriteRegistrCmt2300(CUS_FIFO_CLR,0x03);// чистим tx_fifo
    while(1){
        if(Interval._1s){
            Interval._1s = 0;
            ModeCmt2300 = CheckModeCmt2300();
            if(ModeCmt2300 == _STBY){
                
                ModeCmt2300 = GotoNewModeCmt2300(_RFS);//переходим в RFS
                 __delay_us(350);
                ModeCmt2300 = GotoNewModeCmt2300(_RX);//переходим в RX
                LED_A2 = !LED_A2;
            }
        }
        if(Interval._100ms){
            TempData = ReadRegistrCmt2300(CUS_FIFO_FLAG);
            if(TempData){
                Nop();
            }
            TempData = (TempData & 0x78);
            if(TempData){
                Nop();
                ModeCmt2300 = GotoNewModeCmt2300(_STBY);
                WriteRegistrCmt2300(CUS_FIFO_CTL,0); // снимаем флаг записи в FIFO
                ReadFifoCmt2300(RxFifoCmt2300, 32);
                ClrRxFifoCmt2300();
                LED_A1 = !LED_A1;
                ModeCmt2300 = CheckModeCmt2300();
//                ReadFifoCmt2300(RxFifoCmt2300, 32);
                Nop();
            }
            Interval._100ms = 0;
        }
    }
    return;
}
void high_priority interrupt myHiIsr(void) {
    //======
    INTCONbits.INT0IF = 0;
    LED_B2 = !LED_B2;
}

void low_priority interrupt myLoIsr(void) {
    if(PIE1bits.TMR2IE && PIR1bits.TMR2IF){
        PIR1bits.TMR2IF = 0;
        if(Counter1ms){
            Counter1ms --;
            if(!Counter1ms){
                Counter1ms = 10;
                Interval._10ms = 1;
                if(Counter10ms){
                    Counter10ms --;
                    if(!Counter10ms){
                        Counter10ms = 10;
                        Interval._100ms = 1;
                        if(Counter100ms){
                            Counter100ms --;
                            if(!Counter100ms){
                                Counter100ms = 10;
                                Interval._1s = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}