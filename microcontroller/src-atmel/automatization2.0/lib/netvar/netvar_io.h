

typedef void (*set_output_func_t)(void * ref, uint8_t val)  ; 

typedef struct{
	set_output_func_t  set_output_func; //handler that is called to switch/dim the phsical lamp
	void *             set_output_ref;  //user supplied reference which is given to the handler (e.g. the number representing the io-port, or pointer to structure which describes physical lamp)
	netvar_desc *      nd;            //the netvar which is regsitered during netvar_lamp_new registered
} netvar_output_8_t;

netvar_output_8_t * new_netvar_output_8(uint16_t           idx, 
                                        uint8_t            sidx, 
								        set_output_func_t  set_output_func,
								        void *             set_output_ref     );
