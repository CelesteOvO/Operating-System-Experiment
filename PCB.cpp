#include "PCB.h"
ProcessControlBlock::PCB *
ProcessControlBlock::create_PCB(std::string name, int state, int priority, int need_time) {
    PCB* pcb = new PCB();
    pcb->name = std::move(name);
    pcb->state = state;
    pcb->priority = priority;
    pcb->need_time = need_time;
    pcb->execution_time = 0;
    pcb->next = nullptr;
    return pcb;
}

void ProcessControlBlock::init_PCB(std::vector<PCB *> &pcb_list, int n) {
    srand((unsigned)time(nullptr)); // 用系统时间作为种子，确保每次运行的随机数不同
    for (int i = 0; i < n; i++) {
        int state = rand() % 2;
        int priority = rand() % 10 + 1;
        int need_time = rand() % 50 + 1;
        PCB* pcb = create_PCB("P" + std::to_string(i + 1), state == 0 ? READY : WAITING, priority, need_time);
        pcb_list.push_back(pcb);
        if (state == 0) {
            ready_queue.push(pcb);
        } else {
            wait_queue.push(pcb);
        }
        display_PCB(pcb);
    }
}

ProcessControlBlock::PCB * ProcessControlBlock::FIFO() {

    // 如果就绪队列不为空，则取出队头进程执行
    if (!ready_queue.empty()) {
        PCB* pcb = ready_queue.front();
        ready_queue.pop();
        return pcb;
    } else {
        while(ready_queue.empty() && !wait_queue.empty()){
            std::cout << "Ready queue is empty" << std::endl;
            int num = rand() % 2;
            std::cout << "flag for wait queue is: " << num << std::endl;
            if (num == 0 && !wait_queue.empty()) {
                PCB *wait_pcb = wait_queue.front();
                wait_queue.pop();
                wait_pcb->state = READY;
                ready_queue.push(wait_pcb);
                std::cout << "So, process " << wait_pcb->name << " added to ready queue" << std::endl;
                std::cout << std::endl;
                PCB* pcb = ready_queue.front();
                ready_queue.pop();
                return pcb;
            }
        }
    }
    return nullptr;
}

ProcessControlBlock::PCB * ProcessControlBlock::Priority()
{
    // 按照优先级从高到低的顺序执行就绪队列中的进程
    if (!ready_queue.empty()){
        //从就绪队列中取出队首的进程对应的PCB，将其作为当前选择的具有最高优先级的进程，并将其优先级赋值给min_priority变量。
        PCB* pcb = ready_queue.front();
        int min_priority = pcb->priority;
        std::queue<PCB*> priority_queue; // 创建一个临时队列，用于存放比当前进程优先级更高的进程
        //遍历就绪队列，将优先级比当前选择的进程优先级更高的进程加入优先级队列，将优先级比当前选择的进程优先级更低的进程加入就绪队列。
        ready_queue.pop();
        while (!ready_queue.empty()){
            PCB* next_pcb = ready_queue.front(); // 取出队首进程
            ready_queue.pop(); // 将队首进程出队
            if (next_pcb->priority < min_priority) { // 如果队首进程的优先级比当前选择的进程优先级更高
                priority_queue.push(pcb); // 将当前选择的进程加入优先级队列
                pcb = next_pcb; // 将队首进程作为当前选择的进程
                min_priority = next_pcb->priority; // 更新当前选择的进程的优先级
            } else {
                priority_queue.push(next_pcb); // 将队首进程加入优先级队列
            }
        }
        while (!priority_queue.empty()) { // 将优先级队列中的进程加入就绪队列
            ready_queue.push(priority_queue.front());
            priority_queue.pop();
        }
        return pcb;
    }else {
        while(ready_queue.empty() && !wait_queue.empty()){
            std::cout << "Ready queue is empty" << std::endl;
            int num = rand() % 2;
            std::cout << "flag for wait queue is: " << num << std::endl;
            if (num == 0 && !wait_queue.empty()) {
                PCB *wait_pcb = wait_queue.front();
                wait_queue.pop();
                wait_pcb->state = READY;
                ready_queue.push(wait_pcb);
                std::cout << "So, process " << wait_pcb->name << " added to ready queue" << std::endl;
                std::cout << std::endl;
                PCB* pcb = ready_queue.front();
                ready_queue.pop();
                return pcb;
            }
        }
    }
    return nullptr;
}

