#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h> 
#include <stdint.h>    //unint8_t

#if defined(WIN32) || defined(WIN64)
#include "windows.h"
#endif

#include "uart_if.h"    //路径：    "//drivers/hdf_core/framework/include/platform",
#include <fcntl.h>
#include "define.h"
#include "iota.h"
#include "cJSON.h"
#include <termios.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "osal_mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h> 
#include "light_if.h"
#include "light_type.h"


/* Device connection info */
//char* g_server_addr = "192.168.3.98"; // mqtt连接服务IP
char  g_server_addr[20];
int   g_port = 1883;  // mqtt连接端口          
//char* g_device_id = "503D2212X00009"; // 设备标识   
char  g_device_id[256];
//char* g_user_name = "EF6862D4A35A4A6D850C6432E2BAFEC7"; //mqtt连接用户名  
char  g_user_name[256];
//char* g_passwd = "Gateway@123456"; // mqtt连接密码
char  g_passwd[256];
//char* g_client_id = "503D2212X00009"; // mqtt连接客户端  
char  g_client_id[256];
char* g_service_id = "ABC";
char* g_ManufacturerId = "syncTime";//子设备厂商id
char* g_ManufacturerName = "wg_sub_csid";//子设备厂商名称
char* g_Model = "wg_sub_csmc";//子设备产品型号
char* g_prefix = "dev_test";
int   g_ssl_opt = 0;//1：启用ssl，0关闭ssl
char* g_synctime_service_id = "syncTime";//服务ID
//vector<string> g_vct_device;


char* g_workPath = ".";
int   g_keepAlivetime = 120;   /* Set the heartbeat interval to 120 seconds */
int   g_login = 0;
char* g_query_message;
const char *filename = "/data/output.txt"; // 文件名
DevHandle handle = NULL;

/*
    传感器温度读取函数
    返回类型：float
*/
// float TemperatureRead(uint8_t rbuff[], int32_t ret, DevHandle handle) {
    
//     printf("UartRead data\n");
//     int tmp = 0;
//     float temp = 0.0;
//     ret = UartRead(handle, rbuff, 1000); //读数据
//     if(ret < 0) {
//         printf("read fail ret: %d\r\n", ret);
//         goto ERR;
//     }
//     if(ret == 1 || ret == 0) {
//         continue;  //没读到数据就跳出循环再读
//     }
//     if(ret == 6) {
//         tmp = rbuff[3] << 8 | rbuff[4];

//         unsigned short integerPart = (rbuff[3] << 8) | rbuff[4];
//         temp = (float)integerPart / 10.0f;
//         printf("temperature: %.1f\n", temp);
//     }
//     printf("\n");
//     usleep(100*1000);
//     return temp;
// ERR:
//     UartClose(handle);
//     return ret;
// }


// 自定义的清理函数
void cleanup() {
    printf("Performing cleanup before exit...\n");
    // 在这里添加需要执行的代码
    printf("Executing cleanup code...\n");
    UartClose(handle);                                         // 销毁UART设备句柄
}

void handle_sigint(int sig) {
    printf("Received signal: %d\n", sig);
    cleanup(); // 执行清理代码
    // 结束程序
    exit(sig);
}




void timeSleep(int ms)
{
#if defined(WIN32) || defined(WIN64)
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}
void myPrintLog(int level, char* format, va_list args);

void Test_Login()
{
    int ret = IOTA_Login();
    printf("MQTT_Demo: Test_Login(), ret = %d \n", ret);
}

void Test_Logout()
{
    int ret = IOTA_Logout();
    printf("MQTT_Demo: Test_Logout(), ret = %d \n", ret);
}

void Test_updateDeviceStatusOnline(char* strDeviceId)
{
    static int mid = 3000;

    IOTA_DeviceStatusUpdate(++mid, strDeviceId, "ONLINE");
}

//void Test_updateMultiDeviceStatusOnline()
//{
//    static int mid = 20000;
//
//    vector<ST_IOTA_DEVICE_STATUS> vctStatuss;
//    for (int i = 0; i < g_vct_device.size(); i++)
//    {
//        ST_IOTA_DEVICE_STATUS deviceStatus;
//        deviceStatus.strDeviceId = g_vct_device[i];
//        deviceStatus.strStatus = "ONLINE";
//        vctStatuss.push_back(deviceStatus);
//    }
//    IOTA_DeviceStatusUpdate(++mid, vctStatuss);
//}

void Test_updateDeviceStatusOffline(char* strDeviceId)
{
    static int mid = 4000;

    IOTA_DeviceStatusUpdate(++mid, strDeviceId, "OFFLINE");
}

//void Test_updateMultiDeviceStatusOffline()
//{
//    static int mid = 30000;
//
//    vector<ST_IOTA_DEVICE_STATUS> vctStatuss;
//    for (int i = 0; i < g_vct_device.size(); i++)
//    {
//        ST_IOTA_DEVICE_STATUS deviceStatus;
//        deviceStatus.strDeviceId = g_vct_device[i];
//        deviceStatus.strStatus = "OFFLINE";
//        vctStatuss.push_back(deviceStatus);
//    }
//    IOTA_DeviceStatusUpdate(++mid, vctStatuss);
//}

