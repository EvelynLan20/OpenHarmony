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

// int main()
// {
//     printf("uart_test begin\r\n");
//     printf("---------------------------\n");
//     int32_t ret;
//     uint32_t port = 3;
//     uint32_t baud;
//     DevHandle handle = NULL;
//     uint8_t rbuff[1000] = { 0 };

//     struct UartAttribute attribute;
//     attribute.dataBits = UART_ATTR_DATABIT_8;                  // UART传输数据位宽，一次传输7个bit
//     attribute.parity = UART_ATTR_PARITY_NONE;                  // UART传输数据无校检
//     attribute.stopBits = UART_ATTR_STOPBIT_1;                  // UART传输数据停止位为1位
//     attribute.rts = UART_ATTR_RTS_DIS;                         // UART禁用RTS
//     attribute.cts = UART_ATTR_CTS_DIS;                         // UART禁用CTS
//     attribute.fifoRxEn = UART_ATTR_RX_FIFO_EN;                 // UART使能RX FIFO
//     attribute.fifoTxEn = UART_ATTR_TX_FIFO_EN;   

//     handle = UartOpen(port);                                   // 获取UART设备句柄
//     if (handle == NULL) {
//         printf("UartOpen(port) failed \r\n");
//         return HDF_FAILURE;
//     }
 
//     ret = UartSetBaud(handle, 9600);                           // 设置UART波特率为9600
//     if (ret != 0) {
//         printf("UartSetBaud: set baud failed, ret %d\n", ret);
//         goto ERR;
//     }
//     printf("UartOpen successful and uart port = %d\n",port);
 
//     ret = UartGetBaud(handle, &baud);                          // 获取UART波特率
//     if (ret != 0) {
//         printf("UartGetBaud: get baud failed, ret %d\n", ret);
//         goto ERR;
//     }
//     printf("UartSetBaud successful and uart baudrate = %d\n", baud);
 
//     ret = UartSetAttribute(handle, &attribute);                // 设置UART设备属性
//     if (ret != 0) {
//         printf("UartSetAttribute: set attribute failed, ret %d\n", ret);
//         goto ERR;
//     }
//     printf("UartSetAttribute successful\n");

//     ret = UartSetTransMode(handle, UART_MODE_RD_NONBLOCK);     // 设置UART传输模式为非阻塞模式
//     if (ret != 0) {
//         printf("UartSetTransMode: set trans mode failed, ret %d\n", ret);
//         goto ERR;
//     }
//     printf("UartSetTransMode successful\n");

//     printf("UartRead data!!!\n");
//     // int j=1;
//      while(1){
//         int tmp = 0;
//         float temp = 0.0;
//         ret=UartRead(handle, rbuff, 1000);                      // 从UART设备读取数据
//         if(ret<0){
//             printf("read fail ret:%d\r\n",ret);
//             goto ERR;
//         }
//         if(ret==-1||ret==0){
//             continue;    //如果没读到，那就跳出循环再读
//         }

//         if(ret == 6)
//         {
//             tmp = rbuff[3] << 8 | rbuff[4];
//             // printf("temperature:%d\n",tmp);

//             unsigned short integerPart = (rbuff[3]<<8) | rbuff[4];
//             temp = (float)integerPart / 10.0f;
//             printf("temperature:%.1f\n",temp);

//         }
//         printf("\n");
//         usleep(100*1000); //sleep 10ms
//     }
 
//     printf(" function tests end\r\n");
// ERR:
//     UartClose(handle);                                         // 销毁UART设备句柄
//     return ret;
 
//     printf("---------------------------\n");
// }




// int main(){
//     printf("uart_test begin\r\n");
 
//     printf("---------------------------\n");
 
//     int32_t ret;
//     uint32_t port = 3;
//     uint32_t baud;
//     DevHandle handle = NULL;
//     // uint8_t wbuff[5] = { 1, 2, 3, 4, 5 };
//     uint8_t rbuff[1000] = { 0 };
    
//     const struct LightInterface *g_lightDev = NULL;
//     struct LightInfo *g_lightInfo = NULL;
//     uint32_t g_count = 0;

//     struct UartAttribute attribute;
//     attribute.dataBits = UART_ATTR_DATABIT_8;                  // UART传输数据位宽，一次传输7个bit
//     attribute.parity = UART_ATTR_PARITY_NONE;                  // UART传输数据无校检
//     attribute.stopBits = UART_ATTR_STOPBIT_1;                  // UART传输数据停止位为1位
//     attribute.rts = UART_ATTR_RTS_DIS;                         // UART禁用RTS
//     attribute.cts = UART_ATTR_CTS_DIS;                         // UART禁用CTS
//     attribute.fifoRxEn = UART_ATTR_RX_FIFO_EN;                 // UART使能RX FIFO
//     attribute.fifoTxEn = UART_ATTR_TX_FIFO_EN;                 // UART使能TX FIFO

//     struct LightEffect effect;
//     effect.flashEffect.flashMode = 0;
//     effect.flashEffect.onTime = 0;
//     effect.flashEffect.offTime = 0;
//     effect.lightColor.colorValue.rgbColor.r = 255;
//     effect.lightColor.colorValue.rgbColor.g = 0;
//     effect.lightColor.colorValue.rgbColor.b = 0;

