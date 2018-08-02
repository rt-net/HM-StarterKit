/***********************************************
RX631 I2C accel
          Gyro
	  touchSensor(and LED)
MPU6050
I2C Address 110_1000(0x68)
Gyro_Z register 0x47 0x48  read
Accel_X register 0x3B 0x3C read
Accel_Y register 0x3D 0x3E read

PIC1822
I2C Address 000_1000(0x08)
TouchSensor register 0x02 0x03 16bit read Timer Counter
TouchSensor register 0x04       8bit read Touch on/off
LED         register 0x06       8bit read/write LED0,LED1
TouchSensor register 0x08,0x09 16bit read/write Timer Counter Threshold


Start     Address R/W ACK  DATA ACK DATA ACK STOP
Condition 1-7     8    9   1-8  9  1-8   9   Condition

Write Sequence
Master S AD+W     RA      DATA     DATA     P
Slave         ACK    ACK       ACK      ACK

Read Sequence
Master S AD+W     RA     S AD+R          ACK       NACK P
Slave         ACK    ACK        ACK DATA      DATA

S:  StartCondition
AD: Address
ACK:Acknowledge
RA: Register Address
P:  StopCondition
NACK:not ackowlege

R/W
0:Write
1:Read

	  
************************************************/
#include "iodefine.h"
#include "sci.h"

struct str_IIC_data
{
	union {
		unsigned short WORD;
		struct {
			unsigned char GYRO_Z_H;
			unsigned char GYRO_Z_L;
		} BYTE;
	} GYRO_Z;
	union {
		unsigned short WORD;
		struct {
			unsigned char ACC_X_H;
			unsigned char ACC_X_L;
		} BYTE;
	} ACC_X;
	union {
		unsigned short WORD;
		struct {
			unsigned char ACC_Y_H;
			unsigned char ACC_Y_L;
		} BYTE;
	} ACC_Y;
	union {
		unsigned short WORD;
		struct {
			unsigned char TCNT_H;
			unsigned char TCNT_L;
		} BYTE;
	} TCNT;
	unsigned char Touch_Data;
	unsigned char LED_Data;
	union {
		unsigned short WORD;
		struct {
			unsigned char TCNT_TH_H;
			unsigned char TCNT_TH_L;
		} BYTE;
	} TCNT_TH;
}iic_data;

struct str_IIC
{
	unsigned char	SlvAdr;		/* Slave Address, Don't set bit0. It's a Read/Write bit */
	unsigned char   RW;		/* read:1 write:0 */
	unsigned char	RA;		/*Register Address*/
//	unsigned char	PreCnt;		/* Number of Predata */
//	unsigned char	read_buf[10];	/* read data buffer */
	unsigned char	RWCnt;		/* Number of Data */
	unsigned char	write_buf[10];	/* write data buffer */
	 unsigned char	trm_cnt;	/*transmission counter*/
	unsigned char   rcv_cnt;	/*reception counter*/
}iic_buff;

//char iic_trm_cnt;
//char iic_rcv_cnt;

//struct str_IIC iic_buf;
//struct str_IIC *sp;
/*
typedef struct str_IIC_API_T IIC_API_T;
extern IIC_API_T iic_buff;
extern unsigned char I2C_BUF[2];
extern unsigned char GYRO_ACCE_F;
*/

void I2C_START(void);
void I2C_RESTART(void);
void I2C_STOP(void);
void I2C_PUT(unsigned char aaa);
void GYRO_SETTING(void);

