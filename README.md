# cpp-linux-system-stats

Single-header, straight-forward API to read CPU, Memory, thermal zone temperature &amp; Disk usage for linux systems. The data are extracted respectively from `/proc/stat`, `/proc/meminfo` and `statvfs`.

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
    
    //see https://unix.stackexchange.com/questions/304845/discrepancy-between-number-of-cores-and-thermal-zones-in-sys-class-thermal/342023 to realize the correct index for your CPU
    const int cpu_thermal_zone = 2;
    int cpu_temp = get_thermalzone_temperature(cpu_thermal_zone);
    
    std::cout << "CPU temperature is " << (cpu_temp / 1000) << "°C\n";

    // Memory usage
    auto memory_data = read_memory_data();

    std::cout << "Swap usage is " << (100.0f * memory_data.get_swap_usage()) << "%\n";

    std::cout << "Memory usage is " << (100.0f * memory_data.get_memory_usage()) << "%\n";

    // Disk usage
    std::cout << "Disk usage is " << (100.0f * get_disk_usage("/")) << "%\n";

}
```
Expected output:

```
$ c++ -Iinclude main.cpp
$ ./a.out 
CPU usage is 35.9199%
Swap usage is 31.189%
Memory usage is 79.6201%
Disk usage is 67.2953%
$
```
# Why do not use `sysinfo` to get memory consume?

One way to obtain memory usage is via `sysinfo` as follows:

```c++
struct sysinfo memory_info;
sysinfo (&memory_info);
long long mem_used = memory_info.totalram - memory_info.freeram;
```
The problem with this approach is that `freeram` represents the real amount of non-allocated memory. It turns out that linux systems cache lots of memory ram which is actually available to be used for any process. However, this cached memory is considered by `sysinfo` as non-free memory. Since there is no way to measure the cached memory using only `sysinfo` I ended up by extracting memory usage from `/proc/meminfo`. For more information about `sysinfo` see: https://man7.org/linux/man-pages/man2/sysinfo.2.html
