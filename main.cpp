
/* main.cpp */
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <list>
#include "process.h"
#include "performanceStats.h"

//#define DEBUG_MODE

/* Some shorthand abbreviations:
   tat == Turnaround Time
   iwt == Initial Wait Time
   twt == Total Wait Time
   min == Minimum
   avg == Average
   max == Maximum
   (e.g. tat_avg == Average Turnaround Time)
*/

using namespace std;

void fcfs( queue<process> batch );
void sjf( queue<process> batch );
void rr( queue<process> batch );
void psjf( queue<process> batch );
void pp(queue<process> batch );
list<process> sortByPriority( queue<process>& batch );
process createProcess( unsigned int num );
process preemptionCheck( unsigned int startTime,
			 unsigned int endTime,
			 process curProcess,
			 list<process>& batch,
			 string algorithm );

/* Set some global constants */
const unsigned int numProcesses = 8;
const unsigned int t_cs_in = 4;  /* 4ms context switch cost to swap process in */
const unsigned int t_cs_out = 4; /* 4ms context switch cost to swap process out */
const unsigned int minRunTime = 100; // 500;
const unsigned int maxRunTime = 300; // 7500;
const unsigned int timeSlice = 100;

int main (int argc, char ** argv)
{

  /* Seed the random number generator */
  srand( (unsigned) time(0) );

  /* Create batch of random processes */
  queue<process> batch;
  for ( unsigned int i = 0; i < numProcesses; ++i )
  {
    batch.push( createProcess( i+1 ) );
  }
  
  cout << "----------------------------------------------------------------" << endl
       << "    FCFS test" << endl
       << "----------------------------------------------------------------" << endl;

  fcfs( batch );
  
  cout << "----------------------------------------------------------------" << endl
       << "    SJF test" << endl
       << "----------------------------------------------------------------" << endl;
  
  sjf( batch );

  cout << "----------------------------------------------------------------" << endl
       << "    RR test" << endl
       << "----------------------------------------------------------------" << endl;

  rr( batch );
  
  cout << "----------------------------------------------------------------" << endl
       << "    PP test" << endl
       << "----------------------------------------------------------------" << endl;

  pp( batch );
  
  return 0;
}

void fcfs( queue<process> batch )
{
  unsigned int time;
  performanceStats theStats;
  double tat_cur, iwt_cur, twt_cur;
  
  time = 0;
  
  /* Display processes already in the ready queue (create at time = 0) */
  for ( unsigned int i = 0; i < batch.size(); ++i )
  {
    process temp = batch.front();
    cout << "[time " << time << "ms] Process " << temp.getPid() << " created (requiring " << temp.getRunTime() << "ms CPU time)" << endl;
    batch.pop();
    batch.push( temp );
  }
  
  /* Go through the queue, using FIFO to schedule the processes */
  int i = 1;
  while ( ! batch.empty() )
  {
    if (i != 1)
      time += t_cs_in; /* context switch in new process */
    ++i;
    
    iwt_cur = time;
    process temp = batch.front();
    cout << "[time " << time << "ms] Process " << temp.getPid() << " accessed CPU for the first time (wait time " << iwt_cur << "ms)" << endl;
    time += temp.getRunTime();
    batch.pop();
    twt_cur = time;
    tat_cur = time - iwt_cur;
    
    cout << "[time " << time << "ms] Process " << temp.getPid() << " terminated (turnaround time " << tat_cur << "ms, wait time " << twt_cur << "ms)" << endl;
    if ( batch.empty() )
    {
      /* last process - only perform first half of context switch - swap out final process, but don't swap in a new process */
      cout << "[time " << time << "ms] Context switch (swapped out process " << temp.getPid() << ") (final process)" << endl;
    }
    else
    {
      cout << "[time " << time << "ms] Context switch (swapped out process " << temp.getPid() << " for process " << batch.front().getPid() << ")" << endl;
      time += t_cs_in;
    }
    theStats.newProcessStat( tat_cur, iwt_cur, twt_cur );
  }
  cout << "[time " << time << "ms] Done! (completed all " << numProcesses << " processes)" << endl;
  theStats.print( numProcesses );
}

