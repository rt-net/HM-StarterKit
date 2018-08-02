#include "portdef.h"

#define SET_BUZZER_FREQ(f)	MTU0.TGRB=(unsigned short)(12000000/(f));MTU0.TGRA=(unsigned short)(6000000/(f))	//ブザーの発振周波数を算出して、設定

#define ENABLE_BUZZER		PORTB.PMR.BIT.B3=1;MTU.TSTR.BIT.CST0=1	//ブザーの発振を開始
#define DISABLE_BUZZER		PORTB.PMR.BIT.B3=0;MTU.TSTR.BIT.CST0=0	//ブザーの発振を停止

#define TIRE_CIRCUIT	(PI*TIRE_DIAMETER)			//タイヤの円周を直径から算出(PIは円周率)
#define SPEED2GREG(v)	(7500/(((v)/TIRE_CIRCUIT)))		//スピードからジェネラルレジスタの値を計算

#define LEN2STEP(l)	(2*400*(l)/TIRE_CIRCUIT)		//走行距離(mm)からステッピングモータのステップ数を算出
#define STEP2LEN(s) 	(TIRE_CIRCUIT*(s)/(2*400))		//ステッピングモータのステップ数から走行距離を算出

#define CONV_SEN2WALL(w) ((w) ? WALL : NOWALL)			//センサ情報から壁情報へ変換
