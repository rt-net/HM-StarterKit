#include "iodefine.h"
#include "interrupt.h"
#include "glob_var.h"
#include "parameters.h"
#include "spi.h"
#include "i2c.h"
#include "run.h"
#include "mytypedef.h"
#include "portdef.h"
#include "interface.h"
#include "machine.h"


void int_cmt0(void)
{
	/*****************************************************************************************
	目標速度生成
		tar_speedの生成
		など
	*****************************************************************************************/
	//直線の場合の目標速度生成
	if(run_mode == STRAIGHT_MODE){
		tar_speed += accel/1000.0;	//目標速度を設定加速度で更新
		//最高速度制限
		if(tar_speed > max_speed){
			tar_speed = max_speed;	//目標速度を設定最高速度に設定
		}
				
	
	}else if(run_mode == TURN_MODE){
		
		//車体中心速度更新
		tar_speed += accel/1000;
		//最高速度制限
		if(tar_speed > max_speed){
			tar_speed = max_speed;	//目標速度を設定最高速度に設定
		}
		
		//角加速度更新
		tar_ang_vel += ang_acc/1000.0;	//目標角速度を設定加速度で更新
		tar_degree  += (tar_ang_vel*180.0/PI)/1000.0;

		
		//左回転の場合
		if(TURN_DIR == LEFT){
			//最高角速度制限
			if(tar_ang_vel > max_ang_vel){
				tar_ang_vel = max_ang_vel;	//目標速度を設定最高速度に設定
			}
			if(tar_degree > max_degree){
				tar_degree = max_degree;
			}
		}else if(TURN_DIR == RIGHT){
		//右回転の場合
			//最高角速度制限
			if(tar_ang_vel < max_ang_vel){
				tar_ang_vel = max_ang_vel;	//目標速度を設定最高速度に設定
			}
			if(tar_degree < max_degree){
				tar_degree = max_degree;
			}
		}
		
	}else if(run_mode == NON_CON_MODE){
		//何もしない
		nop();	
	}else{
		//何もしない
		nop();	
	}
	
	/*****************************************************************************************
	壁制御
		横壁センサによる目標角度生成
	*****************************************************************************************/
	if(run_mode == STRAIGHT_MODE){
		if(con_wall.enable == true && sen_fr.value + sen_fl.value <= (TH_SEN_FR+TH_SEN_FL)*5 )		//壁制御が許可されているかチェック
		{
			
			con_wall.p_error = con_wall.error;	//過去の偏差を保存
			
			
			//左右のセンサが、それぞれ使える状態であるかどうかチェックして、姿勢制御の偏差を計算
			if( ( sen_r.is_control == true ) && ( sen_l.is_control == true ) )
			{									//両方とも有効だった場合の偏差を計算
				con_wall.error = sen_r.error - sen_l.error;
			}
			else								//片方もしくは両方のセンサが無効だった場合の偏差を計算
			{
				con_wall.error = 2.0 * (sen_r.error - sen_l.error);	//片方しか使用しないので2倍する
			}
			
			
			//DI制御計算
			con_wall.diff = con_wall.error - con_wall.p_error;	//偏差の微分値を計算
			con_wall.sum += con_wall.error;				//偏差の積分値を計算
			
			if(con_wall.sum > con_wall.sum_max)			//偏差の積分値の最大値を制限
			{
				con_wall.sum = con_wall.sum_max;
			}
			else if(con_wall.sum < (-con_wall.sum_max))		//偏差の積分値の最低値を制限
			{
				con_wall.sum = -con_wall.sum_max;
			}

			con_wall.p_omega = con_wall.omega;
			con_wall.omega = con_wall.kp * con_wall.error * 0.5 + con_wall.p_omega * 0.5;	//現在の目標角速度[rad/s]を計算
			tar_ang_vel = con_wall.omega;
		}else{
			tar_ang_vel = 0;
		}
		
	}else if(run_mode == NON_CON_MODE){	
			//何もしない
			nop();
	}else{
		nop();	
	}
	
	/*****************************************************************************************
	壁制御かスラロームの理想値によって生成された
	目標速度と目標角角度の積分
	*****************************************************************************************/
	I_tar_speed += tar_speed;
	if(I_tar_speed >30*10000000000){
		I_tar_speed = 30*10000000000;
	}else if(I_tar_speed < -1*10000000000){
		I_tar_speed = 1*10000000000;
	}
	
	I_tar_ang_vel += tar_ang_vel;
	if(I_tar_ang_vel >30*10000000000){
		I_tar_ang_vel = 30*10000000000;
	}else if(I_tar_ang_vel < -1*10000000000){
		I_tar_ang_vel = 1*10000000000;
	}
	/*****************************************************************************************
	目標速度の偏差から出力電圧にフィードバック
		
	*****************************************************************************************/
	//フィードバック制御
	V_r = V_l = 0.0;
	if(run_mode == STRAIGHT_MODE || run_mode == TURN_MODE){
	//直進時のフィードバック制御
		//左右モータのフィードバック
		//速度に対するP制御
		V_r += 1 * (tar_speed - speed) *SPEED_KP/1.0; //15目標値付近で発振
		V_l += 1 * (tar_speed - speed) *SPEED_KP/1.0;
		//速度に対するI制御
		V_r += 1 * (I_tar_speed - I_speed) *SPEED_KI/1.0; //(0.4-0.3)*0.1 -> 0.01 
		V_l += 1 * (I_tar_speed - I_speed) *SPEED_KI/1.0;
		//速度に対するD制御
		V_r -= 1 * (p_speed - speed) *SPEED_KD/1.0; //(0.4-0.3)*0.1 -> 0.01 
		V_l -= 1 * (p_speed - speed) *SPEED_KD/1.0;

		//角速度に対するP制御
		V_r += 1 * (tar_ang_vel - ang_vel) *(OMEGA_KP/100.0);
		V_l -= 1 * (tar_ang_vel - ang_vel) *(OMEGA_KP/100.0);
		//角速度に対するI制御

		V_r += 1 * (I_tar_ang_vel - I_ang_vel) *(OMEGA_KI/100.0); //(0.4-0.3)*0.1 -> 0.01 
		V_l -= 1 * (I_tar_ang_vel - I_ang_vel) *(OMEGA_KI/100.0);
		//角速度に対するD制御

		V_r += 1 * (p_ang_vel - ang_vel) *(OMEGA_KD/100.0); //(0.4-0.3)*0.1 -> 0.01 
		V_l -= 1 * (p_ang_vel - ang_vel) *(OMEGA_KD/100.0);

	}else if(run_mode == NON_CON_MODE){
		//何もしない
		nop();
	}else{
		//何もしない
		nop();	
	}
	
	/*****************************************************************************************
	出力電圧からモータの回転方向を決定
		電圧は絶対値を入力するため、
		正の値の時はモータを正転設定にして、電圧はそのまま
		負の値の時はモータを逆転設定にして、電圧を反転する
	*****************************************************************************************/
	if(run_mode != TEST_MODE){
		//右モータの出力電圧が正の場合
		if(V_r > 0){
			//モータを正転に設定
			MOT_CWCCW_R = MOT_R_FORWARD;	//右モータを正転に設定
			//V_r = V_r;			//電圧は正なのでそのまま
		}else{
		//右モータの出力電圧が負の場合
			MOT_CWCCW_R  = MOT_R_BACK;	//右モータを逆回転に設定
			V_r = -V_r;			//電圧を正の値へ反転
		}
		
		//左モータの出力電圧が正の場合
		if(V_l > 0){
			//モータを正転に設定
			MOT_CWCCW_L = MOT_L_FORWARD;	//左モータを正転に設定
			//V_l = V_l;			//電圧は正なのでそのまま
		}else{
		//左モータの出力電圧が負の場合
			MOT_CWCCW_L  = MOT_L_BACK;	//左モータを逆回転に設定
			V_l = -V_l;			//電圧を正の値へ反転
		}
	}	
	/*****************************************************************************************
	出力電圧の制限
		モータへの出力電圧の上限を2Vに制限
	*****************************************************************************************/
	//右モータに印加する電圧が2.0Vを超えたら強制的に2.0Vに変更
	if(V_r > 2.0){
		V_r = 2.0;
	}
	//左モータに印加する電圧が2.0Vを超えたら強制的に2.0Vに変更
	if(V_l > 2.0){
		V_l = 2.0;
	}
	
	/*****************************************************************************************
	モータへ出力
		
	*****************************************************************************************/
	
	//バッテリー電圧からデューティを計算
	Duty_r = V_r/V_bat;
	Duty_l = V_l/V_bat;
	
	//モータにPWMを出力
	if(run_mode != TEST_MODE){
		MOT_OUT_R =(short)(240.0 * Duty_r);
		MOT_OUT_L =(short)(240.0 * Duty_l);
	}
	
	/*****************************************************************************************
	タイマのカウント
		
	*****************************************************************************************/
	timer++;
	cnt++;
	
}

