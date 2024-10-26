#ifndef _IOTA_H_
#define _IOTA_H_

#include "define.h"
#include "stdarg.h"

typedef HW_VOID(*PFN_CALLBACK_HANDLER)(HW_VOID* context, HW_INT messageId, HW_INT code, HW_CHAR* message);
typedef HW_VOID(*PFN_CALLBACK_HANDLER_TOPIC)(HW_VOID* context, HW_INT messageId, HW_INT code, const HW_CHAR* topic, HW_CHAR* message);
typedef HW_VOID(*PFN_LOG_CALLBACK_HANDLER)(int level, char* format, va_list args);


#if defined(_WINDOWS) || defined(WIN32)
#ifdef IOTA_EXPORTS
#define IOTA_API extern "C" __declspec(dllexport)
#else
#define IOTA_API extern "C" __declspec(dllimport)
#endif
#else
#define IOTA_API
#endif

IOTA_API HW_INT IOTA_Init(HW_CHAR* pcWorkPath, HW_CHAR* pcLogPath);
IOTA_API HW_VOID IOTA_Destroy();

IOTA_API HW_VOID IOTA_SetCallback(HW_INT iItem, PFN_CALLBACK_HANDLER pfnCallbackHandler);

IOTA_API HW_VOID IOTA_SetCallbackWithTopic(PFN_CALLBACK_HANDLER_TOPIC pfnCallbackTopicHandler);
IOTA_API HW_VOID IOTA_SetPrintLogCallback(PFN_LOG_CALLBACK_HANDLER pfnLogCallbackHandler);

IOTA_API HW_INT IOTA_ConfigSetStr(HW_INT iItem, HW_CHAR* pValue);
IOTA_API HW_INT IOTA_ConfigSetUint(HW_INT iItem, HW_UINT uiValue);

IOTA_API HW_INT IOTA_Login();
IOTA_API HW_INT IOTA_Logout();


IOTA_API HW_INT IOTA_HubDeviceAdd(HW_UINT uiMid, ST_IOTA_DEVICE_INFO* pstInfo);
IOTA_API HW_INT IOTA_HubDeviceRemove(HW_UINT uiMid, HW_CHAR* pcDeviceId);
IOTA_API HW_INT IOTA_DeviceStatusUpdate(HW_UINT uiMid, HW_CHAR* pcDeviceId, HW_CHAR* pcStatus);
IOTA_API HW_INT IOTA_HubDeviceQuery(HW_UINT uiMid, HW_CHAR* pcMarker, HW_UINT uiLimit, ST_IOTA_DEVICE_INFO* pstDeviceInfo);


IOTA_API HW_INT IOTA_ServiceDataReport(HW_CHAR* pcDeviceId, HW_CHAR* pcServiceId, HW_CHAR* pcServiceProperties);
IOTA_API HW_INT IOTA_ServiceCommandResponse(HW_UINT uiMid, HW_UINT uiResultCode, HW_CHAR* pcCommandResponse);

IOTA_API HW_INT IOTA_ServiceCustomTopicReport(ST_IOTA_BatchReportProperties* pstDevInfo, HW_CHAR* pcTopicVersion, HW_CHAR* pcCustomTopic);
IOTA_API HW_INT IOTA_SubscribeCustomTopic(HW_CHAR* pcTopicVersion, HW_CHAR* pcCustomTopic);

IOTA_API HW_INT IOTA_SetCertPath(HW_CHAR* pcCertPath);

#endif // _IOTA_H_
