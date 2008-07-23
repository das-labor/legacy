/* mcp23017.h */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef MCP23017_H_
#define MCP23017_H_



#define IODIRA_B0   0x00   
#define IODIRB_B0   0x01   
#define IPOLA_B0    0x02    
#define IPOLB_B0    0x03    
#define GPINTENA_B0 0x04 
#define GPINTENB_B0 0x05 
#define DEFVALA_B0  0x06  
#define DEFVALB_B0  0x07  
#define INTCONA_B0  0x08  
#define INTCONB_B0  0x09  
#define IOCON_B0    0x0A    
//#define IOCON_B0    0x0B    
#define GPPUA_B0    0x0C    
#define GPPUB_B0    0x0D    
#define INTFA_B0    0x0E    
#define INTFB_B0    0x0F    
#define INTCAPA_B0  0x10  
#define INTCAPB_B0  0x11  
#define GPIOA_B0    0x12    
#define GPIOB_B0    0x13    
#define OLATA_B0    0x14    
#define OLATB_B0    0x15    

#define IODIRA_B1   0x00
#define IODIRB_B1   0x10
#define IPOLA_B1    0x01
#define IPOLB_B1    0x11
#define GPINTENA_B1 0x02
#define GPINTENB_B1 0x12
#define DEFVALA_B1  0x03
#define DEFVALB_B1  0x13
#define INTCONA_B1  0x04
#define INTCONB_B1  0x14
#define IOCON_B1    0x05
//#define IOCON_B1    0x15
#define GPPUA_B1    0x06
#define GPPUB_B1    0x16
#define INTFA_B1    0x07
#define INTFB_B1    0x17
#define INTCAPA_B1  0x08
#define INTCAPB_B1  0x18
#define GPIOA_B1    0x09
#define GPIOB_B1    0x19
#define OLATA_B1    0x0A
#define OLATB_B1    0x1A

#define MCP23017_BASEADDR 0x40

#endif /*MCP23017_H_*/

