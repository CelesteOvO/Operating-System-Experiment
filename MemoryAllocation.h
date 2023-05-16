#ifndef OS_MEMORYALLOCATION_H
#define OS_MEMORYALLOCATION_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

enum FileStatus {
    NotAllocated,
    Allocated,
    Finished
};

class MemoryAllocation {
public:
    struct MemoryBlock{
        float startAddress; //分区起始地址
        float length;  //分区长度，单位为字节
        int flag;        // 是否空闲
    };

    struct Job{
        std::string name; //作业id
        float length; //作业长度，单位为字节
        FileStatus flag;
    };

    std::vector<MemoryBlock*> used_memory; // 已分配区表
    std::vector<MemoryBlock*> free_memory; //空闲区表
    std::vector<Job*> job_queue; //作业队列
    int totalMemorySize;        // Total size of memory
    int blockNum;               // Number of blocks
    int jobNum ;                 // Number of jobs
public:
    void init_blocks(int totalMemorySize, int blockNum);
    void init_jobs(int totalMemorySize, int jobNum);
    void print_memory_status();
    void print_job_status();
    void allocate_memory();
    void merge_memory();
    void reclaim_memory(const std::string& name);
    void first_fit(Job* job);
    void next_fit(Job* job);

};
#endif //OS_MEMORYALLOCATION_H
