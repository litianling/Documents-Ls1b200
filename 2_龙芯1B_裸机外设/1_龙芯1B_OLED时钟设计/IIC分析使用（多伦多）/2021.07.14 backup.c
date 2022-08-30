int main() {
    unsigned int* ptr = 0xbfd010e0;
    unsigned int  init = 0x7ffffffc;
    unsigned int* rtc_ptr = 0xbfe64020;
    unsigned int  sys_toytrim = *rtc_ptr;
    unsigned int* sys_toyread0_ptr = 0xbfe6402c;
    unsigned int  sys_toyread0 = *sys_toyread0_ptr;

    
    

    
    unsigned int tens = 0;
    unsigned int sec = 0;
    unsigned int min = 0;
    unsigned int hr = 0;
    unsigned int day = 0;
    unsigned int mon = 0;
    unsigned int year = 0;

    unsigned int* RTC_Y_W = 0xbfE64028;
    *RTC_Y_W = 0;
    unsigned int* RTC_O_W = 0xbfE64024;
    *RTC_O_W = 0;

    unsigned int* RTC_reg0_ptr = 0xbfe6402c;
    unsigned int* RTC_reg1_ptr = 0xbfe64030;
    unsigned int RTC_reg0 = 0;
    unsigned int RTC_reg1 = 0;
    
    //main loop
    for(;;){
        
        RTC_reg0 = *RTC_reg0_ptr;
        RTC_reg1 = *RTC_reg1_ptr;

        tens = RTC_reg0 % 0x10;

        sec = RTC_reg0 % 0x400;
        sec = sec/ 0x10;

        min = RTC_reg0 % 0x10000;
        min = min / 0x400;

        hr = RTC_reg0 % 0x200000;
        hr = RTC_reg0 / 0x10000;

        day = RTC_reg0 % 0x4000000;
        day = day / 0x200000;

        mon = RTC_reg0 / 0x40000000;

        year = RTC_reg1;
        year = year% 10000;
        //only 4 digit for the year

        //till here we have all the variables for the OLED

        //now convert the numbers to the graph
        int info_array[15];

        info_array[0] = year / 1000;
        year -= info_array[0]*1000;

        info_array[1] = year / 100;
        year -= info_array[1]*100;

        info_array[2] = year / 10;
        year -= info_array[2] * 10;

        info_array[3] = year/1;

        info_array[4] = mon /10;
        mon -= info_array[4]*10;
        info_array[5] = mon /1;

        info_array[6] = day /10;
        day -= info_array[6] * 10;
        info_array[7] = day / 1;

        info_array[8] = hr / 10;
        hr -= info_array[8]*10;
        info_array[9] = hr / 1;

        info_array[10] = min /10;
        min -= info_array[10] * 10;
        info_array[11] = min / 1;

        info_array[12] = sec / 10;
        sec -= info_array[12] * 10;
        info_array[13] = sec / 1;

        info_array[14] = tens / 1;

        unsigned char light_array[15][16];

        int i = 0;
        for(i = 0; i<15; i++){

            int j = 0;
            for(j = 0; j < 16; j++){
                light_array[i][j] = num_to_char[info_array[i]][j];
            }
        }

        //so now we have the value about the oled can display
        //now send them to the OLED
        unsigned int Addr = 0x3d;
        int rw = 0;
        
        int inti_stu = ls1x_i2c_initialize(busI2C0);
        int start_st = ls1x_i2c_send_start(busI2C0, Addr);
            
        int send_st = ls1x_i2c_send_addr(busI2C0, Addr, rw);
        int br = 0;
        int x= 0;
        for(x = 0; x < 2; x++){
            int len = 16;
            ls1x_i2c_write_bytes(busI2C0, light_array[x],len);
        }
        int end_st = ls1x_i2c_send_stop(busI2C0, Addr);


    }
    return 0;
}