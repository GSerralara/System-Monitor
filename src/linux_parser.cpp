#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  string os, kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string lineLabel, value, line;
  long memTotal = 0;
  long memAvailable =0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> lineLabel >> value;
      if(lineLabel == "MemTotal:"){
        memTotal = std::stol(value);
      }
      if(lineLabel == "MemFree:"){
        memAvailable = std::stol(value);
      }
    }    
  }
  if(memTotal != 0){
    long MemUsed = memTotal - memAvailable;
    float percent = (MemUsed * 100) / memTotal;
    return percent * .01;
  }
  return 0.0; 
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line, uptime, idle;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
    return std::stol(uptime);  
  }
  return 0; 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

      long user_long = stol(user);
      long nice_long = stol(nice);
      long system_long = stol(system);
      long idle_long = stol(idle);
      long nowait_long = stol(iowait);
      long irq_long = stol(irq);
      long softirq_long = stol(softirq);
      long steal_long = stol(steal);
      long guest_long = stol(guest);
      long guest_nice_long = stol(guest_nice);

      long total_long = user_long + nice_long + system_long + idle_long + nowait_long +
              irq_long + softirq_long + steal_long + guest_long + guest_nice_long;
      return total_long;
  }
  return 0; 
}

// Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
  return 0; 
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

      long user_long = stol(user);
      long nice_long = stol(nice);
      long system_long = stol(system);
      long irq_long = stol(irq);
      long softirq_long = stol(softirq);
      long steal_long = stol(steal);
      long guest_long = stol(guest);
      long guest_nice_long = stol(guest_nice);

      long active_long = user_long + nice_long + system_long + irq_long + softirq_long +
              steal_long + guest_long + guest_nice_long;
      return active_long;
  }
  return 0; 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line, cpu, user, nice, system, idle, iowait;

  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> cpu >> user >> nice >> system >> idle >> iowait;
      return stoi(idle) + stoi(iowait);
  }
  return 0; 
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string label, usertime, nicetime, systemtime, idletime, iowait, irq, softirq, steal, guest, guestnice;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> label >> usertime >> nicetime >> systemtime >> idletime >> iowait >> irq >> softirq >> steal >> guest >> guestnice;
      return std::vector<string> {usertime, nicetime, systemtime, idletime, iowait, irq, softirq, steal, guest, guestnice};
    }
  return {}; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line, key, value;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          if (std::all_of(value.begin(), value.end(), isdigit)) {
            int num_running_procs = stoi(value);
            return num_running_procs;
          }
        }
      }
    }
  }
  return 0; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line, key, value;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          if (std::all_of(value.begin(), value.end(), isdigit)) {
            int num_running_procs = stoi(value);
            return num_running_procs;
          }
        }
      }
    }
  }
  return 0; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line, cmdLine;

  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> cmdLine;
      return cmdLine;
  }
  return string(); 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, lineLabel, value;
  std::ifstream stream(kProcDirectory + to_string(pid) +  kStatusFilename);
  
  if (stream.is_open()) {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> lineLabel >> value;
      if(lineLabel == "VmSize:"){
        auto ramInKb = value;
        auto ramInMb = std::stoi(ramInKb) / 1024.0; 
        string ramInMbStr = to_string(ramInMb);
        int pos = ramInMbStr.find(".");
        return ramInMbStr.substr(0, pos + 2); 
      }
    }    
  }
  return string(); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value, uid;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
          std::istringstream linestream(line);
          linestream >> key >> value;
          if (key == "Uid:"){
            uid = value;
          }  
      }
      return uid;
  }
  return string(); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid, line, key, value, username;
  string var;
  uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
          std::istringstream linestream(line);
          linestream >> key >> var >> value;
          if (value == uid) {
              username = key;
              return username;
          }
      }
  }
  return string(); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  auto hertz = sysconf(_SC_CLK_TCK);
  std::ifstream stream(kProcDirectory + to_string(pid) +  kStatFilename);
  string key, starttime;
  vector<string> reader;
  string line;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 0;
    while(linestream >> key) {
      if(i == 21) {    
        starttime = key;
        return UpTime() - std::stol(starttime)/hertz;
      }
      i++;
    } 
  }
  return 0; 
}

vector<string> LinuxParser::ProcessConsumption(int pid){
    std::ifstream stream(kProcDirectory + to_string(pid) +  kStatFilename);
    string key;
    vector<string> reader;
    string line;
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      int i = 0;
      while(linestream >> key) {
        if(i == 13 || i == 14 || i == 15 || i == 16 || i == 21) {          
          reader.push_back(key);
          if(i == 21) return reader;
        }
        i++;
      } 
    }

    return {""};
}