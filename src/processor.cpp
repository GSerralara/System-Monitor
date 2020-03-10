#include "processor.h"
#include "linux_parser.h"
Processor::Processor(){
    lastIdle_ = 0.0;
    lastTotal_ = 0.0;
}
// Return the aggregate CPU utilization
float Processor::Utilization() {
    long idle = LinuxParser::IdleJiffies();
    long total = LinuxParser::Jiffies();
    int deltaIdle = idle - lastIdle_;
    int deltaTotal = total - lastTotal_;

    float deltaCpuUtilization = float(deltaTotal - deltaIdle)/deltaTotal;

    lastIdle_ = idle;
    lastTotal_ = total;
    return deltaCpuUtilization; 
}