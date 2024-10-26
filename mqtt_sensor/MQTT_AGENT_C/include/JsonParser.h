#pragma once
#include "define.h"
#include "cJSON.h"


HW_BOOL StructToJson_DeviceAdd(unsigned int uiMid, ST_IOTA_DEVICE_INFO* stDevices, HW_CHAR** pcJson);

HW_BOOL StructToJson_DeviceRemove(unsigned int uiMid, HW_CHAR* pcDeviceId, HW_CHAR** pcJson);

HW_BOOL StructToJson_DeviceStatusUpdate(unsigned int uiMid, ST_IOTA_DEVICE_STATUS* stStatuss, HW_CHAR** pcJson);

HW_BOOL StructToJson_DeviceQuery(unsigned int uiMid, HW_CHAR* pcNodeId, HW_CHAR** pcJson);

HW_BOOL StructToJson_ServiceDataReport(HW_CHAR* pcDeviceId, HW_CHAR* pcServiceId, HW_CHAR* pcServiceProperties, HW_CHAR** pcJson);

HW_BOOL StructToJson_ServiceCommandResponse(unsigned int uiMid, unsigned int uiResultCode, HW_CHAR* pcCommandResponse, HW_CHAR** pcJson);

HW_BOOL StructToJson_ServiceCustomTopicReport(ST_IOTA_BatchReportProperties* pstDevInfo, HW_CHAR** pcJson);




HW_BOOL JsonToStruct_DeviceAddRsp(HW_CHAR* pcJson, unsigned int* uiMid, int* iStatusCode, HW_CHAR* pcStatusDecs, ST_IOTA_ADD_DEVICE_RSP* stRsp);

HW_BOOL JsonToStruct_DeviceStatusUpdateRsp(HW_CHAR* pcJson, unsigned int* uiMid, int* iStatusCode, HW_CHAR* pcStatusDecs, ST_IOTA_UPDATE_STATUS_RSP* stRsp);

HW_BOOL JsonToStruct_DeviceQueryRsp(HW_CHAR* pcJson, unsigned int* uiMid, int* iStatusCode, HW_CHAR* pcStatusDecs, ST_IOTA_DEVICE_INFO* stRsp, HW_CHAR* pcCount, HW_CHAR* pcMarker);

HW_BOOL JsonToStruct_Command(HW_CHAR* pcJson, ST_IOTA_COMMAND* stCommand);

HW_BOOL JsonToStruct_Response(HW_CHAR* pcJson, unsigned int* uiMid, int* iStatusCode, HW_CHAR* pcStatusDecs);


