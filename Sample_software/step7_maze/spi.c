/***********************************************************************/
/*                                                                     */
/*  FILE        :spi.c			                               */
/*  DATE        :Tue, Jun 08, 2017                                     */
/*  DESCRIPTION :SPI Program                                           */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/

#include <machine.h>
#include <stdarg.h>
#include "iodefine.h"
#include "sci.h"
#include "glob_var.h"

int gyro_data = 0;
int gyro_address = 0;
//unsigned int enc_data = 0;
int ovre = 0;
int read = 0;
int gyro_write_cnt = 0;

//For encoder
int enc_r_data = 0;
int enc_r_address = 0;
int enc_write_cnt = 0;


void init_spi_gyro(void)
{
	/*****************************************************************************************
	SPIの設定
		ジャイロの設定	
	*****************************************************************************************/

	//RSPI1のプロテクト解除
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP_RSPI1 = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	RSPI1.SSLP.BYTE	= 	0x0e;	//SSLB0のみアクティブLOW　残りはアクティブHIGH
	RSPI1.SPPCR.BYTE=	0x00;	//RSPI通常モード　MOSIの出力値は前回転送の最終データ
	
	//ビットレート設定
	//ビットレート= PCLK/(2*(n+1)*2^N
	//PCLK = 48[MHz] n = 5, N = 2
	//ビットレート = 48/(2*6*4) = 1[MHz]
	RSPI1.SPBR =		5;	//n = 5, N = 2(後ほど設定)
	
	//RSPIデータコントロールレジスタ設定
	RSPI1.SPDCR.BIT.SPFC =	 0x00;	//使用するフレーム数の設定 一回のやり取りで一つのフレームを格納
	RSPI1.SPDCR.BIT.SPRDTD = 0x00;	//SPDRは受信バッファを読み出す
	RSPI1.SPDCR.BIT.SPLW =	 0x01;	//SPDRレジスタへはロングアクセス(SPLE = 1の時はロングワードアクセス)
	
	//RSPCK遅延値の設定
	RSPI1.SPCR.BIT.MSTR = 	0x0;	//RSPCKとSSLNDレジスタの書き込みエラー回避用に設定
	RSPI1.SPCR.BIT.SPE = 	0x0;	//RSPCKとSSLNDレジスタの書き込みエラー回避用に設定
	RSPI1.SPCKD.BYTE= 	0x02;	//RSPIの遅延を3RSPCKに設定
	
	//SSLネゲート遅延値の設定
	RSPI1.SSLND.BYTE= 	0x02;	//RSPIのSSLネゲート遅延を3RSPCKに設定
	
	//次アクセス遅延値の設定
	RSPI1.SPND.BYTE = 	0x00;	//次アクセス遅延を 1RSPCK + 2PCLK に設定
	
	//パリティ機能の設定
	//割り込みマスクの設定
	RSPI1.SPCR2.BYTE = 	0x00;	//パリティ機能を無効　アイドル割り込み要求を禁止
	
	//シーケンス長の設定
	RSPI1.SPSCR.BYTE = 	0x00;	//シーケンス長1に設定
	
	//コマンドレジスタの設定
	//シーケンス長の長さ分だけ行う
	RSPI1.SPCMD0.BIT.CPHA = 	0x01;	//偶数エッジでデータサンプル、奇数エッジでデータ変化
	RSPI1.SPCMD0.BIT.CPOL = 	0x01;	//アイドル時のRSPCKがHigh
	RSPI1.SPCMD0.BIT.BRDV = 	0x03;	//ビットレート分周をベースのビットレート(ビットレート設定のNの数字=8)を選択
	RSPI1.SPCMD0.BIT.SSLA = 	0x00;	//SSL信号アサート設定(SSL0を使う)
	RSPI1.SPCMD0.BIT.SSLKP = 	0x00;	//転送終了後から次アクセス開始までSSL信号レベルを保持
	RSPI1.SPCMD0.BIT.SPB =	 	0x1;	//RSPIデータ長を24ビットに設定
	RSPI1.SPCMD0.BIT.LSBF = 	0x00;	//MSBファースト
	RSPI1.SPCMD0.BIT.SPNDEN = 	0x00;	//RSPI次アクセス遅延許可ビット
	RSPI1.SPCMD0.BIT.SLNDEN = 	0x01;	//SSLネゲートの遅延はSSLNDの設定値
	RSPI1.SPCMD0.BIT.SCKDEN = 	0x01;	//RSPCKの遅延はSPCKDの設定値
	
	//割り込み優先レベル設定
	ICU.IPR[42].BYTE = 0x0F;
	ICU.IPR[43].BYTE = 0x0F;
	ICU.IPR[44].BYTE = 0x0F;
	
	//割り込みの許可
	ICU.IER[5].BIT.IEN2 = 1;	//SPRI1の割り込み許可
	ICU.IER[5].BIT.IEN3 = 1;	//SPTI1の割り込み許可
	ICU.IER[5].BIT.IEN4 = 1;	//SPII1の割り込み許可
	
	//I/Oポートの設定
	PORT2.PMR.BIT.B7 =	0x01;	//P27を周辺機能として使用
	PORTE.PMR.BIT.B2 =	0x01;	//PE2を周辺機能として使用
	PORTE.PMR.BIT.B3 =	0x01;	//PE3を周辺機能として使用
	PORTE.PMR.BIT.B4 =	0x01;	//PE4を周辺機能として使用

	
	//周辺機能の設定
	MPC.PWPR.BYTE=0x00;		//プロテクト解除
	MPC.PWPR.BYTE=0x40;		//プロテクト解除
	
	MPC.P27PFS.BIT.PSEL=	0x0D;	//P27をRSPCKBとして利用
	MPC.PE2PFS.BIT.PSEL=	0x0E;	//PE2をMOSIBとして利用
	MPC.PE3PFS.BIT.PSEL=	0x0D;	//PE3をMISOBとして利用
	MPC.PE4PFS.BIT.PSEL=	0x0D;	//PE4をSSLBBとして利用
	
	MPC.PWPR.BYTE=0x80;	//プロテクト作動
	
	//RSPI制御レジスタの設定
	RSPI1.SPCR.BIT.SPMS = 		0x00;	//SPI動作動作
	RSPI1.SPCR.BIT.TXMD = 		0x00;	//全二重同期式シリアル通信
	RSPI1.SPCR.BIT.MODFEN =		0x00;	//モードフォルトエラー検出を禁止
	RSPI1.SPCR.BIT.MSTR =		0x01;	//マスターモード
	RSPI1.SPCR.BIT.SPE =		0x00;	//RSPI機能が無効
	RSPI1.SPCR.BIT.SPEIE =		0x00;	//RSPIエラー割り込み要求の発生を禁止
	RSPI1.SPCR.BIT.SPTIE =		0x00;	//RSPI送信割り込み要求の発生を禁止
	RSPI1.SPCR.BIT.SPRIE =		0x00;	//RSPI受信割り込み要求の発生を禁止
	//SCI_printf("init_spi\n\r");
}



