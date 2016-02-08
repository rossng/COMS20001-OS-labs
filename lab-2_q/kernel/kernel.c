#include "kernel.h"

void kernel_handler_rst() {
  /* Configure the mechanism for interrupt handling by
   *
   * - configuring UART st. an interrupt is raised every time a byte is
   *   received,
   * - configuring GIC st. the selected interrupts are forwarded to the 
   *   processor via the IRQ interrupt signal, then
   * - enabling IRQ interrupts.
   */

  UART0->IMSC           |= 0x00000010; // enable UART    (Rx) interrupt
  UART0->CR              = 0x00000301; // enable UART (Tx+Rx)

  GICC0->PMR             = 0x000000F0; // unmask all          interrupts
  GICD0->ISENABLER[ 1 ] |= 0x00001000; // enable UART    (Rx) interrupt
  GICC0->CTLR            = 0x00000001; // enable GIC interface
  GICD0->CTLR            = 0x00000001; // enable GIC distributor

  irq_enable();

  /* Force execution into an infinite loop, each iteration of which will
   *
   * - delay for some period of time, which is realised by executing a
   *   large, fixed number of nop instructions in an inner loop, then
   * - execute a supervisor call (i.e., svc) instruction, thus raising
   *   a software-interrupt (i.e., a trap or system call).
  */

  while( 1 ) {
    for( int i = 0; i < 0x20000000; i++ ) {
      asm volatile( "nop" );
    }

    asm( "svc 0" );
  }

  return;
}

void kernel_handler_irq() {
  // Step 2: read  the interrupt identifier so we know the source.

  uint32_t id = GICC0->IAR;

  // Step 4: handle the interrupt, then clear (or reset) the source.

  if( id == GIC_SOURCE_UART0 ) {
    uint8_t x = PL011_getc( UART0 );

    PL011_putc( UART0, 'K' ); 
    PL011_putc( UART0, '<' ); 
    PL011_putc( UART0,  x  ); 
    PL011_putc( UART0, '>' ); 

    UART0->ICR = 0x10;
  }

  // Step 5: write the interrupt identifier to signal we're done.

  GICC0->EOIR = id;
}

void kernel_handler_svc() {
  /* Each time execution reaches this point, we are tasked with handling
   * a supervisor call (aka. software interrupt, i.e., a trap or system 
   * call).
   */

  PL011_putc( UART0, 'T' );  
}

