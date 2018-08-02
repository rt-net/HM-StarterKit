
extern volatile unsigned int timer;


void wait_ms(int wtime)		//mS’PˆÊ‚Å‘Ò‚¿ŠÔ‚ğ¶¬‚·‚é
{
	unsigned int start_time;
	
	start_time = timer;
	
	while( (timer - start_time) < wtime)	;

}