void preprocess_spi_gyro(int address)
{
	/*****************************************************************************************
	SPI転送前処理
		ジャイロ用
	*****************************************************************************************/
	//SCI_printf("preprocess start\n\r");
	
	long dummy = 0;
	dummy = RSPI1.SPDR.LONG;
	
	RSPI1.SPCR.BIT.SPE = 0;		//RSPI機能が無効
	RSPI1.SPCMD0.BIT.SPB =	 	0x1;	//RSPIデータ長を24ビットに設定
	
	//エラー要因のクリア
	RSPI1.SPSR.BIT.MODF = 0;
	RSPI1.SPSR.BIT.OVRF = 0;
	RSPI1.SPSR.BIT.PERF = 0;
	
	//SPII割り込みを禁止
	RSPI1.SPCR2.BIT.SPIIE = 0;
	
	gyro_write_cnt = 0;
	//SPEビットの許可　同時に必要な割り込みを許可
	///*
	RSPI1.SPCR.BIT.SPTIE = 1;	//RSPI送信割り込み要求の発生を許可
	RSPI1.SPCR.BIT.SPRIE = 1;	//RSPI受信割り込み要求の発生を許可
	RSPI1.SPCR.BIT.SPEIE = 1;	//RSPIエラー割り込み要求の発生を許可
	RSPI1.SPCR.BIT.SPE = 1;		//RSPI機能が有効
	//*/
	//RSPI1.SPCR.BYTE |= 0xF0;
	gyro_address = address;
	
}

