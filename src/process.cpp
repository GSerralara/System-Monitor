#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;
using std::stoi;
Process::Process(int pid){
    _pid = pid;
    _user_uid = LinuxParser::Uid(pid);
    _user = LinuxParser::User(stoi(_user_uid));
    CpuUtilization();
}
// Return this process's ID
int Process::Pid() { return _pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
    auto procData = LinuxParser::ProcessConsumption(_pid);
    auto total_time = stoi(procData.at(0)) + stoi(procData.at(1));
    total_time += stoi(procData.at(2)) + stoi(procData.at(3));
    auto seconds = LinuxParser::UpTime();
    seconds -= std::stol(procData.at(4)) / sysconf(_SC_CLK_TCK);
    if(seconds != 0){
        _cpu = ((float)(total_time / sysconf(_SC_CLK_TCK)) / (float)seconds);
        return _cpu;
    }
    return 0; 
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(_pid); }

// Return this process's memory utilization
string Process::Ram() {  return LinuxParser::Ram(_pid) + " MB"; }

// Return the user (name) that generated this process
string Process::User() { return _user; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return this->_cpu < a._cpu; }