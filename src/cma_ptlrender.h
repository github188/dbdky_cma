#ifndef __CMA_PTLRENDER_H_
#define __CMA_PTLRENDER_H_

#include "cma_frame.h"
#include "cma_callbacks.h"


namespace dbdky
{
namespace cma_server
{
class cma_ptlrender
{
public:
    static cma_ptlrender* getInstance();
    ~cma_ptlrender(){}

    CmaFrameParserFunc renderParser(const cma_frame&); 
private:
    cma_ptlrender(){}
    static cma_ptlrender* instance_;
     
};
}
}

#endif    //__CMA_PTLRENDER_H_
