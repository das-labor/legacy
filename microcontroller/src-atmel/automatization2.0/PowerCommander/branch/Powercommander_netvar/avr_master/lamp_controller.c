


/*
  Netvar lamp-controller by Peter Fuhrmann

  The Lamp controllers function is to control a group of lamps, like for example
  the lamps & spots in the lounge, or the four flourescent lamps in the vortragsraum.
  
  the lamp controller uses all the subindexes of one netvar.
  different functionalities are mapped on the subindexes.
  Mapping:
  sidx        function
  0x00-0x0f   lamp outputs
  0x20-0x2f   power button inputs
  0x30-0x3f   power indicator outputs
  0x40-0x4f   slider inputs
  0x50-0x5f   slider outputs
  
  power and slider channels 0x0f are considered the master channel
  for the lamp group that is controlled.
  
  additional master sliders for subgroups could be added later.
    
  additional functionality like labor master power or alarm blink modes
  can be added later to listen on additional netvars
  
  The idea is to put the lamp controller onto any single can node of choice,
  from where it then will listen for the switches and sliders, and controlls the
  lamps and indicators accordingly.
  It doesn't matter if the lamps and switches/sliders are on the same node as
  the lamp controller, as all communication is done by setting and reacting to
  netvars, which works locally and remotely.
  the lamps and switches/sliders need to be configured to listen or talk on the
  netvar index of the lamp controller and with a subindex matching the channel
  of the lamp that is supposed to be implemented or controlled.
*/

#include <stdlib.h>

#include "netvar/netvar.h"
#include "lamp_controller.h"


void lamp_controller_nv_handler(netvar_desc *nd, void *ref) {
	lamp_controller_t *s = (lamp_controller_t*) ref;
	uint8_t sidx = nd->sidx;
	
	if ( (sidx >= 0x40) && (sidx < (0x40 + s->num_lamps)) ) {
		//slider inputs
		uint8_t i = sidx - 0x40;
		uint8_t val;
		netvar_read(nd, &val);
		
		s->lamp_values[i] = val;
		netvar_write(s->slider_out_netvars[i], &val); //set all sliders to display new value
		
		//calculate mean brightnes to set master slider to that value
		uint16_t master_val = 0;
		uint8_t x;
		for (x = 0; x < s->num_lamps; x++) {
			master_val += s->lamp_values[x]; 
		}
		master_val /= s->num_lamps;
		netvar_write(s->master_slider_out_netvar, &master_val); //set master slider
		
		
	} else if (sidx == 0x4f) {
		//master slider
	}
	
	
}

//create lamp controller on idx with a umber of lamps. a maximum of 15 Lamps is allowed.
lamp_controller_t * new_lamp_controller(uint16_t idx, uint8_t num_lamps){
	lamp_controller_t * s = (lamp_controller_t *) malloc(sizeof(lamp_controller_t));
	netvar_desc * tmp_nv;
	uint8_t i;
	
	s->num_lamps = num_lamps;
	
	s->lamp_values = malloc(num_lamps);
	
	s->master_slider_out_netvar = netvar_register(idx, 0x5f, 1); //master slider out
	tmp_nv = netvar_register(idx, 0x4f, 1); //master slider input
	netvar_add_handler(tmp_nv, lamp_controller_nv_handler, s);
		
	s->slider_out_netvars = malloc(sizeof(netvar_desc) * num_lamps);
	
	for (i = 0; i < num_lamps; i++){
		tmp_nv = netvar_register(idx, 0x40 + i, 1); //slider inputs
		netvar_add_handler(tmp_nv, lamp_controller_nv_handler, s);
		
		s->slider_out_netvars[i] = netvar_register(idx, 0x50 + i, 1); //slider outputs
	}
	
	return s;
}



void lamp_controller_tick(lamp_controller_t * lc){

}