void sjf( queue<process> batch )
{
  unsigned int time;
  queue<process> sjf_batch;
  performanceStats theStats;
  double tat_cur, iwt_cur, twt_cur;
  
  time = 0;
  
  /* Display processes already in the ready queue (create at time = 0) */
  for ( unsigned int i = 0; i < batch.size(); ++i )
  {
    process temp = batch.front();
    cout << "[time " << time << "ms] Process " << temp.getPid() << " created (requiring " << temp.getRunTime() << "ms CPU time)" << endl;
    batch.pop();
    batch.push( temp );
  }
  
  /* Copy ready queue to a linked list (for sorting purposes) */
  list<process> batch_list;
  batch_list.push_back( batch.front() );
  batch.pop();
  while ( ! batch.empty() )
  {
    process temp = batch.front();
    bool inserted = false;
    for ( list<process>::iterator iter = batch_list.begin(); iter != batch_list.end(); ++iter )
    {
      if ( temp.getRunTime() < iter->getRunTime() )
      {
	batch_list.insert( iter, temp );
	inserted = true;
	break;
      }
    }
    if ( ! inserted )
      batch_list.push_back( temp );
    batch.pop();
  }

  // cout << "SJF-sorted ready queue:" << endl;
  // cout << "RUNTIME   " << "PID       " << "PRIORITY  " << endl;
  // for ( list<process>::iterator iter = batch_list.begin(); iter != batch_list.end(); ++iter )
  // {
  //   cout << iter->getRunTime() << "      " << iter->getPid() << "     " << iter->getPriority() << endl;
  // }
  
  /* Iterate through the SJF-sorted list, outputting each process's CPU access */
  for ( list<process>::iterator iter = batch_list.begin(); iter != batch_list.end(); ++iter )
  {
    if ( iter != batch_list.begin() )
      time += t_cs_in; /* context switch in new process */
    
    iwt_cur = time;
    cout << "[time " << time << "ms] Process " << iter->getPid() << " accessed CPU for the first time (wait time " << iwt_cur << "ms)" << endl;
    time += iter->getRunTime();
    twt_cur = time;
    tat_cur = time - iwt_cur;
    
    cout << "[time " << time << "ms] Process " << iter->getPid() << " terminated (turnaround time " << tat_cur << "ms, wait time " << twt_cur << "ms)" << endl;
    if ( ++iter == batch_list.end() )
    {
      /* last process - only perform first half of context switch - swap out final process, but don't swap in a new process */ 
      cout << "[time " << time << "ms] Context switch (swapped out process " << (--iter)->getPid() << ") (final process)" << endl;
    }
    else
    {
      unsigned int next_pid = iter->getPid();
      cout << "[time " << time << "ms] Context switch (swapped out process " << (--iter)->getPid() << " for process " << next_pid << ")" << endl;
      time += t_cs_in;
    }
    theStats.newProcessStat( tat_cur, iwt_cur, twt_cur );
  }
  cout << "[time " << time << "ms] Done! (completed all " << numProcesses << " processes)" << endl;
  theStats.print( numProcesses );
}


