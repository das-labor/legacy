#ifndef CARDIO_H_
#define CARDIO_H_


bool card_inserated(void);
bool card_erase(void);
bool card_format(void);
bool card_readAB(authblock_t * ab);
bool card_writeAB(authblock_t * ab);
#endif /*CARDIO_H_*/
