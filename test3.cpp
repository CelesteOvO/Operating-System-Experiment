#include "MemoryAllocation.h"

int main()
{
    srand(time(nullptr));
    MemoryAllocation memoryAllocation;
    int totalMemorySize, jobNum, blockNum;
    std::cout << "Please input the total memory size: ";
    std::cin >> totalMemorySize;
    std::cout << "Please input the number of blocks: ";
    std::cin >> blockNum;
    std::cout << "Please input the number of jobs: ";
    std::cin >> jobNum;
    memoryAllocation.totalMemorySize = totalMemorySize;
    memoryAllocation.blockNum = blockNum;
    memoryAllocation.jobNum = jobNum;
    memoryAllocation.init_blocks(totalMemorySize, blockNum);
    memoryAllocation.init_jobs(totalMemorySize, jobNum);
    memoryAllocation.print_memory_status();
    memoryAllocation.print_job_status();
    memoryAllocation.allocate_memory();
    return 0;
}