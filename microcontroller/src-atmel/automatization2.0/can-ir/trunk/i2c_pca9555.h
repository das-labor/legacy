#pragma once

extern uint16_t pca9555_get_output(void);
extern void pca9555_write_byte(uint8_t byte, uint8_t cmd);
extern void pca9555_write_word(uint16_t byte, uint8_t cmd);
extern uint16_t pca9555_read_byte(uint8_t cmd);

enum {
	PCA9555_IN0,
	PCA9555_IN1,
	PCA9555_OUT0,
	PCA9555_OUT1,
	PCA9555_POL0,
	PCA9555_POL1,
	PCA9555_CFG0,
	PCA9555_CFG1,
} e_PCA9555_CMDS;

