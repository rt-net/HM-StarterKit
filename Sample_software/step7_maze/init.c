/***********************************************************************/
/*                                                                     */
/*  FILE        :init.c                                                */
/*  DATE        :2017/6/27	                                       */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :RX631 48P                                             */
/*                                                                     */
/*  NOTE:Initiarise micro controler.                                   */
/*                                                                     */
/***********************************************************************/
#include "iodefine.h"
#include "sci.h"
#include "spi.h"
#include "glob_var.h"
#include "DataFlash.h"
#include "i2c.h"

extern wait_ms(int wtime);

/*****************************************************************************************
クロック設定

*****************************************************************************************/
void clock_init(void){

	SYSTEM.PRCR.WORD = 0xa50b;		//クロックソース選択の保護の解除

	SYSTEM.PLLCR.WORD = 0x0F00;		/* PLL 逓倍×16 入力1分周 (12.000MHz * 16 = 192MHz)*/
	SYSTEM.PLLCR2.BYTE = 0x00;		/* PLL ENABLE */
	
	SYSTEM.PLLWTCR.BYTE     = 0x0F;		/* 4194304cycle(Default) */
	
	
	// ICK   : 192/2 = 96MHz 		// システムクロック CPU DMAC DTC ROM RAM
	// PCLKA : 192/2 = 96MHz 		// 周辺モジュールクロックA ETHERC、EDMAC、DEU
	// PCLKB : 192/4 = 48MHz 		// 周辺モジュールクロックB 上記以外 PCLKB=PCLK
/*	
	SYSTEM.SCKCR.BIT.FCK=0x02;		//FCLK MAX 50MHz  192/4
	SYSTEM.SCKCR.BIT.ICK=0x01;		//ICLK MAX 100MHz 192/2
	SYSTEM.SCKCR.BIT.PSTOP1=0x01;		//BCLK 出力停止
	SYSTEM.SCKCR.BIT.PSTOP0=0x01;		//SDCLK 出力停止
	SYSTEM.SCKCR.BIT.BCK=0x02;		//BCLK MAX 100MHz ICLK以下にする必要がある192/4
	SYSTEM.SCKCR.BIT.PCKA=0x01;		//PCLKA MAX 100MHz 192/2
	SYSTEM.SCKCR.BIT.PCKB=0x02;		//PCLKB MAX 50MHz 192/4
	//上記の設定では正しくclock設定ができないため下記のように一括で設定すること
*/
	SYSTEM.SCKCR.LONG = 0x21C21211;		//FCK1/4 ICK1/2 BCLK停止 SDCLK停止 BCK1/4 PCLKA1/2 PCLKB1/4
/*
	SYSTEM.SCKCR2.BIT.UCK=0x03;		//UCLK MAX 48MHz 192/4
	SYSTEM.SCKCR2.BIT.IEBCK=0x02;		//IECLK MAX 50MHz 192/4
*/
	SYSTEM.SCKCR2.WORD = 0x0032;		/* UCLK1/4 IEBCK1/4 */
	SYSTEM.BCKCR.BYTE = 0x01;		/* BCLK = 1/2 */
	
	SYSTEM.SCKCR3.WORD = 0x0400;		//PLL回路選択
	
}

