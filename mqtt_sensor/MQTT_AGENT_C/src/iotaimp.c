#include "iotaimp.h"
#include <stdlib.h>
//callback
PFN_LOG_CALLBACK_HANDLER g_pfnLogCb = NULL;
PFN_CALLBACK_HANDLER g_pfnCbConnectSuccess = NULL;
PFN_CALLBACK_HANDLER g_pfnCbConnectFailure = NULL;
PFN_CALLBACK_HANDLER g_pfnCbDisconnectSuccess = NULL;
PFN_CALLBACK_HANDLER g_pfnCbDisconnectFailure = NULL;
PFN_CALLBACK_HANDLER g_pfnCbConnectionLost = NULL;
PFN_CALLBACK_HANDLER g_pfnCbPublishSuccess = NULL;
PFN_CALLBACK_HANDLER g_pfnCbPublishFailure = NULL;
PFN_CALLBACK_HANDLER g_pfnCbSubscribeSuccess = NULL;
PFN_CALLBACK_HANDLER g_pfnCbSubscribeFailure = NULL;
PFN_CALLBACK_HANDLER g_pfnCbCommandArrived = NULL;
PFN_CALLBACK_HANDLER g_pfnCbDeviceAdditionResult = NULL;
PFN_CALLBACK_HANDLER g_pfnCbDeviceDeletionResult = NULL;
PFN_CALLBACK_HANDLER g_pfnCbDeviceUpdateResult = NULL;
PFN_CALLBACK_HANDLER g_pfnCbDeviceQueryResult = NULL;

PFN_CALLBACK_HANDLER_TOPIC g_pfnCallbackTopicHandler = NULL;

HW_CHAR g_pcDeviceId[IOTA_BUF_SIZE_64] = { 0 };
HW_CHAR g_pcPassword[IOTA_BUF_SIZE_64] = { 0 };
HW_CHAR g_pcUserName[IOTA_BUF_SIZE_64] = { 0 };
HW_CHAR g_pcClientId[IOTA_BUF_SIZE_64] = { 0 };
HW_CHAR g_pcServerAddr[IOTA_BUF_SIZE_64] = { 0 };
HW_CHAR g_pcCertPath[IOTA_BUF_SIZE_512] = { 0 };

HW_UINT g_uiPort = 0;
HW_UINT g_uiUrlPrefix = 0;
HW_UINT g_uiAuthMode = 0;
HW_UINT g_uiLogLocalNumber = 0;
HW_UINT g_uiLogLevel = 0;
HW_UINT g_uiKeepaliveTime = 0;
HW_UINT g_uiConnectTimeout = 0;
HW_UINT g_uiRetryInterval = 0;
HW_UINT g_uiQos = 0;

HW_CHAR g_pcTopicTopoAdd[IOTA_BUF_SIZE_128] = { 0 };
HW_CHAR g_pcTopicTopoAddResponse[IOTA_BUF_SIZE_128] = { 0 };
HW_CHAR g_pcTopicTopoUpdate[IOTA_BUF_SIZE_128] = { 0 };
HW_CHAR g_pcTopicTopoUpdateResponse[IOTA_BUF_SIZE_128] = { 0 };
HW_CHAR g_pcTopicTopoDelete[IOTA_BUF_SIZE_128] = { 0 };
HW_CHAR g_pcTopicTopoQuery[IOTA_BUF_SIZE_128] = { 0 };
HW_CHAR g_pcTopicTopoQueryResponse[IOTA_BUF_SIZE_128] = { 0 };
HW_CHAR g_pcTopicCommand[IOTA_BUF_SIZE_128] = { 0 };
HW_CHAR g_pcTopicCommandResponse[IOTA_BUF_SIZE_128] = { 0 };
HW_CHAR g_pcTopicDatas[IOTA_BUF_SIZE_128] = { 0 };


//mqtt
MQTTAsync g_client = NULL;

HW_CHAR g_pcWorkPath[IOTA_BUF_SIZE_512] = { 0 };
HW_CHAR g_pcLogPath[IOTA_BUF_SIZE_512] = { 0 };
	
static HW_VOID subscribe(void);

