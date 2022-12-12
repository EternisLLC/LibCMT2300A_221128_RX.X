#include    <xc.h>

#include    "ConfigPinsLibCmt2300A.h"
#include    "LibCmt2300.h"

//#include    "CheckFunction.h"

unsigned char   ModeCmt2300 = 0;
unsigned char   RxFifoCmt2300[32]= {0,0,0,0,0,0,0};
//unsigned char   TxFifoCmt2300[32] = {1,2,3,4,5,6,};
unsigned char   TxFifoCmt2300[32] = {0x01,0x05,0x30,0x45,0x00,0x60,0x00,0x02,0x10};
                                   /*room contr    ID     dest      source  comand*/   
void WriteRegistrCmt2300(unsigned char addr, unsigned char data){
    unsigned int temp;
    unsigned int mask = 0x8000;
    unsigned char i;
    temp = (unsigned int)(addr & 0x7F);
    temp = temp << 8;
    temp = temp + data;
    SDIO_OUT(); // выставляем ножку данных на выход
    CSB_SELECT();    // выбираем работу с регистрами
    if((mask & temp)){ // setting 7 bit addr
        SDIO_1();
    }else{
        SDIO_0();
    }
    for(i = 0; i < 16; i++){
        SCLK_1();    // тактируем очередной бит
        mask = (unsigned int)(mask>>1);
        SCLK_0();
        if((mask & temp)){ // setting next bit addr
            SDIO_1();
        }else{
            SDIO_0();
        }
    }
    SDIO_0();
    CSB_UN_SELECT();    // запрещаем работу с регистрами
}
unsigned char ReadRegistrCmt2300(unsigned char addr){
    unsigned char data = 0;
    unsigned char mask = 0x80;
    unsigned char i;
    data = (unsigned char)(addr | 0x80);
    SDIO_OUT(); // выставляем ножку данных на выход
    CSB_SELECT();    // выбираем работу с регистрами
    if((mask & data)){ // setting 7 bit addr
        SDIO_1();
    }else{
        SDIO_0();
    }
    for(i = 0; i < 7; i++){
        SCLK_1();    // тактируем очередной бит
        mask = (unsigned char)(mask>>1);
        SCLK_0();
        if((mask & data)){ // setting next bit addr
            SDIO_1();
        }else{
            SDIO_0();
        }
    }
    SCLK_1();    // тактируем 0 bit addr
    SDIO_IN(); // выставляем ножку данных на вход
    SCLK_0();
    data = 0;
    mask = 0x80;
    for(i = 0; i < 8; i++){
        SCLK_1();    // тактируем next bit data
        if(READ_SDIO()){
            data = (unsigned char)(mask | data); // читаем очередной бит.
        }
        SCLK_0();
        mask = (unsigned char)(mask>>1);
    }
    SDIO_0();
    SDIO_OUT(); // выставляем ножку данных на выход
    CSB_UN_SELECT();    // запрещаем работу с регистрами
    return data;
}
unsigned char CheckModeCmt2300(void){
    unsigned char temp;
    temp = ReadRegistrCmt2300(0x61);
    return (unsigned char)(temp & 0x0F);
}

