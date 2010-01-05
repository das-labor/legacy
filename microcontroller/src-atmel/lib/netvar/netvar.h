

typedef struct{
	uint16_t var;
	uint8_t flags;
}netvar_uint16;

typedef struct{
	uint16_t id;
	uint8_t type;
	void * netvar;
	void * handler;
}netvar_def;




extern netvar_def listen_table[];



