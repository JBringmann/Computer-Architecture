/******************************
 * Submitted by: Johnathan Bringmann jab479
 * CS 3339 - Spring 2019
 * Project 4 Branch Predictor
 * Copyright 2019, all rights reserved
 * Updated by Lee B. Hinkle based on prior work by Martin Burtscher and Molly O'Neil
 ******************************/
#include <iostream>
#include <iomanip>
#include <cstdint>
#include "BranchPred.h"
using namespace std;

BranchPred::BranchPred() {
  /* TODO Initialize your private variables and pred[] array here*/
  cout << "Branch Predictor Entries: " << BPRED_SIZE << endl;
  for(int i = 0; i < BPRED_SIZE; i++){
     pred[i] = 0;
     btb[i] = 0;
  }
  
  predictions = 0;
  pred_takens = 0;
  mispredictions = 0;
  mispred_direction = 0;
  mispred_target = 0;
}

bool BranchPred::predict(uint32_t pc, uint32_t &target) {
  /* target value updated by reference to match the btb entry
   * returns true if prediction is taken, false otherwise
   * increments predictions and if necessary pred_takens count
   */   
  predictions++;  
  int index = (pc >> 2) % BPRED_SIZE;
  bool predTaken = false;
  target = btb[index]; //predicted target address
  
  if(pred[index] >= 2){
     pred_takens++;
     predTaken = true;
  }
  
      
  D(cout << endl << "    BPRED: bne/beq@addr " << hex << setw(8) << pc << "pred[] value = "  
  << pred[index] << " predTaken is " << boolalpha << (bool)predTaken; (predTaken==0)? cout 
  << " not taken":cout << " taken target addr " << hex << setw(8) << target;)
  return predTaken;
}

bool BranchPred::isMispredict(bool predTaken, uint32_t predTarget, bool taken, uint32_t target) {
  /* implement a function which will return:
   *  false if prediction is correct, both predTaken and predTarget match actual values
   *  true if prediction is incorrect
   *  also updates mispred_direction, mispred_target, and mispredictions counts
   */
 bool wrongDirection = predTaken != taken;
 bool wrongTarget = predTarget != target;
   if(wrongDirection)                
      mispred_direction++;         
   else if(predTaken && wrongTarget)       
     mispred_target++;      
  
   if( wrongDirection || (predTaken && wrongTarget)){
      mispredictions++;
      return true;     //predicted wrong
   }
   else               
      return false;  // predicted correctly
}


void BranchPred::update(uint32_t pc, bool taken, uint32_t target) {
  /* pred counter value should be updated and
   * if branch is taken, the target address also gets updated
   */
  int index = (pc >> 2) % BPRED_SIZE;
  pred[index] = transition(pred[index], taken);     //update 2 bit counter for this branch
  if(taken)
     btb[index] = target;  // new target = 'pc + (simm << 2 ) if taken, else doesn't change
}

int BranchPred::transition(int counter, bool taken) {
  /* This updates the 2-bit saturating counter values
   * You will need to use it, but shouldn't have to modify.
   */
  int transition;
  switch(counter) {
    case 0: transition = (taken ? 1 : 0);
            break;
    case 1: transition = (taken ? 2 : 0);
            break;
    case 2: transition = (taken ? 3 : 1);
            break;
    case 3: transition = (taken ? 3 : 2);
            break;
    default: cerr << "ERROR: 2-bit saturating counter FSM in illegal state" << endl;
  }
  return transition;
}

void BranchPred::printFinalStats() {
  int correct = predictions - mispredictions;
  int not_takens = predictions - pred_takens;

  cout << setprecision(1);
  cout << "Branches predicted: " << predictions << endl;
  cout << "  Pred T: " << pred_takens << " ("
       << (100.0 * pred_takens/predictions) << "%)" << endl;
  cout << "  Pred NT: " << not_takens << endl;
  cout << "Mispredictions: " << mispredictions << " ("
       << (100.0 * mispredictions/predictions) << "%)" << endl;
  cout << "  Mispredicted direction: " << mispred_direction << endl;
  cout << "  Mispredicted target: " << mispred_target << endl;
  cout << "Predictor accuracy: " << (100.0 * correct/predictions) << "%" << endl;
}
