#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//DONE: Return this process's ID
int Process::Pid() { return pid_; }

 // DONE: Return this process's CPU utilization
 float Process::CpuUtilization() { 
    // Stackoverflow #16736599
     long total_time = LinuxParser::ActiveJiffies(Pid());
     long uptime = LinuxParser::UpTime();               // System uptime (sec)
     long starttime = LinuxParser::UpTime(Pid());       // Pid uptime (sec)
     float seconds = uptime - starttime;
     float  cpu_utilization = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);   
     return cpu_utilization; 
}

// Use accessor function to return cpu_utilization_.
float Process::GetCpuUtilization() const { return cpu_utilization_;}

 // DONE: Return the command that generated this process
 string Process::Command() { 
     return LinuxParser::Command(Pid()); 
}

 // DONE: Return this process's memory utilization
 string Process::Ram() { 
     return LinuxParser::Ram(Pid()); 
}

 // DONE: Return the user (name) that generated this process
 string Process::User() { 
     return LinuxParser::User(Pid());
}

 // DONE: Return the age of this process (in seconds)
 long int Process::UpTime() { 
     return LinuxParser::UpTime(Pid());
}

 // DONE: Overload the "less than" comparison operator for Process objects
 bool Process::operator<(Process const& a) const { 
     return GetCpuUtilization() < a.GetCpuUtilization();
}
 