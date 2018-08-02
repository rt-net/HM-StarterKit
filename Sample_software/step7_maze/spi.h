void init_spi_gyro(void);
void preprocess_spi_gyro(int address);
void preprocess_spi_gyro_2byte(int address);
int ovre_check(void);
int gyro_read_check(void);
int gyro_write_check(void);
int read_gyro_data(void);


//For encoder
void init_spi_enc(void);
void preprocess_spi_enc(int address);
int gyro_read_check(void);
int gyro_write_check(void);
int read_gyro_data(void);
int enc_write_cnt_check(void);
int Get_enc_data(void);