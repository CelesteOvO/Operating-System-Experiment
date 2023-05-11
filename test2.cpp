#include "ProducerAndConsumer.h"

int main()
{
    srand(time(NULL));
    int buffer_size, count;
    int i = 0;
    int num_producers = 0, num_consumers = 0;
    std::cout << "Please input the buffer size: ";
    std::cin >> buffer_size;
    ProcessScheduler process_scheduler(buffer_size, num_producers, num_consumers);
    std::cout << "Please input the number of PCB: ";
    std::cin >> count;
    std::cout << "Please input the PCB message: " << std::endl;
    while(count--){
        std::cout << "PCB: " << i << std::endl;
        std::cout << "Please input the PCB type (0: Producer,  1: consumer):  " << std::endl;
        int type;
        std::cin >> type;
        std::string product;
        if(type == 0){
            std::cout << "Please input the product: ";
            std::cin >> product;
        }else{
            product = "";
        }
        std::cout << std::endl;
        PCB* pcb = ProcessScheduler::create_PCB(i, ProcessType(type), product, READY);
        process_scheduler.ready_queue.push_back(pcb);
        i++;
    }
    process_scheduler.run_schedule();
    process_scheduler.clean();
    return 0;
}