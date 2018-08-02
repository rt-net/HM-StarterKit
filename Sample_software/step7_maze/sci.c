#include <machine.h>
#include <stdarg.h>
#include "iodefine.h"

char SCI_putc(unsigned char c);
short SCI_printf(char *string , ...);
short SCI_getc(unsigned char *c);



void init_sci(void){
	SYSTEM.PRCR.WORD = 0xA502;
        MSTP(SCI1) = 0;
	SYSTEM.PRCR.WORD = 0xA500;

	SCI1.SCR.BYTE = 0x00;
	while (0x00 != (SCI1.SCR.BYTE & 0xF0));	//割り込み要求が禁止されるまで待つ
	PORT2.PODR.BIT.B6 = 1;			//TXDのDirctionの切り替え後の値をhigh
    	PORT2.PDR.BIT.B6 = 1;			//出力に設定
    	PORT3.PDR.BIT.B0 = 0;			//入力に設定
	PORT2.PMR.BIT.B6 = 0;			//汎用ポートに設定
    	PORT3.PMR.BIT.B0 = 0;			//汎用ポートに設定
    	MPC.PWPR.BIT.B0WI  = 0;
    	MPC.PWPR.BIT.PFSWE = 1;
    	MPC.P26PFS.BIT.PSEL = 0x0A;		//TXD1
    	MPC.P30PFS.BIT.PSEL = 0x0A;		//RXD1
    	MPC.PWPR.BIT.PFSWE = 0;
    	MPC.PWPR.BIT.B0WI  = 1;
    	PORT3.PMR.BIT.B0 = 1;			//周辺機能(RXD1)として使用
        SCI1.SCR.BIT.CKE = 0;
    	SCI1.SMR.BYTE = 0x00;			//1stopbit parityなし　8bit 調歩同期
        SCI1.SCMR.BYTE = 0xF2;			//S=32clock
    	SCI1.SEMR.BYTE = 0x00;
    	SCI1.BRR =38; 				//@48MHz 38400bps
    	SCI1.SCR.BYTE =0x30;			//送信割り込み禁止
	PORT2.PMR.BIT.B6 = 1;			//周辺機能(TXD1)として使用
	SCI1.SCR.BIT.TE = 1;
	SCI1.SCR.BIT.RE = 1;
}		



char SCI_putc(unsigned char c){
	//summary
	//  1文字送信
	//parameter
	//	c : 送信文字
	//return
	//	0:error
	//	1:success

	unsigned long i;
	
	for(i=0;i<2000000;i++){
		if(SCI1.SSR.BIT.TEND){					//フラグ確認
			SCI1.TDR = c;					//送信データ書き込み
			return 1;
		}
	}
	return 0;							//タイムアウト
}


static void int_to_Dec(unsigned long n, char *buf){
	//summary
	//	数値をキャラクタ（10進法）に変換
    char c;
    short length = 0;
    short i, half;

    do{
        if(n == 0)  i = 0;
        else        i = n % 10;
        buf[length] = (char)(i + '0');
        length++;
        n /= 10;
    }while(n != 0);

												    // 文字の並び順を直す
    half = length >> 1;
    for(i=0; i < half; i++){
        c = buf[i];
        buf[i] = buf[(length-1)-i];
        buf[(length-1)-i] = c;
    }
    buf[length]='\0';								//終端コード
}

// --------------------------------------------------------
//  数値を16進数文字変換
// --------------------------------------------------------
static void int_to_Hex(unsigned long n, short upper, char *buf){
	//summary
	//	数値をキャラクタ（16進法）に変換
    char c;
    char a = 'a';
    short len = 0;
    short i, half;

    // 大文字/小文字の設定
    if(upper) a = 'A';
    
    // 16進文字列へ変換し文字数をカウント
    do{
        i = n & 0x0F;
        if(i > 9)  buf[len] = (unsigned char)(i + a - 10);
        else       buf[len] = (unsigned char)(i + '0');
        len++;
        n >>= 4;
    }while(n != 0);

    // 文字の並び順を直す
    half = len >> 1;
    for(i=0; i < half; i++){
        c = buf[i];
        buf[i] = buf[(len-1)-i];
        buf[(len-1)-i] = c;
    }

    buf[len]='\0';   								//終端コードの挿入 
}

