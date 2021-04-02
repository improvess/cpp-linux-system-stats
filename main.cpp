#include <chrono>
#include <thread>
#include "linux-system-usage.hpp"

using namespace get_system_usage_linux;

int main() {

    CPU_stats t1 = read_cpu_data();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    CPU_stats t2 = read_cpu_data();

    std::cout << "CPU usage is " << (100.0f * get_cpu_usage(t1, t2)) << "%\n";

    std::cout << "Virtual memory usage is " << (100.0f * get_virtual_memory_usage()) << "%\n";

    std::cout << "Physical memory usage is " << (100.0f * get_physical_memory_usage()) << "%\n";

}