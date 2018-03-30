#ifndef _COMMON_H_
#define _COMMON_H_

#define FATAL_ERROR         -1
#define STATUS_SUCCESS      0

#define REQUEST_COMPLETE        1
#define REQUEST_PENDING     2
#define CONNECTION_CLOSED   -2
#define REQUEST_AVAILABLE   4
#define CLIENT_AUTHENTICATION_FAILED    -3
#define CLIENT_DISCONNECTED             -4

#define X_STATUS_SUCCESS            0
#define X_STATUS_PENDING            1
#define X_FATAL_ERROR               2

#define X_INITIAL_STATE             0
#define X_SENDING_FINAL_MESSAGE     1
#define X_SENDING_SERVER_RESPONSE   2
#define X_IDLE_STATE                3
#define X_READING_REQUEST           4
#define X_SENDING_SERVER_INFO       5

#define STATE_NO_PACKET             0

#define INITIAL_CLIENT_SIZE         12

#define pad(x, y) (x % y == 0 ? x : (y - (x % y)))

#define SWAPBYTES_16(x) (x << 8 | x >> 8)
#define SWAPBYTES_32(x) ((x >> 24) | ((x >> 8) & 0xFF00) | (x << 24) | ((x << 8) & 0x00FF0000))

#define REQUEST_FAILED              -1
#define REQUEST_SUCCESS             0

#define POWER_OF_TWO(x) (!(x & (x - 1)))

#endif