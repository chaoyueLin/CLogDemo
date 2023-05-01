#include "appender.h"

#include "xloggerbase.h"
#include "xlogger_category.h"


namespace mars {

    namespace xlog {

        mars::comm::XloggerCategory *NewXloggerInstance(const XLogConfig &_config, TLogLevel _level);


        mars::comm::XloggerCategory *GetXloggerInstance(const char *_nameprefix);

        void ReleaseXloggerInstance(const char *_nameprefix);

        void XloggerWrite(uintptr_t _instance_ptr, const XLoggerInfo *_info, const char *_log);

        bool IsEnabledFor(uintptr_t _instance_ptr, TLogLevel _level);

        TLogLevel GetLevel(uintptr_t _instance_ptr);

        void SetLevel(uintptr_t _instance_ptr, TLogLevel _level);

        void SetAppenderMode(uintptr_t _instance_ptr, TAppenderMode _mode);

        void Flush(uintptr_t _instance_ptr, bool _is_sync);

        void FlushAll(bool _is_sync);

        void SetConsoleLogOpen(uintptr_t _instance_ptr, bool _is_open);

        void SetMaxFileSize(uintptr_t _instance_ptr, long _max_file_size);

        void SetMaxAliveTime(uintptr_t _instance_ptr, long _alive_seconds);
    }
}