void Test_DataReport(char* strDeviceId)
{
    char* strData = "{\"key\":\"value\"}";

    int  messageId = IOTA_ServiceDataReport(strDeviceId, g_service_id, strData);

    printf("MQTT_Demo: Test_DataReport(), report data with messageId %d \n", messageId);
}
void Test_SyncTimeRequest()
{
    char* strData = "{\"syncTimeRequest\":\"true\"}";

    int  messageId = IOTA_ServiceDataReport(g_device_id, g_synctime_service_id, strData);

    printf("MQTT_Demo: Test_SyncTimeRequest(), Request data with messageId %d \n", messageId);
}

//void Test_MultiDataReport()
//{
//    ST_IOTA_SERVICES service;
//    service.strServiceId = g_service_id;
//    service.strData = "{\"key\":\"value\"}";
//
//    vector<ST_IOTA_SERVICES> vctServices;
//    vctServices.push_back(service);
//
//    for (int i = 0; i < g_vct_device.size(); i++)
//    {
//        timeSleep(10);
//        int  messageId = IOTA_ServiceDataReport(g_vct_device[i], vctServices);
//        printf("MQTT_Demo: Test_dataReport(), report data with messageId %d \n", messageId);
//    }
//}


void Test_addSubDevice(char* strDeviceId)
{
    static int mid = 1000;
    ST_IOTA_DEVICE_INFO device;
    device.pcNodeId = strDeviceId;
    device.pcDeviceId = "";
    device.pcName = "";
    device.pcDescription = "";
    device.pcManufacturerId = g_ManufacturerId;
    device.pcModel = g_Model;
    IOTA_HubDeviceAdd(++mid, &device);
}

// 17
void Test_NewCustomTopicReport(char* strDeviceId, char* version, char* topic, char* data)
{
    ST_IOTA_BatchReportProperties brp = {0};
    brp.pcDeviceId = strDeviceId;
    brp.pcServiceId = g_service_id;
    char str[50] = "{\"msg\":\"";
    snprintf(str + strlen(str), sizeof(str) - strlen(str), "%s", data);
    snprintf(str + strlen(str), sizeof(str) - strlen(str), "%s", "\"}");
    brp.pcPayload = str;
    printf("str: %s\n", str);

    int  messageId = IOTA_ServiceCustomTopicReport(&brp, version, topic);

    printf("MQTT_Demo: Test_CustomTopicReport(), report data with messageId %d \n", messageId);
}

//void Test_multiAddSubDevice()
//{
//    static int mid = 10000;
//    ST_IOTA_DEVICE_INFO device;
//    device.strDeviceId = "";
//    device.strName = "";
//    device.strDescription = "";
//    device.strManufacturerId = g_ManufacturerId;
//    device.strModel = g_Model;
//
//    vector<ST_IOTA_DEVICE_INFO> vctDevices;
//    for (int i = 0; i < 1000; i++)
//    {
//        device.strNodeId = g_prefix + to_string(i);
//        vctDevices.push_back(device);
//    }
//
//    IOTA_HubDeviceAdd(++mid, vctDevices);
//}

void Test_deleteSubDevice(char* strDeviceId)
{
    static int mid = 5000;

    ST_IOTA_DEVICE_INFO device;
    device.pcNodeId = "";
    device.pcDeviceId = strDeviceId;
    device.pcName = "";
    device.pcDescription = "";
    device.pcManufacturerId = g_ManufacturerId;
    device.pcManufacturerName = g_ManufacturerName;
    device.pcModel = g_Model;

    IOTA_HubDeviceRemove(++mid, strDeviceId);
}

void Test_commandResponse(int mid)
{
    char* strData = "{\"commandresult\":\"123\"}";
    int messageId = IOTA_ServiceCommandResponse(mid, 0, strData);
    printf("MQTT_Demo: Test_commandResponse(), response data with messageId %d, command mid %d \n", messageId, mid);
}

// void Test_

void Test_queryDeviceInfo(char* pcNodeId)
{
     static int mid = 2000;

     ST_IOTA_DEVICE_INFO device;
     if (pcNodeId == NULL)
     {
         device.pcNodeId = g_device_id;
     }
     else
     {
         device.pcNodeId = pcNodeId;
     }
     device.pcDeviceId = "";
     device.pcName = "";
     device.pcDescription = "";
     device.pcManufacturerId = g_ManufacturerId;
     device.pcModel = g_Model;
    IOTA_HubDeviceQuery(++mid, NULL, 0, &device);
}

void Test_CustomTopicReport(char* strDeviceId)
{
    ST_IOTA_BatchReportProperties brp = {0};
    brp.pcDeviceId = strDeviceId;
    brp.pcServiceId = g_service_id;
    brp.pcPayload = "{\"key\":\"value\"}";

    int  messageId = IOTA_ServiceCustomTopicReport(&brp, "v2.0", "MQTTNTP");

    printf("MQTT_Demo: Test_CustomTopicReport(), report data with messageId %d \n", messageId);
}