static void writeLog(int level, char* format, ...)
{
	if (g_pfnLogCb)
	{
		va_list vl;
		va_start(vl, format);
		g_pfnLogCb(level, format, vl);
	}
}

static void delivered(void* context, MQTTAsync_token dt)
{
	writeLog(2, "Message with token value %d delivery confirmed\n", dt);
}

static int msgarrvd(void* context, char* topicName, int topicLen, MQTTAsync_message* message)
{
	int iRet = 1;
	((char*)message->payload)[message->payloadlen] = '\0';
	writeLog(2, "Message arrived, topic: %s, len: %d, msgid: %d, message: %s\n", topicName, message->payloadlen, message->msgid, (char*)message->payload);

	HW_BOOL bRet = HW_FALSE;
	HW_UINT uiMid = 0;
	HW_INT iStatusCode = 0;
	HW_CHAR pcStatusDecs[IOTA_BUF_SIZE_64] = { 0 };

	if (0 == strcmp(topicName, g_pcTopicTopoAddResponse)
		|| 0 == strcmp(topicName, g_pcTopicTopoUpdateResponse)
		|| 0 == strcmp(topicName, g_pcTopicTopoQueryResponse))
	{
		bRet = JsonToStruct_Response((char*)message->payload, &uiMid, &iStatusCode, pcStatusDecs);
		if (bRet)
		{
			if (0 == strcmp(topicName, g_pcTopicTopoAddResponse))
			{
				if (g_pfnCbDeviceAdditionResult)
				{
					g_pfnCbDeviceAdditionResult(NULL, uiMid, iStatusCode, (char*)message->payload);
				}
			}
			else if (0 == strcmp(topicName, g_pcTopicTopoUpdateResponse))
			{
				if (g_pfnCbDeviceUpdateResult)
				{
					g_pfnCbDeviceUpdateResult(NULL, uiMid, iStatusCode, (char*)message->payload);
				}
			}
			else if (0 == strcmp(topicName, g_pcTopicTopoQueryResponse))
			{
				if (g_pfnCbDeviceQueryResult)
				{
					g_pfnCbDeviceQueryResult(NULL, uiMid, iStatusCode, (char*)message->payload);
				}
			}
		}
		else
		{
			writeLog(2, "JsonToStruct_Response, message: %s\n", (char*)message->payload);
		}
	}
	else if (0 == strcmp(topicName, g_pcTopicCommand))
	{
		if (g_pfnCbCommandArrived)
		{
			g_pfnCbCommandArrived(NULL, uiMid, iStatusCode, (char*)message->payload);
		}
	}
	else
	{
		if (g_pfnCallbackTopicHandler)
		{
			g_pfnCallbackTopicHandler(NULL, uiMid, iStatusCode, topicName, (char*)message->payload);
		}
	}

	MQTTAsync_freeMessage(&message);
	MQTTAsync_free(&topicName);

	return iRet;
}

static void connlost(void* context, char* cause)
{
	writeLog(2, "Connection lost! cause: %s\n", cause);

	if (g_pfnCbConnectionLost)
	{
		g_pfnCbConnectionLost(NULL, 0, 0, cause);
	}
}

static void onConnectSuccess(void* context, MQTTAsync_successData* response)
{
	writeLog(2, "onConnectSuccess! token: %d\n", response->token);

	if (g_pfnCbConnectSuccess)
	{
		g_pfnCbConnectSuccess(NULL, 0, 0, "");
	}

	subscribe();
}

static void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	writeLog(2, "onConnectFailure! token: %d, code: %d, message = %s\n", response->token, response->code, response->message);

	if (g_pfnCbConnectFailure)
	{
		g_pfnCbConnectFailure(NULL, 0, response->code, (HW_CHAR*)response->message);
	}
}

static void onSubscribeSuccess(void* context, MQTTAsync_successData* response)
{
	writeLog(2, "onSubscribeSuccess! token: %d\n", response->token);

	if (g_pfnCbSubscribeSuccess)
	{
		g_pfnCbSubscribeSuccess(NULL, 0, 0, "");
	}
}

