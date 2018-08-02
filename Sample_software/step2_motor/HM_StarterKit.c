/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#include "iodefine.h"
#include "mathf.h"
#include "sci.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void main(void)
{
	
	/*****************************************************************************************
	クロック設定
		
	*****************************************************************************************/
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


	/*****************************************************************************************
	I/O設定
		LEDの設定	
	*****************************************************************************************/	
	//モータ系ピン設定
	//MOT_POWER
	PORTC.PDR.BIT.B6 = 1;//motor SLEEP (STBY)
	//MOT_CWCCW
	PORTC.PDR.BIT.B5 = 1;//Rmotor PH
	PORTB.PDR.BIT.B3 = 1;//Rmotor EN
	PORTC.PDR.BIT.B4 = 1;//Lmotor PH
	PORTB.PDR.BIT.B1 = 1;//Lmotor EN
	
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
	MTU0.TGRA = 48;		//左
	MTU0.TGRC = 48;		//右
	MTU.TSTR.BIT.CST0 =1; 
	
	PORTC.PODR.BIT.B6 = 1;
	PORTC.PODR.BIT.B5 = 0;	//R_PH
	PORTC.PODR.BIT.B4 = 1;	//L_PH
	while(1);	

}

#ifdef __cplusplus
void abort(void)
{

}
#endif
