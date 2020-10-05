//                                                                
//   _           _   _            _____             _   _         
//  | |_ ___ ___| |_|_|___ ___   |   __|_ _ ___ ___| |_|_|___ ___ 
//  |   | .'|_ -|   | |   | . |  |   __| | |   |  _|  _| | . |   |
//  |_|_|__,|___|_|_|_|_|_|_  |  |__|  |___|_|_|___|_| |_|___|_|_|
//                        |___|                                   
//
// The structure of of the char array will be [Data = d, hours = h, minuite = m, previous hash=p]:
// block= previous hash = pppppp current timestamp
//
//
//
//

#include"hash.h"
#include"math.h"
#include"blockchain.h"

int *hash(char prevhash[16], char transactions[16]){
    int firsthash[16];
    if(genblk==false){
        for(int i=0; i<16; i++){
            if(i<6){
                firsthash[i]=transactions[i];
            }
            else{
                firsthash[i]=0;
            }
            //firsthash++;
            //transactions++;
        }
        for(int i=0; i<16; i++){
            firsthash[i]=firsthash[i]+1;
        }
    }
    else{
       for(int i=0; i<16; i++){
            if(i<6){
                firsthash[i]=prevhash[i]+transactions[i];
            }
            //firsthash++;
            //transactions++;   
        }
        for(int i=0; i<16; i++){
            firsthash[i] = firsthash[i] + 1;
        } 
    }
    return firsthash;
}