static void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	writeLog(2, "onSubscribeFailure! token: %d, code: %d, message = %s\n", response->token, response->code, response->message);

	if (g_pfnCbSubscribeFailure)
	{
		g_pfnCbSubscribeFailure(NULL, 0, response->code, (HW_CHAR*)response->message);
	}
}

static void onDisconnectSuccess(void* context, MQTTAsync_successData* response)
{
	writeLog(2, "onDisconnectSuccess! token: %d\n", response->token);

	if (g_pfnCbDisconnectSuccess)
	{
		g_pfnCbDisconnectSuccess(NULL, 0, 0, "");
	}
}

static void onDisconnectFailure(void* context, MQTTAsync_failureData* response)
{
	writeLog(2, "onDisconnectFailure! token: %d, code: %d, message = %s\n", response->token, response->code, response->message);

	if (g_pfnCbDisconnectFailure)
	{
		g_pfnCbDisconnectFailure(NULL, 0, response->code, (HW_CHAR*)response->message);
	}
}

static void onSendSuccess(void* context, MQTTAsync_successData* response)
{
	writeLog(2, "onSendSuccess! token: %d\n", response->token);

	if (g_pfnCbPublishSuccess)
	{
		g_pfnCbPublishSuccess(NULL, 0, 0, "");
	}
}

static void onSendFailure(void* context, MQTTAsync_failureData* response)
{
	writeLog(2, "onSendFailure! token: %d, code: %d, message = %s\n", response->token, response->code, response->message);

	if (g_pfnCbPublishFailure)
	{
		g_pfnCbPublishFailure(NULL, 0, response->code, (HW_CHAR*)response->message);
	}
}



static HW_INT publishMessage(HW_CHAR* pcTopicName, HW_CHAR* pcMessage)
{
	HW_INT iRet = IOTA_SUCCESS;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	//MQTTAsync_token token;

	pubmsg.payload = (void*)pcMessage;
	pubmsg.payloadlen = strlen(pcMessage);
	pubmsg.qos = g_uiQos;
	pubmsg.retained = 0;

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = onSendSuccess;
	opts.onFailure = onSendFailure;
	opts.context = NULL;
	iRet = MQTTAsync_sendMessage(g_client, pcTopicName, &pubmsg, &opts);
	if (iRet != MQTTASYNC_SUCCESS)
	{
		writeLog(2, "Failed to publish message, return code %d\n", iRet);
		iRet = IOTA_FAILURE;
	}
	else
	{
		writeLog(2, "MQTTAsync_sendMessage, topic: %s, msg: %s\n", pcTopicName, pcMessage);
	}

	return iRet;
}

static HW_VOID subscribe(void)
{
	HW_CHAR* arraySubs[4];
	arraySubs[0] = g_pcTopicTopoAddResponse;
	arraySubs[1] = g_pcTopicTopoUpdateResponse;
	arraySubs[2] = g_pcTopicTopoQueryResponse;
	arraySubs[3] = g_pcTopicCommand;

	HW_INT iRet = IOTA_SUCCESS;
	for (int i = 0; i < 4; i++)
	{
		MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
		opts.onSuccess = onSubscribeSuccess;
		opts.onFailure = onSubscribeFailure;
		opts.context = NULL;
		iRet = MQTTAsync_subscribe(g_client, arraySubs[i], g_uiQos, &opts);
		if (iRet != MQTTASYNC_SUCCESS)
		{
			writeLog(2, "Failed to subscribe, return code %d\n", iRet);
		}
		else
		{
			writeLog(2, "MQTTAsync_subscribe, topic: %s\n", arraySubs[i]);
		}
	}
}



HW_INT IOTA_init(HW_CHAR* pcWorkPath, HW_CHAR* pcLogPath)
{
	strncpy(g_pcWorkPath, pcWorkPath, IOTA_BUF_SIZE_512 - 1);
	strncpy(g_pcLogPath, pcLogPath, IOTA_BUF_SIZE_512 - 1);
	return IOTA_SUCCESS;
}

HW_VOID IOTA_destroy()
{
	MQTTAsync_destroy(&g_client);
}