//     g_lightDev = NewLightInterfaceInstance();               //初始化灯光设备接口实例
//     if (g_lightDev == NULL) {
//         printf("test light get Module instance fail\n\r");
//     }
//     ret = g_lightDev->GetLightInfo(&g_lightInfo, &g_count);     //获取灯光设备信息
//     if (ret == -1) {
//         printf("get light informations fail\n\r");
//     }


//     handle = UartOpen(port);                                   // 获取UART设备句柄
//     if (handle == NULL) {
//         printf("UartOpen(port) failed \r\n");
//         return HDF_FAILURE;
//     }
 
//     ret = UartSetBaud(handle, 9600);                           // 设置UART波特率为9600
//     if (ret != 0) {
//         printf("UartSetBaud: set baud failed, ret %d\n", ret);
//         goto ERR;
//     }
//     printf("UartOpen successful and uart port = %d\n",port);
 
//     ret = UartGetBaud(handle, &baud);                          // 获取UART波特率
//     if (ret != 0) {
//         printf("UartGetBaud: get baud failed, ret %d\n", ret);
//         goto ERR;
//     }
//     printf("UartSetBaud successful and uart baudrate = %d\n", baud);
 
//     ret = UartSetAttribute(handle, &attribute);                // 设置UART设备属性
//     if (ret != 0) {
//         printf("UartSetAttribute: set attribute failed, ret %d\n", ret);
//         goto ERR;
//     }
//     printf("UartSetAttribute successful\n");
 
//     // ret = UartGetAttribute(handle, &attribute);                // 获取UART设备属性
//     // if (ret != 0) {
//     //     printf("UartGetAttribute: get attribute failed, ret %d\n", ret);
//     //     goto ERR;
//     // }
 
//     ret = UartSetTransMode(handle, UART_MODE_RD_NONBLOCK);     // 设置UART传输模式为非阻塞模式
//     if (ret != 0) {
//         printf("UartSetTransMode: set trans mode failed, ret %d\n", ret);
//         goto ERR;
//     }
//     printf("UartSetTransMode successful\n");
 
//     // ret = UartWrite(handle, wbuff, 5);                         // 向UART设备写入5字节的数据
//     // if (ret != 0) {
//     //     printf("UartWrite: write data failed, ret %d\n", ret);
//     //     goto ERR;
//     // }
//     //printf("UartWrite successful and wbuff:  %u\n",wbuff);       //这里输出的为整型，所以引用符号应为d
//     //printf(wbuff);
//     //  for(int i=0;i<5;i++){
//     //     printf("write:%hhu\r\n",wbuff[i]);
//     //  }
 
//     // ret = UartRead(handle, rbuff, 5);                          // 从UART设备读取5字节的数据
//     // if (ret < 0) {
//     //     printf("UartRead: read data failed, ret %d\n", ret);
//     //     goto ERR;
//     // }
//     //printf("UartRead successful and rbuff:   %u\n",rbuff);
//     // //printf(rbuff);
//     //  for(int i=0;i<5;i++){
//     //     printf("%u",rbuff[i]);
//     //  }
//     printf("UartRead data!!!\n");
//     // int j=1;
//      while(1){
        // int tmp = 0;
        // float temp = 0.0;
        // ret=UartRead(handle, rbuff, 1000);                      // 从UART设备读取数据
        // if(ret<0){
        //     printf("read fail ret:%d\r\n",ret);
        //     goto ERR;
        // }
        // if(ret==-1||ret==0){
        //     continue;    //如果没读到，那就跳出循环再读
        // }
        // // printf("readlen=:%d\n",ret);
        // // for(int i=0;i<ret;i++){
        // //     printf("read:%02x\r\n",rbuff[i]); //按十六进制输出
        // // }
        // if(ret == 6)
        // {
        //     tmp = rbuff[3] << 8 | rbuff[4];
        //     printf("temperature:%d\n",tmp);

        //     unsigned short integerPart = (rbuff[3]<<8) | rbuff[4];
        //     // unsigned char decimalPart = rbuff[4] & 0x0F;
        //     temp = (float)integerPart / 10.0f;
        //     printf("temperature:%.1f\n",temp);
        //     if(temp > 30)
        //     {
        //         g_lightDev->TurnOnLight(g_lightInfo[0].lightId, &effect);       // 打开灯光
        //     }
        //     else
        //     {
        //         g_lightDev->TurnOffLight(g_lightInfo[0].lightId);               // 关闭灯光
        //     }
        // }
        // printf("\n");
        // usleep(100*1000); //sleep 10ms
//     }
 
//     printf(" function tests end\r\n");
// ERR:
//     if(g_lightDev != NULL){
//         FreeLightInterfaceInstance();                          //释放灯光设备接口实例
//         g_lightDev = NULL;
//     }

//     UartClose(handle);                                         // 销毁UART设备句柄
//     return ret;
 
//     printf("---------------------------\n");
 
// }