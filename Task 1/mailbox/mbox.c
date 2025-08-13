#include "./mbox.h"

/* Mailbox buffer must be 16-byte aligned */
static volatile unsigned int __attribute__((aligned(16))) mbox[36];

static int mbox_call(unsigned int chan)
{
    unsigned int addr = (unsigned int)((unsigned long)mbox);
    while (MBOX_STATUS & MBOX_FULL)
    {
    }
    MBOX_WRITE = (addr & ~0xF) | (chan & 0xF);
    for (;;)
    {
        while (MBOX_STATUS & MBOX_EMPTY)
        {
        }
        unsigned int resp = MBOX_READ;
        if ((resp & 0xF) == (chan & 0xF) && (resp & ~0xF) == (addr & ~0xF))
        {
            return mbox[1] == 0x80000000; /* SUCCESS */
        }
    }
}

int mbox_get_board_revision(uint32_t *rev)
{
    mbox[0] = 8 * 4;
    mbox[1] = 0x00000000; /* request */
    mbox[2] = TAG_GET_BOARD_REVISION;
    mbox[3] = 4;
    mbox[4] = 0;
    mbox[5] = 0;
    mbox[6] = TAG_END;

    if (mbox_call(8))
    {
        *rev = mbox[5];
        return 0;
    }
    return -1;
}

int mbox_get_mac(uint8_t mac[6])
{
    mbox[0] = 9 * 4;
    mbox[1] = 0x00000000;
    mbox[2] = TAG_GET_MAC_ADDRESS;
    mbox[3] = 6;
    mbox[4] = 0;
    mbox[5] = 0; /* bytes 0..3 */
    mbox[6] = 0; /* bytes 4..5 in low halfword */
    mbox[7] = TAG_END;

    if (mbox_call(8))
    {
        unsigned int hi = mbox[5];
        unsigned int lo = mbox[6];
        mac[0] = (hi >> 24) & 0xFF;
        mac[1] = (hi >> 16) & 0xFF;
        mac[2] = (hi >> 8) & 0xFF;
        mac[3] = (hi >> 0) & 0xFF;
        mac[4] = (lo >> 8) & 0xFF;
        mac[5] = (lo >> 0) & 0xFF;
        return 0;
    }
    return -1;
}
