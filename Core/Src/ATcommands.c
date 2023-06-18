/*
 * ATcommands.c
 *
 *  Created on: Jun 18, 2023
 *      Author: rini
 */

#include "ATcommands.h"

int at_test_command(char *buffer, transmit_t tx, char *cmd) {
	int length;
	if((length = sprintf_(buffer, "%s=?\r\n", cmd)) < 0) {
		return AT_CMD_ERROR;
	}
	if(tx != NULL) {
		tx(buffer, length);
	}
	return length;
}

int at_query_command(char *buffer, transmit_t tx, char *cmd) {
	int length;
	if((length = sprintf_(buffer, "%s?\r\n", cmd)) < 0) {
		return AT_CMD_ERROR;
	}
	if(tx != NULL) {
		tx(buffer, length);
	}
	return length;
}

int at_set_command(char *buffer, transmit_t tx, char *cmd, const char* param_format, ...) {
	va_list parameters;
	int length = 0;
	int tmp = 0;
	if((tmp = sprintf_(buffer, "%s=", cmd)) < 0) {
		return AT_CMD_ERROR;
	}
	length += tmp;
	tmp = 0;
	va_start(parameters, param_format);
	if((tmp = vsprintf_(buffer+length, param_format, parameters)) < 0) {
		return AT_CMD_ERROR;
	}
	va_end(parameters);
	length += tmp;
	tmp = 0;
	if((tmp = sprintf_(buffer+length, "\r\n")) < 0) {
		return AT_CMD_ERROR;
	}
	length += tmp;
	if(tx != NULL) {
		tx(buffer, length);
	}
	return length;
}

int at_setn_command(char *buffer, transmit_t tx, size_t limit, char *cmd, const char* param_format, ...) {
	va_list parameters;
	int length = 0;
	int tmp = 0;
	if((tmp = sprintf_(buffer, "%s=", cmd)) < 0) {
		return AT_CMD_ERROR;
	}
	length += tmp;
	tmp = 0;
	va_start(parameters, param_format);
	if((tmp = vsnprintf_(buffer+length, limit, param_format, parameters)) < 0) {
		return AT_CMD_ERROR;
	}
	va_end(parameters);
	length += tmp;
	tmp = 0;
	if((tmp = sprintf_(buffer+length, "\r\n")) < 0) {
		return AT_CMD_ERROR;
	}
	length += tmp;
	if(tx != NULL) {
		tx(buffer, length);
	}
	return length;
}


int at_execute_command(char *buffer, transmit_t tx, char *cmd) {
	int length;
	if((length = sprintf_(buffer, "%s\r\n", cmd)) < 0) {
		return AT_CMD_ERROR;
	}
	if(tx != NULL) {
		tx(buffer, length);
	}
	return length;
}