void rr( queue<process> batch )
{
  unsigned int time, numTerminatedProcesses;
  performanceStats theStats;

  time = 0;
  
  /* Display processes already in the ready queue (create at time = 0) */
  for ( unsigned int i = 0; i < batch.size(); ++i )
  {
    process temp = batch.front();
    cout << "[time " << time << "ms] Process " << temp.getPid() << " created (requiring " << temp.getRunTime() << "ms CPU time)" << endl;
    batch.pop();
    batch.push( temp );
  }
  int counter = 0, rc, garbage;
  numTerminatedProcesses = 0;
  while( 1 )
  {
    counter++;
    process temp = batch.front();
    if(numTerminatedProcesses < numProcesses)
      batch.pop();
    if(temp.getRunTimeRemaining() != 0)
    {
      if(temp.getInitialWaitTime() == -1)
      {
	temp.setInitialWaitTime(time);
	cout << "[time " << time << "ms] Process " << temp.getPid() << " accessed CPU for the first time (wait time " << time << "ms)" << endl;
      }
      else
      {
	cout << "[time " << time << "ms] Process " << temp.getPid() << " accessed CPU again (runTimeRemaining = " << temp.getRunTimeRemaining() << ")" << endl;
      }
      
      rc = temp.reduceRunTimeRemaining(timeSlice);
      if(rc == -1)
      {
	/* Process does not complete in this burst period */
	time += timeSlice;
      }
      else
      {
	/* Process completes in this burst period */
	time += timeSlice - rc;
	
	++numTerminatedProcesses;
	temp.setTotalWaitTime(time);
	cout << "[time " << time << "ms] Process " << temp.getPid() << " terminated (turnaround time " << temp.getTurnAroundTime() << "ms, wait time " << temp.getTotalWaitTime() << "ms)" << endl;
      }
      
      if(numTerminatedProcesses == numProcesses)
      {
	/* ready queue now "empty" -- context switch out, and end algorithm */
	cout << "[time " << time << "ms] Context switch (swapped out process " << temp.getPid() << ") (final process)" << endl;
	temp.setTotalWaitTime(time);
	cout << "Total wait time for final process " << temp.getPid() << " is = " << temp.getTotalWaitTime() << endl;
	garbage = temp.getTurnAroundTime();
	cout << "Turn Around time for final process " << temp.getPid() << " is = " << temp.getTurnAroundTime() << endl;
	time += t_cs_out;
	cout << "[time " << time << "ms] All processes complete. PEACE, GAME OVER!" << endl;
	batch.push(temp);
	break;
      }
      else
      {
	/* ready queue has unfinished processes */
	cout << "[time " << time << "ms] Context switch (swapped out process " << temp.getPid() << " for process " << batch.front().getPid() << ")" << endl;
	time += t_cs_out;
	time += t_cs_in;
      }
    }
    batch.push(temp);
  }

  /* Display processes info about times */
  // unsigned int j = batch.size();
  // cout << "size of the batch = " << j << endl;
  // for ( unsigned int i = 0; i < j; ++i )
  // {
  //   process temp = batch.front();
  //   cout << "Process id [" << temp.getPid() << "] Turnaround Time =  [" 
  // 	 << temp.getTurnAroundTime() << "] *** Initial Wait time =  [" 
  // 	 << temp.getInitialWaitTime() << "] *** Total Wait time = [" 
  // 	 << temp.getTotalWaitTime() << "]" << endl;
  //   theStats.newProcessStat( temp.getTurnAroundTime(), temp.getInitialWaitTime(), temp.getTotalWaitTime() );
  //   batch.pop();
  //   batch.push( temp );
  // }
  cout << "Size of batch is still " << batch.size() << endl;
  while( ! batch.empty() )
    {
      process temp = batch.front();
      cout << "Process id [" << temp.getPid() << "] Turnaround Time =  [" 
	   << temp.getTurnAroundTime() << "] *** Initial Wait time =  [" 
	   << temp.getInitialWaitTime() << "] *** Total Wait time = [" 
	   << temp.getTotalWaitTime() << "]" << endl;
      theStats.newProcessStat( temp.getTurnAroundTime(), temp.getInitialWaitTime(), temp.getTotalWaitTime() );
      batch.pop();
      
    }
  theStats.print(numProcesses);
}

