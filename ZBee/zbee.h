#ifndef ZBEE_H
#define ZBEE_H

#include "misc.h"
#include <windows.h>


int Read_Port_Blocking(HANDLE fd, char* buff);
int write_AT_command(HANDLE fd, char* command);
void finalize_serial(HANDLE fd);
int force_exit(HANDLE fd, char* string);
HANDLE initialize_serial(char* puerto);
int read_all(HANDLE fd, char* data);
void send_to(HANDLE fd, address dest, char* data);

void send_data_to(HANDLE fd, address dest, char* data, int size);
void send_string_to(HANDLE fd, address dest, char* data);


int check_OK_retrieved(HANDLE fd);
int Read_Port_Blocking(HANDLE fd, char* buff);
int write_AT_command(HANDLE fd, char* command);
void initialize_zigbee_module(HANDLE* fdr, char* serial_port, address source);
void change_local_address(HANDLE fd, address source);

#endif /* ZBEE_H */
