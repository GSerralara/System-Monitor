#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int hours  = seconds/3600;
    int secs = seconds % 3600;
    int minutes = secs/60;
    seconds = secs % 60;
    char buffer[9];
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, secs);
    return buffer;
}