void Test_CustomTopicSubscribe()
{

    int  messageId = IOTA_SubscribeCustomTopic("v2.0", "MQTTNTPResponse");

    printf("MQTT_Demo: Test_CustomTopicSubscribe(), report data with messageId %d \n", messageId);
}

// cmd 16
void Test_SubscribeTopic(char* pcTopicVersion, char* pcCustomTopic)
{
    int  messageId = IOTA_SubscribeCustomTopic(pcTopicVersion, pcCustomTopic);

    printf("MQTT_Demo: Test_CustomTopicSubscribe(), report data with messageId %d \n", messageId);
}


/* int logincount = 0; */
void handleLoginSuccess(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handleLoginSuccess(), login success\n");
    g_login = 1;
}

void handleLoginFailure(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handleConnectFailure() error, code %d, messsage %s\n", code, message);
    /* judge if the network is available etc. and login again */
    printf("MQTT_Demo: handleConnectFailure() login again\n");
    int ret = IOTA_Login();
    if (ret != 0) {
        printf("MQTT_Demo: handleConnectionLost() error, login again failed, result %d\n", ret);
    }
}

void handleConnectionLost(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handleConnectionLost() warning, code %d, messsage %s\n", code, message);
    timeSleep(1000);
    /* judge if the network is available etc. and login again */
    int ret = IOTA_Login();
    if (ret != 0) {
        printf("MQTT_Demo: handleConnectionLost() error, login again failed, result %d\n", ret);
    }
}

void handleSubscribeSuccess(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handleSubscribeSuccess() messageId %d\n", messageId);
}

void handleSubscribeFailure(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handleSubscribeFailure() warning, messageId %d, code %d, messsage %s\n", messageId, code, message);
}

void handlePublishSuccess(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handlePublishSuccess() messageId %d\n", messageId);
}

void handlePublishFailure(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handlePublishFailure() warning, messageId %d, code %d, messsage %s\n", messageId, code, message);
}

void handleLogoutSuccess(void* context, int messageId, int code, char* message)
{
    printf("AgentLiteDemo: handleLogoutSuccess, login again\n");
    g_login = 0;
}

void handleLogoutFailure(void* context, int messageId, int code, char* message)
{
    printf("AgentLiteDemo: handleLogoutFailure, login again\n");
    int ret = IOTA_Login();
    if (ret != 0) {
        printf("MQTT_Demo: handleConnectionLost() error, login again failed, result %d\n", ret);
    }
}

void handleCommandArrived(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handleCommandArrived(), messageId %d, code %d, messsage %s\n", messageId, code, message);
}

void handleCustomTopicMessageArrived(void* context, int messageId, int code, const char* topic, char* message)
{
    printf("MQTT_Demo: handleCustomTopicMessageArrived(), messageId %d, code %d, topic %s, messsage %s\n", messageId, code, topic, message);
}

void __JsonToStruct_DeviceAddRsp(const char* strJson)
{
    cJSON* json = NULL;
    cJSON* dataList = NULL;
    cJSON* data = NULL;
    cJSON* dev = NULL;
    json = cJSON_Parse(strJson);
    if (json)
    {
        dataList = cJSON_GetObjectItem(json, "data");
        for (int i = 0; i < cJSON_GetArraySize(dataList); i++)
        {
            data = cJSON_GetArrayItem(dataList, i);
            if (cJSON_HasObjectItem(data, "deviceInfo"))
            {
                dev = cJSON_GetObjectItem(data, "deviceInfo");
                //if (cJSON_HasObjectItem(dev, "description"))
                //{
                //    strcpy(stRsp->stDeviceInfos.pcDescription, cJSON_GetObjectItem(dev, "description")->valuestring);
                //}
                //strcpy(stRsp->stDeviceInfos.pcDeviceId, cJSON_GetObjectItem(dev, "deviceId")->valuestring);
                //strcpy(stRsp->stDeviceInfos.pcManufacturerId, cJSON_GetObjectItem(dev, "manufacturerId")->valuestring);
                //strcpy(stRsp->stDeviceInfos.pcModel, cJSON_GetObjectItem(dev, "model")->valuestring);
                //strcpy(stRsp->stDeviceInfos.pcName, cJSON_GetObjectItem(dev, "name")->valuestring);
                //strcpy(stRsp->stDeviceInfos.pcNodeId, cJSON_GetObjectItem(dev, "nodeId")->valuestring);
            }
            break;
        }

        cJSON_Delete(json);
    }
    return;
}

void handleSubDeviceAddResult(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handleSubDeviceAddResult(), messageId %d, code %d, messsage %s\n", messageId, code, message);
    __JsonToStruct_DeviceAddRsp(message);
}

void handleSubDeviceDeleteResult(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handleSubDeviceDeleteResult(), messageId %d, code %d, messsage %s\n", messageId, code, message);
}