void init_I2C(void)
{
		
	//I2Cのイニシャライズ
	//r_iic_drv_io_open
	PORT1.PCR.BIT.B6=0;//入力プルアップ無効
	PORT1.PDR.BIT.B6=0;//Input
	PORT1.PCR.BIT.B7=0;//入力プルアップ無効
	PORT1.PDR.BIT.B7=0;//Input

	//r_iic_drv_mpc_disable	
	PORT1.PMR.BIT.B6= 0 ;//SCL　汎用ポート
	PORT1.PMR.BIT.B7= 0 ;//SDA　汎用ポート
    	MPC.PWPR.BIT.B0WI  = 0;
    	MPC.PWPR.BIT.PFSWE = 1;
    	MPC.P16PFS.BIT.PSEL = 0x0;//汎用ポート
    	MPC.P17PFS.BIT.PSEL = 0x0;//汎用ポート
    	MPC.PWPR.BIT.PFSWE = 0x80;
	
	//r_iic_drv_cancel_mstp
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(RIIC2) = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	//r_iic_drv_iic_disable
	RIIC2.ICCR1.BIT.ICE =0;		//SCLn SDAn端子非駆動状態
	RIIC2.ICCR1.BIT.IICRST=1;	//IICRST 内部I2Cリセット状態 IICRST=1 & ICE=0でRIIC全レジスタリセット
	
	//パワーオン時の暴走対策で割り込み禁止に設定
	//r_iic_drv_int_disable
	IEN(RIIC2,EEI2) = 0;
	IEN(RIIC2,RXI2) = 0;
	IEN(RIIC2,TXI2) = 0;
	IEN(RIIC2,TEI2) = 0;
	IPR (RIIC2,EEI2) =0x02;//割り込み順位
	IPR (RIIC2,RXI2) =0x02;
	IPR (RIIC2,TXI2) =0x02;
	IPR (RIIC2,TEI2) =0x02;
	
	//r_iic_drv_int_icier_setting
	RIIC2.ICIER.BYTE = 0x00;//割り込み禁止

	RIIC2.ICCR1.BIT.IICRST = 0;//内部セット解除
	
	RIIC2.ICSER.BYTE = 0x00;//SARL0,SARL1無効
	
	RIIC2.ICMR1.BYTE = 0x28;//bitcounter=9 PCLK/4=10MHz 0.08us 		12M -> 28, 10M -> 38
	RIIC2.ICBRL.BYTE = 0xF0;//転送速度=1/{[ICBRH+1)+(ICBRL+1)]/IICφ}	12M -> F0, 10M -> FD
	RIIC2.ICBRH.BYTE = 0xE7;//IICφ=PCLKx10^6×分周比　Tr=0の場合は400kHz	12M -> E7, 10M -> F8
	
	RIIC2.ICMR2.BYTE = 0x00;
	RIIC2.ICMR3.BYTE = 0x00;
	RIIC2.ICMR3.BIT.ACKWP =0x01;//ACKBT(アクノリッジの書き込み許可
	
	IPR (RIIC2,EEI2) =0x02;//割り込み順位
	IPR (RIIC2,RXI2) =0x02;
	IPR (RIIC2,TXI2) =0x02;
	IPR (RIIC2,TEI2) =0x02;	

	RIIC2.ICFER.BYTE =0x72;
	
    	MPC.PWPR.BIT.B0WI  = 0;
    	MPC.PWPR.BIT.PFSWE = 1;
    	MPC.P16PFS.BIT.PSEL = 0x0F;//SCL
    	MPC.P17PFS.BIT.PSEL = 0x0F;//SDA
    	MPC.PWPR.BIT.PFSWE = 0x80;
	
	PORT1.PMR.BIT.B6= 1 ;//SCL
	PORT1.PMR.BIT.B7= 1 ;//SDA

	
	IR(RIIC2,RXI2) =0x00;//clear
	IR(RIIC2,TXI2) =0x00;//clear
//	RIIC2.ICIER.BYTE =0xfc;//送信データエンプティ、送信終了割り込み 受信データフル割り込み、NACK受信割り込み、ストップコンディション検出割り込み、スタートコンディション検出割り込み

	RIIC2.ICCR1.BYTE |= 0x80;//ICE 内部I2C転送動作可能
	
}

