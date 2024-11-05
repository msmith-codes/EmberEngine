#include <Ember/utils/Helper.hpp>

// -- STD -- //
#include <ctime>

namespace Ember
{
    std::string Helper::getLocalTime()
    {
        time_t now = time(0);
        tm* localTime = localtime(&now);
        std::string time = asctime(localTime);
        time.pop_back();
        return time;
    }
}