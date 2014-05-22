#ifndef __CMA_PTLRENDER_LEGACY_H_
#define __CMA_PTLRENDER_LEGACY_H_

#include "cma_frame_legacy.h"
#include "cma_callbacks.h"


namespace dbdky
{
namespace cma_server
{
class cma_ptlrender_legacy
{
public:
    static cma_ptlrender_legacy* getInstance();
    ~cma_ptlrender_legacy(){}

    CmaLegacyFrameParserFunc renderParser(const cma_frame_legacy&); 
    CmaLegacySqlInsertStringMaker renderSqlInsertStringMaker(const cma_frame_legacy&);
private:
    cma_ptlrender_legacy(){}
    static cma_ptlrender_legacy* instance_;
     
};
}
}

#endif    //__CMA_PTLRENDER_LEGACY_H_
