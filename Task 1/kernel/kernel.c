// -----------------------------------main.c -------------------------------------

#include "../uart/uart0.h"
#include "../CLI/cli.h"

static void print_welcome(void)
{
    uart_puts("\n");
    uart_puts(" ####### ####### #######  #####  #        #####    ###   \n");
    uart_puts(" #       #          #    #     # #    #  #     #  #   #  \n");
    uart_puts(" #       #          #          # #    #  #     # #     # \n");
    uart_puts(" #####   #####      #     #####  #    #   ###### #     # \n");
    uart_puts(" #       #          #    #       #######       # #     # \n");
    uart_puts(" #       #          #    #            #  #     #  #   #  \n");
    uart_puts(" ####### #######    #    #######      #   #####    ###   \n");
    uart_puts("                                                         \n");
    uart_puts("                                                         \n");
    uart_puts(" ######     #    ######  #######    #######  #####       \n");
    uart_puts(" #     #   # #   #     # #          #     # #     #      \n");
    uart_puts(" #     #  #   #  #     # #          #     # #            \n");
    uart_puts(" ######  #     # ######  #####      #     #  #####       \n");
    uart_puts(" #     # ####### #   #   #          #     #       #      \n");
    uart_puts(" #     # #     # #    #  #          #     # #     #      \n");
    uart_puts(" ######  #     # #     # #######    #######  #####       \n");
    uart_puts("\n");
    uart_puts("     Developed by Group 07 – COSC Embedded Systems       \n");
    uart_puts("\n");
}

void main()
{
    // intitialize UART
    uart_init();

    // print welcome
    print_welcome();

    // type help for available commands
    uart_puts("Type 'help' to see list of commands.\n");

    // run the command line entered
    cli_run();
}