/*****************************************************************************************
CMTの設定
		
*****************************************************************************************/
void init_cmt(void)
{
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(CMT0) = 0;
	MSTP(CMT1) = 0;
	MSTP(CMT2) = 0;
    	SYSTEM.PRCR.WORD = 0xA500;	
	
	//CMT0は制御割り込み用タイマとして使用
	CMT0.CMCR.BIT.CKS=1;	// PCLK/32 1.5MHz
	CMT0.CMCR.BIT.CMIE=1;	//割り込みを許可
	CMT0.CMCNT=0;		//カウンターのクリア
	CMT0.CMCOR=1500-1;	//1kHz

	IEN(CMT0,CMI0) = 1;	//割り込み要求を許可 
	IPR(CMT0,CMI0) = 15;	//割り込み優先度 15が最高
	IR(CMT0,CMI0)=0;	//割り込みステータフラグをクリア
	
	//CMT1はセンサー制御用タイマとして使用
	CMT1.CMCR.BIT.CKS=1;	// PCLK/32 1.5MHz
	CMT1.CMCR.BIT.CMIE=1;	//割り込みを許可
	CMT1.CMCNT=0;		//カウンターのクリア
	CMT1.CMCOR=(1500/4)-1;	//4kHz

	IEN(CMT1,CMI1) = 1;	//割り込み要求を許可 
	IPR(CMT1,CMI1) = 14;	//割り込み優先度を次点に設定
	IR(CMT1,CMI1)=0;	//割り込みステータフラグをクリア

	//CMT2はセンサー制御用タイマとして使用
	CMT2.CMCR.BIT.CKS=1;	// PCLK/32 1.5MHz
	CMT2.CMCR.BIT.CMIE=1;	//割り込みを許可
	CMT2.CMCNT=0;		//カウンターのクリア
	CMT2.CMCOR=(1500/2)-1;	//2kHz

	IEN(CMT2,CMI2) = 1;	//割り込み要求を許可 
	IPR(CMT2,CMI2) = 13;	//割り込み優先度を次点に設定
	IR(CMT2,CMI2)=0;	//割り込みステータフラグをクリア
	
	CMT.CMSTR0.BIT.STR0=1;	//カウントスタート
	CMT.CMSTR0.BIT.STR1=1;	//カウントスタート
	CMT.CMSTR1.BIT.STR2=1;	//カウントスタート
	
}

/*****************************************************************************************
I/O設定
	LEDの設定	
*****************************************************************************************/
void io_init(void){
	
	//ブザー関連
	PORTB.PDR.BIT.B5 = 1;
		
	
	//赤外LEDのピン設定
	PORTA.PDR.BIT.B3 = 1;	//PA3を出力用に設定
	PORT1.PDR.BIT.B5 = 1;	//P15を出力用に設定
	PORT1.PDR.BIT.B4 = 1;	//P14を出力用に設定
	PORT3.PDR.BIT.B1 = 1;	//P31を出力用に設定
}


/*****************************************************************************************
A/DCの設定
	光センサとバッテリ電圧
*****************************************************************************************/
void sensor_init(void){

	//A/D変換用のピン設定
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP_S12AD = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	//A/DポートのPMR設定
	PORT4.PMR.BIT.B6=1;	//P46を周辺機器として使用
	PORT4.PMR.BIT.B2=1;	//P42を周辺機器として使用
	PORT4.PMR.BIT.B1=1;	//P41を周辺機器として使用
	PORT4.PMR.BIT.B0=1;	//P40を周辺機器として使用
	PORTE.PMR.BIT.B7=1;	//PE7を周辺機器として使用
	//A/DポートのPFS設定
	MPC.PWPR.BYTE=0x00;	//プロテクト解除
	MPC.PWPR.BYTE=0x40;	//プロテクト解除
	MPC.P46PFS.BIT.ASEL=1;	//A/D SEN_FR	AN006を使用
	MPC.P42PFS.BIT.ASEL=1;	//A/D SEN_R 	AN002を使用
	MPC.P41PFS.BIT.ASEL=1;	//A/D SEN_FR	AN001を使用
	MPC.P40PFS.BIT.ASEL=1;	//A/D SEN_R 	AN000を使用
	MPC.PWPR.BYTE=0x80;	//プロテクト作動
	
	//A/D変換(デフォルトでシングルモード)
	//S12AD.ADCSR.BYTE = 0x0c;	//A/D変換クロックはPCLKB(48M[ha])
	S12AD.ADCSR.BIT.CKS = 3;	//A/D変換のクロックをPCLKの1分周(48M[Hz])に設定
	S12AD.ADANS0.WORD = 0x0047;	//A/D変換をAN006のみ許可する
	S12AD.ADCSR.BIT.ADCS = 0;	//シングルスキャンモードに設定
}

