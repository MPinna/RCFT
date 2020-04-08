short RRQ_OPCODE = 0x01;
short WRQ_OPCODE = 0x02;
short DTA_OPCODE = 0x03;
short ACK_OPCODE = 0x04;
short ERR_OPCODE = 0x05;

short NOT_FOUND_ERR_CODE = 0x1;
short ILLEGAL_OP_ERR_CODE = 0x4;

short MAX_CHUNK_SIZE = 512;

char *NETASCII_MODE_S = "netascii",
     *OCTET_MODE_S = "octet";

#define ADDR_STRING_LENGTH 16
