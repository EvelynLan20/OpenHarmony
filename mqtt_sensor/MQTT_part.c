// 登录
int ret = IOTA_Login();
printf("MQTT_Demo: Test_Login(), ret = %d \n", ret);


// 订阅主题
if (!g_login)
{
    printf("Please login before this operation!\n");
    continue;
}
int  messageId = IOTA_SubscribeCustomTopic('1.0', 'Temperature'); // pcTopicVersion, pcCustomTopic
printf("MQTT_Demo: Test_CustomTopicSubscribe(), report data with messageId %d \n", messageId);

// 往主题发送数据
temperature = 收到的数据
ST_IOTA_BatchReportProperties brp = {0};
brp.pcDeviceId = g_device_id ; // 设备标识 "503D2212X00009"
brp.pcServiceId = g_service_id;
char json_string[50];  // 存储 JSON 字符串的缓冲区
sprintf(json_string, "{\"temperature\":\"%.1f\"}", temperature); // 使用 sprintf 将浮点数格式化为 JSON 格式的字符串
brp.pcPayload = str;
int  messageId = IOTA_ServiceCustomTopicReport(&brp, '1.0', 'Temperature');
printf("MQTT_Demo: Test_CustomTopicReport(), report data with messageId %d \n", messageId);





// char str[50] = "{\"msg\":\"";
// snprintf(str + strlen(str), sizeof(str) - strlen(str), "%s", data);
// snprintf(str + strlen(str), sizeof(str) - strlen(str), "%s", "\"}");
// printf("str: %s\n", str);