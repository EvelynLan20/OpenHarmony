#pragma once

#if defined(WIN32) || defined(WIN64) || defined(_MSC_VER)
#include "windows.h"
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>

#define HW_TRUE      1 /**< Indicates HW_TRUE */
#define HW_FALSE     0 /**< Indicates HW_FALSE */

#define HW_SUCCESS   0 /**< Indicates success */
#define HW_FAILED    1 /**< Indicates failed */
#define HW_NULL     ((void *)0) /**< Indicates null ptr */  

typedef int            HW_INT;  /**< Indicates type of int. */
typedef unsigned int   HW_UINT; /**< Indicates type of unsigned int. */
typedef char           HW_CHAR; /**< Indicates type of char. */
typedef unsigned char  HW_UCHAR;/**< Indicates type of unsigned char. */
typedef int            HW_BOOL; /**< Indicates type of HW_BOOL. */
typedef void           HW_VOID; /**< Indicates type of void. */

#define IOTA_SUCCESS 0
#define IOTA_FAILURE -1
#define IOTA_PARAMETER_EMPTY -101
#define IOTA_RESOURCE_NOT_AVAILABLE -102
#define IOTA_INITIALIZATION_REPEATED -103
#define IOTA_LIBRARY_LOAD_FAILED -104
#define IOTA_SECRET_ENCRYPT_FAILED -105
#define IOTA_MQTT_CONNECT_FAILED -106
#define IOTA_MQTT_CONNECT_EXISTED -107
#define IOTA_CERTIFICATE_NOT_FOUND -108

#define IOTA_TOPIC_PREFIX                               "/v1/devices/"
#define IOTA_TOPIC_POSTFIX_TOPO_ADD                     "/topo/add"
#define IOTA_TOPIC_POSTFIX_TOPO_ADDRESPONSE             "/topo/addResponse"
#define IOTA_TOPIC_POSTFIX_TOPO_UPDATE                  "/topo/update"
#define IOTA_TOPIC_POSTFIX_TOPO_UPDATERESPONSE          "/topo/updateResponse"
#define IOTA_TOPIC_POSTFIX_TOPO_DELETE                  "/topo/delete"
#define IOTA_TOPIC_POSTFIX_TOPO_QUERY                   "/topo/query"
#define IOTA_TOPIC_POSTFIX_TOPO_QUERYRESPONSE           "/topo/queryResponse"
#define IOTA_TOPIC_POSTFIX_COMMAND                      "/command"
#define IOTA_TOPIC_POSTFIX_COMMANDRESPONSE              "/commandResponse"
#define IOTA_TOPIC_POSTFIX_DATAS                        "/datas"


typedef enum enum_IOTA_CALLBACK_SETTING
{
    EN_IOTA_CALLBACK_CONNECT_SUCCESS = 11,
    EN_IOTA_CALLBACK_CONNECT_FAILURE = 12,
    EN_IOTA_CALLBACK_DISCONNECT_SUCCESS = 13,
    EN_IOTA_CALLBACK_DISCONNECT_FAILURE = 14,
    EN_IOTA_CALLBACK_CONNECTION_LOST = 15,

    EN_IOTA_CALLBACK_PUBLISH_SUCCESS = 21,
    EN_IOTA_CALLBACK_PUBLISH_FAILURE = 22,

    EN_IOTA_CALLBACK_SUBSCRIBE_SUCCESS = 31,
    EN_IOTA_CALLBACK_SUBSCRIBE_FAILURE = 32,

    EN_IOTA_CALLBACK_COMMAND_ARRIVED = 41,

    EN_IOTA_CALLBACK_DEVICE_ADDITION_RESULT = 51,
    EN_IOTA_CALLBACK_DEVICE_DELETION_RESULT = 52,
    EN_IOTA_CALLBACK_DEVICE_UPDATE_RESULT = 53,
    EN_IOTA_CALLBACK_DEVICE_QUERY_RESULT = 54,

}EN_IOTA_CALLBACK_SETTING;

