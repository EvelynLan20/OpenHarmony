#include <stdio.h>
#include <stdlib.h>    //goto函数
#include <stdint.h>    //unint8_t
#include <unistd.h>    //sleep()  //s
//#include "hdf_log.h"    //路径：    "//drivers/hdf_core/interfaces/inner_api/utils",
#include "uart_if.h"    //路径：    "//drivers/hdf_core/framework/include/platform",
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "osal_mem.h"
#include "light_if.h"
#include "light_type.h"
#include "stdfunc.h"

/*
    传感器温度读取函数
    返回类型：float
*/
float TemperatureRead(uint8_t rbuff[]) {
    
    printf("UartRead data\n");
    int tmp = 0;
    float temp = 0.0;
    ret = UartRead(handle, rbuff, 1000); //读数据
    if(ret < 0) {
        printf("read fail ret: %d\r\n", ret);
        goto ERR;
    }
    if(ret == 1 || ret == 0) {
        continue;  //没读到数据就跳出循环再读
    }
    if(ret == 6) {
        tmp = rbuff[3] << 8 | rbuff[4];

        unsigned short integerPart = (rbuff[3] << 8) | rbuff[4];
        temp = (float)integerPart / 10.0f;
        printf("temperature: %.1f\n", temp);
    }
    printf("\n");
    usleep(100*1000);
    return temp;
ERR:
    UartClose(handle);
    return ret;
}