void handleSubDeviceUpdateResult(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handleSubDeviceUpdateResult(), messageId %d, code %d, messsage %s\n",
        messageId, code, message);
}

void handleDeviceQueryResult(void* context, int messageId, int code, char* message)
{
    printf("MQTT_Demo: handleDeviceQueryResult(), messageId %d, code %d, messsage %s\n", messageId, code, message);
    g_query_message = message;
}

void setLoginConfig()
{
    IOTA_ConfigSetStr(EN_IOTA_CFG_MQTT_ADDR, g_server_addr);
    IOTA_ConfigSetUint(EN_IOTA_CFG_MQTT_PORT, g_port);
    IOTA_ConfigSetStr(EN_IOTA_CFG_DEVICEID, g_device_id);
    IOTA_ConfigSetStr(EN_IOTA_CFG_CLIENTID, g_client_id);
    IOTA_ConfigSetStr(EN_IOTA_CFG_DEVICESECRET, g_passwd);
    IOTA_ConfigSetStr(EN_IOTA_CFG_USERNAME, g_user_name);

    /**
    * It is not suitable to use MQTT heartbeat for scenes that cannot respond to heartbeat response in time;
    * Please realize the heartbeat function by yourself, This value is set to 0.
    */
    IOTA_ConfigSetUint(EN_IOTA_CFG_KEEP_ALIVE_TIME, g_keepAlivetime);

    /**
    * EN_IOTA_CFG_URL_PREFIX_TCP : TCP Channel , 1883
    * EN_IOTA_CFG_URL_PREFIX_SSL : SSL Channel , 8443
    * The default is TCP, which can be configured according to the actual situation
    */
    IOTA_ConfigSetUint(EN_IOTA_CFG_MQTT_URL_PREFIX, g_ssl_opt);
    IOTA_ConfigSetUint(EN_IOTA_CFG_AUTH_MODE, EN_IOTA_CFG_AUTH_MODE_NODE_ID);
    IOTA_ConfigSetUint(EN_IOTA_CFG_QOS, 0);

#ifdef _SYS_LOG
    IOTA_ConfigSetUint(EN_IOTA_CFG_LOG_LOCAL_NUMBER, LOG_LOCAL7);
    IOTA_ConfigSetUint(EN_IOTA_CFG_LOG_LEVEL, LOG_INFO);
#endif
}

void setMyCallbacks()
{
    IOTA_SetCallback(EN_IOTA_CALLBACK_CONNECT_SUCCESS, handleLoginSuccess);
    IOTA_SetCallback(EN_IOTA_CALLBACK_CONNECT_FAILURE, handleLoginFailure);
    IOTA_SetCallback(EN_IOTA_CALLBACK_CONNECTION_LOST, handleConnectionLost);

    IOTA_SetCallback(EN_IOTA_CALLBACK_DISCONNECT_SUCCESS, handleLogoutSuccess);
    IOTA_SetCallback(EN_IOTA_CALLBACK_DISCONNECT_FAILURE, handleLogoutFailure);

    IOTA_SetCallback(EN_IOTA_CALLBACK_SUBSCRIBE_SUCCESS, handleSubscribeSuccess);
    IOTA_SetCallback(EN_IOTA_CALLBACK_SUBSCRIBE_FAILURE, handleSubscribeFailure);

    IOTA_SetCallback(EN_IOTA_CALLBACK_PUBLISH_SUCCESS, handlePublishSuccess);
    IOTA_SetCallback(EN_IOTA_CALLBACK_PUBLISH_FAILURE, handlePublishFailure);

    IOTA_SetCallback(EN_IOTA_CALLBACK_COMMAND_ARRIVED, handleCommandArrived);

    IOTA_SetCallback(EN_IOTA_CALLBACK_DEVICE_ADDITION_RESULT, handleSubDeviceAddResult);
    IOTA_SetCallback(EN_IOTA_CALLBACK_DEVICE_DELETION_RESULT, handleSubDeviceDeleteResult);
    IOTA_SetCallback(EN_IOTA_CALLBACK_DEVICE_UPDATE_RESULT, handleSubDeviceUpdateResult);
    IOTA_SetCallback(EN_IOTA_CALLBACK_DEVICE_QUERY_RESULT, handleDeviceQueryResult);

    IOTA_SetCallbackWithTopic(handleCustomTopicMessageArrived);
}

void myPrintLog(int level, char* format, va_list args)
{
    vprintf(format, args);
}

