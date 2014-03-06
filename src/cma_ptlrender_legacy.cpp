#include "cma_ptlrender_legacy.h"
#include <utils/Logging.h>


#include "cma_ptl_legacy_dxwd.h"							
#include "cma_ptl_legacy_qx.h"				
			

namespace dbdky
{
namespace cma_server
{
    cma_ptlrender_legacy* cma_ptlrender_legacy::instance_ = NULL;
   
    cma_ptlrender_legacy* cma_ptlrender_legacy::getInstance()
    {
        if (NULL == instance_)
        {
            instance_ = new cma_ptlrender_legacy();
        }

        return instance_;
    }

    CmaUdpSqlInsertStringMaker cma_ptlrender_legacy::renderSqlInsertStringMaker(const cma_frame_legacy& frm)
    {
        switch (frm.getPtype())
        {
            	case cma_frame_legacy::CMA_UDP_PTYPE_QX:		
		{
			return dbdky::cma_legacy_qx_parser::sqlInsertStringMaker;
			
		}
		case cma_frame_legacy::CMA_UDP_PTYPE_DXWD:		
		{
			return dbdky::cma_legacy_dxwd_parser::sqlInsertStringMaker;
		}
		case cma_frame_legacy::CMA_UDP_PTYPE_GTZD:
            	case cma_frame_legacy::CMA_UDP_PTYPE_GTQX:		
            	case cma_frame_legacy::CMA_UDP_PTYPE_WFZD:
            	case cma_frame_legacy::CMA_UDP_PTYPE_DXHC:
            	case cma_frame_legacy::CMA_UDP_PTYPE_FB_WEIGHT_MEATHD:		
           	case cma_frame_legacy::CMA_UDP_PTYPE_FB_ANGLE_MEATHD1:		
           	case cma_frame_legacy::CMA_UDP_PTYPE_FB_ANGLE_MEATHD2:		
            	case cma_frame_legacy::CMA_UDP_PTYPE_DXFP:
            	case cma_frame_legacy::CMA_UDP_PTYPE_WD:		
            	case cma_frame_legacy::CMA_UDP_PTYPE_JJWD:
		case cma_frame_legacy::CMA_UDP_PTYPE_INSULATOR_FP:
		case cma_frame_legacy::CMA_UDP_PTYPE_INSULATOR_WH:
		case cma_frame_legacy::CMA_UDP_PTYPE_INSULATOR_XLDL:
		case cma_frame_legacy::CMA_UDP_PTYPE_REQ_UP_IMAGE:
		case cma_frame_legacy::CMA_UDP_PTYPE_REMOTE_IMAGE:
		case cma_frame_legacy::CMA_UDP_PTYPE_REMOTE_IMAGE_UP_COMPLETE:
		case cma_frame_legacy::CMA_UDP_PTYPE_REMOTE_IMAGE_COMPLEMENT:
            	default:
            	{
                	return UdpdefaultSqlInsertStringMaker;
            	} 
        }

        return UdpdefaultSqlInsertStringMaker;
    }
 

    CmaUdpFrameParserFunc cma_ptlrender_legacy::renderParser(const cma_frame_legacy& frm)
    {
        switch (frm.getPtype())
        {
	    	case cma_frame_legacy::CMA_UDP_PTYPE_QX:
		{
			return dbdky::cma_legacy_qx_parser::frameParserFunc;
		}
		case cma_frame_legacy::CMA_UDP_PTYPE_DXWD:		
		{
			return dbdky::cma_legacy_dxwd_parser::frameParserFunc;
		}
	    	case cma_frame_legacy::CMA_UDP_PTYPE_GTZD:	
	    	case cma_frame_legacy::CMA_UDP_PTYPE_GTQX:
	    	case cma_frame_legacy::CMA_UDP_PTYPE_WFZD:
	    	case cma_frame_legacy::CMA_UDP_PTYPE_DXHC:		
        	case cma_frame_legacy::CMA_UDP_PTYPE_FB_WEIGHT_MEATHD:		
        	case cma_frame_legacy::CMA_UDP_PTYPE_FB_ANGLE_MEATHD1:		
        	case cma_frame_legacy::CMA_UDP_PTYPE_FB_ANGLE_MEATHD2:
        	case cma_frame_legacy::CMA_UDP_PTYPE_DXFP:		
        	case cma_frame_legacy::CMA_UDP_PTYPE_WD:
		case cma_frame_legacy::CMA_UDP_PTYPE_JJWD:
		case cma_frame_legacy::CMA_UDP_PTYPE_INSULATOR_FP:
		case cma_frame_legacy::CMA_UDP_PTYPE_INSULATOR_WH:
		case cma_frame_legacy::CMA_UDP_PTYPE_INSULATOR_XLDL:
		case cma_frame_legacy::CMA_UDP_PTYPE_REQ_UP_IMAGE:
		case cma_frame_legacy::CMA_UDP_PTYPE_REMOTE_IMAGE:
		case cma_frame_legacy::CMA_UDP_PTYPE_REMOTE_IMAGE_UP_COMPLETE:
		case cma_frame_legacy::CMA_UDP_PTYPE_REMOTE_IMAGE_COMPLEMENT:
        	default:
        	{
           	 	return UdpdefaultParseFunc;
        	}
        }
        return UdpdefaultParseFunc; 
    } 

    map<string,string> UdpdefaultParseFunc(cma_frame_legacy& frm)
    {
        LOG_INFO << "UDP DEFAULTPARSEFUNC";
        map<string,string> ret;
        return ret;
    }

    string UdpdefaultSqlInsertStringMaker(const cma_frame_legacy& frm)
    {
        LOG_INFO << "UDP DEFAULTSQLINSERTSTRINGMAKER";
        return "";
    }
    
}
}
