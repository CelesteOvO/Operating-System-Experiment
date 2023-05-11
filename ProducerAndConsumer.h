#ifndef OS_PRODUCERANDCONSUMER_H
#define OS_PRODUCERANDCONSUMER_H
#include <iostream>
#include <queue>
#include <utility>
#include <string>

// 进程状态枚举类型
enum ProcessState {
    READY,      // 就绪态
    BLOCKED,    // 阻塞态
    RUNNING,    // 运行态
    OVER   // 结束态
};

enum ProcessType{
    PRODUCER,
    CONSUMER
};

struct PCB {
    int pid;  // 进程系统号
    ProcessType type;  // 进程类型标号
    std::string product;  // 进程产品（字符）
    ProcessState status;  // 进程状态
    /*PCB* next;  // 链表指针*/
};

class ProcessScheduler{
public:
    ProcessScheduler(int bufferSize, int producerNum, int consumerNum);
public:
    const int producerNum; // 生产者数量
    const int consumerNum; // 消费者数量
    const int bufferSize; // 缓冲区大小
public:
    std::queue<std::string> buffer; // 缓冲区
    std::deque<PCB*> ready_queue; // 就绪队列
    std::queue<PCB*> producer_wait_queue; // 生产者等待队列
    std::queue<PCB*> consumer_wait_queue; // 消费者等待队列
    std::queue<PCB*> over_queue; // 结束队列
public:
    static PCB* create_PCB(int pid, ProcessType type, std::string product, ProcessState status);
    void run_schedule();
    void display_ready() const;
    void display_over() const;
    void display_wait() const;
    void display_buffer() const;
    void clean();
};

#endif //OS_PRODUCERANDCONSUMER_H
