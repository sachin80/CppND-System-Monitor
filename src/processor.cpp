#include "processor.h"
// Accessor functions to return member variables.
 float Processor::GetActiveTime() const { return active_time_; }
 float Processor::GetIdleTime() const { return idle_time_; }
 float Processor::GetTotalTime() const { return total_time_; }

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    // Sleep for 10 ms. 
    usleep(10);

    // Update sample. 
    float active_time = static_cast<float>(LinuxParser::ActiveJiffies());
    float idle_time = static_cast<float>(LinuxParser::IdleJiffies());
    float total_time = active_time + idle_time;

    // Deltas between samples. 
    float delta_active_time = active_time - GetActiveTime();
    float delta_total_time = total_time - GetTotalTime();

    return delta_active_time / delta_total_time;

}