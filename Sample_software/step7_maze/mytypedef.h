
#ifdef _MYTYPEDEF

#else

typedef enum
{
	false = 0,	//偽
	true = 1,	//真
}t_bool;		//真偽値を取り扱う列挙型


typedef struct
{
	short value;		//現在の値
	short d_value;		//差分フィルタ用
	short p_value;		//１mS過去の値
	short p2_value;
	short error;		//value - ref
	short ref;		//リファレンス値
	short th_wall;		//壁があるか否かの閾値
	short th_control;	//制御をかけるか否かの閾値
	t_bool is_wall;		//壁があるか無いか ( true = 壁あり false = 壁なし )
	t_bool is_control;	//制御に使うか否か
}t_sensor;			//センサ構造体

typedef struct
{
	float control;		//最終的な制御量
	float omega;		//目標角速度
	float p_omega;
	float theta;		//目標角度
	float p_theta;		//過去の目標角度
	float error;		//偏差
	float p_error;		//過去の偏差
	float diff;		//偏差の微分値
	float sum;		//偏差の積分値
	float sum_max;		//積分値の制限値
	float kp;		//比例制御定数
	float kd;		//微分制御定数
	float ki;		//積分制御定数
	t_bool enable;		//制御のon/off
}t_control;			//制御構造体

typedef struct
{
	
	t_bool enable;
}t_testmode;

typedef enum
{
	front=0,		//前
	right=1,		//右
	rear=2,			//後
	left=3,			//左
	unknown,		//方向不明
}t_local_dir;	//自分から見た方向を示す列挙型

typedef enum
{
	north=0,
	east=1,
	south=2,
	west=3,
}t_direction;

typedef struct
{
	short x;
	short y;
	t_direction dir;
}t_position;

typedef struct
{
	unsigned char north:2;	//北の壁情報
	unsigned char east:2;	//東の壁情報
	unsigned char south:2;	//南の壁情報
	unsigned char west:2;	//西の壁情報
}t_wall;			//壁情報を格納する構造体(ビットフィールド)


#define _MYTYPEDEF

#endif