void preprocess_spi_gyro_2byte(int address)
{
	/*****************************************************************************************
	SPI転送前処理
		ジャイロ用
	*****************************************************************************************/
	//SCI_printf("preprocess start\n\r");
	long dummy = 0;
	dummy = RSPI1.SPDR.LONG;
	
	RSPI1.SPCR.BIT.SPE = 0;		//RSPI機能が有効
	RSPI1.SPCMD0.BIT.SPB =	 	0xF;	//RSPIデータ長を16ビットに設定
	//エラー要因のクリア
	RSPI1.SPSR.BIT.MODF = 0;
	RSPI1.SPSR.BIT.OVRF = 0;
	RSPI1.SPSR.BIT.PERF = 0;
	
	//SPII割り込みを禁止
	RSPI1.SPCR2.BIT.SPIIE = 0;
	
	gyro_write_cnt = 0;
	//SPEビットの許可　同時に必要な割り込みを許可
	///*
	RSPI1.SPCR.BIT.SPTIE = 1;	//RSPI送信割り込み要求の発生を許可
	RSPI1.SPCR.BIT.SPRIE = 1;	//RSPI受信割り込み要求の発生を許可
	RSPI1.SPCR.BIT.SPEIE = 1;	//RSPIエラー割り込み要求の発生を許可
	RSPI1.SPCR.BIT.SPE = 1;		//RSPI機能が有効
	//*/
	//RSPI1.SPCR.BYTE |= 0xF0;
	gyro_address = address;
	
}

void write_spdr_gyro(void)
{
	/*****************************************************************************************
	送信用
	SPDRに送信データを書き込む関数
	今回はwho am I のアドレスにアクセス
	*****************************************************************************************/
	if(gyro_write_cnt == 0){
		RSPI1.SPDR.LONG = gyro_address;	//アドレス送信 
		RSPI1.SPCR.BIT.SPTIE = 0;		//RSPI送信割り込み要求の発生を禁止
		RSPI1.SPCR2.BIT.SPIIE = 1;		//RSPIアイドル割り込み要求の発生を許可
	}
	gyro_write_cnt++;
}

void spii_int_gyro(void)
{
	/*****************************************************************************************
	送信用
	SPII割り込み
	*****************************************************************************************/
	RSPI1.SPCR.BIT.SPE = 0;		//RSPI機能は無効
	RSPI1.SPCR2.BIT.SPIIE = 0;	//RSPIアイドル割り込み要求の発生を禁止
	
}

void read_spdr_gyro(void)
{
	/*****************************************************************************************
	受信用
	SPDRから受信データを読み込む関数
	*****************************************************************************************/
	read++;
	gyro_data = RSPI1.SPDR.LONG;	//データの読み込み
	//gyro_data = 0xffff;
	RSPI1.SPCR.BIT.SPRIE = 0;	//RSPI受信割り込み要求の発生を禁止
	RSPI1.SPCR.BIT.SPEIE = 0;	//RSPIエラー割り込み要求の発生を禁止
	
}


int ovre_check(void)
{
	return ovre;	
}

int gyro_read_check(void)
{
	return read;	
}

int gyro_write_check(void)
{
	return gyro_write_cnt;	
}

long read_gyro_data(void)
{
	return gyro_data;
}

