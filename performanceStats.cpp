/* performanceStats.cpp */
#include "performanceStats.h"

performanceStats::performanceStats()
{
  turnAroundTime_min = (unsigned int) pow(2, 32) - 1;
  turnAroundTime_avg = 0;
  turnAroundTime_max = 0;
  initialWaitTime_min = (unsigned int) pow(2, 32) - 1;
  initialWaitTime_avg = 0;
  initialWaitTime_max = 0;
  totalWaitTime_min = (unsigned int) pow(2, 32) - 1;
  totalWaitTime_avg = 0;
  totalWaitTime_max = 0;
}
void performanceStats::newProcessStat( unsigned int newTurnAroundTime, unsigned int newInitialWaitTime, unsigned int newTotalWaitTime )
{
  if ( newTurnAroundTime < turnAroundTime_min )
    turnAroundTime_min = newTurnAroundTime;
  if ( newTurnAroundTime > turnAroundTime_max )
    turnAroundTime_max = newTurnAroundTime;
  turnAroundTime_avg += newTurnAroundTime;
  
  if ( newInitialWaitTime < initialWaitTime_min )
    initialWaitTime_min = newInitialWaitTime;
  if ( newInitialWaitTime > initialWaitTime_max )
    initialWaitTime_max = newInitialWaitTime;
  initialWaitTime_avg += newInitialWaitTime;
  
  if ( newTotalWaitTime < totalWaitTime_min )
    totalWaitTime_min = newTotalWaitTime;
  if ( newTotalWaitTime > totalWaitTime_max )
    totalWaitTime_max = newTotalWaitTime;
  totalWaitTime_avg += newTotalWaitTime;
}
void performanceStats::calculateAverages( unsigned int numProcesses )
{
  turnAroundTime_avg  /= numProcesses;
  initialWaitTime_avg /= numProcesses;
  totalWaitTime_avg   /= numProcesses;
}
void performanceStats::print( unsigned int numProcesses )
{
  calculateAverages( numProcesses );
  cout << "----------PERFORMANCE STATISTICS----------" << endl
       << "Turnaround Time" << endl
       << "   minimum: " << turnAroundTime_min << endl
       << "   maximum: " << turnAroundTime_max << endl
       << "   average: " << turnAroundTime_avg << endl
    
       << "Initial Wait Time" << endl
       << "   minimum: " << initialWaitTime_min << endl
       << "   maximum: " << initialWaitTime_max << endl
       << "   average: " << initialWaitTime_avg << endl

       << "Total Wait Time" << endl
       << "   minimum: " << totalWaitTime_min << endl
       << "   maximum: " << totalWaitTime_max << endl
       << "   average: " << totalWaitTime_avg << endl
       << "------------------------------------------" << endl
       << endl;
}
void performanceStats::addProcess( unsigned int pid, unsigned int runTime, unsigned int priority )
{
  processes[pid];
}
