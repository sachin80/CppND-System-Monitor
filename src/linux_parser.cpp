#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "linux_parser.h"

using std::stol;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float memory_utilization{0.0};
  std::unordered_map<std::string, float> memutil_map;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'k', ' ');
      std::replace(line.begin(), line.end(), 'B', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal" || key == "MemFree" || key == "MemAvailable" || key ==  "Buffers" ) {
          memutil_map[key] = stof(value);
        }
      }
    }
  }
  memory_utilization = (memutil_map["MemTotal"]  -  memutil_map["MemFree"]) / memutil_map["MemTotal"];
  return memory_utilization;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptime, idle;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  }
  // system uptime is in seconds
  return uptime;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {  
  std::unordered_map<string, vector<string>> util_map;
  string line, cpu;
  string match{"cpu"};
  vector<string> cpu_utilization;
  std::ifstream filestream (kProcDirectory+kStatFilename);
  while (std::getline(filestream, line)) {
    if (!line.compare(0, match.size(), match)) {
      std::istringstream linestream(line);
      linestream >> cpu;
      util_map[cpu] = vector<string>();
      string jiffy;
      while (linestream >> jiffy) {
        util_map[cpu].push_back(jiffy);
      }
    }
  }
  cpu_utilization = util_map["cpu"];
  return cpu_utilization;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> cpu_util = LinuxParser::CpuUtilization();
  long total_jiffies{0};
  for (auto jiffy : cpu_util)
      total_jiffies += stol(jiffy);
  return total_jiffies;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
     string line, stat;
     vector<string> stats;
     long active_jiffies{0};
     string kPID = to_string(pid);
     std::ifstream filestream(kProcDirectory+kPID+kStatFilename);
     if (filestream.is_open()) {
         getline(filestream, line);
         std::istringstream linestream(line);
         while (linestream >> stat) {
             stats.push_back(stat);
         }
     }
     for (int i = 0; i < stats.size(); ++i) {
         if (i > 12 && i < 17)
              active_jiffies += stol(stats[i]);
     }
     // In "clock ticks" 
     return active_jiffies;
}

 // DONE: Read and return the number of active jiffies for the system
 long LinuxParser::ActiveJiffies() {
     vector<string> cpu_util = LinuxParser::CpuUtilization();
     long active_jiffies{0};
     // Active jiffies correspond to everyting except kIdle_ and kIOwait
     for (int i = 0; i < cpu_util.size() ; ++i) {
         if (i ==  CPUStates::kIdle_ || i == CPUStates::kIOwait_)
             continue;
         active_jiffies += stol(cpu_util[i]);
     }
     return active_jiffies;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpu_util = LinuxParser::CpuUtilization();
  long idle_jiffies = stol(cpu_util[CPUStates::kIdle_]) + stol(cpu_util[CPUStates::kIOwait_]);
  return idle_jiffies;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  int totalprocs;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          totalprocs = std::stoi(value);
        }
      }
    }
  }
  return totalprocs;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  int runningprocs;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          runningprocs = std::stoi(value);
        }
      }
    }
  }
  return runningprocs;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  string kPID = to_string(pid);
  std::ifstream filestream(kProcDirectory+kPID+kCmdlineFilename);
  if (filestream.is_open()) {
      getline(filestream, line);
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  float ramkb;
  string kPID = to_string(pid);
  std::ifstream filestream(kProcDirectory+kPID+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          ramkb = std::stof(value);
        }
      }
    }
  }
  float rammb = 0.001 * ramkb;
  return std::to_string(static_cast<long>(rammb));
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value, uid;
  string kPID = to_string(pid);
  std::ifstream filestream(kProcDirectory+kPID+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          uid = value;
        }
      }
    }
  }
  return uid; 
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
     string line, key, value, user;
     string uid = LinuxParser::Uid(pid);
     string kPID = to_string(pid);
     std::ifstream filestream(kPasswordPath);
     if (filestream.is_open()) {
         getline(filestream, line);
         std::replace(line.begin(), line.end(), ':', ' ');
         std::replace(line.begin(), line.end(), '/', ' ');
         std::replace(line.begin(), line.end(), 'x', ' ');
         std::istringstream linestream(line);
         while(linestream>>key>>value) {
             if (value == uid)
                 user = key;
         }
      }
      return user; 
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, stat;
  vector<string> stats;
  long start_time;
  string kPID = to_string(pid);
  std::ifstream filestream(kProcDirectory+kPID+kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> stat) {
      stats.push_back(stat);
    }
}
// Convert from "clock ticks" to seconds
start_time = stol(stats[21]) / sysconf(_SC_CLK_TCK);
return start_time; 
}