#include "PL011.h"

/* Per Table 4.2 (for example: the information is in several places) of
 *
 * http://infocenter.arm.com/help/topic/com.arm.doc.dui0417d/index.html
 *
 * we know the device(s) are mapped to fixed addresses in memory, so to
 * allow easier access to the device registers we can simply overlay an
 * associated structure instance for each one.
 */

PL011_t* const UART0 = ( PL011_t* )( 0x10009000 );
PL011_t* const UART1 = ( PL011_t* )( 0x1000A000 );
PL011_t* const UART2 = ( PL011_t* )( 0x1000B000 );
PL011_t* const UART3 = ( PL011_t* )( 0x1000C000 );

/**
 * Convert a character to an integer. Inverse of itox(int x)
 * @param x The character to be converted
 * @return An integer corresponding to the character.
 */
int  xtoi( char x ) {
  if      ( ( x >= '0' ) && ( x <= '9' ) ) {
    return (  0 + ( x - '0' ) );
  }
  else if ( ( x >= 'a' ) && ( x <= 'f' ) ) {
    return ( 10 + ( x - 'a' ) );
  }
  else if ( ( x >= 'A' ) && ( x <= 'F' ) ) {
    return ( 10 + ( x - 'A' ) );
  }

  return -1;
}

/**
 * Convert a integer to a character. Inverse of xtoi(char x)
 * @param x The integer to be converted
 * @return An character corresponding to the integer.
 */
char itox( int  x ) {
  if     ( ( x >=  0 ) && ( x <=  9 ) ) {
    return '0' + ( x -  0 );
  }
  else if( ( x >= 10 ) && ( x <= 15 ) ) {
    return 'A' + ( x - 10 );
  }

  return -1;
}

/**
 * Put a character onto the provided PL011 serial link.
 * Waits while the buffer is full.
 * @param d The UART link to send over.
 * @param x The char to transmit.
 */
void    PL011_putc( PL011_t* d, uint8_t x ) {
  while( d->FR & 0x20 ) {
    /* wait while transmit FIFO is full */
  }

  d->DR = x;
}

/**
 * Read a character from the provided PL011 serial link.
 * Blocks until a character is received.
 * @param d The UART link to receive from.
 * @return The character that was read from the link.
 */
uint8_t PL011_getc( PL011_t* d            ) {
  while( d->FR & 0x10 ) {
    /* wait while receive FIFO is empty */
  }

  return d->DR;
}

/**
 * Put the most-significant nibble then least significant
 * nibble onto a PL011 serial link.
 * @param d The UART link to send over.
 * @param x The char to transmit.
 */
void    PL011_puth( PL011_t* d, uint8_t x ) {
  PL011_putc( d, itox( ( x >> 4 ) & 0xF ) );
  PL011_putc( d, itox( ( x >> 0 ) & 0xF ) );
}

/**
 * Read two half-byte characters from the provided PL011 serial link
 * and assemble them into a single byte.
 * @param d The UART link to receive from.
 * @return The assembled char.
 */
uint8_t PL011_geth( PL011_t* d            ) {
  uint8_t x  = ( xtoi( PL011_getc( d ) ) << 4 );
          x |= ( xtoi( PL011_getc( d ) ) << 0 );

  return x;
}
