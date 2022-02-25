
#include <fstream>
#include <iomanip>
#include <sstream>

#include "shimizu_sys_utils/memory.hpp"

using namespace shimizu;

MemoryMonitor::MemoryMonitor()
: human_readable(false), _peak_rss(0)
{}

MemoryMonitor::MemoryMonitor(bool human_readable)
: human_readable(human_readable), _peak_rss(0)
{}

MemoryMonitor::~MemoryMonitor()
{}

static
long parse_line(const std::string& line) {
    std::string name;
    long value = 0;
    std::stringstream ss(line);
    ss >> name >> value;
    return value;
}

MemoryMonitor& MemoryMonitor::update() {
    _peak_rss = 0;
    _rss = 0;
    
    std::ifstream ifs("/proc/self/status");

    if (ifs) {
        std::string line;
        bool have_VmHWM = false;
        bool have_VmRSS = false;
        while ( std::getline(ifs, line) ) {
            if ( !have_VmHWM && line.find("VmHWM") != std::string::npos ) {
                _peak_rss = parse_line(line);
                have_VmHWM = true;
            }

            if ( !have_VmRSS && line.find("VmRSS") != std::string::npos ) {
                _rss = parse_line(line);
                have_VmRSS = true;
            }

            if ( have_VmHWM && have_VmRSS ) break;
        }
    }

    return *this;
}

std::pair<double, std::string> 
MemoryMonitor::convert_2_human_readable( long original ) const {
    auto value = static_cast<double>( original ); // KB;

    if ( value < MEM_BASE ) {
        return { value, "KB" };
    }

    value /= MEM_BASE;
    if ( value < MEM_BASE ) {
        return { value, "MB" };
    }

    value /= MEM_BASE;
    // Only as large as GB.
    return { value, "GB" };
}

std::string MemoryMonitor::string() const {
    std::stringstream ss;
    ss << std::fixed;

    if ( human_readable ) {
        const auto peak_rss = convert_2_human_readable( _peak_rss );
        const auto rss = convert_2_human_readable( _rss );

        ss << std::setprecision(2) << rss.first << "/" << peak_rss.first << " " << peak_rss.second;
    } else {
        ss << std::setprecision(2) << _rss << "/" << _peak_rss << " KB";
    }

    return ss.str();
}

namespace shimizu
{

std::ostream& operator << (std::ostream& os, const MemoryMonitor& mm) {
    os << mm.string();
    return os;
}

} // namespace shimizu