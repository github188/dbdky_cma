#include <utils/Logging.h>
#include <utils/Timestamp.h>

using namespace dbdky;

int main(int argc, char** argv)
{
    Timestamp st;
    LOG_INFO << "st: toString = [" << st.toString() << "]";
    LOG_INFO << "st: toFormattedString = [" << st.toFormattedString() << "]";
    LOG_INFO << "st: toFormattedStringDash = [" << st.toFormattedStringDash() << "]";
    LOG_INFO << "st: valid = [" << (st.valid() ? "TRUE" : "FALSE") << "]";
    LOG_INFO << "st: microSecondsSinceEpoch() = [" << st.microSecondsSinceEpoch() << "]";
    
    Timestamp nowst;
    Timestamp tmp = Timestamp::now();
    nowst.swap(tmp);
    
    LOG_INFO << "nowst: toString = [" << nowst.toString() << "]";
    LOG_INFO << "nowst: toFormattedString = [" << nowst.toFormattedString() << "]";
    LOG_INFO << "nowst: toFormattedStringDash = [" << nowst.toFormattedStringDash() << "]";
    LOG_INFO << "nowst: valid = [" << (nowst.valid() ? "TRUE" : "FALSE") << "]";
    LOG_INFO << "nowst: microSecondsSinceEpoch() = [" << nowst.microSecondsSinceEpoch() << "]";
    return 0;
}
