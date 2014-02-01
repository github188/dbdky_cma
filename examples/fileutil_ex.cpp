#include <utils/Logging.h>
#include <utils/FileUtil.h>

#include <utils/Timestamp.h>

#include <string>

using namespace dbdky;
using namespace dbdky::FileUtil;
using std::string;

int main(int argc, char** argv)
{
    SmallFile f("abc");
  
    int size;
    int ret;
   
    int64_t fileSize;
    int64_t modifyTime;
    int64_t createTime;

    string content; 

    LOG_INFO << "ReadToBuffer done. Size = " << size;

    ret = readFile<string>("abc", 65535, &content,
	&fileSize, &modifyTime, &createTime);

    if (ret == -1)
    {
        LOG_INFO << "readToString fail";
        return -2;
    }
   
    LOG_INFO << "ReadToString: fileSize = " << fileSize
        << " modifyTime = " << (Timestamp(modifyTime)).toFormattedString()
        << " createTime = " << (Timestamp(createTime)).toFormattedString()
        << " content = " << content;

    
    return 0;
}
