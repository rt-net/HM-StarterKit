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
#include <sci.h>
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
	
	//赤外LEDのピン設定
	PORTA.PDR.BIT.B3 = 1;	//PA3を出力用に設定
	PORT1.PDR.BIT.B5 = 1;	//P15を出力用に設定
	PORT1.PDR.BIT.B4 = 1;	//P14を出力用に設定
	PORT3.PDR.BIT.B1 = 1;	//P31を出力用に設定
	/*****************************************************************************************
	その他イニシャライズ
		SCIとか
	*****************************************************************************************/	
	//sciのイニシャライズ
	init_sci();
	
	/*****************************************************************************************
	A/DCの設定
		光センサ
	*****************************************************************************************/
	//A/D変換用のピン設定
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP_S12AD = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	//PE1用設定
	PORTE.PDR.BIT.B7 = 0;
	
	//A/DポートのPMR設定
	PORT4.PMR.BIT.B6=1;	//P46を周辺機器として使用
	PORT4.PMR.BIT.B2=1;	//P42を周辺機器として使用
	PORT4.PMR.BIT.B1=1;	//P41を周辺機器として使用
	PORT4.PMR.BIT.B0=1;	//P40を周辺機器として使用
	PORTE.PMR.BIT.B1=0;	//PE1を周辺機器として使用 電源 PE1
	//A/DポートのPFS設定
	MPC.PWPR.BYTE=0x00;	//プロテクト解除
	MPC.PWPR.BYTE=0x40;	//プロテクト解除
	MPC.P46PFS.BIT.ASEL=1;	//A/D SEN_FR	AN006を使用
	MPC.P42PFS.BIT.ASEL=1;	//A/D SEN_R 	AN002を使用
	MPC.P41PFS.BIT.ASEL=1;	//A/D SEN_FR	AN001を使用
	MPC.P40PFS.BIT.ASEL=1;	//A/D SEN_R 	AN000を使用
	MPC.PE1PFS.BIT.ASEL=1;	//A/D V_BAT	AN009を使用
	MPC.PWPR.BYTE=0x80;	//プロテクト作動
	
	//A/D変換(デフォルトでシングルモード)
	//S12AD.ADCSR.BYTE = 0x0c;	//A/D変換クロックはPCLKB(48M[ha])
	S12AD.ADCSR.BIT.CKS = 3;	//A/D変換のクロックをPCLKの1分周(48M[Hz])に設定
	S12AD.ADCSR.BIT.ADCS = 0;	//シングルスキャンモードに設定
	
	unsigned long i = 0;
	while(1){
		S12AD.ADANS0.WORD = 0x0047;	//A/D変換をAN000,1,2,6のみ許可する
		//センサ発光
		PORTA.PODR.BIT.B3 = 1;	//PA3をHighに設定
		PORT1.PODR.BIT.B5 = 1;	//P15をHighに設定
		PORT1.PODR.BIT.B4 = 1;	//P14をHighに設定
		PORT3.PODR.BIT.B1 = 1;	//P31をHighに設定
		for(i = 0; i < 100*1000/40; i++); //40で100usec 
		S12AD.ADCSR.BIT.ADST = 1;	//A/D変換を開始
		while(S12AD.ADCSR.BIT.ADST);	//A/D変換が終わるまで待つ
		PORTA.PODR.BIT.B3 = 0;	//PA3をLowに設定
		PORT1.PODR.BIT.B5 = 0;	//P15をLowに設定
		PORT1.PODR.BIT.B4 = 0;	//P14をLowに設定
		PORT3.PODR.BIT.B1 = 0;	//P31をLowに設定
		//センサの値を表示
		SCI_printf("sensor_fr = %d\n\r",S12AD.ADDR6);
		SCI_printf("sensor_r = %d\n\r",S12AD.ADDR2);
		SCI_printf("sensor_l = %d\n\r",S12AD.ADDR1);
		SCI_printf("sensor_fl = %d\n\r",S12AD.ADDR0);
		//バッテリー電圧のA/D変換
		S12AD.ADANS0.WORD = 0x0200;	//A/D変換をAN006のみ許可する
		S12AD.ADCSR.BIT.ADST = 1;	//A/D変換を開始
		while(S12AD.ADCSR.BIT.ADST);	//A/D変換が終わるまで待つ
		
		SCI_printf("V_BAT = %d\n\r",S12AD.ADDR9);
		for(i = 0; i < 100*1000*100; i++);
		
		//画面クリアシーケンス
		SCI_printf("\x1b[2J");			//クリアスクリーン[CLS]
		SCI_printf("\x1b[0;0H");		//カーソルを0,0に移動
		
	}
	

}



#ifdef __cplusplus
void abort(void)
{

}
#endif