unsigned char IOex_SWITCH(void){
	unsigned char sw;
	I2C_START();
	I2C_PUT((0x18)<<1);//アドレス write
	I2C_PUT(0x00);//コントロールレジスタ
	I2C_RESTART();
	I2C_PUT((0x18<<1)|0x01);//アドレス read

	while(RIIC2.ICSR2.BIT.RDRF==0);
	if(RIIC2.ICSR2.BIT.NACKF==1){	RIIC2.ICSR2.BIT.STOP=0;}
	
	RIIC2.ICMR3.BIT.WAIT=1;
	RIIC2.ICMR3.BIT.ACKBT=1;
	RIIC2.ICDRR;//dummy
	
	while(RIIC2.ICSR2.BIT.RDRF==0);
	sw=RIIC2.ICDRR;
	
	while(RIIC2.ICSR2.BIT.RDRF==0);
	RIIC2.ICSR2.BIT.STOP=0;
	RIIC2.ICCR2.BIT.SP=1;
	RIIC2.ICDRR;//dummy
	RIIC2.ICMR3.BIT.WAIT=0;
	
	while(RIIC2.ICSR2.BIT.STOP==0);
	RIIC2.ICSR2.BIT.NACKF=0;
	RIIC2.ICSR2.BIT.STOP=0;
	
	return sw;
	//SCI_printf("who=%x\n\r",who);
	
}

void IOex_LED(short led_num){
//ポーリングによる設定
//*
	I2C_START();//Startbit発生
	I2C_PUT((0x18<<1));//アドレス write
	I2C_PUT(0x01);//PWR_MGMT_2
	I2C_PUT(0x00 | led_num);//sleep解除
	I2C_STOP();
}

void IOex_SETTING(void){
//ポーリングによる設定
//*
	I2C_START();//Startbit発生
	I2C_PUT((0x18<<1));//アドレス write
	I2C_PUT(0x03);//PWR_MGMT_2
	I2C_PUT(0xF0);//sleep解除
	I2C_STOP();
}

//割り込みによるデータ送信
void IIC_PIC_LED_WRITE(char data){
	if(RIIC0.ICCR2.BIT.BBSY !=0){//busがビジーなら受信しない
		return;
	}
	iic_buff.SlvAdr=0x08;	//PICのスレーブアドレス
	iic_buff.RW=0;		//0:write,1:read
	iic_buff.RA=0x06;	//レジスタのアドレス
	iic_buff.RWCnt=1;	//データ数
	iic_buff.write_buf[0] = data;
	iic_buff.trm_cnt=0;	//送信データカウンタ
	iic_buff.rcv_cnt=0;	//受信データカウンタ
	
	RIIC2.ICIER.BIT.TIE  = 0;
	
	RIIC2.ICIER.BIT.STIE=1;
	RIIC2.ICSR2.BIT.START = 0;
	RIIC2.ICCR2.BIT.ST = 1;//割り込みフラグ:START 割り込み要因:EEI
	
	
}	

//割り込みによるデータ受信
void IIC_PIC_TOUCH_READ(char data){
	if(RIIC0.ICCR2.BIT.BBSY !=0){//busがビジーなら受信しない
		return;
	}
	iic_buff.SlvAdr=0x08;	//PICのスレーブアドレス
	iic_buff.RW=1;		//0:write,1:read
	if(data==0){//on/offデータの確認
		iic_buff.RA=0x04;	//レジスタのアドレス
		iic_buff.RWCnt=1;	//データ数
	}else if(data==1){
		iic_buff.RA=0x02;	//レジスタのアドレス
		iic_buff.RWCnt=1;	//データ数
	}else{
		iic_buff.RA=0x03;	//レジスタのアドレス
		iic_buff.RWCnt=1;	//データ数
	}
	iic_buff.trm_cnt=0;	//送信データカウンタ
	iic_buff.rcv_cnt=0;	//受信データカウンタ
	
	RIIC2.ICIER.BIT.TIE  = 0;
	
	RIIC2.ICIER.BIT.STIE=1;
	RIIC2.ICSR2.BIT.START = 0;
	RIIC2.ICCR2.BIT.ST = 1;//割り込みフラグ:START 割り込み要因:EEI
	
	
}	
	

void I2C_START(void){
	while(RIIC2.ICCR2.BIT.BBSY==1){	
	}//1:バスビジー 0:バスフリー　パスがフリーになるまで待つ
	RIIC2.ICSR2.BIT.START = 0;//STARTコンディション未検出
	RIIC2.ICCR2.BIT.ST    = 1;//スタートコンディション発行
	
}

