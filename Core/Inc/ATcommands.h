/*
 * ATcommands.h
 *
 *  Created on: Jun 18, 2023
 *      Author: rini
 */

#ifndef INC_ATCOMMANDS_H_
#define INC_ATCOMMANDS_H_

#include "printf.h"
#include "stdarg.h"

// It is assumed that there is no AT command longer than this
#define AT_CMD_BUFFER_SIZE			128
#define AT_CMD_ERROR				-1


// ------------------------------------------------------------ EspressIf AT-Command-Macros
// --------------------------------------------- Basic AT Commands (Page 10)
#define AT_Startup							"AT"					// Tests AT startup.
#define AT_Reset							"AT+RST"				// Restarts a module.
#define AT_Version							"AT+GMR"				// Checks version information.
#define AT_DeepSleepMode					"AT+GSLP"				// Enters Deep-sleep mode.
#define	AT_Echo								"ATE"					// Configures echoing of AT commands.
#define AT_FactoryReset						"AT+RESTORE"			// Restores the factory default settings of the module.
#define AT_UARTConfig						"AT+UART_CUR"			// Current UART configuration.
#define AT_UARTDefault						"AT+UART_DEF"			// Default UART configuration, saved in flash.
#define AT_SleepMode						"AT+SLEEP"				// Sets the sleep mode.
#define AT_RAMSpace							"AT+SYSRAM"				// Checks the remaining space of RAM.
#define AT_UserFlash						"AT+SYSFLASH"			// Sets user partitions in flash
#define AT_FileSystemOperation				"AT+SYSFS"				// File systems operations
#define AT_RFPower							"AT+RFPOWER"			// Sets RF TX power

// --------------- General parameters
#define AT_Enable							1U
#define AT_Disable							0U

// --------------------------------------------- Wi-Fi AT Commands (Page 18)
#define AT_WIFI_Set_Mode					"AT+CWMODE"				// Sets the Wi-Fi mode (STA/AP/STA+AP).
#define AT_WIFI_Connect						"AT+CWJAP"				// Connects to an AP.
#define AT_WIFI_Set_ListAP_Config			"AT+CWLAPOPT"			// Sets the configuration of command AT+CWLAP.
#define AT_WIFI_ListAP						"AT+CWLAP"				// Lists available APs.
#define AT_WIFI_Disconnect					"AT+CWQAP"				// Disconnects from the AP.
#define AT_WIFI_Set_SoftAP_Config			"AT+CWSAP"				// Sets the configuration of the ESP32 SoftAP.
#define AT_WIFI_Get_Station_IP				"AT+CWLIF"				// Gets the Station IP to which the ESP32 SoftAP is connected.
#define AT_WIFI_Enable_DHCP					"AT+CWDHCP"				// Enables/disables DHCP.
#define AT_WIFI_Set_DHCP_Range				"AT+CWDHCPS"			// Sets the IP range of the ESP32 SoftAP DHCP server. Saves the setting in flash.
#define AT_WIFI_SoftAP_Autoconnect			"AT+CWAUTOCONN"			// Connects to the AP automatically on power-up.
#define AT_WIFI_SmartConfig_Start			"AT+CWSTARTSMART"		// Starts SmartConfig.
#define AT_WIFI_SmartConfig_Stop			"AT+CWSTOPSMART"		// Stops SmartConfig.
#define AT_WIFI_Enable_WPS					"AT+WPS"				// Enables the WPS function.
#define AT_WIFI_Set_Hostname				"AT+CWHOSTNAME"			// Configure the host name of ESP32 station.
#define AT_WIFI_MDNS						"AT+MDNS"				// MDNS function
// --------------- WIFI Mode Parameter (AT+CWMODE)
#define AT_WIFI_Null_Mode					0U
#define AT_WIFI_Station_Mode				1U
#define AT_WIFI_SoftAP_Mode					2U
#define AT_WIFI_Mixed_Mode					3U
// --------------- Access Point connection error codes (AT+CWJAP)
#define AT_WIFI_AP_Timeout					1U
#define AT_WIFI_AP_InvalidPassword			2U
#define AT_WIFI_AP_UnavailableTarget		3U
#define AT_WIFI_AP_ConnectionFailed			4U
// --------------- ListAP Configuration parameters (AT+CWLAPOPT)
#define AT_WIFI_ListAP_Ordered_RSSI			0U
#define AT_WIFI_ListAP_Unordered_RSSI		1U
#define AT_WIFI_Mask_ListAP_ECN				(1 << 0)
#define AT_WIFI_Mask_ListAP_SSID			(1 << 1)
#define AT_WIFI_Mask_ListAP_RSSI			(1 << 2)
#define AT_WIFI_Mask_ListAP_MAC				(1 << 3)
#define AT_WIFI_Mask_ListAP_CHANNEL			(1 << 4)
// --------------- WiFi Encryption method parameters (AT+CWLAP,AT+CWSAP)
// (Warning: Not all are applicable for every command!)
#define AT_WIFI_Encryption_OPEN				0U
#define AT_WIFI_Encryption_WEP				1U
#define AT_WIFI_Encryption_WPA_PSK			2U
#define AT_WIFI_Encryption_WPA2_PSK			3U
#define AT_WIFI_Encryption_WPA_WPA2_PSK		4U
#define AT_WIFI_Encryption_WPA2_Enterprise	5U
// --------------- DHCP Enable parameters (AT+CWDHCP)
#define AT_WIFI_Mask_Enable_DHCP			(1 << 0)
#define AT_WIFI_Mask_Enable_Station_DHCP	(1 << 0)
#define AT_WIFI_Mask_Enable_SoftAP_DHCP		(1 << 1)
// --------------- SmartConfig parameters (AT+CWSTARTSMART)
#define AT_WIFI_SmartConfig_Type_ESP_TOUCH	1U
#define AT_WIFI_SmartConfig_Type_AirKiss	2U
#define AT_WIFI_SmartConfig_Type_Mixed		3U

