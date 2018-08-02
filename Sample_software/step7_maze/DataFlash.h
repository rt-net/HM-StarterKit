#define REF_SEN_R_ADD	0x00100000
#define REF_SEN_L_ADD	0x00100002
#define TH_SEN_R_ADD 	0x00100004
#define TH_SEN_L_ADD 	0x00100006
#define TH_SEN_FR_ADD	0x00100008
#define TH_SEN_FL_ADD	0x0010000a

#define MAP_ADD		  0x00100800

void map_write(void);
void hw_dflash_init(void);
void write_eeflash(void *addr, short data);
void erase(void *addr);
char blank_check(void *addr);
void map_copy(void);
void map_view(void);
