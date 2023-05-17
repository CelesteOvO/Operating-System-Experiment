
#include "MemoryAllocation.h"

void MemoryAllocation::init_blocks(int totalMemorySize, int blockNum) {
    // 初始化内存块
    // 第一块
    auto* block_first = new MemoryBlock;
    block_first->startAddress = 0;
    block_first->length = 2;
    block_first->flag = true;
    free_memory.push_back(block_first);
    // 其余块
    for(int i = 1; i < blockNum; i++){
        auto* block = new MemoryBlock;
        block->startAddress = free_memory[i-1]->startAddress + free_memory[i-1]->length;
        block->length = totalMemorySize/(pow(2,blockNum-i));
        block->flag = true;
        free_memory.push_back(block);
    }
    for(int i = 0; i < blockNum; i++){
        auto* block = new MemoryBlock;
        block->flag = false;
        used_memory.push_back(block);
    }
}

void MemoryAllocation::init_jobs(int totalMemorySize, int jobNum) {
    bool flag = true;
    while(flag){
        int sum = 0;
        //srand(time(nullptr));
        for (int i = 0; i < jobNum; i++) {
            Job* job = new Job;
            job->name = std::to_string(i+1);
            job->length = rand() % (totalMemorySize / 4) + 1; //随机内存需求
            job->flag = NotAllocated;
            job_queue.push_back(job);
            sum += job->length;
        }
        if(sum == totalMemorySize){
            flag = false;
        }
    }
}

static int jobAllocatedNum = 0;    // Number of jobs allocated
void MemoryAllocation::allocate_memory() {
    int count = 0;
    while (jobAllocatedNum < jobNum) {
        std::cout << "Start a round of allocation!" << std::endl;
        for(int job = 0; job < jobNum; job++){
            if(job_queue[job]->flag == NotAllocated){
                std::cout << "Allocating job: " << job_queue[job]->name << std::endl;
                std::cout << "Job size: " << job_queue[job]->length << std::endl;
                first_fit(job_queue[job]);
                if(job_queue[job]->flag == Allocated){
                    std::cout << "Success Allocated!" << std::endl;
                    print_memory_status();
                    print_job_status();
                } else {
                    std::cout << "Failed to allocate!" << std::endl;
                    std::cout << std::endl;
                }
            }
        }
        if(jobAllocatedNum == jobNum){
            std::cout << "All jobs have been allocated!" << std::endl;
            break;
        }
        if(count == 0)
            merge_memory();
        print_memory_status();
        std::cout << "Start a round of recycle!" << std::endl;
        for(int i = 0; i < jobNum; i++){
            if(job_queue[i]->flag == NotAllocated){
                continue;
            }
            srand(time(nullptr));
            int randNum = rand() % 2;
            std::cout << "The rand number is: " << randNum << std::endl;
            if(randNum == 0)
            {
                std::cout << "Reclaiming job: " << job_queue[i]->name << std::endl;
                reclaim_memory(job_queue[i]->name);
                job_queue[i]->flag = Finished;
            }else{
                std::cout << "Job: " << job_queue[i]->name << " is still running!" << std::endl;
            }
        }
        count++;
    }
}

void MemoryAllocation::first_fit(Job* job) {
    int j;
    for (int i = 0; i < free_memory.size(); i++) {  // 遍历空闲区表
        if(free_memory[i]->length > job->length || free_memory[i]->length == job->length){
            std::cout << "The job is allocated, the block size is: " << free_memory[i]->length << std::endl;
            for(j = 0; j < blockNum; j++) // 遍历已分配区表
            {
                if(!used_memory[j]->flag) // 找到第一个空闲的内存块
                {
                    used_memory[j]->startAddress = free_memory[i]->startAddress; // 将空闲区表中的内存块分配给作业
                    used_memory[j]->length = job->length; // 将空闲区表中的内存块分配给作业
                    used_memory[j]->flag = stoi(job->name);
                    job->flag = Allocated;
                    jobAllocatedNum++;
                    break;
                }
            }
            if(free_memory[i]->length == job->length){ // 如果空闲区表中的内存块大小等于作业大小
                free_memory[i]->flag = false;
                free_memory.erase(free_memory.begin() + i);
                /*for (j = i; j < blockNum-1; j++) // 将空闲区表中的内存块删除 // 空闲区大小少1
                    free_memory[j] = free_memory[j + 1];
                free_memory[blockNum-1]->flag = false; // 标记改空闲块已经删除*/
                break;
            } else { // 如果空闲区表中的内存块大小大于作业大小
                free_memory[i]->startAddress += job->length; // 将空闲区表中的内存块的起始地址向后移动
                free_memory[i]->length -= job->length; // 将空闲区表中的内存块的大小减去作业大小
                break;
            }
        }
    }
}

void MemoryAllocation::next_fit(Job* job) {

}

