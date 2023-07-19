//读写锁

#include <iostream>
#include <thread>
#include <shared_mutex>

std::shared_mutex g_mutex;  // 全局读写锁
int g_counter = 0;   // 共享资源

void incrementCounter()
{
    std::unique_lock<std::shared_mutex> lock(g_mutex);  // 上写锁
    ++g_counter;
}

void readCounter()
{
    std::shared_lock<std::shared_mutex> lock(g_mutex);  // 上读锁
    std::cout << "g_counter = " << g_counter << std::endl;
}

int main()
{
    std::thread t1(incrementCounter);
    std::thread t2(readCounter);

    t1.join();
    t2.join();
    std::cout << "over" << std::endl;
    return 0;
}