HW_VOID IOTA_setCallback(HW_INT iItem, PFN_CALLBACK_HANDLER pfnCallbackHandler)
{
	switch (iItem)
	{
	case EN_IOTA_CALLBACK_CONNECT_SUCCESS:
		g_pfnCbConnectSuccess = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_CONNECT_FAILURE:
		g_pfnCbConnectFailure = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_DISCONNECT_SUCCESS:
		g_pfnCbDisconnectSuccess = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_DISCONNECT_FAILURE:
		g_pfnCbDisconnectFailure = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_CONNECTION_LOST:
		g_pfnCbConnectionLost = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_PUBLISH_SUCCESS:
		g_pfnCbPublishSuccess = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_PUBLISH_FAILURE:
		g_pfnCbPublishFailure = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_SUBSCRIBE_SUCCESS:
		g_pfnCbSubscribeSuccess = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_SUBSCRIBE_FAILURE:
		g_pfnCbSubscribeFailure = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_COMMAND_ARRIVED:
		g_pfnCbCommandArrived = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_DEVICE_ADDITION_RESULT:
		g_pfnCbDeviceAdditionResult = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_DEVICE_DELETION_RESULT:
		g_pfnCbDeviceDeletionResult = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_DEVICE_UPDATE_RESULT:
		g_pfnCbDeviceUpdateResult = pfnCallbackHandler;
		break;
	case EN_IOTA_CALLBACK_DEVICE_QUERY_RESULT:
		g_pfnCbDeviceQueryResult = pfnCallbackHandler;
		break;
	default:
		break;
	}
}

HW_VOID IOTA_setCallbackWithTopic(PFN_CALLBACK_HANDLER_TOPIC pfnCallbackTopicHandler)
{
	g_pfnCallbackTopicHandler = pfnCallbackTopicHandler;
}

HW_VOID IOTA_setPrintLogCallback(PFN_LOG_CALLBACK_HANDLER pfnLogCallbackHandler)
{
	g_pfnLogCb = pfnLogCallbackHandler;
}

HW_INT IOTA_configSetStr(HW_INT iItem, HW_CHAR* pValue)
{
	int iRet = IOTA_SUCCESS;
	switch (iItem)
	{
	case EN_IOTA_CFG_DEVICEID:
		strncpy(g_pcDeviceId, pValue, IOTA_BUF_SIZE_64 - 1);
		sprintf(g_pcTopicTopoAdd, "%s%s%s", IOTA_TOPIC_PREFIX, g_pcDeviceId, IOTA_TOPIC_POSTFIX_TOPO_ADD);
		sprintf(g_pcTopicTopoAddResponse, "%s%s%s", IOTA_TOPIC_PREFIX, g_pcDeviceId, IOTA_TOPIC_POSTFIX_TOPO_ADDRESPONSE);
		sprintf(g_pcTopicTopoUpdate, "%s%s%s", IOTA_TOPIC_PREFIX, g_pcDeviceId, IOTA_TOPIC_POSTFIX_TOPO_UPDATE);
		sprintf(g_pcTopicTopoUpdateResponse, "%s%s%s", IOTA_TOPIC_PREFIX, g_pcDeviceId, IOTA_TOPIC_POSTFIX_TOPO_UPDATERESPONSE);
		sprintf(g_pcTopicTopoDelete, "%s%s%s", IOTA_TOPIC_PREFIX, g_pcDeviceId, IOTA_TOPIC_POSTFIX_TOPO_DELETE);
		sprintf(g_pcTopicTopoQuery, "%s%s%s", IOTA_TOPIC_PREFIX, g_pcDeviceId, IOTA_TOPIC_POSTFIX_TOPO_QUERY);
		sprintf(g_pcTopicTopoQueryResponse, "%s%s%s", IOTA_TOPIC_PREFIX, g_pcDeviceId, IOTA_TOPIC_POSTFIX_TOPO_QUERYRESPONSE);
		sprintf(g_pcTopicCommand, "%s%s%s", IOTA_TOPIC_PREFIX, g_pcDeviceId, IOTA_TOPIC_POSTFIX_COMMAND);
		sprintf(g_pcTopicCommandResponse, "%s%s%s", IOTA_TOPIC_PREFIX, g_pcDeviceId, IOTA_TOPIC_POSTFIX_COMMANDRESPONSE);
		sprintf(g_pcTopicDatas, "%s%s%s", IOTA_TOPIC_PREFIX, g_pcDeviceId, IOTA_TOPIC_POSTFIX_DATAS);
		break;
	case EN_IOTA_CFG_DEVICESECRET:
		strncpy(g_pcPassword, pValue, IOTA_BUF_SIZE_64 - 1);
		break;
	case EN_IOTA_CFG_USERNAME:
		strncpy(g_pcUserName, pValue, IOTA_BUF_SIZE_64 - 1);
		break;
	case EN_IOTA_CFG_CLIENTID:
		strncpy(g_pcClientId, pValue, IOTA_BUF_SIZE_64 - 1);
		break;
	case EN_IOTA_CFG_MQTT_ADDR:
		strncpy(g_pcServerAddr, pValue, IOTA_BUF_SIZE_64 - 1);
		break;
	default:
		iRet = IOTA_FAILURE;
		break;
	}
	return iRet;
}

