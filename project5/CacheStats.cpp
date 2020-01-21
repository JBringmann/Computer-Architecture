/******************************
 * CacheStats.cpp submitted by: Johnathan Bringmann jab479
 * CS 3339 - Spring 2019
 * Project 4 Branch Predictor
 * Copyright 2019, all rights reserved
 * Updated by Lee B. Hinkle based on prior work by Martin Burtscher and Molly O'Neil
 ******************************/
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "CacheStats.h"
using namespace std;

CacheStats::CacheStats() {
  cout << "Cache Config: ";
  if(!CACHE_EN) {
    cout << "cache disabled" << endl;
  } else {
    cout << (SETS * WAYS * BLOCKSIZE) << " B (";
    cout << BLOCKSIZE << " bytes/block, " << SETS << " sets, " << WAYS << " ways)" << endl;
    cout << "  Latencies: Lookup = " << LOOKUP_LATENCY << " cycles, ";
    cout << "Read = " << READ_LATENCY << " cycles, ";
    cout << "Write = " << WRITE_LATENCY << " cycles" << endl;
  }

  loads = 0;
  stores = 0;
  load_misses = 0;
  store_misses = 0;
  writebacks = 0;
  
  
  for(int set = 0; set < SETS; set++){
     for(int way = 0; way < WAYS; way++){
        validBits[set][way] = false;
        modifiedBits[set][way] = false;
        tags[set][way] = 0;
     }
     RR[set] = 0;   
   }  
}

int CacheStats::access(uint32_t addr, ACCESS_TYPE type) {
  if(!CACHE_EN) { // cache is off
    return (type == LOAD) ? READ_LATENCY : WRITE_LATENCY;
  }
  uint32_t index = (addr >> 5) % SETS;     // 3 word offset + 2 byte offset % # of sets
  uint32_t tag = addr >> 8;               //  3 index bits + 3 word offset + 2 byte offset 
  ///////////////check for HIT///////////////////
  for(int way = 0; way < WAYS; way++){  
     if(validBits[index][way] && tags[index][way] == tag){ 
        if(type == LOAD)
           loads++;
        if(type == STORE){
           stores++;
           modifiedBits[index][way] = true;
        }
        return LOOKUP_LATENCY;                  // HIT detected, return 0 
     }
  }
  //////////////// MISS//////////////////                                              
  bool dirty = modifiedBits[index][RR[index]];
  validBits[index][RR[index]] = true;        // "Load block into cache" 
  tags[index][RR[index]] = tag;
  if(type == LOAD){
     loads++; 
     load_misses++;
     modifiedBits[index][RR[index]] = false;    //reset bit
  }
  if(type == STORE){
     stores++; 
     store_misses++;
     modifiedBits[index][RR[index]] = true;     //comes in as modified
  }
  RR[index] = (RR[index] + 1) % WAYS;          // update to point to next way
 
  if(dirty){
     writebacks++;  
     return READ_LATENCY + WRITE_LATENCY;     // MISS & DIRTY return 40
  }
  return READ_LATENCY;                        // MISS & CLEAN return 30
} 



void CacheStats::printFinalStats() {
   
  for(int set = 0; set < SETS; set++)
     for(int way = 0; way < WAYS; way++)
        if(modifiedBits[set][way])
           writebacks++;

  int accesses = loads + stores;
  int misses = load_misses + store_misses;
  cout << "Accesses: " << accesses << endl;
  cout << "  Loads: " << loads << endl;
  cout << "  Stores: " << stores << endl;
  cout << "Misses: " << misses << endl;
  cout << "  Load misses: " << load_misses << endl;
  cout << "  Store misses: " << store_misses << endl;
  cout << "Writebacks: " << writebacks << endl;
  cout << "Hit Ratio: " << fixed << setprecision(1) << 100.0 * (accesses - misses) / accesses;
  cout << "%" << endl;
}