// --------------------------------------------- TCP/IP-Related AT Commands (Page 29)
#define AT_IP_ConnectionStatus				"AT+CIPSTATUS"			// Gets the connection status.
#define AT_IP_Domain						"AT+CIPDOMAIN"			// DNS function.
#define AT_IP_Set_DNS_Config				"AT+CIPDNS"				// Sets user-defined DNS server.
#define AT_IP_Set_Station_MAC				"AT+CIPSTAMAC"			// Sets the MAC address of ESP32 Station.
#define AT_IP_Set_SoftAP_MAC				"AT+CIPAPMAC"			// Sets the MAC address of ESP32 SoftAP.
#define AT_IP_Set_Station_IP				"AT+CIPSTA"				// Sets the IP address of ESP32 Station.
#define AT_IP_Set_SoftAP_IP					"AT+CIPAP"				// Sets the IP address of ESP32 SoftAP.
#define AT_IP_OpenConnection				"AT+CIPSTART"			// Establishes TCP connection, UDP transmission or SSL connection.
#define AT_IP_Set_SSL_Config				"AT+CIPSSLCCONF"		// Sets configuration of SSL client
#define AT_IP_Send							"AT+CIPSEND"			// Sends data.
#define AT_IP_Send_Length					"AT+CIPSENDEX"			// Sends data when length of data is <length>, or when \0 appears in the data.
#define AT_IP_CloseConnection				"AT+CIPCLOSE"			// Closes TCP/UDP/SSL connection.
#define AT_IP_Get_Local_Address				"AT+CIFSR"				// Gets the local IP address.
#define AT_IP_Set_MultiConnectionMode		"AT+CIPMUX"				// Configures the multiple connections mode.
#define AT_IP_Server						"AT+CIPSERVER"			// Deletes/Creates TCP or SSL server.
#define AT_IP_Set_Server_Max_Connections	"AT+CIPSERVERMAXCONN"	// Set the maximum connections that server allows
#define AT_IP_Set_TransmissionMode			"AT+CIPMODE"			// Configures the transmission mode.
#define AT_IP_SaveTransmissionLink			"AT+SAVETRANSLINK"		// Saves the transparent transmission link in flash.
#define AT_IP_Set_TCP_Timeout				"AT+CIPSTO"				// Sets timeout when ESP32 runs as a TCP server.
#define AT_IP_SoftwareUpdate				"AT+CIUPDATE"			// Updates the software through Wi-Fi.
#define AT_IP_Get_Remote_Information		"AT+CIPDINFO"			// Shows remote IP and remote port with +IPD.
#define AT_IP_Set_SNTP_Config				"AT+CIPSNTPCFG"			// Configures the time domain and SNTP server.
#define AT_IP_Get_SNTP_Time					"AT+CIPSNTPTIME"		// Queries the SNTP time.
#define AT_IP_Ping							"AT+PING"				// Ping packets
// --------------- Connection Status responses (AT+CIPSTATUS)
#define AT_IP_Connected						2U
#define AT_IP_Transmission_Created			3U
#define AT_IP_Transmission_Disconnected		4U
#define AT_IP_Disconnected					5U
#define AT_IP_Connection_Type_Client		0U
#define AT_IP_Connection_Type_Server		1U
// --------------- SSL Client authentication types (AT+CIPSSLCCONF)
#define AT_IP_SSL_Type_NoAuthentication		0U
#define AT_IP_SSL_Type_LoadingCert			1U
#define AT_IP_SSL_Type_LoadingCA			2U
#define AT_IP_SSL_Type_BiDirectional		3U
// --------------- Multiple connections mode parameter (AT+CIPMUX)
#define AT_IP_ConnectionMode_Single			0U
#define AT_IP_ConnectionMode_Multiple		1U
// --------------- Server mode parameter (AT+CIPSERVER)
#define AT_IP_Server_Delete					0U
#define AT_IP_Server_Create					1U
// --------------- Transmission mode parameter (AT+CIPMODE,AT+SAVETRANSLINK)
#define AT_IP_Normal_Transmission			0U
#define AT_IP_UART_Passthrough				1U

