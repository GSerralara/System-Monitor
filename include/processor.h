#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
  public:
  Processor();
  float Utilization();  
 private:
   long lastIdle_;
   long lastTotal_;
};

#endif