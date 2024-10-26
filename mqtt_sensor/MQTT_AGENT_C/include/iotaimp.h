#pragma once
#include "define.h"
#include "iota.h"
#include "JsonParser.h"
#include "iota.h"
#include "MQTTAsync.h"


#ifndef _IOTA_IMP_H_
#define _IOTA_IMP_H_

#define IOTA_BUF_SIZE_64		64
#define IOTA_BUF_SIZE_128		128
#define IOTA_BUF_SIZE_256		256
#define IOTA_BUF_SIZE_512		512
#define IOTA_BUF_SIZE_1K		1024


HW_INT IOTA_init(HW_CHAR* pcWorkPath, HW_CHAR* pcLogPath);
HW_VOID IOTA_destroy();

HW_VOID IOTA_setCallback(HW_INT iItem, PFN_CALLBACK_HANDLER pfnCallbackHandler);

HW_VOID IOTA_setCallbackWithTopic(PFN_CALLBACK_HANDLER_TOPIC pfnCallbackTopicHandler);
HW_VOID IOTA_setPrintLogCallback(PFN_LOG_CALLBACK_HANDLER pfnLogCallbackHandler);

HW_INT IOTA_configSetStr(HW_INT iItem, HW_CHAR* pValue);
HW_INT IOTA_configSetUint(HW_INT iItem, HW_UINT uiValue);

HW_INT IOTA_login();
HW_INT IOTA_logout();


HW_INT IOTA_hubDeviceAdd(HW_UINT uiMid, ST_IOTA_DEVICE_INFO* pstInfo);
HW_INT IOTA_hubDeviceRemove(HW_UINT uiMid, HW_CHAR* pcDeviceId);
HW_INT IOTA_deviceStatusUpdate(HW_UINT uiMid, HW_CHAR* pcDeviceId, HW_CHAR* pcStatus);
HW_INT IOTA_hubDeviceQuery(HW_UINT uiMid, HW_CHAR* pcMarker, HW_UINT uiLimit, ST_IOTA_DEVICE_INFO* pstDeviceInfo);


HW_INT IOTA_serviceDataReport(HW_CHAR* pcDeviceId, HW_CHAR* pcServiceId, HW_CHAR* pcServiceProperties);
HW_INT IOTA_serviceCommandResponse(HW_UINT uiMid, HW_UINT uiResultCode, HW_CHAR* pcCommandResponse);

HW_INT IOTA_serviceCustomTopicReport(ST_IOTA_BatchReportProperties* pstDevInfo, HW_CHAR* pcTopicVersion, HW_CHAR* pcCustomTopic);
HW_INT IOTA_subscribeCustomTopic(HW_CHAR* pcTopicVersion, HW_CHAR* pcCustomTopic);

HW_INT IOTA_setCertPath(HW_CHAR* pcCertPath);

void getUrl(HW_CHAR* pcUrl);


#endif // _IOTA_IMP_H_
