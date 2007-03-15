
#define REQUEST_OPEN_CRED 0x80


typedef struct{
	uint8_t type;
	uint8_t data[10];
}request_t;

typedef struct{
	uint16_t index;
	uint8_t[8] key;	
}credentials_t;

typedef struct{
	uint8_t type;
	credentials_t credentials;
}request_cred_t;

