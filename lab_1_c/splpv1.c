/*
 * SPLPv1.c
 * The file is part of practical task for System programming course.
 * This file contains validation of SPLPv1 protocol.
 */

 /*
 #error Specify your name and group
   �������, ���, ��������
   � ������
 */



 /*
 ---------------------------------------------------------------------------------------------------------------------------
 # |      STATE      |         DESCRIPTION       |           ALLOWED MESSAGES            | NEW STATE | EXAMPLE
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 1 | INIT            | initial state             | A->B     CONNECT                      |     2     |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 2 | CONNECTING      | client is waiting for con-| A<-B     CONNECT_OK                   |     3     |
   |                 | nection approval from srv |                                       |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 3 | CONNECTED       | Connection is established | A->B     GET_VER                      |     4     |
   |                 |                           |        -------------------------------+-----------+----------------------
   |                 |                           |          One of the following:        |     5     |
   |                 |                           |          - GET_DATA                   |           |
   |                 |                           |          - GET_FILE                   |           |
   |                 |                           |          - GET_COMMAND                |           |
   |                 |                           |        -------------------------------+-----------+----------------------
   |                 |                           |          GET_B64                      |     6     |
   |                 |                           |        ------------------------------------------------------------------
   |                 |                           |          DISCONNECT                   |     7     |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 4 | WAITING_VER     | Client is waiting for     | A<-B     VERSION ver                  |     3     | VERSION 2
   |                 | server to provide version |          Where ver is an integer (>0) |           |
   |                 | information               |          value. Only a single space   |           |
   |                 |                           |          is allowed in the message    |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 5 | WAITING_DATA    | Client is waiting for a   | A<-B     CMD data CMD                 |     3     | GET_DATA a GET_DATA
   |                 | response from server      |                                       |           |
   |                 |                           |          CMD - command sent by the    |           |
   |                 |                           |           client in previous message  |           |
   |                 |                           |          data - string which contains |           |
   |                 |                           |           the following allowed cha-  |           |
   |                 |                           |           racters: small latin letter,|           |
   |                 |                           |           digits and '.'              |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 6 | WAITING_B64_DATA| Client is waiting for a   | A<-B     B64: data                    |     3     | B64: SGVsbG8=
   |                 | response from server.     |          where data is a base64 string|           |
   |                 |                           |          only 1 space is allowed      |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 7 | DISCONNECTING   | Client is waiting for     | A<-B     DISCONNECT_OK                |     1     |
   |                 | server to close the       |                                       |           |
   |                 | connection                |                                       |           |
 ---------------------------------------------------------------------------------------------------------------------------

 IN CASE OF INVALID MESSAGE THE STATE SHOULD BE RESET TO 1 (INIT)

 */


#include "splpv1.h"
#include "string.h"

 /* FUNCTION:  validate_message
  *
  * PURPOSE:
  *    This function is called for each SPLPv1 message between client
  *    and server
  *
  * PARAMETERS:
  *    msg - pointer to a structure which stores information about
  *    message
  *
  * RETURN VALUE:
  *    MESSAGE_VALID if the message is correct
  *    MESSAGE_INVALID if the message is incorrect or out of protocol
  *    state
  */
int STATE = 1;
int CurrentCommand; //0 - "GET_DATA", 1 - "GET_COMMAND", 2 - "GET_FILE"

const char data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0 };

const char base64[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0 };

enum test_status validate_message(struct Message* msg)
{
	if (msg->direction == A_TO_B) {
		if (STATE == 1) {
			if (!strcmp(msg->text_message, "CONNECT"))
			{
				STATE = 2;
				return MESSAGE_VALID;
			}
		}
		else if (STATE == 3) {
			if (!strcmp(msg->text_message, "GET_VER")) {
				STATE = 4;
				return MESSAGE_VALID;
			}
			else if (!strcmp(msg->text_message, "GET_DATA")) {
				STATE = 5;
				CurrentCommand = 0;
				return MESSAGE_VALID;
			}
			else if (!strcmp(msg->text_message, "GET_COMMAND")) {
				STATE = 5;
				CurrentCommand = 1;
				return MESSAGE_VALID;
			}
			else if (!strcmp(msg->text_message, "GET_FILE")) {
				STATE = 5;
				CurrentCommand = 2;
				return MESSAGE_VALID;
			}
			else if (!strcmp(msg->text_message, "GET_B64")) {
				STATE = 6;
				return MESSAGE_VALID;
			}
			else if (!strcmp(msg->text_message, "DISCONNECT")) {
				STATE = 7;
				return MESSAGE_VALID;
			}
		}
	}
	else {
		if (STATE == 2 && !strcmp(msg->text_message, "CONNECT_OK")) {
			STATE = 3;
			return MESSAGE_VALID;
		}
		else if (STATE == 4 && !strncmp(msg->text_message, "VERSION ", 8)) {
			char* message = msg->text_message;
			message += 8;
			if (*message > 48 && *message < 58) {
				while (*message != '\0') {
					if (*message < 48 || *message > 57)
					{
						STATE = 1;
						return MESSAGE_INVALID;
					}
					message++;
				}
				STATE = 3;
				return MESSAGE_VALID;
			}
		}
		else if (STATE == 5) {
			char* command;
			if (CurrentCommand == 0)
				command = "GET_DATA";
			else if (CurrentCommand == 1)
				command = "GET_COMMAND";
			else if (CurrentCommand == 2)
				command = "GET_FILE";
			int len = strlen(command);
			if (!strncmp(msg->text_message, command, len))
			{
				char* message = msg->text_message;
				message += (len + 1);
				while (data[*message]) message++;
				if (*message == ' ') {
					message++;
					if (!strcmp(message, command))
					{
						STATE = 3;
						return MESSAGE_VALID;
					}
				}
			}
		}
		else if (STATE == 6 && !strncmp(msg->text_message, "B64: ", 5)) {
			char* message = msg->text_message;
			int len = strlen(msg->text_message);
			if ((len - 5) % 4 == 0) {
				message += 5;
				while (base64[*message]) message++;
				int check = 0;
				while (*message == '=') {
					message++;
					check++;
				}
				if (check <= 2 && *message == '\0')
				{
					STATE = 3;
					return MESSAGE_VALID;
				}
			}

		}
		else if (STATE == 7 && !strcmp(msg->text_message, "DISCONNECT_OK")) {
			STATE = 1;
			return MESSAGE_VALID;
		}
	}
	STATE = 1;
	return MESSAGE_INVALID;
}
