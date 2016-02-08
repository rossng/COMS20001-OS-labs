handler_rst: msr   cpsr, #0xD3             @ enter SVC mode with no interrupts
             ldr   sp, =tos_svc            @ initialise SVC mode stack

             bl    kernel_handler_rst      @ invoke C function
             b     .                       @ halt
