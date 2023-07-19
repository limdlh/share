#include <iostream>
#include <thread>

thread_local int g_counter = 0;  // 线程本地变量

void incrementCounter()
{
    ++g_counter;
    std::cout << std::this_thread::get_id() << ": " << g_counter << std::endl;
}

int main()
{
    std::thread t1(incrementCounter);
    std::thread t2(incrementCounter);

    t1.join();
    t2.join();

    return 0;
}