unsigned char GotoNewModeCmt2300(unsigned char mode){
    unsigned char temp;
    if(mode == _RESET){
        WriteRegistrCmt2300(0x7F, _RESET);
        return 0;
    }
    switch(mode){
        case _SLEEP:
            temp = 0x10;
            break;
        case _STBY:
            temp = 0x02;
            break;
        case _RFS:
            temp = 0x04;
            break;
        case _TFS:
            temp = 0x20;
            break;
        case _RX:
            temp = 0x08;
            break;
        case _TX:
            temp = 0x40;
            break;
        default:
            temp = CheckModeCmt2300();
            return temp;
            break;
    }
    WriteRegistrCmt2300(0x60, temp);
    temp = CheckModeCmt2300();
    while(temp != mode){ // здесь вставить выход по таймеру из возможно бесконечного цикла
        temp = CheckModeCmt2300();
    }
    return temp;
}
#define AddrCMTBank 0x00
const unsigned char ValueCMTBank[12] = {0x00,0x66,0xEC,0x1C,0xF0,0x80,0x14,0x08,0x91,0x02,0x02,0xD0};
#define AddrSistemBank 0x0C
const unsigned char ValueSistemBank[12] = {0xAE,0xE0,0x35,0x00,0x00,0xF4,0x10,0xE2,0x42,0x20,0x00,0x81};
#define AddrFrequencyBank 0x18
//const unsigned char ValueFrequencyBank[8] = {0x42,0x08,0xE1,0x1A,0x42,0xF2,0x2E,0x1A};
const unsigned char ValueFrequencyBank[8] = {0x42,0xCD,0x2F,0x1B,0x42,0xB7,0x7D,0x1A};
//const unsigned char ValueFrequencyBank[8] = {0x42,0x02,0x77,0x1F,0x42,0xEC,0xC4,0x1E};//435
//const unsigned char ValueFrequencyBank[8] = {0x44,0x64,0x9E,0x15,0x44,0x4E,0xEC,0x14};  //444
#define AddrDataRateBank 0x20
const unsigned char ValueDataRateBank[24] = {0x2A,0x83,0x11,0x10,0xA2,0x13,0x0C,0x0A,0x9F,0x5A,\
                                             0x29,0x29,0xC0,0xA5,0x02,0x53,0x18,0x00,0xB4,0x00,\
                                             0x00,0x01,0x00,0x00};
#define AddrBasebandBank 0x38
const unsigned char ValueBasebandBank[29] = {0x22,0x04,0x00,0x55,0x02,0x00,0x00,0x00,0x00,0x00,
                                             0x00,0x91,0xD3,0x05,0x1F,0x11,0x00,0x00,0x00,0x00,
                                             0x01,0x00,0x00,0x64,0xFF,0x02,0x00,0x1F,0x0A};
#define AddrTXBank 0x55
const unsigned char ValueTXBank[11] = {0x70,0x9A,0x0C,0x00,0x03,0x50,0x00,0x1F,0x04,0x3F,0x7F};

void InitCmtBanks(unsigned char addr,unsigned char count,const unsigned char * value){
    unsigned char i = 0;
    unsigned char startAddr = addr;
    for(i = 0; i < count; i ++){
        WriteRegistrCmt2300(startAddr,*value);
        startAddr ++;
        value ++;
    }
}
CusIoSelBits ValueCusIoSel;
void InitGpioCmt2300(void){
    GPIO1_INT1();
    GPIO2_INT2();
    GPIO3_CLKO();
    WriteRegistrCmt2300(CUS_IO_SEL,(unsigned char)ValueCusIoSel.ByteIO);
}
// инициализация прерываний