/*****************************************************************************************
モータの設定
	左右モータ
*****************************************************************************************/
void motor_init(void){
	
	//モータ系ピン設定
	//MOT_POWER
	PORTC.PDR.BIT.B6 = IO_OUT;//motor SLEEP (STBY)
	//MOT_CWCCW
	PORTC.PDR.BIT.B5 = IO_OUT;//Rmotor PH (実際は左)
	PORTB.PDR.BIT.B3 = IO_OUT;//Rmotor EN (実際は左)
	PORTC.PDR.BIT.B4 = IO_OUT;//Lmotor PH (実際は右)
	PORTB.PDR.BIT.B1 = IO_OUT;//Lmotor EN (実際は右)
	
	//機能ピン設定	
	MPC.PWPR.BIT.B0WI=0;
	MPC.PWPR.BIT.PFSWE=1;
	MPC.PB1PFS.BIT.PSEL=1;	//PWM R MTIOC0C
	MPC.PB3PFS.BIT.PSEL=1;	//PWM L MTIOC0A
	MPC.PWPR.BYTE=0x80;
	
	//MTUのイニシャライズ
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(MTU) = 0;//MTUモジュールON
	SYSTEM.PRCR.WORD = 0xA500;	
	
	//ピンや機能設定時にはタイマストップ
	MTU.TSTR.BYTE=0;	//タイマ動作ストップ
	
	//左右モータ用MTU0 PWM2 時定数τ=L/R=17uH/(1.07+0.5+0.3)=110kHz
	MTU0.TCR.BIT.TPSC=0;	//PCLK/1 48MHz
	MTU0.TCR.BIT.CCLR=6;	//PWM TGRDのコンペアマッチでTCNTクリア
	MTU0.TIORH.BIT.IOA=5;	//初期出力0コンペアマッチ0出力
	MTU0.TIORL.BIT.IOC=5;	//初期出力0コンペアマッチ0出力
	MTU0.TIORL.BIT.IOD=2;	//初期出力0コンペアマッチ1出力
	MTU0.TGRA = 0;		//4以下は動作しない
	MTU0.TGRC = 0;
	MTU0.TGRD = 240;	//周期 200kHz
	MTU0.TMDR.BIT.MD=3;	//PWM2
	
	PORTB.PMR.BIT.B3=1;	//右PWM
	PORTB.PMR.BIT.B1=1;	//左PWM
	MTU0.TGRA = 0;		//左
	MTU0.TGRC = 0;		//右
	MTU.TSTR.BIT.CST0 =1; 
	
	MOT_POWER_OFF;
	MOT_CWCCW_R  = 1;	//R_PH
	MOT_CWCCW_L  = 1;	//L_PH
}

/*****************************************************************************************
光センサー系のパラメータ初期化
	リファレンスとか壁の閾値とか
*****************************************************************************************/
void init_parameters(void)
{
			
	sen_r.ref = REF_SEN_R;				//右センサのリファレンス値を初期化
	sen_l.ref = REF_SEN_L;				//左センサのリファレンス値を初期化
	
	sen_r.th_wall = TH_SEN_R;			//右センサの壁有無判断の閾値を初期化
	sen_l.th_wall = TH_SEN_L;			//左センサの壁有無判断の閾値を初期化
	
	sen_fr.th_wall = TH_SEN_FR;			//右前センサの壁有無判断の閾値を初期化
	sen_fl.th_wall = TH_SEN_FL;			//左前センサの壁有無判断の閾値を初期化
	
	sen_r.th_control = CONTH_SEN_R;			//右センサの壁制御かけるか否かの閾値を初期化
	sen_l.th_control = CONTH_SEN_L;			//左センサの壁制御かけるか否かの閾値を初期化
	
	con_wall.kp = CON_WALL_KP/10000.0;			//壁比例制御の比例定数を初期化
}

