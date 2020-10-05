#ifndef _BLOCKCHAIN_H
#define _BLOCKCHAIN_H

#include<stdbool.h>
#include<string.h>
#include "node-id.h"


/* Rime channel used for this application. */
#define BLOCKCHAIN_CHANNEL 130
/* IEEE 802.15.4 channel used for this application. */
#define IEEE802_15_4_CHANNEL 26
/* Transmission power for this application. */
#define CC2420_TX_POWER 31

struct blocks{
    int prevhash[16];
    int transactions[6];
    int hash[16];
    int ID;
    struct blocks * next;
};


struct accesslist
{
    /* The ->next pointer is needed since we are placing these on a
     Contiki list. */
    struct accesslist *next;

    /* The ->addr field holds the Rime address of the neighbor. */
    rimeaddr_t addr;
};


typedef struct blocks *blocks_t;

//char previoushash[16];
//char transactions[6];
bool genblk = false;
bool init = false;
int16_t ctr = 0;

void accessinit();
    //blocks_t blockinit(int data[]);
blocks_t createBlock();
blocks_t addblock(blocks_t head, int data[6]);
int *hash(int prevhash[16], int transactions[16]);

void getPreviousHash();
void getPreviousTxn();
void getBlockHash();
#endif
