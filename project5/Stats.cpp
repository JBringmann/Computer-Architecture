/******************************
 * Name:  Johnathan Bringmann jab479
 * CS 3339 - Spring 2019
 ******************************/
#include "Stats.h"

Stats::Stats() {
  cycles = PIPESTAGES - 1; // pipeline startup cost
  flushes = 0;
  bubbles = 0;

  memops = 0;
  branches = 0;
  taken = 0;
  stalls = 0;

  for(int i = IF1; i < PIPESTAGES; i++) {
    resultReg[i] = -1;
  }
}

void Stats::clock() {
  cycles++;
  
  // advance all pipeline flip flops
  for(int i = WB; i > IF1; i--) {
    resultReg[i] = resultReg[i-1];
  }
  // inject a no-op into IF1
  resultReg[IF1] = -1;
}

void Stats::registerSrc(int r) {
  for(int i = EXE1; i < WB; i++)
  {
    if(resultReg[i] == r){        // if R is in flight somewhere
      do{
        bubble();                 // bubble until
      }while(resultReg[WB] != r); // R reaches WB  
      break;
    }
  }   
}

void Stats::registerDest(int r) {
  resultReg[ID] = r;
}

void Stats::flush(int count) { // count == how many ops to flush
  for ( int i = 0; i < count; i++)
  {
     flushes++;
     clock();
  } 
}

void Stats::bubble() {
  cycles++;
  bubbles++;
  // advance all pipeline flip flops
  for(int i = WB; i > EXE1; i--) {
    resultReg[i] = resultReg[i-1];
  }
  // inject a no-op into EXE1 
  resultReg[EXE1] = -1;
}

void Stats::stall(int count) { // count == how many ops to stall
  for ( int i = 0; i < count; i++)
  {
     cycles++;
     stalls++;
  } 
}