/*****************************************************************************************
SPI転送処理
	エンコーダ用
*****************************************************************************************/
void init_spi_enc(void)
{
	/*****************************************************************************************
	SPIの設定
		エンコーダの設定	
	*****************************************************************************************/
	//RSPI1のプロテクト解除
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP_RSPI0 = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	RSPI0.SSLP.BYTE	= 	0x00;	//SSLA0,1,2,3 全てアクティブLow
	RSPI0.SPPCR.BYTE=	0x00;	//RSPI通常モード　MOSIの出力値は前回転送の最終データ
	
	//ビットレート設定
	//ビットレート= PCLK/(2*(n+1)*2^N
	//PCLK = 48[MHz] n = 5, N = 2
	//ビットレート = 48/(2*6*4) = 1[MHz]
	RSPI0.SPBR =		5;	//n = 5, N = 2(後ほど設定)
	
	//RSPIデータコントロールレジスタ設定
	RSPI0.SPDCR.BIT.SPFC =	 0x00;	//使用するフレーム数の設定 一回のやり取りで一つのフレームを格納
	RSPI0.SPDCR.BIT.SPRDTD = 0x00;	//SPDRは受信バッファを読み出す
	RSPI0.SPDCR.BIT.SPLW =	 0x00;	//SPDRレジスタへはワードアクセス(SPLE = 1の時はロングワードアクセス)
	
	//RSPCK遅延値の設定
	RSPI0.SPCR.BIT.MSTR = 	0x0;	//SPCKDとSSLNDレジスタの書き込みエラー回避用に設定
	RSPI0.SPCR.BIT.SPE = 	0x0;	//SPCKDとSSLNDレジスタの書き込みエラー回避用に設定
	RSPI0.SPCKD.BYTE= 	0x02;	//RSPIの遅延を3RSPCKに設定
	
	//SSLネゲート遅延値の設定
	RSPI0.SSLND.BYTE= 	0x02;	//RSPIのSSLネゲート遅延を3RSPCKに設定
	
	//次アクセス遅延値の設定
	RSPI0.SPND.BYTE = 	0x02;	//次アクセス遅延を 3RSPCK + 2PCLK に設定
	
	//パリティ機能の設定
	//割り込みマスクの設定
	RSPI0.SPCR2.BYTE = 	0x01;	//パリティ機能を有効　偶数パリティ　アイドル割り込み要求を禁止
	
	//シーケンス長の設定
	RSPI0.SPSCR.BYTE = 	0x00;	//シーケンス長1に設定
	
	//コマンドレジスタの設定
	//シーケンス長の長さ分だけ行う
	RSPI0.SPCMD0.BIT.CPHA = 	0x01;	//偶数エッジでデータサンプル、奇数エッジでデータ変化
	RSPI0.SPCMD0.BIT.CPOL = 	0x01;	//アイドル時のRSPCKがHigh
	RSPI0.SPCMD0.BIT.BRDV = 	0x03;	//ビットレート分周をベースのビットレート(ビットレート設定のNの数字=8)を選択
	RSPI0.SPCMD0.BIT.SSLA = 	0x02;	//SSL信号アサート設定(SSL0を使う)(2 SSL2)
	RSPI0.SPCMD0.BIT.SSLKP = 	0x00;	//転送終了後から次アクセス開始までSSL信号レベルをネゲート
	RSPI0.SPCMD0.BIT.SPB =	 	0x9;	//RSPIデータ長を10ビットに設定(9~16まで設定可能この設定がエンコーダの分解能となる)
	RSPI0.SPCMD0.BIT.LSBF = 	0x00;	//MSBファースト
	RSPI0.SPCMD0.BIT.SPNDEN = 	0x01;	//RSPI次アクセス遅延許可ビット
	RSPI0.SPCMD0.BIT.SLNDEN = 	0x01;	//SSLネゲートの遅延はSSLNDの設定値
	RSPI0.SPCMD0.BIT.SCKDEN = 	0x01;	//RSPCKの遅延はSPCKDの設定値
	
	//割り込みの禁止(IPRのエラー回避のため)
	ICU.IER[4].BIT.IEN7 = 0;	//SPRI0の割り込み禁止
	ICU.IER[5].BIT.IEN0 = 0;	//SPTI0の割り込み禁止
	ICU.IER[5].BIT.IEN1 = 0;	//SPII0の割り込み禁止
	
	///*
	//割り込み優先レベル設定
	ICU.IPR[39].BYTE = 0x0F;	//SPRI0
	//ICU.IPR[40].BYTE = 0x0F;	//SPTI0
	//ICU.IPR[41].BYTE = 0x0F;	//SPII0
	
	//割り込みの許可
	ICU.IER[4].BIT.IEN7 = 1;	//SPRI0の割り込み許可
	ICU.IER[5].BIT.IEN0 = 1;	//SPTI0の割り込み許可
	ICU.IER[5].BIT.IEN1 = 1;	//SPII0の割り込み許可
	//*/
	
	//I/Oポートの設定
	//PORT切換え
	PORT.PSRB.BIT.PSEL0 =	0;	//PB0を有効
	PORTA.PMR.BIT.B1 =	0x01;	//PA1を周辺機能として使用
	PORTA.PMR.BIT.B4 =	0x01;	//PA4を周辺機能として使用
	PORTA.PMR.BIT.B6 =	0x01;	//PA6を周辺機能として使用
	PORTB.PMR.BIT.B0 =	0x01;	//PB0を周辺機能として使用
	PORTC.PMR.BIT.B7 =	0x01;	//PC7を周辺機能として使用
	//PORTC.PMR.BIT.B5 = 	0x01;	//test
	
	//周辺機能の設定
	MPC.PWPR.BYTE=0x00;		//プロテクト解除
	MPC.PWPR.BYTE=0x40;		//プロテクト解除
	
	MPC.PA1PFS.BIT.PSEL=	0x0D;	//PA1をSSLA2として利用
	MPC.PA4PFS.BIT.PSEL=	0x0D;	//PA4をSSLA0として利用
	MPC.PA6PFS.BIT.PSEL=	0x0D;	//PA6をMOSIAとして利用
	MPC.PB0PFS.BIT.PSEL=	0x0D;	//PB0をRSPCKAとして利用
	MPC.PC7PFS.BIT.PSEL=	0x0D;	//PC7をMISOAとして利用
	//MPC.PC5PFS.BIT.PSEL=	0x0D;	//test
	
	MPC.PWPR.BYTE=0x80;	//プロテクト作動
	
	//RSPI制御レジスタの設定
	RSPI0.SPCR.BIT.SPMS = 		0x00;	//SPI動作(4線式)
	RSPI0.SPCR.BIT.TXMD = 		0x00;	//全二重同期式シリアル通信
	RSPI0.SPCR.BIT.MODFEN =		0x00;	//モードフォルトエラー検出を禁止
	RSPI0.SPCR.BIT.MSTR =		0x01;	//マスターモード
	RSPI0.SPCR.BIT.SPE =		0x00;	//RSPI機能が無効
	RSPI0.SPCR.BIT.SPEIE =		0x00;	//RSPIエラー割り込み要求の発生を禁止
	RSPI0.SPCR.BIT.SPTIE =		0x00;	//RSPI送信割り込み要求の発生を禁止
	RSPI0.SPCR.BIT.SPRIE =		0x00;	//RSPI受信割り込み要求の発生を禁止
	//SCI_printf("init_spi_enc\n\r");
}