CusIntEnBits ValueCusIntEn;
CusIntCtlBits ValueCusInt1Ctl;
CusIntCtlBits ValueCusInt2Ctl;
void InitInterruptCmt2300(void){
    unsigned char temp;
    ValueCusIntEn.sl_tmo = 0;
    ValueCusIntEn.rx_tmo = 0;
    ValueCusIntEn.tx_done = 1;
    ValueCusIntEn.preamb_ok = 0;
    ValueCusIntEn.sync_ok = 0;
    ValueCusIntEn.node_ok = 0;
    ValueCusIntEn.crc_ok = 0;
    ValueCusIntEn.pkt_done = 1;
    
    WriteRegistrCmt2300(CUS_INT_EN,(unsigned char)ValueCusIntEn.ByteINT); // разрешаем маскированные прерывания
    temp = ReadRegistrCmt2300(CUS_INT_EN);
    
    ValueCusInt1Ctl.INT_SEL = PKT_DONE;
    ValueCusInt1Ctl.INT_POLAR = 0;
    WriteRegistrCmt2300(CUS_INT1_CTL,(unsigned char)ValueCusInt1Ctl.ByteIntCTL); // привязываем прерывания к INT1
    
    ValueCusInt2Ctl.INT_SEL = TX_DONE;
    WriteRegistrCmt2300(CUS_INT2_CTL,(unsigned char)ValueCusInt2Ctl.ByteIntCTL); // привязываем прерывания к INT2
    
    InitGpioCmt2300();// изменяем конфигурацию ножек привязываем прерывания к ножкам
}
void CrcControlCmt2300(unsigned char control_reg){
    WriteRegistrCmt2300(CUS_PKT21,control_reg);
}
void NodeIdControlCmt2300(unsigned char control_reg){
    WriteRegistrCmt2300(CUS_PKT16,control_reg);
}
void InitCmt2300(void){
    unsigned char temp;
    InitCmtBanks(AddrCMTBank,12,ValueCMTBank);
    InitCmtBanks(AddrSistemBank,12,ValueSistemBank);
    InitCmtBanks(AddrFrequencyBank,8,ValueFrequencyBank);
    InitCmtBanks(AddrDataRateBank,24,ValueDataRateBank);
    InitCmtBanks(AddrBasebandBank,29,ValueBasebandBank);
    InitCmtBanks(AddrTXBank,11,ValueTXBank);
    Nop();
    InitInterruptCmt2300();
    NodeIdControlCmt2300(0);    // выключили фильтрацию по Node Id
    CrcControlCmt2300(0);       // выключили фильтрацию по CRC
    Nop();
}

unsigned char ReadByteFifoCmt2300(void){
    unsigned char data = 0;
    unsigned char mask = 0x80;
    unsigned char i;
    FCSB_SELECT();    // выбираем работу с FIFO
    data = 0;
    mask = 0x80;
    for(i = 0; i < 1; i++);// задержка 1 такт SCLK
    for(i = 0; i < 8; i++){
        SCLK_1();    // тактируем next bit data
        if(READ_SDIO()){
            data = (unsigned char)(mask | data); // читаем очередной бит.
        }
        SCLK_0();
        mask = (unsigned char)(mask>>1);
    }
    Delay2us();
    FCSB_UN_SELECT();    // запрещаем работу с FIFO
    return data;
}
void ReadFifoCmt2300(unsigned char* data, unsigned char counter){
    unsigned char i;
    SDIO_IN(); // выставляем ножку данных на вход
    SCLK_0(); 
    for(i = 0; i < counter; i++){
        *data = ReadByteFifoCmt2300();
        data ++;
        Delay4us();
    }
    SDIO_OUT(); // выставляем ножку данных на выход
}

void WriteByteFifoCmt2300(unsigned char data){
    unsigned int temp;
    unsigned int mask = 0x80;
    unsigned char i;
    temp = data;
    FCSB_SELECT();    // выбираем работу с FIFO
    for(i = 0; i < 1; i++);// задержка 1 такт SCLK
    for(i = 0; i < 8; i++){
        SCLK_1();    // тактируем очередной бит
        mask = (unsigned int)(mask>>1);
        SCLK_0();
        if((mask & temp)){ // setting next bit addr
            SDIO_1();
        }else{
            SDIO_0();
        }
    }
    Delay2us();
    FCSB_UN_SELECT();    // запрещаем работу с FIFO
}

void WriteFifoCmt2300(unsigned char * data, unsigned char counter){
    unsigned char i;
    SDIO_OUT(); // выставляем ножку данных на выход
    SCLK_0(); 
    for(i = 0; i < counter; i++){
        WriteByteFifoCmt2300(*data);
        data ++;
        Delay4us();
    }
}
void WritePktLength(unsigned char length){
    WriteRegistrCmt2300(CUS_PKT15,length);
}
void WriteNodeId(unsigned char room){
    WriteRegistrCmt2300(CUS_PKT17,room);
}