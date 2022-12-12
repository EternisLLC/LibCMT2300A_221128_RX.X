/* 
 * File:   LibCmt2300.h
 * Author: 11
 *
 * Created on 30 ноября 2022 г., 10:16
 */
/*
 * определения управления ножками процессора и функцции задержки
 * подлежат изменению при использовании платформы отличной от Pic18F25k20 
*/
#define _XTAL_FREQ 32000000 //определение частоты кварцевого резонатора

#define CSB_SELECT()    CSB_CMT2300 = 0
#define CSB_UN_SELECT() CSB_CMT2300 = 1
#define FCSB_SELECT()   FCSB_CMT2300 = 0
#define FCSB_UN_SELECT()   FCSB_CMT2300 = 1
#define SDIO_OUT()      TRIS_D_OUT_CMT2300()
#define SDIO_IN()       TRIS_D_IN_CMT2300()
#define SDIO_1()        DOUT_CMT2300 = 1
#define SDIO_0()        DOUT_CMT2300 = 0
#define SCLK_1()        SCK_CMT2300 = 1
#define SCLK_0()        SCK_CMT2300 = 0
#define READ_SDIO()     DIN_CMT2300

#define Delay2us()      __delay_us(2)
#define Delay4us()      __delay_us(4)




// определения регистров конфигудации используемых в ПО индивидуально
#define CUS_PKT1        0x38
#define CUS_PKT14       0x45
#define CUS_PKT15       0x46    // в этот регистр загоняем длину пакета
#define CUS_PKT16       0x47    // регистр управления NODE ID
#define CUS_PKT17       0x48    // в этот регистр записываем номер помещения
#define CUS_PKT21       0x4C    // регистр управления CRC
#define CUS_PKT29       0x54
#define CUS_TX1         0x55
#define CUS_FIFO_CTL    0x69
#define CUS_FIFO_CLR    0x6C
#define CUS_FIFO_FLAG   0x6E
#define CUS_IO_SEL      0x65
#define CUS_INT_EN      0x68
#define CUS_FIFO_FLAG   0x6E 
#define CUS_INT1_CTL    0x66
#define CUS_INT2_CTL    0x67
#define CUS_INT_CLR1    0x68

// определения команд управлекния CMT2300
#define SoftResetCmt2300()  WriteRegistrCmt2300(0x7F, 0xFF)
#define go_stby()           WriteRegistrCmt2300(0x60, 0x02) // переводим в STBY  0b0000 0010
#define go_rfs()            WriteRegistrCmt2300(0x60, 0x04) // переводим в RFS   0b0000 0100
#define go_rx()             WriteRegistrCmt2300(0x60, 0x08) // переводим в RX    0b0000 1000
#define go_sleep()          WriteRegistrCmt2300(0x60, 0x10) // переводим в SLEEP 0b0001 0000
#define go_tfs()            WriteRegistrCmt2300(0x60, 0x20) // переводим в TFS   0b0010 0000
#define go_tx()             WriteRegistrCmt2300(0x60, 0x40) // переводим в TX    0b0100 0000
#define go_switch()         WriteRegistrCmt2300(0x60, 0x80) // переводим переключение RX/TX и обратно 0b1000 0000
#define ClrTxFifoCmt2300()         WriteRegistrCmt2300(CUS_FIFO_CLR , 0x01) // очистка TX FIFO
#define ClrRxFifoCmt2300()         WriteRegistrCmt2300(CUS_FIFO_CLR , 0x02) // очистка RX FIFO    
#define RestoreTxFifoCmt2300()     WriteRegistrCmt2300(CUS_FIFO_CLR , 0x04) // восстановление TX FIFO
// определения и переменные связанные с режимом работы CMT2300
#define _IDLE       0x00
#define _SLEEP      0x01
#define _STBY       0x02
#define _RFS        0x03
#define _TFS        0x04
#define _RX         0x05
#define _TX         0x06
#define _LOW_VDD    0x08
#define _CAL        0x09
#define _SWITCH     0x10
#define _RESET      0xFF
extern unsigned char   ModeCmt2300;