/*****************************************************************************************
迷路情報の初期化

*****************************************************************************************/
void init_maze(void)	//迷路情報の初期化
{
	int i,j;
	
	for(i = 0; i < MAZESIZE_X; i++)
	{
		for(j = 0; j < MAZESIZE_Y; j++)
		{
			wall[i][j].north = wall[i][j].east = wall[i][j].south = wall[i][j].west = UNKNOWN;	//迷路の全体がわからない事を設定する
		}
	}
	
	for(i = 0; i < MAZESIZE_X; i++)
	{
		wall[i][0].south = WALL;		//四方の壁を追加する(南)
		wall[i][MAZESIZE_Y-1].north = WALL;	//四方の壁を追加する(北)
	}
	
	for(j = 0; j < MAZESIZE_Y; j++)
	{
		wall[0][j].west = WALL;			//四方の壁を追加する(西)
		wall[MAZESIZE_X-1][j].east = WALL;	//四方の壁を追加する(東)
	}
	
	wall[0][0].east = wall[1][0].west = WALL;	//スタート地点の右の壁を追加する
	
}


/*****************************************************************************************
ジャイロのリファレンス取得

*****************************************************************************************/
void gyro_get_ref(void){
	long i = 0;
	float gyro_ref_temp = 0;
	gyro_ref = 0;
	//ジャイロのリファレンス取得
	for(i = 0; i < 2500; i++){
		gyro_ref_temp += (float)gyro_x_new;
		wait_ms(1);
	}
	gyro_ref = (gyro_ref_temp/2500.0);
	degree = 0;
	wait_ms(100);
}

/*****************************************************************************************
全ての機能のイニシャライズ
	
*****************************************************************************************/
void init_all(void){
	int i;
	clock_init();
	io_init();
	sensor_init();
	motor_init();
	init_sci();
	init_spi_gyro();
	init_spi_enc();
	init_I2C();
	IOex_SETTING();
	void LED(short led_num);
	LED(0);
	MOT_POWER_OFF;
	init_parameters();
	init_maze();
	//Encoderの初期化
	/*
	RSPI0.SPCMD0.BIT.SSLA = 	0x00;	//SSL信号アサート設定(SSL0を使う)
	preprocess_spi_enc(0x7E40);
	for(i = 0; i < 100*1000*10; i++);
	preprocess_spi_enc(0x5040);
	for(i = 0; i < 100*1000*10; i++);
	RSPI0.SPCMD0.BIT.SSLA = 	0x02;	//SSL信号アサート設定(SSL2を使う)	preprocess_spi_enc(0x7E40);
	for(i = 0; i < 100*1000*10; i++);
	preprocess_spi_enc(0x5040);
	for(i = 0; i < 100*1000*10; i++);
	*/
	//Gyro初期設定
	preprocess_spi_gyro_2byte(0x0681);		//ジャイロリセット
	for(i = 0; i < 100*1000*10; i++);
	preprocess_spi_gyro_2byte(0x0601);		//Low Power Mode OFF
	for(i = 0; i < 100*1000*10; i++);

	//ジャイロの設定
	preprocess_spi_gyro_2byte(0x7F20);		//User Bank2に変更
	for(i = 0; i < 100*1000*10; i++);
	preprocess_spi_gyro_2byte(0x0107);		//Range を最大2000dpsへ変更
	for(i = 0; i < 100*1000*10; i++);
	preprocess_spi_gyro_2byte(0x7F00);		//User Bank0に変更
	for(i = 0; i < 100*1000*10; i++);

	preprocess_spi_gyro_2byte(0x0621);		//ジャイロスタート
	//変数初期化
	timer = 0;
	
	//コンペアマッチタイマ開始
	init_cmt();	

	//E2フラッシュの初期化
	hw_dflash_init();

	
}