// --------------------------------------------- BLE-Related AT Commands (Page 46)
/* NOTE: (in the context of the following macros)
 * Char == Characteristics
 * GATTS == Generic Attributes Server
 * GATTC == Generic Attributes Client
 * SPP == Serial Port Profile
 * SMP == Security Manager Specification
 */
#define AT_BLE_Initialization				"AT+BLEINIT"			// Bluetooth Low Energy (BLE) initialization
#define AT_BLE_Set_Address					"AT+BLEADDR"			// Sets BLE device's address
#define AT_BLE_Set_DeviceName				"AT+BLENAME"			// Sets BLE device's name
#define AT_BLE_Set_ScanParameters			"AT+BLESCANPARAM"		// Sets parameters of BLE scanning
#define AT_BLE_EnableScanning				"AT+BLESCAN"			// Enables BLE scanning
#define AT_BLE_Set_ScanResponse				"AT+BLESCANRSPDATA"		// Sets BLE scan response
#define AT_BLE_Advertising_Config			"AT+BLEADVPARAM"		// Sets parameters of BLE advertising
#define AT_BLE_Advertising_Data				"AT+BLEADVDATA"			// Sets BLE advertising data
#define AT_BLE_Advertising_Start			"AT+BLEADVSTART"		// Starts BLE advertising
#define AT_BLE_Advertising_Stop				"AT+BLEADVSTOP"			// Stops BLE advertising
#define AT_BLE_Connect						"AT+BLECONN"			// Establishes BLE connection
#define AT_BLE_Connection_Config			"AT+BLECONNPARAM"		// Updates parameters of BLE connection
#define AT_BLE_Disconnect					"AT+BLEDISCONN"			// Ends BLE connection
#define AT_BLE_Set_DataLength				"AT+BLEDATALEN"			// Sets BLE data length
#define AT_BLE_Set_MTULength				"AT+BLECFGMTU"			// Sets BLE MTU length
#define AT_BLE_GATTS_Services				"AT+BLEGATTSSRVCRE"		// Generic Attributes Server (GATTS) creates services
#define AT_BLE_GATTS_StartServices			"AT+BLEGATTSSRVSTART"	// GATTS starts services
#define AT_BLE_GATTS_StopServices			"AT+BLEGATTSSRVSTOP"	// GATTS stops services
#define AT_BLE_GATTS_DiscoverServices		"AT+BLEGATTSSRV"		// GATTS discovers services
#define AT_BLE_GATTS_DiscoverChar			"AT+BLEGATTSCHAR"		// GATTS discovers characteristics
#define AT_BLE_GATTS_NotifyChar				"AT+BLEGATTSNTFY"		// GATTS notifies of characteristics
#define AT_BLE_GATTS_IndicateChar			"AT+BLEGATTSIND"		// GATTS indicates characteristics
#define AT_BLE_GATTS_Set_Attributes			"AT+BLEGATTSSETATTR"	// GATTS sets attributes
#define AT_BLE_GATTC_DiscoverPrimary		"AT+BLEGATTCPRIMSRV"	// Generic Attributes Client (GATTC) discovers primary services
#define AT_BLE_GATTC_DiscoverIncluded		"AT+BLEGATTCINCLSRV"	// GATTC discovers included services
#define AT_BLE_GATTC_DiscoverChar			"AT+BLEGATTCCHAR"		// GATTC discovers characteristics
#define AT_BLE_GATTC_ReadChar				"AT+BLEGATTCRD"			// GATTC reads characteristics
#define AT_BLE_GATTC_WriteChar				"AT+BLEGATTCWR"			// GATTC writes characteristics
#define AT_BLE_Set_SPP_Config				"AT+BLESPPCFG"			// Configures BLE SPP (Serial Port Profile)
#define AT_BLE_Enable_SPP					"AT+BLESPP"				// Enables BLE SPP
#define AT_BLE_Set_SMP_Parameters			"AT+BLESECPARAM"		// Sets Parameters of BLE SMP (Security Manager Specification)
#define AT_BLE_StartPairing					"AT+BLEENC"				// Starts a Pairing Request
#define AT_BLE_SetPairingResponse			"AT+BLEENCRSP"			// Sets a Pairing Response
#define AT_BLE_ReplyToPairingKey			"AT+BLEKEYREPLY"		// Reply to a Pairing Key
#define AT_BLE_ReplyToPairingResult			"AT+BLECONFREPLY"		// Reply to a Pairing Result
#define AT_BLE_ListBondedDevices			"AT+BLEENCDEV"			// Lists All Devices that Bonded
#define AT_BLE_UnbindDevice					"AT+BLEENCCLEAR"		// Unbinds Device

// ------------------------------------------------------------ Command functions

typedef void (*transmit_t)(char* buffer, int len);

/* NOTE: These functions construct a finalized command string according to the AT-command-specification
 * They all return the number of characters written into the global buffer.
 * On failure, the value of AT_CMD_ERROR is returned.
 * Internally, those functions make use of sprintf() and vsprintf()
 * If a transmit function is provided, then it will be called instead.
 * The function must conform to this prototype:
 * void transmit_callback(char* buffer, int len) {}
 */
int at_test_command(char *buffer, transmit_t tx, char *cmd);
int at_query_command(char *buffer, transmit_t tx, char *cmd);
int at_set_command(char *buffer, transmit_t tx, char *cmd, const char* param_format, ...);
int at_setn_command(char *buffer, transmit_t tx, size_t limit, char *cmd, const char* param_format, ...);
int at_execute_command(char *buffer, transmit_t tx, char *cmd);

#endif /* INC_ATCOMMANDS_H_ */