ProcessControlBlock::PCB *ProcessControlBlock::SJF() {
    if (!ready_queue.empty()) {
        // 从就绪队列中取出队首的进程对应的PCB，将其作为当前选择的具有最短执行时间的进程，并将其执行时间赋值给min_time变量。
        PCB *pcb = ready_queue.front();
        int min_time = pcb->need_time;
        std::queue <PCB*> time_queue;
        ready_queue.pop();
        // 遍历就绪队列，将执行时间比当前选择的进程执行时间更短的进程加入时间队列，将执行时间比当前选择的进程执行时间更长的进程加入就绪队列。
        while (!ready_queue.empty()) {
            PCB *next_pcb = ready_queue.front(); // 取出队首进程
            ready_queue.pop(); // 将队首进程出队
            if (next_pcb->need_time < min_time) { // 如果队首进程的执行时间比当前选择的进程执行时间更短
                time_queue.push(pcb); // 将当前选择的进程加入时间队列
                pcb = next_pcb; // 将队首进程作为当前选择的进程
                min_time = next_pcb->need_time; // 更新当前选择的进程的执行时间
            } else {
                time_queue.push(next_pcb); // 将队首进程加入时间队列
            }
        }
        while (!time_queue.empty()) { // 将时间队列中的进程加入就绪队列
            ready_queue.push(time_queue.front());
            time_queue.pop();
        }
        return pcb;
    }else {
        while(ready_queue.empty() && !wait_queue.empty()){
            std::cout << "Ready queue is empty" << std::endl;
            int num = rand() % 2;
            std::cout << "flag for wait queue is: " << num << std::endl;
            if (num == 0 && !wait_queue.empty()) {
                PCB *wait_pcb = wait_queue.front();
                wait_queue.pop();
                wait_pcb->state = READY;
                ready_queue.push(wait_pcb);
                std::cout << "So, process " << wait_pcb->name << " added to ready queue" << std::endl;
                std::cout << std::endl;
                PCB* pcb = ready_queue.front();
                ready_queue.pop();
                return pcb;
            }
        }
    }
    return nullptr;
}

void ProcessControlBlock::run(PCB* pcb, int algorithm) {
    pcb->state = RUNNING;
    std::cout << "Current executing process: " << pcb->name << std::endl;
    // 随机生成一个时间片，模拟进程执行
    int time_slice = rand() % (pcb->need_time - pcb->execution_time)+ 1;
    std::cout << "random time slice length: " << time_slice <<std::endl;
    pcb->execution_time += time_slice;
    std::cout << "Progress bar for this process: "<< pcb->execution_time << " / " << pcb->need_time <<std::endl;
    // 如果进程未执行完
    if (pcb->execution_time < pcb->need_time) {
        int num = rand() % 2;
        // 产生随机数0或1，当该随机数为0，将执行进程加入就绪队列队尾
        std::cout << "The process has not completed execution, so the random flag is: " << num << std::endl;
        if(num == 0)
        {
            pcb->state = READY;
            ready_queue.push(pcb);
            std::cout << "Process " << pcb->name << " added back to ready queue" << std::endl;
        }else // 否则，将执行进程加入等待队列队尾
        {
            pcb->state = WAITING;
            wait_queue.push(pcb);
            std::cout << "Process " << pcb->name << " added to wait queue" << std::endl;
        }

        if(algorithm == SJFAlgorithm){
            pcb->need_time = pcb->need_time - pcb->execution_time;
            pcb->execution_time = 0;
            std::cout << "Update the need time: " << pcb->need_time << std::endl;
        }
    } else {
        pcb->state = FINISHED;
        std::cout << "Process " << pcb->name << " finished" << std::endl;
    }
    // 在进程执行过程中，产生随机数0或1，该随机数为1时，将等待队列中的第一个PCB加入就绪队列的队尾；
    int num = rand() % 2;
    std::cout << "flag for wait queue is: " << num << std::endl;
    if (num == 0 && !wait_queue.empty()) {
        PCB* wait_pcb = wait_queue.front();
        wait_queue.pop();
        wait_pcb->state = READY;
        ready_queue.push(wait_pcb);
        std::cout << "So, process " << wait_pcb->name << " added to ready queue" << std::endl;
    }
}

void ProcessControlBlock::display_status() const {
// 显示就绪队列
    if (ready_queue.empty()) {
        std::cout << "Ready Queue: empty" << std::endl;
    }
    else {
        std::cout << "Ready Queue: ";
        std::queue<PCB*> tempReadyQueue = ready_queue;
        while (!tempReadyQueue.empty()) {
            std::cout << tempReadyQueue.front()->name << " ";
            tempReadyQueue.pop();
        }
        std::cout << std::endl;
    }

    // 显示等待队列
    if (wait_queue.empty()) {
        std::cout << "Waiting Queue: empty" << std::endl;
    }
    else {
        std::cout << "Waiting Queue: ";
        std::queue<PCB*> tempWaitingQueue = wait_queue;
        while (!tempWaitingQueue.empty()) {
            std::cout << tempWaitingQueue.front()->name << " ";
            tempWaitingQueue.pop();
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void ProcessControlBlock::display_PCB(PCB* pcb) {
    std::cout << "name: " <<pcb->name << "\tstate: " << pcb->state << "\tpriority: " << pcb->priority << "\tneed_time: " << pcb->need_time << std::endl;
}




