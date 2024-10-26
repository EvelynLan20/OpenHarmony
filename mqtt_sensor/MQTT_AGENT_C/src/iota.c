#define IOTA_EXPORTS
#include "iota.h"
#include "iotaimp.h"

IOTA_API HW_INT IOTA_Init(HW_CHAR* pcWorkPath, HW_CHAR* pcLogPath)
{
	return IOTA_init(pcWorkPath, pcLogPath);
}

IOTA_API HW_VOID IOTA_Destroy()
{
	return IOTA_destroy();
}

IOTA_API HW_VOID IOTA_SetCallback(HW_INT iItem, PFN_CALLBACK_HANDLER pfnCallbackHandler)
{
	return IOTA_setCallback(iItem, pfnCallbackHandler);
}

IOTA_API HW_VOID IOTA_SetCallbackWithTopic(PFN_CALLBACK_HANDLER_TOPIC pfnCallbackTopicHandler)
{
	return IOTA_setCallbackWithTopic(pfnCallbackTopicHandler);
}

IOTA_API HW_VOID IOTA_SetPrintLogCallback(PFN_LOG_CALLBACK_HANDLER pfnLogCallbackHandler)
{
	return IOTA_setPrintLogCallback(pfnLogCallbackHandler);
}

IOTA_API HW_INT IOTA_ConfigSetStr(HW_INT iItem, HW_CHAR* pValue)
{
	return IOTA_configSetStr(iItem, pValue);
}

IOTA_API HW_INT IOTA_ConfigSetUint(HW_INT iItem, HW_UINT uiValue)
{
	return IOTA_configSetUint(iItem, uiValue);
}


IOTA_API HW_INT IOTA_Login()
{
	return IOTA_login();
}

IOTA_API HW_INT IOTA_Logout()
{
	return IOTA_logout();
}

IOTA_API HW_INT IOTA_HubDeviceAdd(HW_UINT uiMid, ST_IOTA_DEVICE_INFO* pstInfo)
{
	return IOTA_hubDeviceAdd(uiMid, pstInfo);
}

IOTA_API HW_INT IOTA_HubDeviceRemove(HW_UINT uiMid, HW_CHAR* pcDeviceId)
{
	return IOTA_hubDeviceRemove(uiMid, pcDeviceId);
}

IOTA_API HW_INT IOTA_DeviceStatusUpdate(HW_UINT uiMid, HW_CHAR* pcDeviceId, HW_CHAR* pcStatus)
{
	return IOTA_deviceStatusUpdate(uiMid, pcDeviceId, pcStatus);
}

IOTA_API HW_INT IOTA_HubDeviceQuery(HW_UINT uiMid, HW_CHAR* pcMarker, HW_UINT uiLimit, ST_IOTA_DEVICE_INFO* pstDeviceInfo)
{
	return IOTA_hubDeviceQuery(uiMid, pcMarker, uiLimit, pstDeviceInfo);
}

IOTA_API HW_INT IOTA_ServiceDataReport(HW_CHAR* pcDeviceId, HW_CHAR* pcServiceId, HW_CHAR* pcServiceProperties)
{
	return IOTA_serviceDataReport(pcDeviceId, pcServiceId, pcServiceProperties);
}

IOTA_API HW_INT IOTA_ServiceCommandResponse(HW_UINT uiMid, HW_UINT uiResultCode, HW_CHAR* pcCommandResponse)
{
	return IOTA_serviceCommandResponse(uiMid, uiResultCode, pcCommandResponse);
}

IOTA_API HW_INT IOTA_ServiceCustomTopicReport(ST_IOTA_BatchReportProperties* pstDevInfo, HW_CHAR* pcTopicVersion, HW_CHAR* pcCustomTopic)
{
	return IOTA_serviceCustomTopicReport(pstDevInfo, pcTopicVersion, pcCustomTopic);
}

IOTA_API HW_INT IOTA_SubscribeCustomTopic(HW_CHAR* pcTopicVersion, HW_CHAR* pcCustomTopic)
{
	return IOTA_subscribeCustomTopic(pcTopicVersion, pcCustomTopic);
}

IOTA_API HW_INT IOTA_SetCertPath(HW_CHAR* pcCertPath)
{
	return IOTA_setCertPath(pcCertPath);
}
