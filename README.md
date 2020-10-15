# Blockchain based Data Storage System on the TelosB Mote
### Implementation of a Blockchain based Data Storage System on the TelosB mote simulated on Contiki 2.7

This is a simulation of an "proof of concept" for running a Blockchain network on the TelosB mote using Contiki 2.7 to store sensor data.
Since the processing power and the storage space is limited, sacrifices had to be made that would ensure that the Blockchain Network would run efficiently on limited resources. 

These Include:

* **Weak Hashing Algorithm:** A blockchain can only be as secure as its hashing algorithm, and unfortunately, the blockchain generated in this project is not secure. The hashing algorithm used in the project is a
very weak hash, mainly meant as a placeholder until a better algorithm is implemented.

* **No Concensus Algorithm Implemented:** Without sufficient processing power and limited data bandwidth, this step would slow down the entire Blockchain Processes in theis project and hence had to be eliminated.

Since the concensus algoritm has not been implemented, this blockchain network ensures that each and evevry node in the network has a single identical copy of the data thus reducing the need for synchronization.

## Result
The final result was the creation of a private blockchain for the TelosB platform that generates blocks with temperature data recorded every n seconds (can be changed). 
The maximum number of possible blocks in the blockchain was found to be around 39 blocks. This includes both transmitted and generated blocks. The number of generated blocks would
decrease with an increase in the number of nodes.