void preprocess_spi_enc(int address)
{
	/*****************************************************************************************
	SPI転送前処理
		エンコーダ用
	*****************************************************************************************/
	int dummy = 0;
	
	enc_r_address = address;
	enc_write_cnt = 0;
	
	//エラー要因のクリア
	RSPI0.SPSR.BIT.MODF = 0;
	RSPI0.SPSR.BIT.OVRF = 0;
	RSPI0.SPSR.BIT.PERF = 0;
	
	//SPII割り込みを禁止
	RSPI0.SPCR2.BIT.SPIIE = 0;
	
	//dummy read
	dummy = RSPI0.SPDR.WORD.H;
	
	//SPEビットの許可　同時に必要な割り込みを許可
	RSPI0.SPCR.BIT.SPTIE = 1;	//RSPI送信割り込み要求の発生を許可
	RSPI0.SPCR.BIT.SPRIE = 1;	//RSPI受信割り込み要求の発生を許可
	RSPI0.SPCR.BIT.SPEIE = 1;	//RSPIエラー割り込み要求の発生を許可
	RSPI0.SPCR.BIT.SPE = 1;		//RSPI機能が有効	
}

void write_spdr_enc(void)
{
	/*****************************************************************************************
	送信用
	SPDRに送信データを書き込む関数
	今回はwho am I のアドレスにアクセス
	*****************************************************************************************/
	if(enc_write_cnt == 0){
		RSPI0.SPDR.WORD.H = 0x0000;		//アドレス送信 
		RSPI0.SPCR.BIT.SPTIE = 0;		//RSPI送信割り込み要求の発生を禁止
		RSPI0.SPCR2.BIT.SPIIE = 1;		//RSPIアイドル割り込み要求の発生を許可
		//SCI_printf("write_spdr\n\r");
	}
	enc_write_cnt++;
}

void spii_int_enc(void)
{
	/*****************************************************************************************
	送信用
	SPII割り込み
	*****************************************************************************************/
	RSPI0.SPCR.BIT.SPE = 0;		//RSPI機能は無効
	RSPI0.SPCR2.BIT.SPIIE = 0;	//RSPIアイドル割り込み要求の発生を禁止
	//SCI_printf("send_spdrdata\n\r");
}

void read_spdr_enc(void)
{
	/*****************************************************************************************
	受信用
	SPDRから受信データを読み込む関数
	*****************************************************************************************/
	enc_r_data = RSPI0.SPDR.WORD.H;	//データの読み込み
	RSPI0.SPCR.BIT.SPRIE = 0;	//RSPI受信割り込み要求の発生を禁止
	//SCI_printf("read_spdr\n\r");
}

int enc_write_cnt_check(void)
{
	return enc_write_cnt;	
}

int Get_enc_data(void)
{
	return enc_r_data;	
}