void usage()
{
    printf("\n");
    printf("0/h/?/help\t\t\thelp\n");
    printf("1/login\t\t\t\tlogin\n");
    printf("2/logout\t\t\tlogout\n");
    printf("3/query\t\t\t\tquery device info\n");
    printf("4/add\t\t\t\tadd subdevice\n");
    printf("5/online\t\t\tupdate device status to online\n");
    printf("6/offline\t\t\tupdate device status to offline\n");
    printf("7/report\t\t\tdevice data report\n");
    printf("8/synctime\t\t\tdevice request syncTime\n");
    //printf("8/madd\t\t\t\tadd multisubdevices\n");
    //printf("9/monline\t\t\tupdate multidevices status to online\n");
    //printf("10/moffline\t\t\tupdate multidevices status to offline\n");
    //printf("11/mreport\t\t\tmultidevices data report\n");
    printf("12/response\t\t\tresponse command\n");
    //printf("13/delete\t\t\tdelete subdevice\n");
    printf("14/ctr\t\t\t\tcustom topic report\n");
    printf("15/cts\t\t\t\tcustom topic subscribe\n");
    printf("16/act\t\t\t\tadd custom topic\n");
    printf("17\t\t\t\tnew custom topic report\n");
    printf("98/clear\t\t\tclear print\n");
    printf("99/q/quit\t\t\tquit demo\n");
    printf("\n");
}

