// -----------------------------------mbox.c -------------------------------------
#include "mbox.h"
#include "../kernel/gpio.h"
#include "../uart/uart0.h"

/* Mailbox Data Buffer (each element is 32-bit)*/
/*
* The keyword attribute allows you to specify special attributes
*
* The aligned(N) attribute aligns the current data item on an address
* which is a multiple of N, by inserting padding bytes before the data item
*
* __attribute__((aligned(16)) : allocate the variable on a 16-byte boundary.
* *
* We must ensure that our our buffer is located at a 16 byte aligned address,
* so only the high 28 bits contain the address
* (last 4 bits is ZERO due to 16 byte alignment)
*
*/
volatile unsigned int __attribute__((aligned(16))) mBuf[36];

/**
* Read from the mailbox
*/
uint32_t mailbox_read(unsigned char channel)
{
    //Receiving message is buffer_addr & channel number
    uint32_t res;
    // Make sure that the message is from the right channel
    do {
        // Make sure there is mail to receive
        do {
            asm volatile("nop");
        } while (MBOX0_STATUS & MBOX_EMPTY);
        // Get the message
        res = MBOX0_READ;
    } while ( (res & 0xF) != channel);

    return res;
}

/**
* Write to the mailbox
*/
void mailbox_send(uint32_t msg, unsigned char channel)
{
    //Sending message is buffer_addr & channel number
    // Make sure you can send mail
    do {
        asm volatile("nop");
    } while (MBOX1_STATUS & MBOX_FULL);
    // send the message
    MBOX1_WRITE = msg;
}

/**
* Make a mailbox call. Returns 0 on failure, non-zero on success
*/
int mbox_call(unsigned int buffer_addr, unsigned char channel)
{
    //Check Buffer Address
    // uart_puts("Buffer Address: ");
    // uart_hex(buffer_addr);
    // uart_sendc('\n');

    //Prepare Data (address of Message Buffer)
    unsigned int msg = (buffer_addr & ~0xF) | (channel & 0xF);
    mailbox_send(msg, channel);

    /* now wait for the response */
    /* is it a response to our message (same address)? */
    if (msg == mailbox_read(channel)) {
        /* is it a valid successful response (Response Code) ? */
        if (mBuf[1] == MBOX_RESPONSE)
            // uart_puts("Got successful response \n");

        return (mBuf[1] == MBOX_RESPONSE);
    }

    return 0;
}

void mbox_get_board_revision()
{
    //Set up a message to request the board revision
    mBuf[0] = 7 * 4;         //Total size
    mBuf[1] = MBOX_REQUEST;  //This is a request message

    mBuf[2] = MBOX_TAG_GET_BOARD_REVISION;  //Tag identifier
    mBuf[3] = 4;                            //Max size of the buffer
    mBuf[4] = 0;                            //Size of the data (response)
    mBuf[5] = 0;                            //Space for the response (4 bytes)

    mBuf[6] = MBOX_TAG_LAST;  //End tag

    //Send the message to the GPU and wait for the response
    if (mbox_call((unsigned int)((unsigned long)&mBuf), MBOX_CH_PROP)) {
        //The response should now be in mBuf[5]
        uart_puts("Board Revision: ");
        uart_hex(mBuf[5]);
        uart_sendc('\n');
    } else {
        uart_puts("Unable to query board revision.\n");
    }
}

void mbox_get_mac(){
    //Set up a message to request the MAC address
    mBuf[0] = 8 * 4;         //Total size
    mBuf[1] = MBOX_REQUEST;  //This is a request message

    mBuf[2] = MBOX_TAG_GET_MAC_ADDRESS;  //Tag identifier
    mBuf[3] = 6;                         //Max size of the buffer
    mBuf[4] = 0;                         //Size of the data (response)
    mBuf[5] = 0;                         //Space for the response (first 4 bytes)
    mBuf[6] = 0;                         //Space for the response (last 2 bytes)

    mBuf[7] = MBOX_TAG_LAST;  //End tag

    //Send the message to the GPU and wait for the response
    if (mbox_call((unsigned int)((unsigned long)&mBuf), MBOX_CH_PROP)) {
        //The response should now be in mBuf[5] and mBuf[6]
        uart_puts("MAC Address: ");
        for (int i = 0; i < 6; i++) {
            if (i > 0) uart_sendc(':');
            unsigned char byte;
            if (i < 4) {
                byte = (mBuf[5] >> (8 * (3 - i))) & 0xFF;
            } else {
                byte = (mBuf[6] >> (8 * (5 - i))) & 0xFF;
            }
            if (byte < 16) uart_sendc('0'); // Leading zero for single digit
            uart_dec(byte);
        }
        uart_sendc('\n');
    } else {
        uart_puts("Unable to query MAC address.\n");
    }
}