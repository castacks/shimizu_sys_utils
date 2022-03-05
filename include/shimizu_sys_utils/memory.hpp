
#ifndef __SHIMIZU_SYS_UTILS_MEMORY_HPP___
#define __SHIMIZU_SYS_UTILS_MEMORY_HPP___

#include <iostream>
#include <string>

namespace shimizu
{

/**
 * @brief MemoryMonitor class.
 * 
 * See https://man7.org/linux/man-pages/man5/proc.5.html for more information.
 * 
 */
class MemoryMonitor {

public:
    MemoryMonitor();
    MemoryMonitor(bool human_readable);
    ~MemoryMonitor();

    void set_human_readable(bool b) { human_readable = b; }
    MemoryMonitor& update();

    /**
     * @brief Return the peak RSS in KB.
     * 
     * @return long the peak RSS (VmHWM)
     */
    long peak_rss() const { return _peak_rss; }

    /**
     * @brief Return the RSS is KB.
     * 
     * @return long the RSS (VmRSS)
     */
    long rss() const { return _rss; }

    std::string string() const;

    MemoryMonitor& operator = ( const MemoryMonitor& other );

    friend std::ostream& operator << (std::ostream& os, const MemoryMonitor& mm);

private:
    std::pair<double, std::string> convert_2_human_readable( long original ) const;

private:
    const int MEM_BASE = 1024;

    bool human_readable;

    long _peak_rss; // KB, VmHWM.
    long _rss; // KB, VmRSS.
};

} // namespace shimizu

#endif // __SHIMIZU_SYS_UTILS_MEMORY_HPP___
