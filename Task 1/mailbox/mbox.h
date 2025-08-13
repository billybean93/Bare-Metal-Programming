// -----------------------------------mbox.h -------------------------------------
#ifndef MBOX_H
#define MBOX_H
#include "../kernel/types.h"
#include "../kernel/gpio.h"

/* Mailbox property interface on channel 8 */
#define MBOX_BASE   (MMIO_BASE + 0x00B880)
#define MBOX_READ   (*(volatile unsigned int*)(MBOX_BASE + 0x00))
#define MBOX_POLL   (*(volatile unsigned int*)(MBOX_BASE + 0x10))
#define MBOX_SENDER (*(volatile unsigned int*)(MBOX_BASE + 0x14))
#define MBOX_STATUS (*(volatile unsigned int*)(MBOX_BASE + 0x18))
#define MBOX_CONFIG (*(volatile unsigned int*)(MBOX_BASE + 0x1C))
#define MBOX_WRITE  (*(volatile unsigned int*)(MBOX_BASE + 0x20))

#define MBOX_FULL   0x80000000
#define MBOX_EMPTY  0x40000000

/* Tags */
#define TAG_GET_BOARD_REVISION  0x00010002
#define TAG_GET_MAC_ADDRESS     0x00010003
#define TAG_END                 0

int mbox_get_board_revision(uint32_t *rev);
int mbox_get_mac(uint8_t mac[6]);

#endif