static char *PrintFormat(char *format,void *value){
	//summary
	//  %5d　など、数値等を指定したフォーマットで出力する
	//parameter
	//	-
	//return
	//	-
	short n = 0;							//桁数
	short m = 0;							//数の正負
	short z = 0;							//0で埋めるかどうかのフラグ
	short i = 0;							//作業用変数
	char buf[20];
	char *ptr = buf;
	unsigned char length=0; 
	char base = ' ';

	if(*format=='0'){						//ゼロで埋める
		 z = 1;
		 format++;
	}
	if((*format>='0')&&(*format<='9')){				//桁数指定
		n = *format-'0';
		format++;
	}

	switch(*format){
		case 'd':						// 符号あり 10進数(short型)
			if((signed short)value < 0){			//負数
				m = 1;
				int_to_Dec((unsigned long)(-(signed short)value) ,buf); 
			}
			else
			{
				int_to_Dec((unsigned long)value , buf);
			}
			break;
		case 'l': 						// 符号あり 10進数(long型)
			if((signed long)value < 0){			//負数
				m = 1;
				int_to_Dec((unsigned long)(-(signed long)value) ,buf); 
			}
			else
			{
				int_to_Dec((unsigned long)value , buf);
			}
			break;
		case 'u':   						// 符号なし 10進数
			int_to_Dec((unsigned short)value ,buf);
			break;
		case 'x':   						// 符号なし 16 進整数。"abcdef" を使用。
			int_to_Hex((unsigned long)value ,0 ,buf);
			break;
		case 'X':   						// 符号なし 16 進整数。"ABCDEF" を使用。
			int_to_Hex((unsigned long)value ,1 ,buf);
			break;
		case 's':   						// String出力
			ptr = (char *)value;
			break;
		case 'c':   						// 1byte文字
			buf[0] = (char)((unsigned long)value & 0xFF);
			buf[1] = '\0';					//終端コード
			break;
		default: 
			buf[0] = '\0';					//終端コード
			break;
	}
	
									//表示
	if(m){length ++;}						//負数の場合文字数調整
	for(;buf[length]!='\0';length++){}				//文字数のカウント
	if(n>length){n -=length;}					//桁数の調整
    else{n  = 0;}

	if(z){								//０で埋める場合
        base = '0';
        if(m){	        						// マイナス表示
            SCI_putc('-');
            m = 0;
			//length --;
        }
	}
    for(i=0; i<n; i++){							//桁数合わせ
	    SCI_putc(base);
	}
    if(m){								//マイナスの表示
		SCI_putc('-');
		//length --;
	}
	for(i=0;i<length;i++){						//データの表示
		if(!SCI_putc(buf[i])) break;
	}

	format++;
	return(format);
}


short SCI_printf(char *string , ...){
	//summary
	//  簡易文字列送信
	//parameter
	//	文字列、
	//return
	//	0:	error
	//	else:送信文字数
	va_list ap;
	char *ptr;

//	unsigned long value = 0;
	
	ptr = string;
	va_start(ap,string);
	for(;;){
	    while (*ptr != '%') {					//普通の文字列を送信
	      if (!*ptr) {						//全送信
	        va_end (ap);
	        return (ptr-string);					//送信文字数を返す
	      }
		  else {
			SCI_putc(*ptr);					//1文字送信
			ptr++;
	      }
	    }
		ptr++;							//%に続く文字で判断する
		if(*ptr=='%'){ SCI_putc('%'); ptr++;	}		//%%  -> %と表示
		else{
			ptr = PrintFormat(ptr,va_arg(ap,void *));
			if(ptr == 0) return 0;
		}
	
	}
}
