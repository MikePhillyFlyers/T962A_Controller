#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <string.h>
/* custom includes */
#include "common_defs.h"
#include "circbuffer.h"




/**** Circular Buffer used by UART ****/

/**
**  @brief init_circ_buf
**	@param[in,out]
**	@return 
*/
void init_circ_buf(tcirc_buf *cbuf)
{
	cbuf->head = cbuf->tail = 0;
	cbuf->dropped = 0;
}


/**
**  @brief add_to_circ_buf
**	@param[in,out]
**	@return 
*/
void add_to_circ_buf(tcirc_buf *cbuf, char ch, int block)
{
	// Add char to buffer
	unsigned int newhead = cbuf->head;
	newhead++;
	if (newhead >= CIRCBUFSIZE) {
		newhead = 0;
	}
	while (newhead == cbuf->tail) {
		if (!block) {
			cbuf->dropped++;
			return;
		}

		// If blocking, this just keeps looping. Due to interrupt-driven
		// system the buffer might eventually have space in it, however
		// if this is called when interrupts are disabled it will stall
		// the system, so the caller is cautioned not to fsck it up.
	}

	cbuf->buf[cbuf->head] = ch;
	cbuf->head = newhead;
}



/**
**  @brief get_from_circ_buf
**	@param[in,out]
**	@return 
*/
char get_from_circ_buf(tcirc_buf *cbuf)
{
	// Get char from buffer
	// Be sure to check first that there is a char in buffer
	unsigned int newtail = cbuf->tail;
	uint8_t retval = cbuf->buf[newtail];

	if (newtail == cbuf->head) {
		return 0xFF;
	}

	newtail++;
	if (newtail >= CIRCBUFSIZE) {
		// Rollover
		newtail = 0;
	}
	cbuf->tail = newtail;

	return retval;
}



/**
**  @brief circ_buf_has_char
**	@param[in,out]
**	@return 
*/
int circ_buf_has_char(tcirc_buf *cbuf)
{
	// Return true if buffer empty
	unsigned int head = cbuf->head;
	return (head != cbuf->tail);
}


/**
**  @brief circ_buf_count
**	@param[in,out]
**	@return 
*/
unsigned int circ_buf_count(tcirc_buf *cbuf)
{
	int count;

	count = cbuf->head;
	count -= cbuf->tail;
	if (count < 0) {
		count += CIRCBUFSIZE;
	}
	return (unsigned int)count;
}
