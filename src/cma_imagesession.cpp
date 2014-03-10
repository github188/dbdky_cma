#include "cma_imagesession.h"

#include <boost/bind.hpp>

namespace dbdky
{
namespace cma_server
{
cma_imagesession::cma_imagesession(const string name)
    : cma_session(name)
{
}

cma_imagesession::~cma_imagesession()
{
}

void cma_imagesession::workFunction()
{
}
}
}
