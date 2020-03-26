#include "ssd1366.h"
#include "font.h"

/* @arg1 uint8_t address: pass the address where to write the data
 * @arg2 uint8_t data_h : pass the data to write
 * @arg3 int Nodata     : pass the number of byte to be write
*/
void i2c_write(uint8_t address, char* data_h ,int Nodata)
{

	int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, OLED_I2C_ADDRESS << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, address, ACK_CHECK_EN);
	
	for(int i =0;i<Nodata;i++)
	i2c_master_write_byte(cmd, data_h[i], ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
  //  if (ret != ESP_OK) {
     //   return ret;
   // }

}

/*
 * @arg1 uint8_t address: pass the address1 where to write the data
 * @arg2 uint8_t data_h : pass the data 1
 * @arg3 uint8_t address2: pass the address2 where to write the data
 * @arg2 uint8_t data_h2 : pass the data 2
 */
void i2c_write_multi(uint8_t address, uint8_t data_h ,uint8_t address2, uint8_t *data_h2 )
{
int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, OLED_I2C_ADDRESS << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, address, ACK_CHECK_EN);
	
	
	i2c_master_write_byte(cmd, data_h, ACK_CHECK_EN);
	 i2c_master_write_byte(cmd, address2, ACK_CHECK_EN);
	
	i2c_master_write(cmd, data_h2, 128, true);

    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);


}


/**
 * @brief i2c master initialization
 */
void i2c_example_master_init()
{
    int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                       I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
}
/*
 * oled initialization
 */
void oled_init(void)
{

char data[4];
data[0] = OLED_CONTROL_BYTE_CMD_STREAM;
data[1] =	OLED_CMD_SET_CHARGE_PUMP;
data[2] =	OLED_CMD_SET_COM_SCAN_MODE;
data[3] =	OLED_CMD_DISPLAY_ON;

i2c_write(OLED_CONTROL_BYTE_CMD_STREAM,data, 4);

}

/*
 * @arg1 char: pointer to hold the address of string
 * return	 : void
 */
void display(char *text) 
{
	uint8_t text_len = strlen(text);
	i2c_cmd_handle_t cmd;
	uint8_t cur_page = 0;
	//i2c_write(OLED_CONTROL_BYTE_CMD_STREAM,data,3);
	for(int i = 0 ;i<text_len;i++)
	
	i2c_write(OLED_CONTROL_BYTE_DATA_STREAM,font8x8_basic_tr[(uint8_t)text[i]],8);
	
}

/*
 * @arg1 row: accept the row value for OLED
 * @arg2 col: accept the column value for OLED
 * @arg3 h:   height of the pixel
 * @arg4 w:   width of the pixel
 * @arg5char : pointer to hold the address of buffer
 * return	 : void
 */

void display_pixel(int row,int col, int h,int w,char buffer[])
{
	char data[3];
	int i=0,j=0;
	char page[1];

	data[0] = 0x00;
	data[1] = 0x10 + col;
	data[2] = 0xb0 + row;
	i2c_write(OLED_CONTROL_BYTE_CMD_STREAM,data,3);


	for(i=0;i<h/8;i++)
	{
		for(j=0;j<w;j++)
		{
			i2c_write(OLED_CONTROL_BYTE_DATA_STREAM,buffer++,1);


		}
		data[2] = data[2]+ 0x01;
		i2c_write(OLED_CONTROL_BYTE_CMD_STREAM,data,3);

	}

}
