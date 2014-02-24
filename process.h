/* process.h */
#ifndef PROCESS_H
#define PROCESS_H
#include <iostream>

using namespace std;

class process
{
 private:
  unsigned int num;
  unsigned int runTime;
  unsigned int priority;
  int initialWaitTime;
  unsigned int totalWaitTime;
  unsigned int turnAroundTime;
  unsigned int runTimeRemaining;
  unsigned int arrivalTime;

 public:
  process();
  process(unsigned int aNum, unsigned int aRunTime, unsigned int aPriority);
  unsigned int getPid();
  unsigned int getRunTime();
  unsigned int getPriority();
  int getInitialWaitTime();
  unsigned int getTotalWaitTime();
  unsigned int getTurnAroundTime();
  unsigned int getRunTimeRemaining();
  unsigned int getArrivalTime();
  void setInitialWaitTime( unsigned int time );
  void setTotalWaitTime( unsigned int time );
  int reduceRunTimeRemaining( unsigned int time );
  
};
#endif
