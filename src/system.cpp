#include "system.h"
#include "process.h"
#include "processor.h"


using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(){
    if(cpu_ == nullptr) cpu_ = new Processor();
    auto pids = LinuxParser::Pids();
    processes_ = new vector<Process>;
    for(std::size_t i=0;i <pids.size();i++){
        Process* pro = new Process(pids.at(i));
        processes_->push_back(*pro);
    }
    std::sort(processes_->begin(),processes_->end());
    std::reverse(processes_->begin(),processes_->end());
}
// Return the system's CPU
Processor& System::Cpu() { return *cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { return *processes_; }

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }