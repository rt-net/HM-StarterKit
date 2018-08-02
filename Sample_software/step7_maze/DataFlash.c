
//hardware
#include "iodefine.h"
//#include "sci.h"
#include "glob_var.h"
#include "dataFlash.h"
#include "sci.h"

extern void wait_ms(int wtime);

#define BASEADDR	0x00100000
#define FLASHSIZE	0x7fff		//32kB

#define BLOCKSIZE	32		//bytes
#define BLOCKCOUNT	1024		//count

#define write_byte(A,D)	*(unsigned char *)(A)=(D)
#define write_word(A,D)	*(unsigned short *)(A)=(D)


const unsigned char *BaseAddr = (const unsigned char *)BASEADDR;

static int isErr = 0;
 
static void wait_fcuRdy(int timeout);
static void check_error(void);
static void fcu_reset(void);
void erase(void *addr); 
void write_eeflash(void *addr, short data);

static void transition_read(void);
static void transition_pe(void);


void map_write(void)
{
	short i;
	unsigned short *map_add;
	map_add = (unsigned short *)&wall;

	//DataFlashイレース
	for(i=0;i<32;i++){
		erase((unsigned short *)(MAP_ADD+i*32));
	}
	//マップデータをDataFlashに書き込む
	for(i=0;i<512;i++){
		write_eeflash((unsigned short *)(MAP_ADD+i*2),*map_add);
		map_add++;
	}
}

void map_copy(void)
{
	short i;
	unsigned short *map_add;
	map_add = (unsigned short *)&wall;

	//マップデータをRAMにコピー
	for(i=0;i<512;i++){
		*map_add = *(unsigned short *)(MAP_ADD+i*2);
		map_add++;
	}
}

void map_view(void){
	signed char i,j;
	
	SCI_printf("\x1b[0;0H");			//カーソルを0,0に移動
	SCI_printf("\n\r+");
	for(i = 0 ; i < MAZESIZE_X; i++){
		switch(wall[i][MAZESIZE_Y-1].north){		//黒色は"[30m"
			case NOWALL:	SCI_printf("\x1b[37m  +");//NOWALL
				break;
			case WALL:	SCI_printf("\x1b[37m--+");//WALL
				break;
			case UNKNOWN:	SCI_printf("\x1b[31m--+");//UNNOWN
				break;
			default:	SCI_printf("\x1b[33m--+");//VWALL
				break;
		}
	}

	SCI_printf("\n\r");
	for(j=(MAZESIZE_Y-1);j>-1;j--){
		switch(wall[0][j].west){
			case NOWALL:	SCI_printf("\x1b[37m ");//NOWALL
				break;
			case WALL:	SCI_printf("\x1b[37m|");//WALL
				break;
			case UNKNOWN:	SCI_printf("\x1b[31m|");//UNNOWN
				break;
			default:	SCI_printf("\x1b[33m|");//VWALL
				break;
		}	
		for(i=0;i<MAZESIZE_X; i++){
			switch(wall[i][j].east){
				case NOWALL:	SCI_printf("\x1b[37m   ");//NOWALL
					break;
				case WALL:	SCI_printf("\x1b[37m  |");//WALL
					break;
				case UNKNOWN:	SCI_printf("\x1b[31m  |");//UNNOWN
					break;
				default:	SCI_printf("\x1b[33m  |");//VWALL
					break;
			}
		}
		SCI_printf("\n\r+");
		for(i=0;i<MAZESIZE_X; i++){
			switch(wall[i][j].south){
				case NOWALL:	SCI_printf("\x1b[37m  +");//NOWALL
					break;
				case WALL:	SCI_printf("\x1b[37m--+");//WALL
					break;
				case UNKNOWN:	SCI_printf("\x1b[31m--+");//UNNOWN
					break;
				default:	SCI_printf("\x1b[33m--+");//VWALL
					break;
			}
		}
		SCI_printf("\n\r");
	}
	return;

}



//FCUをリセットする。エラー処理用
static void fcu_reset(void)
{
	FLASH.FRESETR.BIT.FRESET = 1;
	wait_ms(2);
	FLASH.FRESETR.BIT.FRESET = 0;
}


//FCU処理待ち timeoutにタイムアウトカウント[mS]を設定
static void wait_fcuRdy(int timeout)
{
	int isTimeout = 0;
	
	wait_ms(timeout);
	
	if(FLASH.FSTATR0.BIT.FRDY == 0)//P/E処理中
	{
		isTimeout = 1;
//		//SCI_printf("FCU time out\n\r");
	}
	
	//タイムアウトしていたらリセット
	if(isTimeout == 1)
	{
		fcu_reset();
	}
	
	return;
}


//エラーを確認し、エラーがあれば修正する
static void check_error(void)
{
	int iserr = 0;
	
	iserr |= FLASH.FSTATR0.BIT.ILGLERR;//FCUは不正なコマンドや不正やE2データフラッシュアクセスを検出
	iserr |= FLASH.FSTATR0.BIT.ERSERR;//イレース中にエラー発生
	iserr |= FLASH.FSTATR0.BIT.PRGERR;//プログラム中にエラー発生
	
	//printf("[%s]",tmp_source);
	if(iserr == 0)
	{
		//printf("No error\n\n");
		return;	//no error
	}
	
	isErr = 1;

//	//SCI_printf("FCU Error\n\r");
	
	if(FLASH.FSTATR0.BIT.ILGLERR == 1)
	{
		//printf("FSTATR0:%02X\nFSTATR1:%02X\nFASTAT:%02X\n\n", FLASH.FSTATR0.BYTE, FLASH.FSTATR1.BYTE, FLASH.FASTAT.BYTE);
		
		if(FLASH.FASTAT.BYTE != 0x10)
		{
			FLASH.FASTAT.BYTE = 0x10;//フラグをクリア
		}
	}
	
	write_byte(BaseAddr, 0x50);	//status clear
}



//指定したブロックをイレースする
//P1888 図47.14 ROM/E2データフラッシュイレース方法 
//イレースは32バイト
void erase(void *addr)
{
	unsigned char *a = (unsigned char *)addr;

	transition_pe();
	{
		write_byte(a, 0x20);
		write_byte(a, 0xD0);
		
		wait_fcuRdy(5);
		check_error();
	}
	transition_read();
}


//指定した領域に書き込む
//P1887 図47.13 ROM/E2データフラッシュへのプログラム方法 
void write_eeflash(void *addr, short data)
{
	unsigned short *a = (unsigned short *)addr;
	
	transition_pe();
	{
		write_byte(BaseAddr, 0xE8);
		write_byte(BaseAddr, 0x01);
		write_word(a, data);
		write_byte(BaseAddr, 0xD0);	
		wait_fcuRdy(3);
		check_error();
	}
	transition_read();
	
}

char blank_check(void *addr)
{
	unsigned short *a = (unsigned short*)addr;
	
	transition_pe();
	
	FLASH.FMODR.BIT.FRDMD = 1;
	FLASH.DFLBCCNT.WORD = 0x0000;
	write_byte(a, 0x71);
	write_byte(a, 0xD0);
	wait_fcuRdy(2);
	check_error();
	transition_read();
	
	return FLASH.DFLBCSTAT.BIT.BCST;
	
}

//FCUを読み込みモードに遷移させる
static void transition_read(void)
{
	FLASH.FENTRYR.WORD = 0xAA00;	//E2Flash P/E mode

	while(FLASH.FENTRYR.WORD != 0x0000);
	
	/* Flash write/erase disabled */
	FLASH.FWEPROR.BYTE = 0x02;
}

//FCUをP/Eモードに遷移させる
static void transition_pe(void)
{
	FLASH.FENTRYR.WORD = 0xAA00;
	while(0x0000 != FLASH.FENTRYR.WORD);
	
	FLASH.FENTRYR.WORD = 0xAA80;	//E2Flash P/E mode
	check_error();
	
	FLASH.FWEPROR.BYTE = 0x01;	//フラッシュP/Eプロテクトレジスタ
					//P/E ロックビットのP/E ロックビットの読み出し、ブランクチェックの許可
}

//DataFlashを初期化する
void hw_dflash_init(void)
{
	unsigned int i;
	
	FLASH.DFLRE0.WORD = 0x2DFF;	// block read enable E2データフラッシュ読み出し許可レジスタ
	FLASH.DFLWE0.WORD = 0x1EFF;	// block P/E enable E2データフラッシュP/E許可レジスタ
//P1883 図47.10 P/E処理の概略フロー
//FCU RAMへのファームウェア転送
	if(FLASH.FENTRYR.WORD != 0x0000)
	{
		FLASH.FENTRYR.WORD = 0xAA00;//FCU停止
	}
	FLASH.FCURAME.WORD=0xC401;
	
	for(i=0;i<8192;i++)
	{
		*(unsigned char *)(0x007F8000+i) = *(unsigned char *)(0xFEFFE000+i);
	}
	
	transition_pe();
	FLASH.PCKAR.BIT.PCKA = 48;	//FlashIF clock is 48MHz
	
	//周辺クロック通知コマンド
	write_byte(BaseAddr, 0xE9);
	write_byte(BaseAddr, 0x03);
	write_word(BaseAddr, 0x0F0F);
	write_word(BaseAddr, 0x0F0F);
	write_word(BaseAddr, 0x0F0F);
	write_byte(BaseAddr, 0xD0);
	
	wait_fcuRdy(2);//63us tPCKAは63usでよい。タイマーとして1msがありでそれを使用しているため1msにしている
	transition_read();


	
}

