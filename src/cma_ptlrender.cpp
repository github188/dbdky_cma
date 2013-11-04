#include "cma_ptlrender.h"
#include <utils/Logging.h>


#include "cma_ptl_fbjc.h"

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
        switch (frm.getPtype())
        {
            case cma_frame::CMA_PTYPE_QX:
            case cma_frame::CMA_PTYPE_GT:
            case cma_frame::CMA_PTYPE_ZDTZ:
            case cma_frame::CMA_PTYPE_ZDBX:
            case cma_frame::CMA_PTYPE_DXHC:
            case cma_frame::CMA_PTYPE_WD:
            case cma_frame::CMA_PTYPE_FP:
            case cma_frame::CMA_PTYPE_WDTZ:
            case cma_frame::CMA_PTYPE_WDGJ:
            case cma_frame::CMA_PTYPE_WH:
            {
                return defaultParseFunc;
            }
            case cma_frame::CMA_PTYPE_FB:
            {
                return dbdky::cma_fbjc_parser::frameParserFunc;
            }
            default:
            {
                return defaultParseFunc;
            } 
        }

        return defaultParseFunc; 
    } 

    map<string,string> defaultParseFunc(cma_frame& frm)
    {
        LOG_INFO << "DEFAULTPARSEFUNC";
        map<string,string> ret;
        return ret;
    }
    
}
}
