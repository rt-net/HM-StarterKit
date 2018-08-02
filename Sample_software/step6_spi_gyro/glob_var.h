//グローバル変数を一箇所で宣言する。
//glob_var.cで実体を作り(_GLOB_VARをdefine)、他のグローバル変数を使用するファイルからはglob_var.hをincludeする。

//global変数を一括で実体とextern宣言する。
//include元で_GLOB_VARが定義されていれば実体宣言、定義されていなければ外部宣言となる。
#ifdef _GLOB_VAR
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL int gyro_data	//ジャイロ用のデータ

