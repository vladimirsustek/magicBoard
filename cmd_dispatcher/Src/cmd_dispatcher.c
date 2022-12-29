#include "cmd_dispatcher.h"

/* This value must be incremented always when cmdTable increases*/
#define CMD_TABLE_SIZE  (uint8_t)(22)

static const CmdDisp_t cmdTable[CMD_TABLE_SIZE] = {

/* RDA5807M radio commands ------------------------------------*/
/*01*/    {CMD_METHOD_DO,  CMD_RDA5807M_INIT, CmdRDA5807mDoInit},
/*02*/    {CMD_METHOD_DO,  CMD_RDA5807M_RSET, CmdRDA5807mDoReset},

/*03*/    {CMD_METHOD_SET, CMD_RDA5807M_VOLM, CmdRDA5807mSetVolm},
/*04*/    {CMD_METHOD_SET, CMD_RDA5807M_FREQ, CmdRDA5807mSetFreq},
/*05*/    {CMD_METHOD_SET, CMD_RDA5807M_MUTE, CmdRDA5807mSetMute},

/*06*/    {CMD_METHOD_GET, CMD_RDA5807M_RSSI, CmdRDA5807mGetRSSI},
/*07*/    {CMD_METHOD_GET, CMD_RDA5807M_RDSR, CmdRDA5807mGetRDSR},
/*08*/    {CMD_METHOD_GET, CMD_RDA5807M_RDSS, CmdRDA5807mGetRDSS},
/*09*/    {CMD_METHOD_GET, CMD_RDA5807M_STRO, CmdRDA5807mGetSTRO},
/*10*/    {CMD_METHOD_GET, CMD_RDA5807M_CHST, CmdRDA5807mGetCHST},

/*11*/    {CMD_METHOD_GET, CMD_RDA5807M_BLKA, CmdRDA5807mGetBLKA},
/*12*/    {CMD_METHOD_GET, CMD_RDA5807M_BLKB, CmdRDA5807mGetBLKB},
/*13*/    {CMD_METHOD_GET, CMD_RDA5807M_BLKC, CmdRDA5807mGetBLKC},
/*14*/    {CMD_METHOD_GET, CMD_RDA5807M_BLKD, CmdRDA5807mGetBLKD},

/*15*/    {CMD_METHOD_GET, CMD_RDA5807M_ERRA, CmdRDA5807mGetERRA},
/*16*/    {CMD_METHOD_GET, CMD_RDA5807M_ERRB, CmdRDA5807mGetERRB},

/* ESP8266 commands --------------------------------------------*/
/*17*/    {CMD_METHOD_ATS, CMD_ESP8266_ATSET, CmdESPConsoleATCmd},
/*18*/    {CMD_METHOD_ATS, CMD_ESP8266_STRWR, CmdESPConsoleWrStr},

/* 25AA1024 commands -------------------------------------------*/
/*19*/    {CMD_METHOD_EES, CMD_25AAXXX_WBYTE, CmdNVMWriteBytes},
/*20*/    {CMD_METHOD_EEG, CMD_25AAXXX_RBYTE, CmdNVMReadBytes},

/* LEDs commands   ---------------------------------------------*/
/*21*/    {CMD_METHOD_SET, CMD_GREEN_LEDSTAT, CmdGreenLEDSetStat},
/*22*/    {CMD_METHOD_SET, CMD_WHITE_LEDSTAT, CmdWhiteLEDSetStat}

};

uint16_t CmdDispatch(const uint8_t* const pStrCmd, const uint16_t lng) {

    uint16_t result = CMD_RET_UKN;
    char strBuff[32];


    for(uint8_t idx = 0; idx < CMD_TABLE_SIZE; idx++) {

        if (!memcmp(pStrCmd, cmdTable[idx].cmdMethod, CMD_METHOD_LNG) &&
        !memcmp(pStrCmd + CMD_METHOD_LNG + CMD_DELIMITER_LNG, cmdTable[idx].cmdName, CMD_NAME_LNG)) {

            result = cmdTable[idx].cmdFunc(pStrCmd, lng);
            break;
        }
    }
    /* printf redirected to UART in usr_du*/

    switch(result)
    {
    case CMD_RET_UKN: { printf("CMD_RET_UKN\n"); } break;
    case CMD_RET_ERR: { printf("CMD_RET_ERR\n"); } break;
    case CMD_RET_OK: { printf("%s\n", pStrCmd); } break;
    case CMD_CUSTOM: {} break;
    default : {  printf(strBuff, "TBD\n"); }
    }

    return result;
}