void int_cmt1(void)		//センサ読み込み用り込み
{
	/*****************************************************************************************
	A/D変換
		センサとバッテリー電圧取得
	*****************************************************************************************/
	static int state = 0;	//読み込むセンサのローテーション管理用変数
	int i;
	//long log_cnt;
	
	switch(state)
	{
		case 0:		//右センサ読み込み

			//差分フィルタ
			S12AD.ADANS0.BIT.ANS0=0x0004;			//AN002
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			sen_r.d_value = S12AD.ADDR2;			//環境値を保存
			
			SLED_R = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++);		//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0004;			//AN002
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			SLED_R = 0;					//LED消灯
			
			sen_r.value = (S12AD.ADDR2 - sen_r.d_value);	//値を保存

			if(sen_r.value > sen_r.th_wall)			//壁の有無を判断
			{
				sen_r.is_wall = true;			//右壁あり
			}
			else
			{
				sen_r.is_wall = false;			//右壁なし
			}
			
			if(sen_r.value > sen_r.th_control)		//制御をかけるか否かを判断
			{
				sen_r.error = sen_r.value - sen_r.ref;	//制御をかける場合は偏差を計算
				sen_r.is_control = true;		//右センサを制御に使う
			}
			else
			{
				sen_r.error = 0;			//制御に使わない場合は偏差を0にしておく
				sen_r.is_control = false;		//右センサを制御に使わない
			}			
			break;


		case 1:		//前左センサ読み込み

			//差分フィルタ
			S12AD.ADANS0.BIT.ANS0=0x0001;			//AN000
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			sen_fl.d_value = S12AD.ADDR0;			//値を保存
		
			SLED_FL = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++);		//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0001;			//AN000
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			SLED_FL = 0;					//LED消灯

			sen_fl.value = (S12AD.ADDR0 - sen_fl.d_value);	//値を保存

			if(sen_fl.value > sen_fl.th_wall)		//壁の有無を判断
			{
				sen_fl.is_wall = true;			//左前壁あり
			}
			else
			{
				sen_fl.is_wall = false;			//左前壁なし
			}
			break;


		case 2:		//前右センサ読み込み
		
			//差分フィルタ
			S12AD.ADANS0.BIT.ANS0=0x0040;			//AN006
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			sen_fr.d_value = S12AD.ADDR6;			//値を保存
		
			SLED_FR = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++);		//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0040;			//AN006
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			SLED_FR = 0;					//LED消灯
			
			sen_fr.value = (S12AD.ADDR6 - sen_fr.d_value);	//値を保存

			if(sen_fr.value > sen_fr.th_wall)		//壁の有無を判断
			{
				sen_fr.is_wall = true;			//右前壁あり
			}
			else
			{
				sen_fr.is_wall = false;			//右前壁なし
			}			
			break;


		case 3:		//左センサ読み込み
		
			//差分フィルタ
			S12AD.ADANS0.BIT.ANS0=0x0002;			//AN001
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			sen_l.d_value = S12AD.ADDR1;			//値を保存
			
			SLED_L = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0002;			//AN001
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			SLED_L = 0;					//LED消灯
			
			sen_l.value = (S12AD.ADDR1 - sen_l.d_value);	//値を保存
			
			if(sen_l.value > sen_l.th_wall)			//壁の有無を判断
			{
				sen_l.is_wall = true;			//左壁あり
			}
			else
			{
				sen_l.is_wall = false;			//左壁なし
			}
			
			if(sen_l.value > sen_l.th_control)		//制御をかけるか否かを判断
			{
				sen_l.error = sen_l.value - sen_l.ref;	//制御をかける場合は偏差を計算する
				sen_l.is_control = true;		//左センサを制御に使う
			}
			else
			{
				sen_l.error = 0;			//制御に使わない場合は偏差を0にしておく
				sen_l.is_control = false;		//左センサを制御に使わない
			}

			break;
	}
	
	state++;		//4回ごとに繰り返す
	if(state > 3)
	{
		state = 0;
	}
	
	S12AD.ADANS0.BIT.ANS0=0x0200;			//AN009
	S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
	while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
	V_bat = (2.0*3.3*(float)(S12AD.ADDR9/4095.0) );
	if(V_bat < 3.5){
		
		//モータ止める
		Duty_r = 0;
		Duty_l = 0;
		MOT_POWER_OFF;	//PC6(SLEEPピン)
		
		//ブザー鳴らし続ける
		while(1){
			BEEP();
		}
	}
	/*****************************************************************************************
	1kHzごとにログを取得
		
	*****************************************************************************************/
	
	if(log_timer % 4 == 0 && log_flag==1 ){
		if(log_timer < (LOG_CNT*4)){

			log[0][log_timer/4] = (int)(len_mouse);
			log[1][log_timer/4] = (int)(1000*tar_speed);
			log[2][log_timer/4] = (int)(1000*speed);
			log[3][log_timer/4] = (int)(100*Duty_r);
			log[4][log_timer/4] = (int)(100*Duty_l);
			log[5][log_timer/4] = (int)(1000*V_bat);
			log[6][log_timer/4] = (int)(tar_degree*10);
			log[7][log_timer/4] = (int)(degree*10);
			log[8][log_timer/4] = (int)(tar_ang_vel*1000);
			log[9][log_timer/4] = (int)(ang_vel*1000);
			log[10][log_timer/4] = (int)(I_tar_ang_vel);
			log[11][log_timer/4] = (int)(ang_acc*1000);
		}
	}	
	
	log_timer++;
		
}

