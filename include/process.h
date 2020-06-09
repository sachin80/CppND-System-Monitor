#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
 // Constructor for Process class. 
  Process(int pid): pid_(pid) {
      cpu_utilization_ = Process::CpuUtilization();
  }
  int Pid();                               // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization();                  // DONE: See src/process.cpp
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp
  // Accessor function for member variable.
  float GetCpuUtilization() const;

  // DONE: Declare any necessary private members
 private:
 int pid_;
 float cpu_utilization_;
};

#endif