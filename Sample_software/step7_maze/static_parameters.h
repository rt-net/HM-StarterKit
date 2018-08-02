
#define IO_OUT	(1)				//PFCのInput/Output レジスタに1を設定すると出力になる
#define IO_IN	(0)				//PFCのInput/Output レジスタに0を設定すると入力になる

#define BATT_MAX		2552//12.4V
#define BATT_MIN		2034//10V

#define SW_OFF	(1)				//スイッチがOFFの時に取る値
#define SW_ON	(0)				//スイッチがONの時に取る値

#define CHATTERING_WAIT	(50)			//チャタリング回避用待ち時間

#define INC_FREQ	(2000)			//モードが増加した時に鳴らす音の周波数
#define DEC_FREQ	(1000)			//モードが減少した時に鳴らす音の周波数

#define MOT_R_FORWARD	(0)			//モータドライバのCWCCW端子にLOWを出力すると前進する
#define MOT_R_BACK	(1)			//モータドライバのCWCCW端子にHIGHを出力するとバックする#define MOT_FORWARD	(0)			//モータドライバのCWCCW端子にLOWを出力すると前進する
#define MOT_L_FORWARD	(1)			//モータドライバのCWCCW端子にLOWを出力すると前進する
#define MOT_L_BACK	(0)			//モータドライバのCWCCW端子にHIGHを出力するとバックする#define MOT_FORWARD	(0)			//モータドライバのCWCCW端子にLOWを出力すると前進する

//#define MIN_SPEED	(0.3)			//最低速度.ジェネラルレジスタが16bitであることと、MTUの動作周波数から求められる値がおおよそ18mm/sなので、余裕を持って30mm/s

#define PI (3.141592653589793)			//円周率

//方位
#define RIGHT	(0)
#define LEFT	(1)
#define FRONT	(2)
#define REAR	(3)

//最短走行終了フラグ
#define END_FAST	(4)

#define HALF_SECTION	(45)			//半区画の距離
#define SECTION			(90)		//一区画の距離

#define MAZESIZE_X		(32)		//迷路の大きさ(MAZESIZE_X * MAZESIZE_Y)迷路
#define MAZESIZE_Y		(32)		//迷路の大きさ(MAZESIZE_X * MAZESIZE_Y)迷路

#define UNKNOWN	2				//壁があるかないか判らない状態の場合の値
#define NOWALL	0				//壁がないばあいの値
#define WALL	1				//壁がある場合の値
#define VWALL	3				//仮想壁の値(未使用)

#define STRAIGHT_MODE	0			//直進時のモード
#define TURN_MODE	1			//超信地旋回時のモード
#define SLA_MODE	2			//スラロームモード
#define NON_CON_MODE	3			//非制御モード
#define TEST_MODE	4			//テストモード(割り込み用モータ制御を切るモード)
#define F_WALL_MODE	5

#define MASK_SEARCH		0x01		//探索走行用マスク値.壁情報とこの値のAND値が０（NOWALL）なら壁なしor未探索区間
#define MASK_SECOND		0x03		//最短走行用マスク値.壁情報とこの値のAND値が０（NOWALL）なら壁なし