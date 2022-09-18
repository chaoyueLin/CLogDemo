//
// Created by Charles on 2022/9/18.
//

#include "appender.h"
#include "ptrbuffer.h"
#include "xloggerbase.h"
#include "logBuffer.h"
#include <fcntl.h>

#define LOG_EXT "clog"

extern void log_formater(const XLoggerInfo* _info, const char* _logbody, PtrBuffer& _log);
extern void ConsoleLog(const XLoggerInfo* _info, const char* _log);

static TAppenderMode sg_mode = kAppednerAsync;

static std::string sg_logdir;
static std::string sg_cache_logdir;
static std::string sg_logfileprefix;

static Mutex sg_mutex_log_file;
static FILE* sg_logfile = NULL;
static time_t sg_openfiletime = 0;
static std::string sg_current_dir;

static Mutex sg_mutex_buffer_async;
#ifdef _WIN32
static Condition& sg_cond_buffer_async = *(new Condition());  // 改成引用, 避免在全局释放时执行析构导致crash
#else
static Condition sg_cond_buffer_async;
#endif

static LogBuffer* sg_log_buff = NULL;

static volatile bool sg_log_close = true;

static Tss sg_tss_dumpfile(&free);

#ifdef DEBUG
static bool sg_consolelog_open = true;
#else
static bool sg_consolelog_open = false;
#endif

static uint64_t sg_max_file_size = 0; // 0, will not split log file.

static void __async_log_thread();
static Thread sg_thread_async(&__async_log_thread);

static const unsigned int kBufferBlockLength = 150 * 1024;
static const long kMaxLogAliveTime = 10 * 24 * 60 * 60;	// 10 days in second

static std::string sg_log_extra_msg;

static boost::iostreams::mapped_file sg_mmmap_file;

namespace {
    class ScopeErrno {
    public:
        ScopeErrno() {m_errno = errno;}
        ~ScopeErrno() {errno = m_errno;}

    private:
        ScopeErrno(const ScopeErrno&);
        const ScopeErrno& operator=(const ScopeErrno&);

    private:
        int m_errno;
    };

#define SCOPE_ERRNO() SCOPE_ERRNO_I(__LINE__)
#define SCOPE_ERRNO_I(line) SCOPE_ERRNO_II(line)
#define SCOPE_ERRNO_II(line) ScopeErrno __scope_errno_##line

}


void appender_open_with_cache(TAppenderMode _mode, const std::string& _cachedir, const std::string& _logdir, const char* _nameprefix) {
    assert(!_cachedir.empty());
    assert(!_logdir.empty());
    assert(_nameprefix);

    sg_logdir = _logdir;

    if (!_cachedir.empty()) {
        sg_cache_logdir = _cachedir;
        boost::filesystem::create_directories(_cachedir);
        __del_timeout_file(_cachedir);
        // "_nameprefix" must explicitly convert to "std::string", or when the thread is ready to run, "_nameprefix" has been released.
        Thread(boost::bind(&__move_old_files, _cachedir, _logdir, std::string(_nameprefix))).start_after(3 * 60 * 1000);
    }

    appender_open(_mode, _logdir.c_str(), _nameprefix);

}