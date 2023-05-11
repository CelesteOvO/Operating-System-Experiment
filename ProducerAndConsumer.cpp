#include "ProducerAndConsumer.h"

ProcessScheduler::ProcessScheduler(int buffersize, int num_producers, int num_consumers) :
        bufferSize(buffersize), producerNum(num_producers), consumerNum(num_consumers) {}

PCB* ProcessScheduler::create_PCB(int pid, ProcessType type, std::string product, ProcessState status) {
    PCB* pcb = new PCB();
    pcb->pid = pid;
    pcb->type = type;
    pcb->product = std::move(product);
    pcb->status = status;
    //pcb->next = nullptr;
    return pcb;
}

void ProcessScheduler::run_schedule() {
    while (true){
        // 从就绪队列中取出一个就绪进程
        PCB* pcb = ready_queue.front();
        ready_queue.pop_front();

        // 运行进程
        pcb->status = RUNNING;
        if (pcb->type == PRODUCER) {
            std::cout << "Running Producer process: " << pcb->pid << std::endl;
            //std::string product = "Product " + std::to_string(rand() % 100);
            std::cout << "PCB Produced product: " << pcb->product << std::endl;

            // 尝试将产品放入缓冲区
            if (buffer.size() == bufferSize){
                // 缓冲区已满，将进程阻塞
                pcb->status = BLOCKED;
                producer_wait_queue.push(pcb);
                std::cout << "Buffer full, blocking process: " << pcb->pid << std::endl;
            } else {
                // 缓冲区未满，将产品放入缓冲区
                pcb->status = OVER;
                // 进程运行结束，将其加入over链表
                over_queue.push(pcb);
                buffer.push(pcb->product);
                std::cout << "Product: " << pcb->product << " added to buffer" << std::endl;
                if(!consumer_wait_queue.empty()){
                    // 唤醒等待的消费者
                    PCB* consumer_pcb = consumer_wait_queue.front();
                    consumer_wait_queue.pop();
                    consumer_pcb->status = READY;
                    ready_queue.push_front(consumer_pcb);
                    std::cout << "Consumer: " << consumer_pcb->pid << " waked up" << std::endl;
                }
            }
        } else if (pcb->type == CONSUMER) {
            std::cout << "Running Consumer process " << pcb->pid << std::endl;
            // 尝试从缓冲区中取出产品
            if (buffer.empty()){
                // 缓冲区为空，将进程加入消费者等待队列
                pcb->status = ProcessState::BLOCKED;
                consumer_wait_queue.push(pcb);
                std::cout << "Buffer empty, blocking process " << pcb->pid << std::endl;
            }else{
                // 缓冲区不为空，从缓冲区中取出一个产品
                pcb->status = OVER;
                // 进程运行结束，将其加入over链表
                over_queue.push(pcb);
                std::string product = buffer.front();// 从缓冲区取出产品
                buffer.pop();
                std::cout << "Product " << product << " consumed" << std::endl;
                // 检查是否有等待的生产者进程
                if (!producer_wait_queue.empty()) {
                    // 有等待的生产者进程，将其唤醒并加入就绪队列
                    PCB* producer_pcb = producer_wait_queue.front();
                    producer_wait_queue.pop();
                    producer_pcb->status = ProcessState::READY;
                    ready_queue.push_front(producer_pcb);
                    std::cout << "Producer process " << producer_pcb->pid << " unblocked" << std::endl;
                }
            }
        }
        display_ready();
        display_wait();
        display_over();
        display_buffer();
        std::cout << std::endl;

        // 检查就绪队列是否为空
        if (ready_queue.empty()) {
            std::cout << "All processes finished" << std::endl;
            break;
        }
    }
}

void ProcessScheduler::clean() {
    while (!over_queue.empty()) {
        PCB* pcb = over_queue.front();
        over_queue.pop();
        delete pcb;
    }
}

void ProcessScheduler::display_wait() const {
    // 显示生产者等待队列
    if (producer_wait_queue.empty()) {
        std::cout << "Producer waiting Queue: empty" << std::endl;
    }
    else {
        std::cout << "Producer waiting Queue: ";
        std::queue<PCB*> tempWaitingQueue = producer_wait_queue;
        while (!tempWaitingQueue.empty()) {
            std::cout << tempWaitingQueue.front() -> pid << " ";
            tempWaitingQueue.pop();
        }
        std::cout << std::endl;
    }
    // 显示消费者等待队列
    if (consumer_wait_queue.empty()) {
        std::cout << "Consumer waiting Queue: empty" << std::endl;
    }
    else {
        std::cout << "Consumer waiting Queue: ";
        std::queue<PCB*> tempWaitingQueue = consumer_wait_queue;
        while (!tempWaitingQueue.empty()) {
            std::cout << tempWaitingQueue.front() -> pid << " ";
            tempWaitingQueue.pop();
        }
        std::cout << std::endl;
    }
}

void ProcessScheduler::display_ready() const {
// 显示就绪队列
    if (ready_queue.empty()) {
        std::cout << "Ready Queue: empty" << std::endl;
    }
    else {
        std::cout << "Ready Queue: ";
        std::deque<PCB*> tempReadyQueue = ready_queue;
        while (!tempReadyQueue.empty()) {
            std::cout << tempReadyQueue.front()->pid << " ";
            tempReadyQueue.pop_front();
        }
        std::cout << std::endl;
    }
}

void ProcessScheduler::display_over() const {
    // 显示over队列
    if (over_queue.empty()) {
        std::cout << "Over Queue: empty" << std::endl;
    }
    else {
        std::cout << "Over Queue: ";
        std::queue<PCB*> tempOverQueue = over_queue;
        while (!tempOverQueue.empty()) {
            std::cout << tempOverQueue.front()->pid << " ";
            tempOverQueue.pop();
        }
        std::cout << std::endl;
    }
}

void ProcessScheduler::display_buffer() const {
    // 显示缓冲区
    if (buffer.empty()) {
        std::cout << "Buffer Queue: empty" << std::endl;
    }
    else {
        std::cout << "Buffer Queue: ";
        std::queue<std::string> tempBuffer = buffer;
        while (!tempBuffer.empty()) {
            std::cout << tempBuffer.front() << " ";
            tempBuffer.pop();
        }
        std::cout << std::endl;
    }
    std::cout << "Used buffer size: " << buffer.size() << std::endl;
    std::cout << "Left buffer size: " << bufferSize - buffer.size() << std::endl;
}



