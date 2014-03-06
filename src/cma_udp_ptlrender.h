#ifndef __CMA_UDP_PTLRENDER_H_
#define __CMA_UDP_PTLRENDER_H_

#include "cma_frame_legacy.h"
#include "cma_callbacks.h"


namespace dbdky
{
namespace cma_server
{
class cma_udp_ptlrender
{
public:
    static cma_udp_ptlrender* getInstance();
    ~cma_udp_ptlrender(){}

    CmaFrameParserFunc renderParser(const cma_frame_legacy&); 
    CmaSqlInsertStringMaker renderSqlInsertStringMaker(const cma_frame_legacy&);
private:
    cma_udp_ptlrender(){}
    static cma_udp_ptlrender* instance_;
     
};
}
}

#endif    //__CMA_UDP_PTLRENDER_H_
