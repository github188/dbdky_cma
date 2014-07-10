#ifndef __SYSLOG_H_
#define __SYSLOG_H_

namespace dbdky
{
	class SysLogger
	{
	public:
		enum LogLevel
		{
			INFO,
			ERROR,
			FATAL,
			NUM_LOG_LEVELS,
		};
	};

// #define SYSLOG_INFO if (dbdky::SysLogger::logLevel() <= dbdky::SysLogger::INFO) \
// 	dbdky::SysLogger(__FILE__, __LINE__, dbdky::SysLogger::INFO, __func__).stream()

// #define SYSLOG_ERROR ""
// #define SYSLOG_FATAL ""
}

#endif    //__SYSLOG_H_