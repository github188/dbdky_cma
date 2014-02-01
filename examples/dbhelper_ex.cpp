#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include <utils/Logging.h>

#include <dbhelper/dbhelper.h>

using namespace dbdky;
using namespace dbdky::cma_server;

int main(int argc, char** argv)
{
    boost::scoped_ptr<DBHelper> dbhelper_(new DBHelper(
	"tcp://127.0.0.1:3306",
	"root",
	"kevinLike$",
	"vmsdb"));

    if (!dbhelper_->isConnected())
    {
        if (!dbhelper_->connect())
        {
            LOG_ERROR << "Connect to database fail.";
            return -1;
        }
    }

    boost::shared_ptr<ResultSet> result(dbhelper_->query("select * from sd_cd"));
    if (!result.get())
    {
        LOG_INFO << "No result";
        return 0; 
    }

    try
    {
        while (result->next())
        {
            LOG_INFO << result->getString("DeviceCode");
        }
    }
    catch (std::exception& e)
    {
        LOG_ERROR << e.what();
    }

    return 0;
}