typedef enum enum_IOTA_CFG_TYPE
{
    EN_IOTA_CFG_DEVICEID = 0,  //the value for this item is deviceId or nodeId
    EN_IOTA_CFG_DEVICESECRET = 1,
    EN_IOTA_CFG_USERNAME = 2,
    EN_IOTA_CFG_CLIENTID = 3,

    EN_IOTA_CFG_MQTT_ADDR = 5,
    EN_IOTA_CFG_MQTT_PORT = 6,
    EN_IOTA_CFG_MQTT_URL_PREFIX = 7,

    EN_IOTA_CFG_AUTH_MODE = 9, //for configuration value of this item, please refer to @enum_IOTA_CFG_AUTH_MODE below

    EN_IOTA_CFG_LOG_LOCAL_NUMBER = 10, //take effect only when syslog is available
    EN_IOTA_CFG_LOG_LEVEL = 11, //take effect only when syslog is available

    EN_IOTA_CFG_KEEP_ALIVE_TIME = 12,
    EN_IOTA_CFG_CONNECT_TIMEOUT = 13,
    EN_IOTA_CFG_RETRY_INTERVAL = 14,

    EN_IOTA_CFG_RESET_SECRET_IN_PROGRESS = 15,

    EN_IOTA_CFG_QOS = 16,

}EN_IOTA_CFG_TYPE;

typedef enum enum_IOTA_CFG_AUTH_MODE
{
    EN_IOTA_CFG_AUTH_MODE_DEVICE_ID = 0,
    EN_IOTA_CFG_AUTH_MODE_NODE_ID = 2,
}ENUM_IOTA_CFG_AUTH_MODE;

typedef enum enum_IOTA_CFG_URL_PREFIX
{
    EN_IOTA_CFG_URL_PREFIX_TCP = 0,
    EN_IOTA_CFG_URL_PREFIX_SSL = 1,
}ENUM_IOTA_CFG_URL_PREFIX;

typedef enum enum_LOG_LEVEL
{
    EN_LOG_LEVEL_DEBUG = 0,
    EN_LOG_LEVEL_INFO = 1,
    EN_LOG_LEVEL_WARNING = 2,
    EN_LOG_LEVEL_ERROR = 3,

}LOGLEVEL;

typedef struct stru_ST_IOTA_DEVICE_INFO
{
    HW_CHAR* pcNodeId;
    HW_CHAR* pcDeviceId;
    HW_CHAR* pcName;
    HW_CHAR* pcDescription;
    HW_CHAR* pcManufacturerId;
    HW_CHAR* pcManufacturerName;
    HW_CHAR* pcModel;
}ST_IOTA_DEVICE_INFO;

typedef struct stru_ST_IOTA_ADD_DEVICE_RSP
{
    HW_INT iStatusCode;
    HW_CHAR* pcStatusDecs;
    ST_IOTA_DEVICE_INFO stDeviceInfos;
}ST_IOTA_ADD_DEVICE_RSP;

typedef struct stru_ST_IOTA_BatchReportProperties
{
    HW_CHAR* pcDeviceId;
    HW_CHAR* pcServiceId;
    HW_CHAR* pcPayload;
}ST_IOTA_BatchReportProperties;

typedef struct stru_ST_IOTA_DEVICE_STATUS
{
    HW_CHAR* pcDeviceId;
    HW_CHAR* pcStatus;
}ST_IOTA_DEVICE_STATUS;

typedef struct stru_ST_IOTA_UPDATE_STATUS_RSP
{
    HW_INT iStatusCode;
    HW_CHAR* pcStatusDecs;
    HW_CHAR* pcDeviceId;
}ST_IOTA_UPDATE_STATUS_RSP;

typedef struct stru_ST_IOTA_COMMAND
{
    HW_CHAR* pcDeviceId;
    HW_CHAR* pcMsgType;
    HW_CHAR* pcServiceId;
    HW_CHAR* pcCmd;
    HW_CHAR* pcParas;        //ObjectNode, json string
    HW_INT iMid;
}ST_IOTA_COMMAND;

typedef struct stru_ST_IOTA_COMMAND_RSP
{
    HW_CHAR* pcMsgType;
    HW_INT iMid;
    HW_INT iErrorCode;
    HW_CHAR* pcBody;        //ObjectNode, json string
}ST_IOTA_COMMAND_RSP;

typedef struct stru_ST_IOTA_SERVICES
{
    HW_CHAR* pcServiceId;
    HW_CHAR* pcData;        //ObjectNode, json string
    HW_CHAR* pcEventTime;
}ST_IOTA_SERVICES;