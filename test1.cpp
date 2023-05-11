#include "PCB.h"

int main() {
    ProcessControlBlock  process_control_block;
    std::vector<ProcessControlBlock::PCB*> pcb_list;
    process_control_block.init_PCB(pcb_list, 10);
    process_control_block.display_status();
    int algorithm;
    std::cout << "Please choose the scheduling algorithm: " << std::endl;
    std::cout << "1. FIFO" << std::endl;
    std::cout << "2. Priority" << std::endl;
    std::cout << "3. SJF" << std::endl;
    std::cin >> algorithm;
    while(!process_control_block.wait_queue.empty()||!process_control_block.ready_queue.empty()){
        ProcessControlBlock::PCB* current_pcb;
        switch(algorithm - 1){
            case FIFOAlgorithm:
                current_pcb = process_control_block.FIFO();
                process_control_block.run(current_pcb,FIFOAlgorithm);
                break;
            case PriorityAlgorithm:
                current_pcb = process_control_block.Priority();
                process_control_block.run(current_pcb,PriorityAlgorithm);
                break;
            case SJFAlgorithm:
                current_pcb = process_control_block.SJF();
                process_control_block.run(current_pcb,SJFAlgorithm);
                break;
        }
        process_control_block.display_status();
    }
    std::cout << "Complete all process!" << std::endl;
    return 0;
}
