//
// Created by cheolsoon on 25. 7. 23.
//
#ifndef BT_H
#define BT_H

#include <stdarg.h>
#include <stdint.h>

#include "../common/BaseDef.h"
#include "../hal/Serial_IO.h"
#include "../app/Log.h"

#define RESPONCE_MAX_LEN 64

extern int32_t BT_Serial_Connection_Flag;
extern int32_t BT_Connection_Flag;
extern int32_t Send_Flag;

// ===== ENUM 정의 =====
typedef enum {
    AT = 0,
    ATZ,
    ATF,
    AT_BTINFO,
    AT_BTINFO_N,
    AT_BTINFODEV_N,
    AT_BTCOUNT,
    AT_BTRSSI,
    AT_BTLQ,
    AT_ESCAPE,
    ATO,
    ATO_N,
    AT_BTCANCEL,
    AT_BTSCAN,
    AT_BTSCAN_N_T0,
    AT_BTSCAN_NUMBER_T0,
    AT_BTINQ,
    ATD,
    ATD_NUMBER,
    ATH,
    AT_BTLPM,
    AT_BTUART,
    AT_BTSEC,
    AT_BTMODE,
    AT_BTOPMODE,
    AT_BTMSG,
    AT_BTNAME,
    AT_BTKEY,
    AT_BTLT,
    AT_BTSETGUI,
    AT_BTROLE,
    AT_BTDEV,
    AT_SETESC,
    AT_SETDEBUG,
    AT_BTBUFF,
    AT_BTADDR,
    AT_BTDUT,
    AT_BTDFU,
    AT_BTFASTCH,
    AT_CMD_COUNT
} AT_CommandID;

// ===== 명령 템플릿 테이블 =====
typedef struct {
    const char* template;
    const char* description;
} AT_CommandEntry;

static const AT_CommandEntry AT_COMMANDS[AT_CMD_COUNT] = {
    [AT]                    = { "AT\r",                 "Connection Test" },
    [ATZ]                   = { "ATZ\r",                "Soft Reset" },
    [ATF]                   = { "AT&F\r",               "Hardware Reset" },
    [AT_BTINFO]             = { "AT+BTINFO?\r",         "Send current BT status to host" },
    [AT_BTINFO_N]           = { "AT+BTINFO?%d\r",       "Send environment values to host depending on n" },
    [AT_BTINFODEV_N]        = { "AT+BTINFODEV?%d\r",    "Send BT address of MASTER connected to SLAVE (1:N)" },
    [AT_BTCOUNT]            = { "AT+BTCOUNT?\r",        "Send count of connected MASTERs to host (1:N)" },
    [AT_BTRSSI]             = { "AT+BTRSSI?\r",         "Send RSSI of current BT connection to host" },
    [AT_BTLQ]               = { "AT+BTLQ?\r",           "Send Link Quality of current BT connection" },
    [AT_ESCAPE]             = { "+++\r",                "Switch from BYPASS to command mode" },
    [ATO]                   = { "ATO\r",                "Switch from command mode to BYPASS" },
    [ATO_N]                 = { "ATO%d\r",              "Switch specific MASTER n to BYPASS mode (1:N)" },
    [AT_BTCANCEL]           = { "AT+BTCANCEL\r",        "Cancel current BT operation" },
    [AT_BTSCAN]             = { "AT+BTSCAN\r",          "Enable inquiry and page scan" },
    [AT_BTSCAN_N_T0]        = { "AT+BTSCAN,%s,%d\r",    "Selectively enable inquiry and page scan" },
    [AT_BTSCAN_NUMBER_T0]   = { "AT+BTSCAN,%d,%d\r",    "Selectively enable inquiry for specific address 'NUMBER'"},
    [AT_BTINQ]              = { "AT+BTINQ?\r",          "Scan nearby Bluetooth devices" },
    [ATD]                   = { "ATD\r",                "Reconnect to the most recently connected device" },
    [ATD_NUMBER]            = { "ATD%s\r",              "Connect to device with specified BT address" },
    [ATH]                   = { "ATH\r",                "Disconnect current connection" },
    [AT_BTLPM]              = { "AT+BTLPM,%d\r",        "Set low power mode usage" },
    [AT_BTUART]             = { "AT+BTUART,%d,%d,%d\r", "Set UART parameters (baud, parity, stop)" },
    [AT_BTSEC]              = { "AT+BTSEC,%d,%d\r",     "Set Authentication and Encryption options" },
    [AT_BTMODE]             = { "AT+BTMODE,%d\r",       "Set BT connection mode" },
    [AT_BTOPMODE]           = { "AT+BTOPMODE,%d\r",     "Enable or disable 1:N communication" },
    [AT_BTMSG]              = { "AT+BTMSG,%d\r",        "Enable/disable status message output" },
    [AT_BTNAME]             = { "AT+BTNAME=%s\r",       "Set Bluetooth device name" },
    [AT_BTKEY]              = { "AT+BTKEY=%s\r",        "Set Bluetooth PIN code" },
    [AT_BTLT]               = { "AT+BTLT=%d\r",         "Set connection check interval" },
    [AT_BTSETGUI]           = { "AT+BTSETGUI\r",        "Temporarily output status message" },
    [AT_BTROLE]             = { "AT+BTROLE=%c\r",       "Set role (Master/Slave)" },
    [AT_BTDEV]              = { "AT+BTDEV=%d\r",        "Set number of target devices (1:N)" },
    [AT_SETESC]             = { "AT+SETESC=%d\r",       "Change escape sequence character" },
    [AT_SETDEBUG]           = { "AT+SETDEBUG=%d\r",     "Set debug character" },
    [AT_BTBUFF]             = { "AT+BTBUFF=%d\r",       "Set send/receive buffer (1:N)" },
    [AT_BTADDR]             = { "AT+BTADDR=%s\r",       "Change last connected device's address" },
    [AT_BTDUT]              = { "AT+BTDUT\r",           "Enter Device Under Test mode" },
    [AT_BTDFU]              = { "AT+BTDFU\r",           "Enter Firmware Update mode" },
    [AT_BTFASTCH]           = { "AT+BTFASTCH,%d\r",     "Change Stream Switching Fast parameter" }
};


int32_t BT_Init();
int32_t BT_Rx_Handler(char* data, size_t max_len);
int32_t BT_Tx_Handler(const char*);
int32_t BT_CMD_Send(AT_CommandID, ...);
int32_t BT_Close();

#endif // BT_H