void psjf( queue<process> batch )
{
  unsigned int time;
  queue<process> sjf_batch;
  performanceStats theStats;
  double tat_cur, iwt_cur, twt_cur;
  
  time = 0;
  
  /* Display processes already in the ready queue (created at time = 0) */
  for ( unsigned int i = 0; i < batch.size(); ++i )
  {
    process temp = batch.front();
    cout << "[time " << time << "ms] Process " << temp.getPid() << " created (requiring " << temp.getRunTime() << "ms CPU time)" << endl;
    batch.pop();
    batch.push( temp );
  }
  
  /* Copy ready queue to a linked list (for sorting purposes) */
  list<process> batch_list;
  batch_list.push_back( batch.front() );
  batch.pop();
  while ( ! batch.empty() )
  {
    process temp = batch.front();
    bool inserted = false;
    for ( list<process>::iterator iter = batch_list.begin(); iter != batch_list.end(); ++iter )
    {
      if ( temp.getRunTime() < iter->getRunTime() )
      {
	batch_list.insert( iter, temp );
	inserted = true;
	break;
      }
    }
    if ( ! inserted )
      batch_list.push_back( temp );
    batch.pop();
  }

  // cout << "SJF-sorted ready queue:" << endl;
  // cout << "RUNTIME   " << "PID       " << "PRIORITY  " << endl;
  // for ( list<process>::iterator iter = batch_list.begin(); iter != batch_list.end(); ++iter )
  // {
  //   cout << iter->getRunTime() << "      " << iter->getPid() << "     " << iter->getPriority() << endl;
  // }
  
  /* Iterate through the SJF-sorted list, outputting each process's CPU access */
  for ( list<process>::iterator iter = batch_list.begin(); iter != batch_list.end(); ++iter )
  {
    if ( iter != batch_list.begin() )
      time += t_cs_in; /* context switch in new process */
    
    iwt_cur = time;
    cout << "[time " << time << "ms] Process " << iter->getPid() << " accessed CPU for the first time (wait time " << iwt_cur << "ms)" << endl;
    time += iter->getRunTime();
    twt_cur = time;
    tat_cur = time - iwt_cur;
    
    cout << "[time " << time << "ms] Process " << iter->getPid() << " terminated (turnaround time " << tat_cur << "ms, wait time " << twt_cur << "ms)" << endl;
    if ( ++iter == batch_list.end() )
    {
      /* last process - only perform first half of context switch - swap out final process, but don't swap in a new process */ 
      cout << "[time " << time << "ms] Context switch (swapped out process " << (--iter)->getPid() << ") (final process)" << endl;
    }
    else
    {
      unsigned int next_pid = iter->getPid();
      cout << "[time " << time << "ms] Context switch (swapped out process " << (--iter)->getPid() << " for process " << next_pid << ")" << endl;
      time += t_cs_in;
    }
    theStats.newProcessStat( tat_cur, iwt_cur, twt_cur );
  }
  cout << "[time " << time << "ms] Done! (completed all " << numProcesses << " processes)" << endl;
  theStats.print( numProcesses );
}