void I2C_RESTART(void){
	while(RIIC2.ICSR2.BIT.TDRE == 0);
	while(RIIC2.ICSR2.BIT.TEND==0);
	if(RIIC2.ICSR2.BIT.TDRE == 0);
	if(RIIC2.ICCR2.BIT.BBSY==1);
	RIIC2.ICSR2.BIT.START = 0;//STARTコンディション未検出
	RIIC2.ICCR2.BIT.RS = 1;//リスタートコンディションの発行を要求する
	while(RIIC2.ICCR2.BIT.RS);

}


void I2C_STOP(void){
	while(RIIC2.ICSR2.BIT.TEND==0);
	
	RIIC2.ICSR2.BIT.STOP=0;
	RIIC2.ICCR2.BIT.SP=1;//ストップコンディションの発行

	while(RIIC2.ICSR2.BIT.STOP==0);

	RIIC2.ICSR2.BIT.NACKF=0;
	RIIC2.ICSR2.BIT.STOP=0;
}

void I2C_PUT(unsigned char aaa){
	if(RIIC2.ICSR2.BIT.NACKF==1){	RIIC2.ICSR2.BIT.STOP=0;}//NACK検出ならstopコンディションを未検出にする
	while(RIIC2.ICSR2.BIT.TDRE==0);//送信レジスタが空かcheck
	RIIC2.ICDRT = aaa;
}

//===================================割り込み===================================
//通信エラー/イベント発生による割り込み
//AL    AL=1 & ALIE=1
//NACKF NACKF=1 & NAKIE=1
//TMOF  TMOF=1 & TMOIE=1
//START START=1 & STIE=1
//STOP  STOP=1 & SPIE=1
void int_i2c_ee(void){
	/* Check stop condition detection */
	if((RIIC2.ICSR2.BIT.STOP!=0) && (RIIC2.ICIER.BIT.SPIE!=0)){
		/* Clear each status flag */
		RIIC2.ICSR2.BIT.NACKF = 0;
		RIIC2.ICSR2.BIT.STOP = 0;
		/* Enable/Disable each interrupt */
		RIIC2.ICIER.BYTE = 0xBB;//TEIEとSTIEの割り込みを禁止

		/* Initialize ram for RIIC */
		iic_buff.trm_cnt = 0;			/* Clear the internal transmission counter for IIC */
		iic_buff.rcv_cnt = 0;			/* Clear the internal reception counter for IIC */
	}

	/* Check NACK reception */
	if((RIIC2.ICSR2.BIT.NACKF != 0) && (RIIC2.ICIER.BIT.NAKIE!=0)){
		RIIC2.ICIER.BIT.NAKIE = 0;
		/* Generate Stop Condition */
		RIIC2.ICSR2.BIT.STOP = 0;
		RIIC2.ICCR2.BIT.SP = 1;
	}

	/* Check start condition detection restart*/
	if((RIIC2.ICSR2.BIT.START != 0) && (RIIC2.ICIER.BIT.STIE!=0)){
		/* Disable Start Condition Detection Interrupt */
		RIIC2.ICSR2.BIT.START = 0;
		RIIC2.ICIER.BIT.STIE = 0;
		RIIC2.ICIER.BIT.TIE = 1;
		/* Transfer slave device address */
		if((iic_buff.trm_cnt==0)&&(iic_buff.rcv_cnt==0)){//最初のスタートコンディション
			RIIC2.ICDRT = (iic_buff.SlvAdr<<1);		/* When master transfer, b0 must be '1' */
		}else{
			RIIC2.ICDRT = (iic_buff.SlvAdr<<1)| 0x01 ;	/* When master transfer, b0 must be '1' */
		}			
	}
}
	
