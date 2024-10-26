#include "JsonParser.h"
#include <stdlib.h>

static void IOTA_getLocalTime(HW_CHAR* pcLocalTime)
{
#if defined(_WINDOWS) || defined(WIN32)
	SYSTEMTIME st;
	::GetLocalTime(&st);
	_snprintf_s(pcLocalTime, 31, 31, "%04d%02d%02dT%02d%02d%02dZ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
#else
	time_t tt;
	struct tm _tm;
	tt = time(&tt);
	localtime_r(&tt, &_tm);

	//struct timeval tv;
	//struct timezone tz;
	//::gettimeofday(&tv, &tz);
	//long int limillsec = tv.tv_usec / 1000;

	snprintf(pcLocalTime, 31, ("%04d%02d%02dT%02d%02d%02dZ"), _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday, _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
#endif

	return ;
}

HW_BOOL StructToJson_DeviceAdd(unsigned int uiMid, ST_IOTA_DEVICE_INFO* stDevices, HW_CHAR** pcJson)
{
	cJSON* json = NULL;
	cJSON* dev = NULL;
	cJSON* infos = NULL;
	json = cJSON_CreateObject();
	if (json)
	{
		cJSON_AddNumberToObject(json, "mid", uiMid);

		cJSON_AddItemToObject(json, "deviceInfos", infos = cJSON_CreateArray());
		cJSON_AddItemToArray(infos, dev = cJSON_CreateObject());
		cJSON_AddStringToObject(dev, "nodeId", stDevices->pcNodeId);
		cJSON_AddStringToObject(dev, "name", stDevices->pcName);
		cJSON_AddStringToObject(dev, "description", stDevices->pcDescription);
		cJSON_AddStringToObject(dev, "manufacturerId", stDevices->pcManufacturerId);
		cJSON_AddStringToObject(dev, "model", stDevices->pcModel);

		{
			*pcJson = cJSON_PrintUnformatted(json);
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL StructToJson_DeviceRemove(unsigned int uiMid, HW_CHAR* pcDeviceId, HW_CHAR** pcJson)
{
	cJSON* json = NULL;
	cJSON* request = NULL;
	json = cJSON_CreateObject();
	if (json)
	{
		cJSON_AddNumberToObject(json, "id", uiMid);
		cJSON_AddStringToObject(json, "deviceId", pcDeviceId);
		char t[32] = { 0 };
		IOTA_getLocalTime(t);
		cJSON_AddStringToObject(json, "requestTime", t);

		cJSON_AddItemToObject(json, "request", request = cJSON_CreateObject());

		{
			*pcJson = cJSON_PrintUnformatted(json);
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL StructToJson_DeviceStatusUpdate(unsigned int uiMid, ST_IOTA_DEVICE_STATUS* stStatuss, HW_CHAR** pcJson)
{
	cJSON* json = NULL;
	cJSON* status = NULL;
	cJSON* statuses = NULL;
	json = cJSON_CreateObject();
	if (json)
	{
		cJSON_AddNumberToObject(json, "mid", uiMid);

		cJSON_AddItemToObject(json, "deviceStatuses", statuses = cJSON_CreateArray());
		cJSON_AddItemToArray(statuses, status = cJSON_CreateObject());
		cJSON_AddStringToObject(status, "deviceId", stStatuss->pcDeviceId);
		cJSON_AddStringToObject(status, "status", stStatuss->pcStatus);

		{
			*pcJson = cJSON_PrintUnformatted(json);
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL StructToJson_DeviceQuery(unsigned int uiMid, HW_CHAR* pcNodeId, HW_CHAR** pcJson)
{
	cJSON* json = NULL;
	// cJSON* status = NULL;
	// cJSON* statuses = NULL;
	json = cJSON_CreateObject();
	if (json)
	{
		cJSON_AddNumberToObject(json, "mid", uiMid);
		cJSON_AddStringToObject(json, "nodeId", pcNodeId);

		{

			*pcJson = cJSON_PrintUnformatted(json);
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL StructToJson_ServiceDataReport(HW_CHAR* pcDeviceId, HW_CHAR* pcServiceId, HW_CHAR* pcServiceProperties, HW_CHAR** pcJson)
{
	cJSON* json = NULL;
	cJSON* data = NULL;
	cJSON* service = NULL;
	cJSON* services = NULL;
	cJSON* dev = NULL;
	cJSON* devs = NULL;
	json = cJSON_CreateObject();
	if (json)
	{
		cJSON_AddItemToObject(json, "devices", devs = cJSON_CreateArray());
		cJSON_AddItemToArray(devs, dev = cJSON_CreateObject());

		cJSON_AddStringToObject(dev, "deviceId", pcDeviceId);
		cJSON_AddItemToObject(dev, "services", services = cJSON_CreateArray());
		cJSON_AddItemToArray(services, service = cJSON_CreateObject());

		cJSON_AddStringToObject(service, "serviceId", pcServiceId);
		cJSON_AddItemToObject(service, "data", data = cJSON_Parse(pcServiceProperties));

		char t[32] = { 0 };
		IOTA_getLocalTime(t);
		cJSON_AddStringToObject(service, "eventTime", t);

		{
			*pcJson = cJSON_PrintUnformatted(json);
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL StructToJson_ServiceCommandResponse(unsigned int uiMid, unsigned int uiResultCode, HW_CHAR* pcCommandResponse, HW_CHAR** pcJson)
{
	cJSON* json = NULL;
	cJSON* body = NULL;
	json = cJSON_CreateObject();
	if (json)
	{
		cJSON_AddNumberToObject(json, "mid", uiMid);
		cJSON_AddStringToObject(json, "msgType", "deviceRsp");
		cJSON_AddNumberToObject(json, "errcode", uiResultCode);
		cJSON_AddItemToObject(json, "body", body = cJSON_Parse(pcCommandResponse));

		{
			*pcJson = cJSON_PrintUnformatted(json);
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL StructToJson_ServiceCustomTopicReport(ST_IOTA_BatchReportProperties* pstDevInfo, HW_CHAR** pcJson)
{
	cJSON* json = NULL;
	cJSON* data = NULL;
	cJSON* service = NULL;
	cJSON* services = NULL;
	cJSON* dev = NULL;
	cJSON* devs = NULL;
	json = cJSON_CreateObject();
	if (json)
	{
		cJSON_AddItemToObject(json, "devices", devs = cJSON_CreateArray());
		cJSON_AddItemToArray(devs, dev = cJSON_CreateObject());

		cJSON_AddStringToObject(dev, "deviceId", pstDevInfo->pcDeviceId);
		cJSON_AddItemToObject(dev, "services", services = cJSON_CreateArray());
		cJSON_AddItemToArray(services, service = cJSON_CreateObject());

		cJSON_AddStringToObject(service, "serviceId", pstDevInfo->pcServiceId);
		cJSON_AddItemToObject(service, "data", data = cJSON_Parse(pstDevInfo->pcPayload));

		char t[32] = { 0 };
		IOTA_getLocalTime(t);
		cJSON_AddStringToObject(service, "eventTime", t);

		{
			*pcJson = cJSON_PrintUnformatted(json);
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL JsonToStruct_DeviceAddRsp(HW_CHAR* pcJson, unsigned int* uiMid, int* iStatusCode, HW_CHAR* pcStatusDecs, ST_IOTA_ADD_DEVICE_RSP* stRsp)
{
	cJSON* json = NULL;
	cJSON* dataList = NULL;
	cJSON* data = NULL;
	cJSON* dev = NULL;
	json = cJSON_Parse(pcJson);
	if (json)
	{
		*uiMid = cJSON_GetObjectItem(json, "mid")->valueint;
		*iStatusCode = cJSON_GetObjectItem(json, "statusCode")->valueint;
		if (cJSON_HasObjectItem(json, "statusDesc"))
		{
			strcpy(pcStatusDecs, cJSON_GetObjectItem(json, "statusDesc")->valuestring);
		}
		dataList = cJSON_GetObjectItem(json, "data");
		for (int i = 0; i < cJSON_GetArraySize(dataList); i++)
		{
			data = cJSON_GetArrayItem(dataList, i);
			if (cJSON_HasObjectItem(data, "deviceInfo"))
			{
				dev = cJSON_GetObjectItem(data, "deviceInfo");
				if (cJSON_HasObjectItem(dev, "description"))
				{
					strcpy(stRsp->stDeviceInfos.pcDescription, cJSON_GetObjectItem(dev, "description")->valuestring);
				}
				strcpy(stRsp->stDeviceInfos.pcDeviceId, cJSON_GetObjectItem(dev, "deviceId")->valuestring);
				strcpy(stRsp->stDeviceInfos.pcManufacturerId, cJSON_GetObjectItem(dev, "manufacturerId")->valuestring);
				strcpy(stRsp->stDeviceInfos.pcModel, cJSON_GetObjectItem(dev, "model")->valuestring);
				strcpy(stRsp->stDeviceInfos.pcName, cJSON_GetObjectItem(dev, "name")->valuestring);
				strcpy(stRsp->stDeviceInfos.pcNodeId, cJSON_GetObjectItem(dev, "nodeId")->valuestring);
			}
			if (cJSON_HasObjectItem(data, "statusDesc"))
			{
				strcpy(stRsp->pcStatusDecs, cJSON_GetObjectItem(data, "statusDesc")->valuestring);
			}
			break;
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL JsonToStruct_DeviceStatusUpdateRsp(HW_CHAR* pcJson, unsigned int* uiMid, int* iStatusCode, HW_CHAR* pcStatusDecs, ST_IOTA_UPDATE_STATUS_RSP* stRsp)
{
	cJSON* json = NULL;
	cJSON* dataList = NULL;
	cJSON* data = NULL;
	json = cJSON_Parse(pcJson);
	if (json)
	{
		*uiMid = cJSON_GetObjectItem(json, "mid")->valueint;
		*iStatusCode = cJSON_GetObjectItem(json, "statusCode")->valueint;
		if (cJSON_HasObjectItem(json, "statusDesc"))
		{
			strcpy(pcStatusDecs, cJSON_GetObjectItem(json, "statusDesc")->valuestring);
		}
		dataList = cJSON_GetObjectItem(json, "data");
		for (int i = 0; i < cJSON_GetArraySize(dataList); i++)
		{
			data = cJSON_GetArrayItem(dataList, i);
			strcpy(stRsp->pcDeviceId, cJSON_GetObjectItem(data, "deviceId")->valuestring);

			if (cJSON_HasObjectItem(data, "statusDesc"))
			{
				strcpy(stRsp->pcStatusDecs, cJSON_GetObjectItem(data, "statusDesc")->valuestring);
			}
			break;
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL JsonToStruct_DeviceQueryRsp(HW_CHAR* pcJson, unsigned int* uiMid, int* iStatusCode, HW_CHAR* pcStatusDecs, ST_IOTA_DEVICE_INFO* stRsp, HW_CHAR* pcCount, HW_CHAR* pcMarker)
{
	cJSON* json = NULL;
	cJSON* dataList = NULL;
	cJSON* data = NULL;
	json = cJSON_Parse(pcJson);
	if (json)
	{
		*uiMid = cJSON_GetObjectItem(json, "mid")->valueint;
		*iStatusCode = cJSON_GetObjectItem(json, "statusCode")->valueint;
		if (cJSON_HasObjectItem(json, "statusDesc"))
		{
			strcpy(pcStatusDecs, cJSON_GetObjectItem(json, "statusDesc")->valuestring);
		}
		dataList = cJSON_GetObjectItem(json, "data");
		for (int i = 0; i < cJSON_GetArraySize(dataList); i++)
		{
			data = cJSON_GetArrayItem(dataList, i);
			strcpy(stRsp->pcDeviceId, cJSON_GetObjectItem(data, "deviceId")->valuestring);
			strcpy(stRsp->pcNodeId, cJSON_GetObjectItem(data, "nodeId")->valuestring);
			strcpy(stRsp->pcName, cJSON_GetObjectItem(data, "name")->valuestring);
			strcpy(stRsp->pcManufacturerId, cJSON_GetObjectItem(data, "manufactureId")->valuestring);
			strcpy(stRsp->pcModel, cJSON_GetObjectItem(data, "model")->valuestring);

			if (cJSON_HasObjectItem(data, "description"))
			{
				strcpy(stRsp->pcDescription, cJSON_GetObjectItem(data, "description")->valuestring);
			}
			break;
		}
		if (cJSON_HasObjectItem(json, "count"))
		{
			strcpy(pcCount, cJSON_GetObjectItem(json, "count")->valuestring);
		}
		if (cJSON_HasObjectItem(json, "marker"))
		{
			strcpy(pcMarker, cJSON_GetObjectItem(json, "marker")->valuestring);
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL JsonToStruct_Command(HW_CHAR* pcJson, ST_IOTA_COMMAND* stCommand)
{
	cJSON* json = NULL;
	cJSON* params = NULL;
	json = cJSON_Parse(pcJson);
	if (json)
	{
		stCommand->iMid = cJSON_GetObjectItem(json, "mid")->valueint;
		strcpy(stCommand->pcDeviceId, cJSON_GetObjectItem(json, "deviceId")->valuestring);
		strcpy(stCommand->pcMsgType, cJSON_GetObjectItem(json, "msgType")->valuestring);
		strcpy(stCommand->pcServiceId, cJSON_GetObjectItem(json, "serviceId")->valuestring);
		strcpy(stCommand->pcCmd, cJSON_GetObjectItem(json, "cmd")->valuestring);
		params = cJSON_GetObjectItem(json, "paras");

		char* s = cJSON_PrintUnformatted(params);
		if (s)
		{
			strcpy(stCommand->pcParas, s);
			free(s);
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}

HW_BOOL JsonToStruct_Response(HW_CHAR* pcJson, unsigned int* uiMid, int* iStatusCode, HW_CHAR* pcStatusDecs)
{
	cJSON* json = NULL;
	json = cJSON_Parse(pcJson);
	if (json)
	{
		*uiMid = cJSON_GetObjectItem(json, "mid")->valueint;
		*iStatusCode = cJSON_GetObjectItem(json, "statusCode")->valueint;
		if (cJSON_HasObjectItem(json, "statusDesc"))
		{
			strcpy(pcStatusDecs, cJSON_GetObjectItem(json, "statusDesc")->valuestring);
		}

		cJSON_Delete(json);
		return HW_TRUE;
	}
	else
	{
		return HW_FALSE;
	}
}


