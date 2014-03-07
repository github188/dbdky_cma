#ifndef __CMA_DATASESSION_H_
#define __CMA_DATASESSION_H_

#include "cma_session.h"

namespace dbdky
{
namespace cma_server
{
class cma_datasession : cma_session
{
public:
    cma_datasession(const string name);
    ~cma_datasession();



private:
    void workFunction();
};
}
}

#endif    //__CMA_DATASESSION_H_
