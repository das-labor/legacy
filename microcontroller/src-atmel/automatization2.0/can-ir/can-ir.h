/* -*- Mode: C; tab-width: 2 -*- */
#ifndef _IR_H
#define _IR_H

void ir_sendCode(uint16_t *code, uint8_t codeLen);

void read_code_to_array(uint16_t *array, uint16_t *pgmData, uint8_t pos, uint8_t len);

#endif // 
