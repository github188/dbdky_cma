#ifndef __CMA_IMAGESESSION_H_
#define __CMA_IMAGESESSION_H_

#include "cma_session.h"

namespace dbdky
{
namespace cma_server
{
class cma_imagesession : cma_session
{
public:
    cma_imagesession(const string name);
    ~cma_imagesession();

private:
    void workFunction();
};
}
}


#endif    //__CMA_IMAGESESSION_H_
