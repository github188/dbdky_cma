#include "cma_ptlrender.h"
#include <utils/Logging.h>

namespace dbdky
{
namespace cma_server
{
    cma_ptlrender* cma_ptlrender::instance_ = NULL;
   
    cma_ptlrender* cma_ptlrender::getInstance()
    {
        if (NULL == instance_)
        {
            instance_ = new cma_ptlrender();
        }

        return instance_;
    }
 
    CmaFrameParserFunc cma_ptlrender::renderParser(const cma_frame& frm)
    {
        return defaultParseFunc; 
    } 

    map<string,string> defaultParseFunc(const cma_frame& frm)
    {
        LOG_INFO << "DEFAULTPARSEFUNC";
        map<string,string> ret;
        return ret;
    }
    
}
}
