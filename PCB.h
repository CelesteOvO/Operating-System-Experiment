#ifndef OS_PCB_H
#define OS_PCB_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <vector>
#include <string>
#include <utility>

// 进程状态枚举类型
enum ProcessState {
    READY,      // 就绪态
    WAITING,    // 等待态
    RUNNING,    // 运行态
    FINISHED    // 结束态
};

// 调度算法
enum SchedulingAlgorithm {
    FIFOAlgorithm,       // 先进先出调度算法
    PriorityAlgorithm,   // 基于优先数的调度算法
    SJFAlgorithm         // 最短执行时间调度算法
};

class ProcessControlBlock {
 public:
     struct PCB {
         std::string name;                    // 进程名
         int state;             // 进程状态
         int priority;                   // 进程优先级
         int need_time;    // 进程需要执行的总时间
         int execution_time;             // 进程已经执行的时间
         PCB* next; // 指向队列中的下一个进程
     };
     // 就绪队列
     std::queue<PCB*> ready_queue;
    // 等待队列
     std::queue<PCB*> wait_queue;

 public:
     static PCB* create_PCB(std::string name, int state, int priority, int need_time);
     void init_PCB(std::vector<PCB*>& pcb_list, int n);
     void display_status() const;
     void display_PCB(PCB* pcb);
     PCB * FIFO();
     PCB * Priority();
     PCB*  SJF();
     void run(PCB* pcb, int algorithm);
};

#endif //OS_PCB_H
