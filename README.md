# BanGeo.X


"CA\n" enable charge mode
"CB\n" disable charge mode
"CC\n" get device name
    Return : "RF4xxxx\n"
"CD*value*\n" set led, valid value: 0bxxxx00xx 
"CE*value*\n" set ADS1115 Mux, valid value: AIN0(0x40), AIN1(0x50), AIN2(0x60), AIN3(0x70) 
"CF\n" get battery temperature and voltage
    Return : "RF4*T_LSB**T_MSB**V_LSB**V_MSB*\n"
"CG\n" get 4 sensors value 
    Return : "RF8*S0_LSB**S0_MSB**S1_LSB**S1_MSB**S2_LSB**S2_MSB**S3_LSB**S3_MSB*\n"
"CH*value1*\n" get ADS value and write in eeprom, user must provide a valid tail address, Beware Overlap!!!!!
    Only used with calibration script
"CI*value*\n clear led, valid value: 0bxxxx00xx 
"CJ*value*\n command not implemented yet
"CK*value*\n" return ZERO, upper bound, lower bound calib user must provide a valid tail address 
"CL*value*\n" return upper bound calib on AIN0, AIN1, AIN2 or AIN3, valid value: AIN0(0x40), AIN1(0x50), AIN2(0x60), AIN3(0x70)  
    feature not implemented yet
"CM\n" Enable continuous conversion on ADS 
    feature not implemented yet
"CN\n" Disable continuous conversion on ADS   
    feature not implemented yet
