#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    string hh, mm, ss;
     // Convert to digital hours
     unsigned long hours = (seconds / 3600) % 24;
     if (seconds < 3600) {
        hh = "00";
     } else if (hours < 10) {
         hh = "0" + std::to_string(hours);
     } else {
         hh = std::to_string(hours);
     }
     // Convert to digital minutes
     unsigned long minutes = (seconds % 3600) / 60;
     if (seconds < 60) {
         mm = "00";
     } else if (minutes < 10) {
         mm = "0" + std::to_string(minutes);
     } else {
         mm = std::to_string(minutes);
     }
     // Convert to digital seconds
     unsigned long remaining_seconds = (seconds % 3600) % 60;
     if (remaining_seconds == 0) {
         ss = "00";
     } else if (remaining_seconds < 10) {
         ss = "0" + std::to_string(remaining_seconds);
     } else {
         ss = std::to_string(remaining_seconds);
     }

     return hh + ":" + mm + ":" + ss;    
}