// определения и переменные связанные с GPIO CMT2300
typedef  union{
    struct{
        unsigned char gpio1_sel   :2;
        unsigned char gpio2_sel   :2;
        unsigned char gpio3_sel   :2;
    };
    unsigned char   ByteIO;
}CusIoSelBits;
#define GPIO3_CLKO()    ValueCusIoSel.gpio3_sel = 0
#define GPIO3_DIO()     ValueCusIoSel.gpio3_sel = 1
#define GPIO3_INT2()    ValueCusIoSel.gpio3_sel = 2
#define GPIO3_DCLK()    ValueCusIoSel.gpio3_sel = 3

#define GPIO2_INT1()    ValueCusIoSel.gpio2_sel = 0
#define GPIO2_INT2()    ValueCusIoSel.gpio2_sel = 1
#define GPIO2_DIO()     ValueCusIoSel.gpio2_sel = 2
#define GPIO2_DCLK()    ValueCusIoSel.gpio2_sel = 3

#define GPIO1_DIO()     ValueCusIoSel.gpio1_sel = 0
#define GPIO1_INT1()    ValueCusIoSel.gpio1_sel = 1
#define GPIO1_INT2()    ValueCusIoSel.gpio1_sel = 2
#define GPIO1_DCLK()    ValueCusIoSel.gpio1_sel = 3
extern CusIoSelBits ValueCusIoSel;

// определения и переменные связанные с прерываниями CMT2300
typedef union{
    struct{
        unsigned char   pkt_done    :1;
        unsigned char   crc_ok      :1;
        unsigned char   node_ok     :1;
        unsigned char   sync_ok     :1;
        unsigned char   preamb_ok   :1;
        unsigned char   tx_done     :1;
        unsigned char   rx_tmo      :1;
        unsigned char   sl_tmo      :1;
    };
    unsigned char ByteINT;
}CusIntEnBits;
extern CusIntEnBits ValueCusIntEn;

#define RX_ACTIVE           0b00000 
#define TX_ACTIVE           0b00001
#define RSSI_VLD            0b00010
#define PREAM_OK            0b00011
#define SYNC_OK             0b00100
#define NODE_OK             0b00101
#define CRC_OK              0b00110
#define PKT_OK              0b00111
#define SL_TMO              0b01000
#define RX_TMO              0b01001
#define TX_DONE             0b01010
#define RX_FIFO_NMTY        0b01011
#define RX_FIFO_TH          0b01100
#define RX_FIFO_FULL        0b01101
#define RX_FIFO_WBYTE       0b01110
#define RX_FIFO_OVF         0b01111
#define TX_FIFO_NMTY        0b10000
#define TX_FIFO_TH          0b10001
#define TX_FIFO_FULL        0b10010
#define STATE_IS_STBY       0b10011
#define STATE_IS_FS         0b10100
#define STATE_IS_RX         0b10101
#define STATE_IS_TX         0b10110
#define LBD                 0b10111
#define TRX_ACTIVE          0b11000
#define PKT_DONE            0b11001

typedef union{
    struct{
        unsigned char INT_SEL   :5;
        unsigned char INT_POLAR :1;
        unsigned char reserv    :2;
    };
    unsigned char ByteIntCTL;
}CusIntCtlBits;
extern CusIntCtlBits ValueCusInt1Ctl;
extern CusIntCtlBits ValueCusInt2Ctl;
// определения и переменные связаные с FIFO
#define MAX_SIZE_FIFO   32
extern unsigned char   RxFifoCmt2300[32];
extern unsigned char   TxFifoCmt2300[32];

void WriteRegistrCmt2300(unsigned char addr, unsigned char data);
unsigned char ReadRegistrCmt2300(unsigned char addr);
unsigned char CheckModeCmt2300(void);
unsigned char GotoNewModeCmt2300(unsigned char mode);
void InitCmt2300(void);
void ReadFifoCmt2300(unsigned char * data, unsigned char counter);
void WriteFifoCmt2300(unsigned char * data, unsigned char counter);

void WritePktLength(unsigned char length);
void WriteNodeId(unsigned char room);