HW_INT IOTA_configSetUint(HW_INT iItem, HW_UINT uiValue)
{
	int iRet = IOTA_SUCCESS;
	switch (iItem)
	{
	case EN_IOTA_CFG_MQTT_PORT:
		g_uiPort = uiValue;
		break;
	case EN_IOTA_CFG_MQTT_URL_PREFIX:
		g_uiUrlPrefix = uiValue;
		break;
	case EN_IOTA_CFG_AUTH_MODE:
		g_uiAuthMode = uiValue;
		break;
	case EN_IOTA_CFG_LOG_LOCAL_NUMBER:
		g_uiLogLocalNumber = uiValue;
		break;
	case EN_IOTA_CFG_LOG_LEVEL:
		g_uiLogLevel = uiValue;
		break;
	case EN_IOTA_CFG_KEEP_ALIVE_TIME:
		g_uiKeepaliveTime = uiValue;
		break;
	case EN_IOTA_CFG_CONNECT_TIMEOUT:
		g_uiConnectTimeout = uiValue;
		break;
	case EN_IOTA_CFG_RETRY_INTERVAL:
		g_uiRetryInterval = uiValue;
		break;
	case EN_IOTA_CFG_QOS:
		g_uiQos = uiValue;
		break;
	default:
		iRet = IOTA_FAILURE;
		break;
	}
	return iRet;
}

HW_INT IOTA_login()
{
	HW_INT iRet = IOTA_SUCCESS;
	
	HW_BOOL bCreate = HW_FALSE, bSet = HW_FALSE, bConnect = HW_FALSE;

	do
	{
		if (g_client == NULL)
		{
			HW_CHAR pcUrl[IOTA_BUF_SIZE_64] = { 0 };
			getUrl(pcUrl);
			iRet = MQTTAsync_create(&g_client, pcUrl, g_pcClientId, MQTTCLIENT_PERSISTENCE_NONE, NULL);
			if (iRet != MQTTASYNC_SUCCESS)
			{
				writeLog(2, "Failed to create client, return code %d\n", iRet);
				break;
			}
			bCreate = HW_TRUE;
			iRet = MQTTAsync_setCallbacks(g_client, NULL, connlost, msgarrvd, delivered);
			if (iRet != MQTTASYNC_SUCCESS)
			{
				writeLog(2, "Failed to set callbacks, return code %d\n", iRet);
				break;
			}
			bSet = HW_TRUE;
		}
		else
		{
			bCreate = HW_TRUE;
			bSet = HW_TRUE;
		}

		MQTTAsync_SSLOptions ssl_opts = MQTTAsync_SSLOptions_initializer;
		ssl_opts.trustStore = strlen(g_pcCertPath) == 0 ? "./mqttClientTrustCert.pem" : g_pcCertPath;
		ssl_opts.sslVersion = MQTT_SSL_VERSION_DEFAULT;

		MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
		conn_opts.keepAliveInterval = 20;
		conn_opts.onSuccess = onConnectSuccess;
		conn_opts.onFailure = onConnectFailure;
		conn_opts.username = g_pcUserName;
		conn_opts.password = g_pcPassword;
		conn_opts.context = NULL;
		conn_opts.ssl = &ssl_opts;
		iRet = MQTTAsync_connect(g_client, &conn_opts);
		if (iRet != MQTTASYNC_SUCCESS)
		{
			writeLog(2, "Failed to connect, return code %d\n", iRet);
			break;
		}
		bConnect = HW_TRUE;
	} while (0);

	if (!bSet && bCreate)
	{
		IOTA_destroy();
	}

	return iRet;
}

