


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

typedef struct{
	
}lamp_controller_t;

lamp_controller_t * new_lamp_controller(uint16_t idx, uint8_t num_lamps){

}



void lamp_controller_update(lamp_controller_t * lc){

}

