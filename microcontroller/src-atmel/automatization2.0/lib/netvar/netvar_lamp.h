

typedef void (*set_lamp_func_t)(void * ref, uint8_t val)  ; 

typedef struct{
	set_lamp_func_t  set_lamp_func; //handler that is called to switch/dim the phsical lamp
	void *           set_lamp_ref;  //user supplied reference which is given to the handler (e.g. the number representing the io-port, or pointer to structure which describes physical lamp)
	netvar_desc *    nd;            //the netvar which is regsitered during netvar_lamp_new registered
} netvar_lamp_t;

netvar_lamp_t * new_netvar_lamp(uint16_t         idx, 
                                uint8_t          sidx, 
								set_lamp_func_t  set_lamp_func,
								void *           set_lamp_ref   );
