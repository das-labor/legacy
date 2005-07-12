#ifndef LAMPE_H
#define LAMPE_H

// PUBLIC Interface Lampe
typedef enum { FKT_LAMPE_SET=0x00, FKT_LAMPE_SETMASK=0x01 } lampe_fkts;

// functions
unsigned char lampe_set_lampe(unsigned char lampe, unsigned char value) ;


#endif
