#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <unistd.h>
#include "linux_parser.h"

class Processor {
 public:
 // Default Constructor for the Processor class.
 Processor() {
     active_time_ = static_cast<float>(LinuxParser::ActiveJiffies());
     idle_time_ = static_cast<float>(LinuxParser::IdleJiffies());
     total_time_ = active_time_ + idle_time_;
 }

 // Accessor methods to return member variables. 
 float GetActiveTime() const;
 float GetIdleTime() const;
 float GetTotalTime() const;

 float Utilization();  // DONE: See src/processor.cpp

// DONE: Declare any necessary private members
 private:
 float active_time_;
 float idle_time_;
 float total_time_;

};

#endif