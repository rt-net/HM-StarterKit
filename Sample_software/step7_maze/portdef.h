#ifdef PORTDEF
	
#else

#define PORTDEF

/*
#define BUZZER	(PORTB.PODR.BIT.B3)

#define LED0	(PORTB.PODR.BIT.B0)
#define LED1	(PORTA.PODR.BIT.B6)
#define LED2	(PORTA.PODR.BIT.B4)
#define LED3	(PORTA.PODR.BIT.B0)
*/

#define SW_C	((push_switch & 0x20) >> 5)
#define SW_U	((push_switch & 0x40) >> 6)
#define SW_D	((push_switch & 0x10) >> 4)

#define MOT_OUT_R	(MTU0.TGRA)
#define MOT_OUT_L	(MTU0.TGRC)

#define SLED_L	(PORT1.PODR.BIT.B5)		//左センサLED
#define SLED_R	(PORT1.PODR.BIT.B4)		//右センサLED
#define SLED_FL	(PORT3.PODR.BIT.B1)		//左前センサLED
#define SLED_FR	(PORTA.PODR.BIT.B3)		//右前センサLED

#define MOT_POWER	(PORTC.PODR.BIT.B6)
#define MOT_POWER_ON	MOT_POWER = 1		// モータの出力を許可
#define MOT_POWER_OFF	MOT_POWER = 0		// モータの出力を不許可

#define MOT_CWCCW_R	(PORTC.PODR.BIT.B5)
#define MOT_CWCCW_L	(PORTC.PODR.BIT.B4)

#endif
