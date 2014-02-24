/* process.cpp */
#include "process.h"

process::process()
{
  initialWaitTime = -1;
}
process::process(unsigned int aNum, unsigned int aRunTime, unsigned int aPriority)
{
  initialWaitTime = -1;
  num = aNum;
  runTime = runTimeRemaining = aRunTime;
  priority = aPriority;
}
unsigned int process::getPid()
{
  return num;
}
unsigned int process::getRunTime()
{
  return runTime;
}
unsigned int process::getPriority()
{
  return priority;
}
int process::getInitialWaitTime()
{
  return initialWaitTime;
}
unsigned int process::getTotalWaitTime()
{
  return totalWaitTime;
}
unsigned int process::getTurnAroundTime()
{
  turnAroundTime = totalWaitTime - initialWaitTime;
  return turnAroundTime;
}
unsigned int process::getRunTimeRemaining()
{
  return runTimeRemaining;
}
unsigned int process::getArrivalTime()
{
  return arrivalTime;
}
void process::setInitialWaitTime( unsigned int time )
{
  initialWaitTime = time;
}
void process::setTotalWaitTime( unsigned int time )
{
  totalWaitTime = time;
}
int process::reduceRunTimeRemaining( unsigned int time )
{
  if ( runTimeRemaining > time )
  {
    runTimeRemaining -= time;
    return -1;
  }
  else if ( runTimeRemaining == time )
  {
    runTimeRemaining = 0;
    return 0;
  }
  else
  {
    unsigned int tempRTR = runTimeRemaining;
    runTimeRemaining = 0;
    return time - tempRTR;
  }
}