void int_cmt2(void)
{
	static unsigned int	enc_data_r;	//エンコーダの生データ
	static unsigned int	enc_data_l;	//エンコーダの生データ 
	static short	state;
	/*****************************************************************************************
	エンコーダ関連
		値の取得　速度更新　距離積分など
	*****************************************************************************************/	
	if(state == 0){
		RSPI0.SPCMD0.BIT.SSLA = 	0x00;	//SSL信号アサート設定(SSL0を使う)
		preprocess_spi_enc(0xFFFF);	//Read Angle
		enc_data_r = Get_enc_data();
		state = 1;
	}else{
		RSPI0.SPCMD0.BIT.SSLA = 	0x02;	//SSL信号アサート設定(SSL2を使う)
		preprocess_spi_enc(0xFFFF);	//Read Angle
		enc_data_l = Get_enc_data();
		
		//左右エンコーダから角度取得
		//4096で一回転(360deg = 0deg)
		locate_r = enc_data_r;
		locate_l = enc_data_l;
		
		//右エンコーダの現在の位置と,1msec前の位置との差分を計算
		//単位時間（1msec）あたりの変位量を計算
		diff_pulse_r = (locate_r - before_locate_r);
		//変化点を1023から0//へ移動したときの補正
		if((diff_pulse_r > ENC_RES_HALF || diff_pulse_r < -ENC_RES_HALF) && before_locate_r >ENC_RES_HALF){
			diff_pulse_r = (((ENC_RES_MAX - 1) - before_locate_r) + locate_r);
		}
		//変化点を0から1023へ移動したときの補正
		else if((diff_pulse_r > ENC_RES_HALF || diff_pulse_r < -ENC_RES_HALF) && before_locate_r <=ENC_RES_HALF){
			diff_pulse_r = 1*(before_locate_r + ((ENC_RES_MAX - 1) - locate_r));
		}
		
		//左エンコーダの現在の位置と,1msec前の位置との差分を計算
		//単位時間（1msec）あたりの変位量を計算
		diff_pulse_l = (-locate_l + before_locate_l);
		//変化点を1023から0//へ移動したときの補正
		if((diff_pulse_l > ENC_RES_HALF || diff_pulse_l < -ENC_RES_HALF) && before_locate_l >ENC_RES_HALF){
			diff_pulse_l = 1*(((ENC_RES_MAX - 1) - before_locate_l) + locate_l);
		}
		//変化点を0から1023へ移動したときの補正
		else if((diff_pulse_l > ENC_RES_HALF || diff_pulse_l < -ENC_RES_HALF) && before_locate_l <=ENC_RES_HALF){
			diff_pulse_l = (before_locate_l + ((ENC_RES_MAX - 1) - locate_l));
		}
				
		//現在速度を算出
		speed_new_r = (float)((float)diff_pulse_r * (float)MMPP);
		speed_new_l = (float)((float)diff_pulse_l * (float)MMPP);
		
		//過去の値を保存
		speed_old_r= speed_r;
		speed_old_l= speed_l;
		
		//速度のローパスフィルタ
		speed_r = speed_new_r * 0.1 + speed_old_r * 0.9;
		speed_l = speed_new_l * 0.1 + speed_old_l * 0.9;
		
		p_speed = speed;
		//車体速度を計算
		speed = ((speed_r + speed_l)/2.0);
		
		//I成分のオーバーフローとアンダーフロー対策
		I_speed += speed;
		if(I_speed >30*10000000000){
			I_speed = 30*10000000000;
		}else if(I_speed < -1*10000000000){
			I_speed = -1*10000000000;
		}


		
		//距離の計算
		len_mouse += (speed_new_r + speed_new_l)/2.0;
		
		//過去の値を保存
		before_locate_r = locate_r;
		before_locate_l = locate_l;
				
		state = 0;
	}
	
	
	/*****************************************************************************************
	ジャイロ関連(ヨー軸)
		値の取得　角度の積分　
	*****************************************************************************************/
	if(state == 1){
		//ジャイロセンサの値の更新
		preprocess_spi_gyro(0xB70000);
		
		//LowPass Filter
		gyro_x_new = (float)((short)(read_gyro_data() & 0x0000FFFF));
		gyro_x = (gyro_x_new -gyro_ref );
		
		//角速度の更新
		p_ang_vel = ang_vel;
		ang_vel = ((2000.0*(gyro_x)/32767.0))*PI/180.0;
		//積分値の更新
		I_ang_vel += ang_vel;
		if(I_ang_vel >30*10000000000){
			I_ang_vel = 30*10000000000;
		}else if(I_ang_vel < -1*10000000000){
			I_ang_vel = -1*10000000000;
		}
		
		//ジャイロの値を角度に変換
		degree += (2.0*(gyro_x_new - gyro_ref)/32767.0);
		
	}	
	
}

