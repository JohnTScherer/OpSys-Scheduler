/* performanceStats.h */
#ifndef PERFORMANCE_STATS_H
#define PERFORMANCE_STATS_H
#include <iostream>
#include <math.h>
#include <map>
#include "process.h"

using namespace std;

class performanceStats
{
 private:
  unsigned int turnAroundTime_min, turnAroundTime_avg, turnAroundTime_max;
  unsigned int initialWaitTime_min, initialWaitTime_avg, initialWaitTime_max;
  unsigned int totalWaitTime_min, totalWaitTime_avg, totalWaitTime_max;
  
 public:
  map<unsigned int, process> processes;

  performanceStats();
  void newProcessStat( unsigned int newTurnAroundTime, unsigned int newInitialWaitTime, unsigned int newTotalWaitTime );
  void calculateAverages( unsigned int numProcesses );
  void print( unsigned int numProcesses );
  void addProcess( unsigned int pid, unsigned int runTime, unsigned int priority );
  
  /* void setInitialWaitTimeForPid( unsigned int pid, unsigned int time ); */
  /* void setTotalWaitTimeForPid( unsigned int pid, unsigned int time ); */
  /* void reduceRunTimeForPid( unsigned int pid, unsigned int time ); */
};
#endif