HW_INT IOTA_logout()
{
	HW_INT iRet = IOTA_SUCCESS;

	MQTTAsync_disconnectOptions disconn_opts = MQTTAsync_disconnectOptions_initializer;
	disconn_opts.onSuccess = onDisconnectSuccess;
	disconn_opts.onFailure = onDisconnectFailure;
	disconn_opts.context = NULL;
	iRet = MQTTAsync_disconnect(g_client, &disconn_opts);
	if (iRet != MQTTASYNC_SUCCESS)
	{
		writeLog(2, "Failed to disconnect, return code %d\n", iRet);
	}

	return iRet;
}

HW_INT IOTA_hubDeviceAdd(HW_UINT uiMid, ST_IOTA_DEVICE_INFO* pstInfo)
{
	int iRet = IOTA_SUCCESS;

	HW_CHAR *pcJson = NULL;
	if (StructToJson_DeviceAdd(uiMid, pstInfo, &pcJson))
	{
		publishMessage(g_pcTopicTopoAdd, pcJson);
	}
	else
	{
		iRet = IOTA_FAILURE;
	}

	if (pcJson)
	{
		free(pcJson);
	}

	return iRet;
}

HW_INT IOTA_hubDeviceRemove(HW_UINT uiMid, HW_CHAR* pcDeviceId)
{
	HW_INT iRet = IOTA_SUCCESS;

	HW_CHAR* pcJson = NULL;
	if (StructToJson_DeviceRemove(uiMid, pcDeviceId, &pcJson))
	{
		publishMessage(g_pcTopicTopoDelete, pcJson);
	}
	else
	{
		iRet = IOTA_FAILURE;
	}

	if (pcJson)
	{
		free(pcJson);
	}

	return iRet;
}

HW_INT IOTA_deviceStatusUpdate(HW_UINT uiMid, HW_CHAR* pcDeviceId, HW_CHAR* pcStatus)
{
	HW_INT iRet = IOTA_SUCCESS;

	HW_CHAR* pcJson = NULL;
	ST_IOTA_DEVICE_STATUS status;
	status.pcDeviceId = pcDeviceId;
	status.pcStatus = pcStatus;
	if (StructToJson_DeviceStatusUpdate(uiMid, &status, &pcJson))
	{
		publishMessage(g_pcTopicTopoUpdate, pcJson);
	}
	else
	{
		iRet = IOTA_FAILURE;
	}

	if (pcJson)
	{
		free(pcJson);
	}

	return iRet;
}

HW_INT IOTA_hubDeviceQuery(HW_UINT uiMid, HW_CHAR* pcMarker, HW_UINT uiLimit, ST_IOTA_DEVICE_INFO* pstDeviceInfo)
{
	int iRet = IOTA_SUCCESS;

	HW_CHAR* pcJson = NULL;
	if (StructToJson_DeviceQuery(uiMid, pstDeviceInfo->pcNodeId, &pcJson))
	{
		publishMessage(g_pcTopicTopoQuery, pcJson);
	}
	else
	{
		iRet = IOTA_FAILURE;
	}

	if (pcJson)
	{
		free(pcJson);
	}

	return iRet;
}

HW_INT IOTA_serviceDataReport(HW_CHAR* pcDeviceId, HW_CHAR* pcServiceId, HW_CHAR* pcServiceProperties)
{
	HW_INT iRet = IOTA_SUCCESS;

	HW_CHAR* pcJson = NULL;
	if (StructToJson_ServiceDataReport(pcDeviceId, pcServiceId, pcServiceProperties, &pcJson))
	{
		publishMessage(g_pcTopicDatas, pcJson);
	}
	else
	{
		iRet = IOTA_FAILURE;
	}

	if (pcJson)
	{
		free(pcJson);
	}

	return iRet;
}