void waitForCommond()
{
    char strInput[1024];

    while (1)
    {
        memset(strInput, 0, 1024);
        gets(strInput);

        if (0 == strlen(strInput)) continue;
    
        if (0 == strcmp(strInput, "0")
            || 0 == strcmp(strInput, "h")
            || 0 == strcmp(strInput, "?")
            || 0 == strcmp(strInput, "help"))
        {
            usage();
        }
        else if (0 == strcmp(strInput, "1")
            || 0 == strcmp(strInput, "login"))
        {
            Test_Login();
        }
        else if (0 == strcmp(strInput, "2")
            || 0 == strcmp(strInput, "logout"))
        {
            Test_Logout();
        }
        else if (0 == strcmp(strInput, "3")
            || 0 == strcmp(strInput, "query"))
        {
            if (!g_login)
            {
                printf("Please login before this operation!\n");
                continue;
            }
            Test_queryDeviceInfo(NULL);
        }
        else if (0 == strncmp(strInput, "3-", 2)
            || 0 == strncmp(strInput, "query-", 6))
        {
            if (!g_login)
            {
                printf("Please login before this operation!\n");
                continue;
            }

            char* strDeviceId;
            if (0 == strncmp(strInput, "3-", 2))
            {
                strDeviceId = strInput + 2;
            }
            else
            {
                strDeviceId = strInput + 6;
            }
            Test_queryDeviceInfo(strDeviceId);
        }
        else if (0 == strcmp(strInput, "4")
            || 0 == strcmp(strInput, "add"))
        {
            printf("please input 4-xxxxx or add-xxxxx, xxxxx is device_id\n");
            printf("e.g. 4-D59eGSxy\n");
        }
        else if (0 == strncmp(strInput, "4-", 2)
            || 0 == strncmp(strInput, "add-", 4))
        {
            if (!g_login)
            {
                printf("Please login before this operation!\n");
                continue;
            }

            char* strDeviceId;
            if (0 == strncmp(strInput, "4-", 2))
            {
                strDeviceId = strInput + 2;
            }
            else
            {
                strDeviceId = strInput + 4;
            }
            Test_addSubDevice(strDeviceId);
        }
        else if (0 == strcmp(strInput, "5")
            || 0 == strcmp(strInput, "online"))
        {
            printf("please input 5-xxxxx or online-xxxxx, xxxxx is device_id\n");
            printf("e.g. 5-D59eGSxy\n");
        }
        else if (0 == strncmp(strInput, "5-", 2)
            || 0 == strncmp(strInput, "online-", 7))
        {
            if (!g_login)
            {
                printf("Please login before this operation!\n");
                continue;
            }

            char* strDeviceId;
            if (0 == strncmp(strInput, "5-", 2))
            {
                strDeviceId = strInput + 2;
            }
            else
            {
                strDeviceId = strInput + 7;
            }
            Test_updateDeviceStatusOnline(strDeviceId);
        }
        else if (0 == strcmp(strInput, "6")
            || 0 == strcmp(strInput, "offline"))
        {
            printf("please input 6-xxxxx or offline-xxxxx, xxxxx is device_id\n");
            printf("e.g. 6-D59eGSxy\n");
        }
        else if (0 == strncmp(strInput, "6-", 2)
            || 0 == strncmp(strInput, "offline-", 8))
        {
            if (!g_login)
            {
                printf("Please login before this operation!\n");
                continue;
            }

            char* strDeviceId;
            if (0 == strncmp(strInput, "6-", 2))
            {
                strDeviceId = strInput + 2;
            }
            else
            {
                strDeviceId = strInput + 8;
            }
            Test_updateDeviceStatusOffline(strDeviceId);
        }
        else if (0 == strcmp(strInput, "7")
        || 0 == strcmp(strInput, "report"))
        {
            printf("please input 7-xxxxx or report-xxxxx, xxxxx is device_id\n");
            printf("e.g. 7-D59eGSxy\n");
        }
        else if (0 == strncmp(strInput, "7-", 2)
        || 0 == strncmp(strInput, "report-", 7))
        {
            if (!g_login)
            {
                printf("Please login before this operation!\n");
                continue;
            }

            char* strDeviceId;
            if (0 == strncmp(strInput, "7-", 2))
            {
                strDeviceId = strInput + 2;
            }
            else
            {
                strDeviceId = strInput + 7;
            }
            Test_DataReport(strDeviceId);
        }
       else if (0 == strcmp(strInput, "8")
                || 0 == strcmp(strInput, "synctime"))
        {
            if (!g_login)
            {
                printf("Please login before this operation!\n");
                continue;
            }
                Test_SyncTimeRequest();
         }
        //else if (0 == strcmp(strInput, "9")
        // || 0 == strcmp(strInput, "monline"))
        //{
        //    if (!g_login)
        //    {
        //        printf("Please login before this operation!\n");
        //        continue;
        //    }
        //    Test_updateMultiDeviceStatusOnline();
        //}
        //else if (0 == strcmp(strInput, "10")
        // || 0 == strcmp(strInput, "moffline"))
        //{
        //    if (!g_login)
        //    {
        //        printf("Please login before this operation!\n");
        //        continue;
        //    }
        //    Test_updateMultiDeviceStatusOffline();
        //}
        //else if (0 == strcmp(strInput, "11")
        // || 0 == strcmp(strInput, "mreport"))
        //{
        //    if (!g_login)
        //    {
        //        printf("Please login before this operation!\n");
        //        continue;
        //    }
        //    Test_MultiDataReport();
        //}
        else if (0 == strcmp(strInput, "12")
        || 0 == strcmp(strInput, "response"))
        {
            printf("please input 12-xxxxx or response-xxxxx, xxxxx is device_id\n");
            printf("e.g. 12-1234\n");
        }
        else if (0 == strncmp(strInput, "12-", 3)
        || 0 == strncmp(strInput, "response-", 9))
        {
            if (!g_login)
            {
                printf("Please login before this operation!\n");
                continue;
            }

            char* strMId;
            if (0 == strncmp(strInput, "12-", 3))
            {
                strMId = strInput + 3;
            }
            else
            {
                strMId = strInput + 9;
            }
            Test_commandResponse(atoi(strMId));
        }
        else if (0 == strcmp(strInput, "14")
        || 0 == strcmp(strInput, "ctr"))
        {
        printf("please input 14-xxxxx or ctr-xxxxx, xxxxx is device_id\n");
        printf("e.g. 14-D59eGSxy\n");
        }
        else if (0 == strncmp(strInput, "14-", 3)
        || 0 == strncmp(strInput, "ctr-", 4))
        {
        if (!g_login)
        {
            printf("Please login before this operation!\n");
            continue;
        }

        char* strDeviceId;
        if (0 == strncmp(strInput, "14-", 3))
        {
            strDeviceId = strInput + 3;
        }
        else
        {
            strDeviceId = strInput + 4;
        }
        Test_CustomTopicReport(strDeviceId);
        }
        else if (0 == strcmp(strInput, "15")
        || 0 == strcmp(strInput, "cts"))
        {
            if (!g_login)
            {
                printf("Please login before this operation!\n");
                continue;
            }
            Test_CustomTopicSubscribe();
        }
        else if (0 == strcmp(strInput, "16") || 0 == strcmp(strInput, "act"))
        {
            printf("please input 16-TopicVersion/CustomTopic or act-TopicVersion/CustomTopic, \n");
            printf("e.g. 16-V2.0/Topic123\n");
        }
        else if (0 == strncmp(strInput, "16-", 3) || 0 == strncmp(strInput, "act-", 4))
        {
            if (!g_login)
            {
                printf("Please login before this operation!\n");
                continue;
            }
            //char start[100];
            //memset(start, 0, 100);
            //char topicStart[100];
            //memset(topicStart, 0, 100);
            char* start = strInput;
            int startN = 0;
            if(0 == strncmp(strInput, "16-", 3))
            {
                start = strInput+3;
                startN = 3;
            }
            else
            {
                start = strInput+4;
                startN = 4;
            }
            char* topicStart = strInput;
            
            for(int i = startN; i < 100; ++i){
                if(strInput[i] == '/'){
                    topicStart = strInput + i+1;
                    //strncpy(start, )
                    //strncpy(topicStart, )
                    strInput[i] = '\0';
                    break;
                }
            }
            printf("version: %s\n", start);
            printf("topic: %s\n", topicStart);
            
            Test_SubscribeTopic(start, topicStart);

        }
        else if (0 == strcmp(strInput, "17"))
        {
            printf("please input 17-TopicVersion/CustomTopic:qwert12345, \n");
            printf("e.g. 16-V2.0/Topic123:oinwvno14inio13\n");
        }
        else if (0 == strncmp(strInput, "17-", 3))
        {
            char* start = strInput+3;
            char* topicStart = start;
            char* dataStart = start;
            int startN = 3;
            for(int i = startN; i < 100; ++i){
                if(strInput[i] == '/'){
                    topicStart = strInput + i+1;
                    //strncpy(start, )
                    //strncpy(topicStart, )
                    strInput[i] = '\0';
                }
                if(strInput[i] == ':'){
                    dataStart = strInput + i+1;
                    strInput[i] = '\0';
                    break;
                }
            }
            printf("version: %s\n", start);
            printf("topic: %s\n", topicStart);
            printf("data: %s\n", dataStart);
            char* strDeviceId = "503D2212X00009";
            Test_NewCustomTopicReport(strDeviceId, start, topicStart, dataStart);
        }
        else if (0 == strcmp(strInput, "98")
        || 0 == strcmp(strInput, "clear"))
        {
            system("clear");
            usage();
        }
        else if (0 == strcmp(strInput, "99")
        || 0 == strcmp(strInput, "q")
        || 0 == strcmp(strInput, "quit"))
        {
            break;
        }
        else
        {
            printf("unknown command!\n");
        }
    }
}

