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

    inline float get_cpu_usage(const CPU_stats & first, const CPU_stats & second) {
        const float active_time	= static_cast<float>(second.get_total_active() - first.get_total_active());
		const float idle_time	= static_cast<float>(second.get_total_idle() - first.get_total_idle());
		const float total_time	= active_time + idle_time;
        return active_time / total_time;
    }

    inline float get_virtual_memory_usage() {
        struct sysinfo memory_info;

        sysinfo (&memory_info);
        long long total_mem = memory_info.totalram;
        total_mem += memory_info.totalswap;

        long long mem_used = memory_info.totalram - memory_info.freeram;
        mem_used += memory_info.totalswap - memory_info.freeswap;

        mem_used *= 1000;

        float result = static_cast<float>(mem_used / total_mem) / 1000.0f;

        return result;
    }

    inline float get_physical_memory_usage() {
        struct sysinfo memory_info;

        sysinfo (&memory_info);
        long long total_mem = memory_info.totalram;

        long long mem_used = memory_info.totalram - memory_info.freeram;

        mem_used *= 1000;

        float result = static_cast<float>(mem_used / total_mem) / 1000.0f;

        return result;
    }

}

#endif