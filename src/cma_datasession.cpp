#include "cma_datasession.h"

#include <boost/bind.hpp>

namespace dbdky
{
namespace cma_server
{
cma_datasession::cma_datasession(const string name)
    : cma_session(boost::bind(&cma_datasession::workFunction, this), name)
{
}

cma_datasession::~cma_datasession()
{
}

void cma_datasession::workFunction()
{
}
}
}
