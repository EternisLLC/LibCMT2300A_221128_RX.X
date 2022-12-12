#include    <xc.h>
#include    "ConfigPinsLibCmt2300A.h"
#include    "InitProcessor.h"
interval_t       Interval;
unsigned char    Counter1ms = 10;
unsigned char    Counter10ms = 10;
unsigned char    Counter100ms = 10;

//void InitTmr0(void){
//	T0CONbits.T0PS = 1;
////    T0PS2 = 0;T0PS1 = 0;T0PS0 = 0;
///*  
//    111 = 1:256 prescale value 8192
//    110 = 1:128 prescale value 4096
//    101 = 1:64 prescale value 2048
//    100 = 1:32 prescale value 1024
//    011 = 1:16 prescale value на 32ћ?ц 512мкс
//    010 = 1:8 prescale value 256
//    001 = 1:4 prescale value 128
//    000 = 1:2 prescale value 64
// */
//	PSA = 0;
//	T08BIT = 1;
//	T0CS = 0;
//    T0CONbits.TMR0ON = 0; // изм. 14
//    TMR0IP = 0; // устанавливаем низкий приоритет дл¤ прерываний от TMR0
//    TMR0IE = 0;
//    TMR0IF = 0;
//    
//}
//void InitTmr1(void){
//    T1CONbits.RD16 = 0; // таймер работает в 16 битном режиме
//    T1CONbits.T1CKPS = 2;//2; //устанавливаем предделитетель таймера =4
//    T1CONbits.T1OSCEN = 1; // включаем генератор таймера 1
//    T1CONbits.nT1SYNC = 1; // устанавливаем таймер в режим асинхронного счетчика
//    T1CONbits.TMR1CS = 1; // таймер тактируетс¤ внешним сигналом
//    T1CONbits.TMR1ON = 0; // включение таймера 1
//    TMR1IP = 0;             // низкий приоритет прерываний от таймера 1
//    TMR1IE = 0;
//    TMR1IF = 0;
//}
void InitTmr2(void){
	// онфигураци¤ таймера 2 регистр T2CON
    T2CONbits.T2OUTPS = 0x0F;
	//T2OUTPS3 = 1;T2OUTPS2 = 1;T2OUTPS1 = 1;T2OUTPS0 = 1;	//коэффициент постделител¤
	//TOUTPS3:TOUTPS0: биты выбора коэффициента постделител¤
	//0000 = 1:1 постделитель
	//0001 = 1:2 постделитель
	//Х
	//Х
	//Х
	//1111 = 1:16 постделитель*/
	T2CONbits.T2CKPS = 1;
//    T2CONbits.T2CKPS1 = 0; T2CONbits.T2CKPS0 = 0;						//
	//коэффициент предделител¤
	//00 = предделитель 1
	//01 = предделитель 4
	//1X = предделитель 16
	PR2 = 124; //
    T2CONbits.TMR2ON = 0; //включение таймера 2
    IPR1bits.TMR2IP = 0; // устанавливаем низкий приоритет дл¤ прерываний от TMR2
	PIE1bits.TMR2IE = 0;									//прерывани¤ от таймера 2
    PIR1bits.TMR2IF = 0;
									
	//Tint_t2=((4*N_pre*(PR2+1))/Fosc)*N_post
	//где Tint_t2 - период следовани¤ прерываний от TMR2
	//N_pre 		- значение предделител¤
	//N_post 		- значение постделител¤
	//PR2			- значение регистра PR2
	//Fosc		- значение частоты генератора в ћ?ц
	//=>PR2= ((Tint_t2*Fosc)/(4*N_pre*N_post))-1
	}
//void InitTmr3(void){
//    T3CONbits.T3RD16 = 0; // таймер работает в 16 битном режиме
//    T3CONbits.T3CKPS = 1;//2; //устанавливаем предделитетель таймера
//    T3CONbits.nT3SYNC = 1; // устанавливаем таймер в режим асинхронного счетчика
//    T3CONbits.TMR3CS = 0; // таймер тактируетс¤ внутренним сигналом
//    T3CONbits.TMR3ON = 0; // включение таймера 3
//    TMR3IP = 0;             // низкий приоритет прерываний от таймера 3
//    TMR3IE = 0;
//    TMR3IF = 0;
//}
//void OpenTmr0(void){
//	TMR0H = 0x00;
//	TMR0L = 0x00;
//	TMR0IF = 0;
//	TMR0IE = 1;
//	T0CONbits.TMR0ON = 1;
//}
//void CloseTmr0(void){
//	TMR0IF = 0;
//	TMR0IE = 0;
//	T0CONbits.TMR0ON = 0;
//}
//void OpenTmr1(void){
//	TMR1IF = 0;
//	TMR1IE = 1;
//	TMR1ON = 1;
//}
//void CloseTmr1(void){
//	TMR1ON = 0;
//	TMR1IE = 0;
//	TMR1IF = 0;
//}
void OpenTmr2(void){
	TMR2 = 0;
	TMR2IF = 0;
	TMR2IE = 1;
	TMR2ON = 1;
}
//void CloseTmr2(void){
//	TMR2ON = 0;
//	TMR2IE = 0;
//	TMR2IF = 0;
//}
//void OpenTmr3(void){
//	TMR3 = 3036;
//	TMR3IF = 0;
//	TMR3IE = 1;
//	TMR3ON = 1;
//}
void InitInterrupt(void){
    RCONbits.IPEN = 1;   // разрешение приоритетности прерываний
    INTCONbits.GIEH = 1;   // разрешение высокоприоритетных прерываний 
    INTCONbits.GIEL = 1;   // разрешение низкоприоритетных прерываний
    INTCONbits.INT0IF = 0; // сбрасываем флаг прерывани¤ INT0
    INTCONbits.INT0IE = 1; // вџключаем прерывание по приему пакета
}
void ProcessorInit(void){
    InitPinsCMT2300();
    InitTmr2();
    OpenTmr2();
    InitInterrupt();
    
}