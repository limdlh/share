//条件变量

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

std::mutex g_mutex;
std::condition_variable g_cv;
bool g_flag = false;

void thread1()
{
	std::cout << "thread 1" << std::endl;
	sleep(1);
    std::unique_lock<std::mutex> lock(g_mutex);
	sleep(1);
    g_flag = true;
    g_cv.notify_one();  // 通知线程 2
	std::cout << "thread 1 finished" << std::endl;
}

void thread2()
{
	std::cout << "thread 2, " << g_flag << std::endl;
    std::unique_lock<std::mutex> lock(g_mutex);
    while (!g_flag)
    {
		printf("limd, g_flag: %d\n", g_flag);
		std::cout << "thread 2 wait, " << g_flag << std::endl;
		sleep(1);
		std::cout << "thread 2 wait" << std::endl;
        g_cv.wait(lock);  // 等待通知
    }
    std::cout << "thread 2 finished, " << g_flag << std::endl;
}

int main()
{
    std::thread t1(thread1);
    std::thread t2(thread2);

    t1.join();
    t2.join();

    return 0;
}

