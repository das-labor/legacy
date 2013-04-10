
typedef struct {
	uint8_t num_lamps;
	uint8_t *lamp_values;
	uint16_t on_mask;

	uint8_t master_power;

	netvar_desc **slider_out_netvars;
	netvar_desc **lamp_out_netvars;

	netvar_desc *master_slider_out_netvar;
	netvar_desc *master_button_out_netvar;
} lamp_controller_t;


//create lamp controller on idx with a umber of lamps. a maximum of 15 Lamps is allowed.
lamp_controller_t *new_lamp_controller(uint16_t idx, uint8_t num_lamps);

void lamp_controller_tick(lamp_controller_t *lc);