void int_i2c_rx(void){
	volatile unsigned char tmp;

	/* Increase internal reception counter on RAM. It is a receive data counter. */
	iic_buff.rcv_cnt++;

	
	if(iic_buff.rcv_cnt == iic_buff.RWCnt){
			RIIC2.ICMR3.BIT.WAIT = 1;
			RIIC2.ICMR3.BIT.ACKBT = 1;
	}
	
	if(iic_buff.rcv_cnt == 1){
		/* dummy read */
		tmp = RIIC2.ICDRR;
	}else if(iic_buff.rcv_cnt == (iic_buff.RWCnt+1)){//データ受信おわり
		if(iic_buff.SlvAdr==0x08){
			if(iic_buff.RA==0x04){
				iic_data.Touch_Data=RIIC2.ICDRR;
			}else if(iic_buff.RA==0x02){
				iic_data.TCNT.BYTE.TCNT_H=RIIC2.ICDRR;
			}else if(iic_buff.RA==0x03){
				iic_data.TCNT.BYTE.TCNT_L=RIIC2.ICDRR;
			}
		}
		RIIC2.ICSR2.BIT.STOP = 0;
		RIIC2.ICCR2.BIT.SP = 1;
		RIIC2.ICMR3.BIT.WAIT = 0;
		
	}else{
		if(iic_buff.SlvAdr==0x08){
			iic_data.TCNT.BYTE.TCNT_H=RIIC2.ICDRR;
		}
	}
		
	
//
//	}else if(iic_buff.RWCnt == 2){
//		RIIC2.ICMR3.BIT.WAIT = 1;
//		RIIC2.ICMR3.BIT.ACKBT = 1;
//		*iic_buff.pRWData++ = RIIC2.ICDRR;
//		iic_buff.RWCnt--;
//	}else if(iic_buff.RWCnt == 1){
//		RIIC2.ICSR2.BIT.STOP = 0;
//		RIIC2.ICCR2.BIT.SP = 1;
//		*iic_buff.pRWData++ = RIIC2.ICDRR;	/* Read final data */
//		iic_buff.RWCnt--;
//		RIIC2.ICMR3.BIT.WAIT = 0;
//		tmp2=(I2C_BUF[1]<<8) & 0xff00;
//		tmp3=I2C_BUF[0] & 0x00ff;
//		if(GYRO_ACCE_F==0){
//			GYRO16=( tmp2 | tmp3) >> 4;
//		}else{
//			ACCE16=( tmp2 | tmp3) >> 4;
//		}
//			
//	}else{
//		*iic_buff.pRWData++ = RIIC0.ICDRR;
//		iic_buff.RWCnt--;
//	}

}

//送信データ エンプティ
//TDRE=1 & TIE=1
void int_i2c_tx(void){
	/* Increase internal transmission counter */
	iic_buff.trm_cnt++;
	if(iic_buff.trm_cnt == 1){
	/* Transfer slave device register address */
		RIIC2.ICDRT = iic_buff.RA;
		if(iic_buff.RW==1){//read のためリスタートの準備
			RIIC2.ICIER.BIT.TEIE = 1;	/* Enable Transmit End Interrupt */
		}
	}else if(iic_buff.trm_cnt == 2){
		if(iic_buff.RW==0){
			RIIC2.ICDRT = iic_buff.write_buf[0];
		}
//		if(GYRO_ACCE_F==0){
//			RIIC2.ICDRT = 0xac;//out_z_L ??read 最上位アドレスが1の場合、アドレスがauto incr
//		}else{
//			RIIC2.ICDRT = 0xaa;//out_y_L ??read
//		}
	}else if(iic_buff.trm_cnt == (iic_buff.RWCnt+2)) {//書き込みデータ終わり
		if(iic_buff.RW==0){
			RIIC2.ICIER.BIT.TEIE = 1;	/* Enable Transmit End Interrupt */
		}
	}
}

void int_i2c_te(void){
	if(iic_buff.RW==0){//writeのみ
		RIIC2.ICIER.BIT.SPIE=1;
		RIIC2.ICSR2.BIT.STOP=0; //STOPコンディション発光
		RIIC2.ICCR2.BIT.SP=1;
	}else{//read
		RIIC2.ICSR2.BIT.START = 0;		/* Clear Start Condition Detection flag */
		RIIC2.ICIER.BIT.STIE = 1;		/* Enable Start Condition Detection Interrupt */
		RIIC2.ICIER.BIT.TEIE = 0;		/* Disable Transmit End Interrupt */
		/* Generate restart condition */
		RIIC2.ICCR2.BIT.RS = 1;
	}
}