void MemoryAllocation::print_memory_status() {
    std::cout << std::endl << "*************************************************************" << std::endl;
    std::cout << "Free area table (from low to high address)" << std::endl;
    std::cout << "*************************************************************" << std::endl;
    std::cout << "Num" << "\t\tAddress" << "\t\tSize" << "\t\tFlag" << std::endl;
    std::cout << "*************************************************************" << std::endl;
    for (int i = 0; i < free_memory.size(); i++)
    {
        if (free_memory[i]->flag == 1)												//2种状态
            std::cout << i + 1 << "\t\t" << free_memory[i]->startAddress << "\t\t" << free_memory[i]->length << "\t\tFree" << std::endl;
        else
            std::cout << i + 1 << "\t--\t--\tEmpty" << std::endl;
    }
    std::cout << "*************************************************************" << std::endl << std::endl;
    std::cout << std::endl << "*************************************************************" << std::endl;
    std::cout << "Allocated area table (no sorting)" << std::endl;
    std::cout << "*************************************************************" << std::endl;
    std::cout << "Num" << "\t\tAddress" << "\t\tSize" << "\t\tFlag" << std::endl;
    std::cout << "*************************************************************" << std::endl;
    for (int j = 0; j < blockNum; j++)
    {
        if (used_memory[j]->flag == 0)												//2种状态
            std::cout << j + 1 << "\t\t--\t\t--\t\tEmpty" << std::endl;
        else
            std::cout << j + 1 << "\t\t" << used_memory[j]->startAddress << "\t\t" << used_memory[j]->length << "\t\t" << "Allocated" << std::endl;
    }
    std::cout << "*************************************************************" << std::endl << std::endl;
}

void MemoryAllocation::print_job_status() {
    std::cout << std::endl << "*************************************************************" << std::endl;
    std::cout << "Job queue" << std::endl;
    std::cout << "*************************************************************" << std::endl;
    std::cout << "Job Name" << "\tSize" << "\t\tFlag" << std::endl;;
    std::cout << "*************************************************************" << std::endl;
    for (int i = 0; i < jobNum; i++)
    {
        std::cout << job_queue[i]->name << "\t\t" << job_queue[i]->length;
        if (job_queue[i]->flag == 0)														//3种状态
            std::cout << "\t\tUnallocated" << std::endl;
        else if (job_queue[i]->flag == 1)
            std::cout << "\t\tAllocated" << std::endl;
        else
            std::cout << "\t\tFinished" << std::endl;
    }
    std::cout << "*************************************************************" << std::endl << std::endl;
}

void MemoryAllocation::reclaim_memory(const std::string& name) {
    for(auto &i :used_memory) // 遍历已分配区表
    {
        if(std::to_string(i->flag) == name) // 找到作业
        {
            i->flag = false;
            bool Find = false;
            // Find adjacent free blocks in the free_memory table
            for (int j = 0; j < free_memory.size(); j++) {
                if (free_memory[j]->flag) {
                    if(free_memory[j]->startAddress + free_memory[j]->length == i->startAddress) // 有上回收区
                    {
                        if(i->startAddress + i->length == free_memory[j+1]->startAddress){ // 也有下回收区
                            free_memory[j]->length = free_memory[j]->length + i->length + free_memory[j+1]->length;
                            free_memory.erase(free_memory.begin() + j + 1 );
                            /*for (int k = j+1; k <= free_memory.size()-2; k++) // 将空闲区表中的内存块删除 // 空闲区大小少1
                                free_memory[k] = free_memory[k + 1];
                            free_memory[blockNum-1]->flag = false; // 标记改空闲块已经删除*/
                        }else{
                            free_memory[j]->length += i->length;
                        }
                        Find = true;
                        break;
                    }else if(i->startAddress + i->length == free_memory[j]->startAddress){
                        free_memory[j]->startAddress = i->startAddress;
                        free_memory[j]->length += i->length;
                        Find = true;
                        break;
                    }
                }
            }

            if(!Find){
                auto* block = new MemoryBlock;
                block->startAddress = i->startAddress;
                block->length = i->length;
                block->flag = true;
                free_memory.push_back(block);
            }

            for(int j = 0; j < jobNum; j++) // 遍历作业队列
            {
                if(job_queue[j]->name == name) // 找到作业
                {
                    job_queue[j]->flag = Finished;
                    break;
                }
            }

            //merge_memory(job_queue[index]);
            std::cout << "Reallocated job! " << std::endl;
            //first_fit(job_queue[index]); // 将作业重新分配
            break;
        }/*else{
            std::cout << "Job: " << name << " is not found!" << std::endl;
        }*/
    }
    print_memory_status();
    print_job_status();
}

void MemoryAllocation::merge_memory() {
    for (int i = 0; i < free_memory.size(); i++) { // 遍历空闲区
        if (free_memory[i]->flag) { // 如果当前内存块是空闲的
            while(i+1 <free_memory.size() && free_memory[i+1]->flag && free_memory[i]->startAddress + free_memory[i]->length == free_memory[i+1]->startAddress) {
                std::cout << "Find the upper neighbor free area!" << std::endl;
                free_memory[i]->length += free_memory[i+1]->length;
                free_memory.erase(free_memory.begin() + i + 1 );
            }
        }
    }
}




