#ifndef _BLOCK_H
#define _BLOCK_H

#import <stdio.h>
#import <math.h>

struct ablock{
    char prevhash[16];
    int transactions[6];
    char hash[16];
} blocks;

typedef struct linkedlist {
    ablock bc;
    struct node * next;
} node_t;

char[16] previoushash;
char[6] transactions;
bool genblk=false;

void blockinit();

void blockgen(ablock* blockarr, int txn[6], char prevhash[16]);

void getPreviousHash();
void getPreviousTxn();
void getBlockHash();
