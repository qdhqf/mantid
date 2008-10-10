#include <iomanip>
#include <iostream>
#include <vector>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "MantidAPI/MemoryManager.h"
#include "MantidKernel/Exception.h"

using namespace Mantid::Kernel;

namespace Mantid
{
  namespace API
  {

    /// Private Constructor for singleton class
    MemoryManagerImpl::MemoryManagerImpl(): g_log(Kernel::Logger::get("MemoryManager"))
    {
      std::cerr << "Memory Manager created." << std::endl;
      g_log.debug() << "Memory Manager created." << std::endl;
    }

    /** Private destructor
    *  Prevents client from calling 'delete' on the pointer handed 
    *  out by Instance
    */
    MemoryManagerImpl::~MemoryManagerImpl(){}

    MemoryInfo MemoryManagerImpl::getMemoryInfo()
    {
        MemoryInfo mi;
#ifdef _WIN32
        MEMORYSTATUSEX memStatus;
        memStatus.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx( &memStatus );
        if (memStatus.ullTotalPhys < memStatus.ullTotalVirtual)
        {
            mi.availMemory = memStatus.ullAvailPhys/1024;
            mi.totalMemory = memStatus.ullTotalPhys/1024;
        }
        else// All virtual memory will be physical, but a process cannot have more than TotalVirtual.
        {
            mi.availMemory = memStatus.ullAvailVirtual/1024;
            mi.totalMemory = memStatus.ullTotalVirtual/1024;
        }
        mi.freeRatio = int(100*double(mi.availMemory)/mi.totalMemory);
#else
        long int totPages = sysconf(_SC_PHYS_PAGES);
        long int avPages = sysconf(_SC_AVPHYS_PAGES);
        long int pageSize = sysconf(_SC_PAGESIZE);
        mi.availMemory = avPages/1024*pageSize;
        mi.totalMemory = totPages/1024*pageSize;
        mi.freeRatio = int(100*double(mi.availMemory)/mi.totalMemory);
#endif
        return mi;
    }


  } // namespace API
} // namespace Mantid
