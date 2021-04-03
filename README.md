# cpp-linux-system-stats

Single-header, straight-forward API to read CPU, Memory &amp; Disk usage for linux systems. The data are extracted respectively from `/proc/stat`, `/proc/meminfo` and `statvfs`.

# Example

```c++
#include <chrono>
#include <thread>
#include "linux-system-usage.hpp"

using namespace get_system_usage_linux;

int main() {
    
    // CPU usage
    CPU_stats t1 = read_cpu_data();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    CPU_stats t2 = read_cpu_data();

    std::cout << "CPU usage is " << (100.0f * get_cpu_usage(t1, t2)) << "%\n";

    // Memory usage
    auto memory_data = read_memory_data();

    std::cout << "Swap usage is " << (100.0f * memory_data.get_swap_usage()) << "%\n";

    std::cout << "Memory usage is " << (100.0f * memory_data.get_memory_usage()) << "%\n";

    // Disk usage
    std::cout << "Disk usage is " << (100.0f * get_disk_usage("/")) << "%\n";

}
```
# Why do not use `sysinfo` to get memory consume?

One way to obtain memory usage is via as `sysinfo` follows:

```c++
struct sysinfo memory_info;
sysinfo (&memory_info);
long long mem_used = memory_info.totalram - memory_info.freeram;
```
The problem is that `freeram` is the amount of real non allocated memory. It turns out that linux caches lots of memory ram which is actually available to be used for any process. However, this cached memory is considered by `sysinfo` as non-free memory and there is no way to measure it  by using only `sysinfo`. For more information about `sysinfo` see: https://man7.org/linux/man-pages/man2/sysinfo.2.html
