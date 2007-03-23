#ifndef TETRIS_LOGIC_H_
#define TETRIS_LOGIC_H_

void tetris();

/* Function:          tetris_logic_calculateLines
 * Description:       calculates no. of lines for the given row mask
 * Argument nRowMask: row mask from which the no. of lines will be calculated
 * Return value:      number of lines of the row mask
 */
uint8_t tetris_logic_calculateLines(uint8_t nRowMask);

#endif /*TETRIS_LOGIC_H_*/
