#include "functions.h"

unsigned long long string_to_timestamp(const std::string& ts){

    const std::locale format = std::locale(std::locale::classic(),new time_input_facet("%Y-%m-%d %H:%M:%s"));
    ptime pt;
    std::istringstream is(ts);
    is.imbue(format);
    is >> pt;
    ptime start(boost::gregorian::date(1970,1,1));
    time_duration dur = pt - start;
    time_t epoch = dur.total_milliseconds();
    return epoch;
}