void pp( queue<process> batch )
{
  unsigned int time, timeSlice, numTerminatedProcesses;
  performanceStats theStats;

  time = 0;
  timeSlice = 100;
  
  /* Display processes already in the ready queue (create at time = 0) */
  for ( unsigned int i = 0; i < batch.size(); ++i )
  {
    process temp = batch.front();
    cout << "[time " << time << "ms] Process " << temp.getPid()
	 << " created (requiring " << temp.getRunTime() 
	 << "ms CPU time, with priority level " << temp.getPriority() << ")" << endl;
    theStats.addProcess( temp.getPid(), temp.getRunTime(), temp.getPriority() );
    batch.pop();
    batch.push( temp );
  }
  /* Display priority-sorted ready queue */
  list<process> sorted_batch = sortByPriority( batch );
  for ( list<process>::iterator itr = sorted_batch.begin(); itr != sorted_batch.end(); ++itr )
  {
    cout << itr->getPid() << " | " << itr->getRunTime() << " | " << itr->getPriority() << endl;
  }

  numTerminatedProcesses = 0;

  /* Go through the queue, using FIFO to schedule the processes */
  int i = 1;
  list<process>::iterator itr = sorted_batch.begin();
  while ( 1 )
  {
    if (i != 1)
      time += t_cs_in; /* context switch in new process */
    ++i;
    
    if ( itr->getInitialWaitTime() == -1 )
    {
      itr->setInitialWaitTime( time );
      cout << "[time " << time << "ms] Process " << itr->getPid() << " accessed CPU for the first time (wait time " << time << "ms)" << endl;
    }
    
    time += itr->getRunTime();
    itr->reduceRunTimeRemaining( itr->getRunTime() );
    
    if ( itr->getRunTimeRemaining() == 0 )
    {
      itr->setTotalWaitTime( time );
      ++numTerminatedProcesses;
    }

    cout << "[time " << time << "ms] Process " << itr->getPid()
	 << " terminated (turnaround time " << itr->getTurnAroundTime() 
	 << "ms, wait time " << itr->getTotalWaitTime() << "ms)" << endl;
    if ( numTerminatedProcesses == numProcesses )
    {
      /* last process - only perform first half of context switch - swap out final process, but don't swap in a new process */
      cout << "[time " << time << "ms] Context switch (swapped out process " << itr->getPid() << ") (final process)" << endl;
      time += t_cs_out;
      break;
    }
    else
    {
      unsigned int oldPid = itr->getPid();
      cout << "[time " << time << "ms] Context switch (swapped out process " << oldPid << " for process " << (++itr)->getPid() << ")" << endl;
      --itr;
      
      time += t_cs_out;
    }
    theStats.newProcessStat( itr->getTurnAroundTime(), itr->getInitialWaitTime(), itr->getTotalWaitTime() );
    ++itr;
  }
  cout << "[time " << time << "ms] Done! (completed all " << numProcesses << " processes)" << endl;
  theStats.print( numProcesses );
}

/* ___function sortByPriority___
   input:    unsorted ready queue (STL queue)
   output:   ready queue sorted by ascending priority (STL list)
   *note - lowest priority number (0) is most urgent, and goes first in queue */
list<process> sortByPriority( queue<process>& batch )
{
  /* Copy ready queue to a linked list (for sorting purposes) */
  list<process> batch_list;
  batch_list.push_back( batch.front() );
  batch.pop();
  while ( ! batch.empty() )
  {
    process temp = batch.front();
    bool inserted = false;
    for ( list<process>::iterator iter = batch_list.begin(); iter != batch_list.end(); ++iter )
    {
      if ( temp.getPriority() < iter->getPriority() )
      {
	batch_list.insert( iter, temp );
	inserted = true;
	break;
      }
    }
    if ( ! inserted )
      batch_list.push_back( temp );
    batch.pop();
  }
  return batch_list;
}

process createProcess( unsigned int num )
{
  /* set process min/max times */
  const unsigned int min = minRunTime, max = maxRunTime;
  /* generate a number between min and max */
  int runTime = min + rand() % (max - min + 1);
  
  /* generate a number between 0 and 4 */
  int priority = rand() % 5;
  process temp( num, runTime, priority );
  return temp;
}

/* ___function preemptionCheck___
   checks for a process that should take over the current running process at some time over the given time interval
   if no process is found to take over, the input process is returned */
process preemptionCheck( unsigned int startTime, unsigned int endTime, process curProcess, list<process> batch, string algorithm )
{
  process temp = curProcess;
  for ( list<process>::iterator itr = batch.begin(); itr != batch.end(); ++itr )
  {
    if ( (itr->getArrivalTime() < startTime) || (itr->getArrivalTime() > endTime) )
      continue;
      
    if ( algorithm == "sjf" )
    {
      if ( itr->getRunTimeRemaining() < curProcess.getRunTimeRemaining() )
      {
	temp = *itr;
      }
    }
    else if ( algorithm == "pp" )
    {
      if ( itr->getPriority() < temp.getPriority() )
      {
	temp = *itr;
      }
    }
  }
  return temp;
}