int getConfigPara()
{
	FILE *fp = fopen("/etc/mqtt_config.json", "r");
    if (fp != NULL) {
		char buffer[1024];
		size_t len;
		
		len = fread(buffer, sizeof(char), sizeof(buffer), fp);
		fclose(fp);
	 
		cJSON *json = cJSON_Parse(buffer);
		// 在这里可以进一步处理JSON对象
		if(NULL != json)
		{
			g_port = cJSON_GetObjectItem(json, "ServicePort")->valueint;
			strncpy(g_server_addr, cJSON_GetObjectItem(json, "ServiceIp")->valuestring, sizeof(g_server_addr) - 1);
			strncpy(g_user_name, cJSON_GetObjectItem(json, "UserName")->valuestring, sizeof(g_user_name) - 1);
			strncpy(g_passwd, cJSON_GetObjectItem(json, "Passwd")->valuestring, sizeof(g_passwd) - 1);
			strncpy(g_device_id, cJSON_GetObjectItem(json, "DeviceId")->valuestring, sizeof(g_device_id) - 1);
			strncpy(g_client_id, cJSON_GetObjectItem(json, "ClientId")->valuestring, sizeof(g_client_id) - 1);
		}
	}
    return 0;
}

int main(int argc, char** argv)
{
#if defined(_DEBUG)
    setvbuf(stdout, NULL, _IONBF, 0); /* in order to make the console log printed immediately at debug mode */
#endif
	getConfigPara();
    IOTA_SetPrintLogCallback(myPrintLog);

    //uart初始化jamd
    printf("uart_test begin\r\n");
 
    printf("---------------------------\n");
 
    int32_t ret;
    uint32_t port = 3;
    uint32_t baud;
    // uint8_t wbuff[5] = { 1, 2, 3, 4, 5 };
    uint8_t rbuff[1000] = { 0 };
    
    const struct LightInterface *g_lightDev = NULL;
    struct LightInfo *g_lightInfo = NULL;
    uint32_t g_count = 0;

    struct UartAttribute attribute;
    attribute.dataBits = UART_ATTR_DATABIT_8;                  // UART传输数据位宽，一次传输7个bit
    attribute.parity = UART_ATTR_PARITY_NONE;                  // UART传输数据无校检
    attribute.stopBits = UART_ATTR_STOPBIT_1;                  // UART传输数据停止位为1位
    attribute.rts = UART_ATTR_RTS_DIS;                         // UART禁用RTS
    attribute.cts = UART_ATTR_CTS_DIS;                         // UART禁用CTS
    attribute.fifoRxEn = UART_ATTR_RX_FIFO_EN;                 // UART使能RX FIFO
    attribute.fifoTxEn = UART_ATTR_TX_FIFO_EN;                 // UART使能TX FIFO

    struct LightEffect effect;
    effect.flashEffect.flashMode = 0;
    effect.flashEffect.onTime = 0;
    effect.flashEffect.offTime = 0;
    effect.lightColor.colorValue.rgbColor.r = 255;
    effect.lightColor.colorValue.rgbColor.g = 0;
    effect.lightColor.colorValue.rgbColor.b = 0;

    g_lightDev = NewLightInterfaceInstance();               //初始化灯光设备接口实例
    if (g_lightDev == NULL) {
        printf("test light get Module instance fail\n\r");
    }
    ret = g_lightDev->GetLightInfo(&g_lightInfo, &g_count);     //获取灯光设备信息
    if (ret == -1) {
        printf("get light informations fail\n\r");
    }

    handle = UartOpen(port);                                   // 获取UART设备句柄
    if (handle == NULL) {
        printf("UartOpen(port) failed \r\n");
        return HDF_FAILURE;
    }
 
    ret = UartSetBaud(handle, 9600);                           // 设置UART波特率为9600
    if (ret != 0) {
        printf("UartSetBaud: set baud failed, ret %d\n", ret);
        goto ERR;
    }
    printf("UartOpen successful and uart port = %d\n",port);
 
    ret = UartGetBaud(handle, &baud);                          // 获取UART波特率
    if (ret != 0) {
        printf("UartGetBaud: get baud failed, ret %d\n", ret);
        goto ERR;
    }
    printf("UartSetBaud successful and uart baudrate = %d\n", baud);
 
    ret = UartSetAttribute(handle, &attribute);                // 设置UART设备属性
    if (ret != 0) {
        printf("UartSetAttribute: set attribute failed, ret %d\n", ret);
        goto ERR;
    }
    printf("UartSetAttribute successful\n");
 
    // ret = UartGetAttribute(handle, &attribute);                // 获取UART设备属性
    // if (ret != 0) {
    //     printf("UartGetAttribute: get attribute failed, ret %d\n", ret);
    //     goto ERR;
    // }
 
    ret = UartSetTransMode(handle, UART_MODE_RD_NONBLOCK);     // 设置UART传输模式为非阻塞模式
    if (ret != 0) {
        printf("UartSetTransMode: set trans mode failed, ret %d\n", ret);
        goto ERR;
    }
    printf("UartSetTransMode successful\n");
    printf("UartRead data!!!\n");
    


    // 注册信号处理函数
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);   // 终止信号
    signal(SIGQUIT, handle_sigint);   // 退出信号

    // 注册程序结束时执行的清理函数
    atexit(cleanup);

    // printf("debug 1 ===================>\n");


    printf("MQTT_Demo: start test ===================>\n");

    char* strLogPath = "";
    if (IOTA_Init(g_workPath, strLogPath) > 0) {
        printf("MQTT_Demo: IOTA_Init() error, init failed\n");
        return 1;
    }

    setLoginConfig();
    setMyCallbacks();

    usage();

    // 登录
    int login = IOTA_Login();
    printf("*****************************************\n");
    printf("MQTT_Demo: Test_Login(), login = %d \n", login);
    printf("*****************************************\n");
    while (!g_login) {
        usleep(100*1000); //sleep 10ms
        printf("relogining...");
        login = IOTA_Login();
    }
    printf("Logging successfully!\n");
    printf("*****************************************\n");

    // printf("debug 2 ===================>\n");
    // 订阅主题
    int  messageId = IOTA_SubscribeCustomTopic("1.0", "Temperature"); // pcTopicVersion, pcCustomTopic
    printf("*****************************************\n");
    printf("MQTT_Demo: Test_CustomTopicSubscribe(), report data with messageId %d \n", messageId);
    printf("*****************************************\n");
    // printf("debug 3 ===================>\n");

    // waitForCommond();
    while(1) {
        int tmp = 0;
        float temp = 0.0;
        ret=UartRead(handle, rbuff, 1000);                      // 从UART设备读取数据
        if(ret<0){
            printf("read fail ret:%d\r\n",ret);
            goto ERR;
        }
        if(ret==-1||ret==0){
            printf("reading failure\n");
            continue;    //如果没读到，那就跳出循环再读
        }
        // printf("readlen=:%d\n",ret);
        // for(int i=0;i<ret;i++){
        //     printf("read:%02x\r\n",rbuff[i]); //按十六进制输出
        // }
        if(ret == 6)
        {
            tmp = rbuff[3] << 8 | rbuff[4];
            // printf("temperature:%d\n",tmp);

            unsigned short integerPart = (rbuff[3]<<8) | rbuff[4];
            // unsigned char decimalPart = rbuff[4] & 0x0F;
            temp = (float)integerPart / 10.0f;
            printf("temperature:%.1f\n",temp);
            if(temp > 30)
            {
                g_lightDev->TurnOnLight(g_lightInfo[0].lightId, &effect);       // 打开灯光
            }
            else
            {
                g_lightDev->TurnOffLight(g_lightInfo[0].lightId);               // 关闭灯光
            }
        }
        printf("\n");
        usleep(100*1000); //sleep 100ms
        // float res = TemperatureRead(rbuff, ret, handle);
        // printf("Temperature: %.1f", res);

        // 往主题发送数据
        float temperature = temp;
        ST_IOTA_BatchReportProperties brp = {0};
        brp.pcDeviceId = g_device_id ; // 设备标识 "503D2212X00009"
        brp.pcServiceId = g_service_id;
        char json_string[50];  // 存储 JSON 字符串的缓冲区
        sprintf(json_string, "{\"temperature\":\"%.1f\"}", temperature); // 使用 sprintf 将浮点数格式化为 JSON 格式的字符串
        brp.pcPayload = json_string;
        int  messageId = IOTA_ServiceCustomTopicReport(&brp, "1.0", "Temperature");
        printf("MQTT_Demo: Test_CustomTopicReport(), report data with messageId %d \n", messageId);
        

        // 以写入模式打开文件，若文件已存在，则覆盖内容
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            perror("无法打开文件");
            return 1;
        }

        // 写入字符串到文件
        fprintf(file, "%.1f\n", temp);

        // 关闭文件
        fclose(file);
    }

    return 0;
ERR:
    if(g_lightDev != NULL){
        FreeLightInterfaceInstance();                          //释放灯光设备接口实例
        g_lightDev = NULL;
    }

    UartClose(handle);                                         // 销毁UART设备句柄
    return ret;
 
    printf("---------------------------\n");

    

}

