#ifndef CARDIO_H_
#define CARDIO_H_


bool cardinserated(void);
bool readABfromCard(authblock_t * ab);
bool writeABtoCard(authblock_t * ab);
#endif /*CARDIO_H_*/
