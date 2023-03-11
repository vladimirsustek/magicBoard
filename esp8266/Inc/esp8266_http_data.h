/*
 * esp8266_http_data.h
 *
 *  Created on: Aug 16, 2022
 *      Author: 42077
 */

#ifndef ESP8266_HTTP_DATA_H_
#define ESP8266_HTTP_DATA_H_

#include "stdint.h"
/*Length of AT+CWJAP= + \r\n */
#define CWJAP_LNG 9
#define ATCMD_TERMINATION 2

extern const char * atCmd;
extern const char * atCmd_RST;
extern const char * atCmd_CWJAP;
extern const char * atCmd_CWQAP;
extern const char * atCmd_CWMODE;
extern const char * atCmd_CIPMUX;
#if STATIC_IP_AND_NEW_WIFI
const char * atCmd_CWSTAIP;
#endif
extern const char * atCmd_CWJAP;
extern const char * atCmd_CIPSERVER;
extern const char * atRsp_OK;
extern const char * atRsp_ready;
extern const char * atCmd_CIPSEND;
extern const char * atCmd_CIPCLOSE;
extern const char * atRsp_WifiGotIp;

#endif /* ESP8266_HTTP_DATA_H_ */
