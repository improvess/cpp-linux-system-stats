/* ---------------------------------------------------------------------
 * Luiz Carlos doleron @ 2021
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero Public License for more details.
 * ----------------------------------------------------------------------
 */

#ifndef _GET_CPU_USAGE_LINUX_
#define _GET_CPU_USAGE_LINUX_

#include <iostream>
#include <fstream>
#include <sstream>

#include "sys/types.h"
#include "sys/sysinfo.h"

namespace get_system_usage_linux
{

    struct CPU_stats
    {
        // see http://www.linuxhowtos.org/manpages/5/proc.htm
        int user;
        int nice;
        int system;
        int idle;
        int iowait;
        int irq;
        int softirq;
        int steal;
        int guest;
        int guest_nice;

        int get_total_idle()
        const {
            return idle + iowait;
        }

        int get_total_active()
        const {
            return user + nice + system + irq + softirq + steal + guest + guest_nice;
        }

    };

    struct Memory_stats
    {
        int total_memory;
        int available_memory;
        int total_swap;
        int free_swap;

        float get_memory_usage() {
            const float result	= static_cast<float>(total_memory - available_memory) / total_memory;
            return result;
        }

        float get_swap_usage() {
            const float result	= static_cast<float>(total_swap - free_swap) / total_swap;
            return result;
        }

    };

    inline CPU_stats read_cpu_data()
    {
        CPU_stats result;
        std::ifstream proc_stat("/proc/stat");

        if (proc_stat.good())
        {
            std::string line;
            getline(proc_stat, line);

            unsigned int *stats_p = (unsigned int *)&result;
            std::stringstream iss(line);
            std::string cpu;
            iss >> cpu;
            while (iss >> *stats_p)
            {
                stats_p++;
            };
        }

        proc_stat.close();

        return result;
    }

    inline int get_val(const std::string &target, const std::string & content) {
        int result = -1;
        std::size_t start = content.find(target);
        if (start != std::string::npos) {
            int begin = start + target.length();
            std::size_t end = content.find("kB", start);
            std::string substr = content.substr(begin, end - begin);
            std::cout << target << " is " << substr << "\n";
            result = std::stoi(substr);
        }
        return result;
    }

    inline Memory_stats read_memory_data()
    {
        Memory_stats result;
        std::ifstream proc_meminfo("/proc/meminfo");

        if (proc_meminfo.good())
        {
            std::string content((std::istreambuf_iterator<char>(proc_meminfo)),
                    std::istreambuf_iterator<char>());

                    std::cout << content << "\n";
            
            result.total_memory = get_val("MemTotal:", content);
            result.total_swap = get_val("SwapTotal:", content);
            result.free_swap = get_val("SwapFree:", content);
            result.available_memory = get_val("MemAvailable:", content);

        }

        proc_meminfo.close();

        return result;
    }

    inline float get_cpu_usage(const CPU_stats & first, const CPU_stats & second) {
        const float active_time	= static_cast<float>(second.get_total_active() - first.get_total_active());
		const float idle_time	= static_cast<float>(second.get_total_idle() - first.get_total_idle());
		const float total_time	= active_time + idle_time;
        return active_time / total_time;
    }

}

#endif