HW_INT IOTA_serviceCommandResponse(HW_UINT uiMid, HW_UINT uiResultCode, HW_CHAR* pcCommandResponse)
{
	HW_INT iRet = IOTA_SUCCESS;

	HW_CHAR* pcJson = NULL;
	if (StructToJson_ServiceCommandResponse(uiMid, uiResultCode, pcCommandResponse, &pcJson))
	{
		publishMessage(g_pcTopicCommandResponse, pcJson);
	}
	else
	{
		iRet = IOTA_FAILURE;
	}

	if (pcJson)
	{
		free(pcJson);
	}

	return iRet;
}

HW_INT IOTA_serviceCustomTopicReport(ST_IOTA_BatchReportProperties* pstDevInfo, HW_CHAR* pcTopicVersion, HW_CHAR* pcCustomTopic)
{
	HW_INT iRet = IOTA_SUCCESS;

	if (pcTopicVersion == NULL || pcCustomTopic == NULL || pstDevInfo == NULL)
	{
		iRet = IOTA_PARAMETER_EMPTY;
		return iRet;
	}

	HW_CHAR* pcJson = NULL;
	HW_CHAR buf[256] = { 0 };
	snprintf(buf, 255, "/%s/%s/%s", pcTopicVersion, g_pcDeviceId, pcCustomTopic);
	if (StructToJson_ServiceCustomTopicReport(pstDevInfo, &pcJson))
	{
		publishMessage(buf, pcJson);
	}
	else
	{
		iRet = IOTA_FAILURE;
	}

	if (pcJson)
	{
		free(pcJson);
	}

	return iRet;
}

HW_INT IOTA_subscribeCustomTopic(HW_CHAR* pcTopicVersion, HW_CHAR* pcCustomTopic)
{
	HW_INT iRet = IOTA_SUCCESS;

	if (pcTopicVersion == NULL || pcCustomTopic == NULL)
	{
		iRet = IOTA_PARAMETER_EMPTY;
		return iRet;
	}

	HW_CHAR buf[256] = { 0 };
	snprintf(buf, 255, "/%s/%s/%s", pcTopicVersion, g_pcDeviceId, pcCustomTopic);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = onSubscribeSuccess;
	opts.onFailure = onSubscribeFailure;
	opts.context = NULL;
	iRet = MQTTAsync_subscribe(g_client, buf, g_uiQos, &opts);
	if (iRet != MQTTASYNC_SUCCESS)
	{
		writeLog(2, "Failed to subscribe, return code %d\n", iRet);
	}
	else
	{
		writeLog(2, "MQTTAsync_subscribe, topic: %s\n", buf);
	}
	return iRet;
}

HW_INT IOTA_setCertPath(HW_CHAR* pcCertPath)
{
	HW_INT iRet = IOTA_SUCCESS;
	strncpy(g_pcCertPath, pcCertPath, IOTA_BUF_SIZE_512 - 1);
	return iRet;
}


// static void timeSleep(int ms)
// {
// #if defined(WIN32) || defined(WIN64)
// 	Sleep(ms);
// #else
// 	usleep(ms * 1000);
// #endif
// }

void getUrl(HW_CHAR* pcUrl)
{
	if (pcUrl == NULL) {
		return;
	}

	if (g_uiPort == 1883)
	{
		sprintf(pcUrl, "tcp://%s:%d", g_pcServerAddr, g_uiPort);
	}
	else if (g_uiPort == 8443)
	{
		sprintf(pcUrl, "ssl://%s:%d", g_pcServerAddr, g_uiPort);
	}
	else if (g_uiPort == 22554)
	{
		sprintf(pcUrl, "tcp://%s:%d", g_pcServerAddr, g_uiPort);
	}
	else if (g_uiPort == 14079)
	{
		sprintf(pcUrl, "ssl://%s:%d", g_pcServerAddr, g_uiPort);
	}

	return ;
}


