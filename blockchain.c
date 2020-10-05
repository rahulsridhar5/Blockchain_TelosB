//Compiles on Cooja

#include <stdio.h>
#include "contiki.h"
#include "net/rime.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "dev/cc2420.h"
#include "node-id.h"
#include "dev/i2cmaster.h"
#include "dev/tmp102.h"
#include "math.h"

#include "blockchain.h"
//#include "hash.h"

#define period 10 //time-period for sampling temperature
#define TMP102_READ_INTERVAL (CLOCK_SECOND / 2) //Read Interavl for the temp sensor

/* Declare our "main" process. */
PROCESS(blockchain, "Blockchain");
//PROCESS(access, "Access_Control");

/* The client process should be started automatically when
 * the node has booted. */
AUTOSTART_PROCESSES(&blockchain);
//AUTOSTART_PROCESSES(&access);
/* Callback function for received packets.
 *
 * Whenever this node receives a packet for its broadcast handle,
 * this function will be called.
 *
 */
static void recv(struct broadcast_conn *c, const rimeaddr_t *from) {
	printf("Called");
	init = true;
	leds_off(LEDS_RED);
	/*Check whether the address that you get the packets from is in the access list*/
	leds_on(LEDS_GREEN);
}

void addtoaccess(rimeaddr_t address)
{
	/*add Function that when receives a button press from a supported mote adds it to this list. It should receive a particular message that should ideally be from a button press that
	  broadscasts it to the motes in its vicinity.*/
}

/*Simple hashing function*/
int *hash(int prevhash[16], int transactions[16])
{
	int i;
	int firsthash[16];
	if (genblk == false)
	{
		for (i = 0; i < 16; i++)
		{
			if (i < 6)
			{
				firsthash[i] = transactions[i];
			}
			else
			{
				firsthash[i] = 0;
			}
			//firsthash++;
			//transactions++;
		}
		for (i = 0; i < 16; i++)
		{
			firsthash[i] = firsthash[i] + 1;
		}
	}
	else
	{
		for (i = 0; i < 16; i++)
		{
			if (i < 6)
			{
				firsthash[i] = prevhash[i] + transactions[i];
			}
			//firsthash++;
			//transactions++;
		}
		for (i = 0; i < 16; i++)
		{
			firsthash[i] = firsthash[i] + 1;
		}
	}
	return firsthash;
}

/*Function to create a block*/
blocks_t createBlock(){
    	blocks_t bc;
    	bc = (blocks_t)malloc(sizeof(struct blocks));
		int i;
		for(i=0; i<16; i++){
			if (i<6)
				bc->transactions[i] = NULL;
			bc->prevhash[i] = NULL;
    		bc->hash[i] = NULL;
		}
		bc->ID = node_id; //Assigning Node ID from node-id.h file in cooja to Block.
		bc->next = NULL;
		return bc; //return created block
}

/*Function that is called to add a block*/
blocks_t addblock(blocks_t head, int data[6]){
	blocks_t a, b;	 // dleds_toggleeclare two nodes a and b
	a = createBlock(); //createNode will return a new node with data = value and next pointing to 0.

	int i; //Initilization in the loop only supported in C99
    for(i=0; i<6; i++){ // add element's value to data part of node
			a->transactions[i] = data[i];
	}
	
	if (head != NULL){ //If a genesis block is present, head wil != NULL
		for(i = 0; i < 16; i++)
			a->prevhash[i] = head->hash[i]; //hash of the previous block as the prevhash of the current block
	}
	int *tem_hash = (int *)malloc(sizeof(int)*16);
	tem_hash = hash(a->prevhash,a->transactions);
	
	for(i=0; i<16; i++){
			a->hash[i] = tem_hash[i];
		}
	
    if(head == NULL){
		for (i = 0; i < 16; i++)
			a->prevhash[i] = NULL; //hash of the previous block as the prevhash of the current block
		head = a;				   //when linked list is empty
		genblk = true;
	}

    else{
        b  = head; //assign head to b 
        while(b->next != NULL){
            b = b->next; //traverse the list until b is the last node.The last node always points to NULL.
        }
        b->next = a; //Point the previous last node to the new node created.
    }
    return head;
}

/* Broadcast handle to receive and send (identified) broadcast
 * packets. */
static struct broadcast_conn bc;
/* A structure holding a pointer to our callback function. */
static struct broadcast_callbacks bc_callback = { recv };

/* Our secondary process to grant access into the network*/
void accessinit(){
	
}


/*Our main process.*/
PROCESS_THREAD(blockchain, ev, data){

	static struct etimer periodic_timer;
	static struct etimer send_timer;

	PROCESS_BEGIN();

	int16_t tempint;
	int16_t raw;
	uint16_t absraw;
	int16_t sign;


	int data[6];
	int initdata[6]={0,0,0,0,0,0};

	tmp102_init();

	blocks_t blkchn = addblock(NULL, initdata);
		/* Activate the button sensor. */
	SENSORS_ACTIVATE(button_sensor);

		/* Open the broadcast handle, use the rime channel
	* defined by CLICKER_CHANNEL. */
	broadcast_open(&bc, BLOCKCHAIN_CHANNEL, &bc_callback);

	/* Set the radio's channel to IEEE802_15_4_CHANNEL */
	cc2420_set_channel(IEEE802_15_4_CHANNEL);

	/* Set the radio's transmission power. */
	cc2420_set_txpower(CC2420_TX_POWER);

	etimer_set(&periodic_timer, period);

	while (1)
	{
		//PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
		if (init==false){
			PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);

			leds_on(LEDS_RED);
			printf("Initilizing");
			/* Copy the string "hej" into the packet buffer. */
			packetbuf_copyfrom("hej", 4);
			/* Send the content of the packet buffer using the
		 * broadcast handle. */
			broadcast_send(&bc);
		}

		etimer_reset(&periodic_timer);
		etimer_set(&send_timer, TMP102_READ_INTERVAL);

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));

		sign = 1;

		printf("Reading Temp...\n");
		raw = tmp102_read_temp_raw();
		absraw = raw;
		if (raw < 0)
		{ // Perform 2C's if sensor returned negative data
			absraw = (raw ^ 0xFFFF) + 1;
			sign = -1;
		}
		tempint = (absraw >> 8) * sign;

		if (ctr < 6)
		{
			data[ctr]=tempint;
			ctr++;
		}
		if (ctr==6)
		{
			addblock(blkchn,data);
			ctr = 0;
		}

	}

		/* This will never be reached, but we'll put it here for
	 	* the sake of completeness: Close the broadcast handle. */
	broadcast_close(&bc);
	PROCESS_END();
}