#include "appender.h"
#include <thread>
#include <mutex>



namespace mars {
    namespace xlog {

        extern void log_formater(const XLoggerInfo *_info, const char *_logbody, PtrBuffer &_log);

        extern void ConsoleLog(const XLoggerInfo *_info, const char *_log);

        static const int kMaxDumpLength = 4096;

        static const unsigned int kBufferBlockLength = 150 * 1024;
        static const long kMinLogAliveTime = 24 * 60 * 60;    // 1 days in second

        static std::mutex sg_mutex_dir_attr;

        void (*g_log_write_callback)(const XLoggerInfo *, const char *) = nullptr;

        namespace {
            class ScopeErrno {
            public:
                ScopeErrno() { m_errno = errno; }

                ~ScopeErrno() { errno = m_errno; }

            private:
                ScopeErrno(const ScopeErrno &);

                const ScopeErrno &operator=(const ScopeErrno &);

            private:
                int m_errno;
            };

#define SCOPE_ERRNO() SCOPE_ERRNO_I(__LINE__)
#define SCOPE_ERRNO_I(line) SCOPE_ERRNO_II(line)
#define SCOPE_ERRNO_II(line) ScopeErrno __scope_errno_##line

        }


        XloggerAppender *XloggerAppender::NewInstance(const XLogConfig &_config, uint64_t _max_byte_size) {
            return nullptr;
        }

        void XloggerAppender::DelayRelease(XloggerAppender *_appender) {
            if (_appender->log_close_) {
                return;
            }

        }

        void XloggerAppender::Release(XloggerAppender *&_appender) {
            _appender->Close();
            delete _appender;
            _appender = nullptr;
        }

        XloggerAppender::XloggerAppender(const XLogConfig &_config, uint64_t _max_byte_size){
            Open(_config);

        }

        void XloggerAppender::Write(const XLoggerInfo *_info, const char *_log) {
            if (log_close_) return;

        }

        void XloggerAppender::SetMode(TAppenderMode _mode) {

        }

        void XloggerAppender::Flush() {

        }

        void XloggerAppender::FlushSync() {

        }

        void XloggerAppender::Close() {

        }

        void XloggerAppender::Open(const XLogConfig &_config) {

        }

        std::string XloggerAppender::__MakeLogFileNamePrefix(const timeval &_tv, const char *_prefix) {
            return nullptr;
        }

        void XloggerAppender::__GetFileNamesByPrefix(const std::string &_logdir,
                                                     const std::string &_fileprefix,
                                                     const std::string &_fileext,
                                                     std::vector <std::string> &_filename_vec) {

        }

        void XloggerAppender::__GetFilePathsFromTimeval(const timeval &_tv,
                                                        const std::string &_logdir,
                                                        const char *_prefix,
                                                        const std::string &_fileext,
                                                        std::vector <std::string> &_filepath_vec) {

        }

        static bool __string_compare_greater(const std::string &s1, const std::string &s2) {
            if (s1.length() == s2.length()) {
                return s1 > s2;
            }
            return s1.length() > s2.length();
        }

        long XloggerAppender::__GetNextFileIndex(const std::string &_fileprefix, const std::string &_fileext) {
            return 0;
        }

        void XloggerAppender::__MakeLogFileName(const timeval &_tv,
                                                const std::string &_logdir,
                                                const char *_prefix,
                                                const std::string &_fileext,
                                                char *_filepath,
                                                unsigned int _len) {

        }

        void XloggerAppender::__DelTimeoutFile(const std::string &_log_path) {

        }

        bool XloggerAppender::__AppendFile(const std::string &_src_file, const std::string &_dst_file) {


            return true;
        }

        void XloggerAppender::__MoveOldFiles(const std::string &_src_path, const std::string &_dest_path,
                                             const std::string &_nameprefix) {

        }

        void XloggerAppender::__GetMarkInfo(char *_info, size_t _info_len) {

        }

        void XloggerAppender::__WriteTips2Console(const char *_tips_format, ...) {


        }

        bool XloggerAppender::__WriteFile(const void *_data, size_t _len, FILE *_file) {
            return true;
        }

        bool XloggerAppender::__OpenLogFile(const std::string &_log_dir) {
            return true;
        }


        void XloggerAppender::__CloseLogFile() {

        }

        bool XloggerAppender::__CacheLogs() {


            return true;
        }

        void XloggerAppender::__Log2File(const void *_data, size_t _len, bool _move_file) {

        }

        void XloggerAppender::WriteTips2File(const char *_tips_format, ...) {

        }


        void XloggerAppender::__AsyncLogThread() {

        }


        void XloggerAppender::__WriteSync(const XLoggerInfo *_info, const char *_log) {

        }


        void XloggerAppender::__WriteAsync(const XLoggerInfo *_info, const char *_log) {

        }

#define HEX_STRING  "0123456789abcdef"

        static unsigned int to_string(const void *signature, int len, char *str) {
            char *str_p = str;
            const unsigned char *sig_p;

            for (sig_p = (const unsigned char *) signature; sig_p - (const unsigned char *) signature < len; sig_p++) {
                char high, low;
                high = *sig_p / 16;
                low = *sig_p % 16;

                *str_p++ = HEX_STRING[(unsigned char) high];
                *str_p++ = HEX_STRING[(unsigned char) low];
                *str_p++ = ' ';
            }

            *str_p++ = '\n';

            for (sig_p = (const unsigned char *) signature; sig_p - (const unsigned char *) signature < len; sig_p++) {
                *str_p++ = char(isgraph(*sig_p) ? *sig_p : ' ');
                *str_p++ = ' ';
                *str_p++ = ' ';
            }

            return (unsigned int) (str_p - str);
        }

        const char *XloggerAppender::Dump(const void *_dumpbuffer, size_t _len) {
            return nullptr;
        }

        static int calc_dump_required_length(int srcbytes) {
            //MUST CHANGE THIS IF YOU CHANGE `to_string` function.
            return srcbytes * 6 + 1;
        }

        bool XloggerAppender::GetCurrentLogPath(char *_log_path, unsigned int _len) {
            if (nullptr == _log_path || 0 == _len) return false;

            if (config_.logdir_.empty()) return false;
            strncpy(_log_path, config_.logdir_.c_str(), _len - 1);
            _log_path[_len - 1] = '\0';
            return true;
        }

        bool XloggerAppender::GetCurrentLogCachePath(char *_logPath, unsigned int _len) {
            if (nullptr == _logPath || 0 == _len) return false;

            if (config_.cachedir_.empty()) return false;
            strncpy(_logPath, config_.cachedir_.c_str(), _len - 1);
            _logPath[_len - 1] = '\0';
            return true;
        }

        void XloggerAppender::SetConsoleLog(bool _is_open) {
            consolelog_open_ = _is_open;
        }

        void XloggerAppender::SetMaxFileSize(uint64_t _max_byte_size) {
            max_file_size_ = _max_byte_size;
        }

        void XloggerAppender::SetMaxAliveDuration(long _max_time) {
            if (_max_time >= kMinLogAliveTime) {
                max_alive_time_ = _max_time;
            }
        }

        bool XloggerAppender::GetfilepathFromTimespan(int _timespan, const char *_prefix,
                                                      std::vector <std::string> &_filepath_vec) {
            if (config_.logdir_.empty()) return false;


            return true;
        }

        bool XloggerAppender::MakeLogfileName(int _timespan, const char *_prefix,
                                              std::vector <std::string> &_filepath_vec) {
            if (config_.logdir_.empty()) return false;

            return true;
        }

        static XloggerAppender *sg_default_appender = nullptr;
        static bool sg_release_guard = true;
        static bool sg_default_console_log_open = false;
        static std::mutex sg_mutex;
        static uint64_t sg_max_byte_size = 0;
        static long sg_max_alive_time = 0;

        void xlogger_appender(const XLoggerInfo *_info, const char *_log) {
            if (sg_release_guard) {
                return;
            }
            sg_default_appender->Write(_info, _log);
        }

        static void appender_release_default_appender() {
            if (sg_release_guard) {
                return;
            }
            sg_release_guard = true;
            sg_default_appender->Close();
            //  本函数只会在 exit 的时候调用，所以干脆不释放对象了，防止多线程导致的 crash
            // XloggerAppender::Release(sg_default_appender);
        }

        void appender_open(const XLogConfig &_config) {

        }

        void appender_flush() {
            if (sg_release_guard) {
                return;
            }
            sg_default_appender->Flush();
        }

        void appender_flush_sync() {
            if (sg_release_guard) {
                return;
            }
            sg_default_appender->FlushSync();
        }

        void appender_close() {

        }

        void appender_setmode(TAppenderMode _mode) {
            if (sg_release_guard) {
                return;
            }
            sg_default_appender->SetMode(_mode);
        }

        bool appender_get_current_log_path(char *_log_path, unsigned int _len) {
            if (sg_release_guard) {
                return false;
            }
            return sg_default_appender->GetCurrentLogPath(_log_path, _len);
        }

        bool appender_get_current_log_cache_path(char *_logPath, unsigned int _len) {
            if (sg_release_guard) {
                return false;
            }
            return sg_default_appender->GetCurrentLogCachePath(_logPath, _len);
        }

        void appender_set_console_log(bool _is_open) {
            sg_default_console_log_open = _is_open;
            if (sg_release_guard) {
                return;
            }
            sg_default_appender->SetConsoleLog(_is_open);
        }

        void appender_set_max_file_size(uint64_t _max_byte_size) {
            sg_max_byte_size = _max_byte_size;
            if (sg_release_guard) {
                return;
            }
            sg_default_appender->SetMaxFileSize(_max_byte_size);
        }

        void appender_set_max_alive_duration(long _max_time) {
            sg_max_alive_time = _max_time;
            if (sg_release_guard) {
                return;
            }
            sg_default_appender->SetMaxAliveDuration(_max_time);
        }

        bool appender_getfilepath_from_timespan(int _timespan, const char *_prefix, std::vector <std::string> &_filepath_vec) {
            if (sg_release_guard) {
                return false;
            }
            return sg_default_appender->GetfilepathFromTimespan(_timespan, _prefix, _filepath_vec);
        }

        bool appender_make_logfile_name(int _timespan, const char *_prefix, std::vector <std::string> &_filepath_vec) {
            if (sg_release_guard) {
                return false;
            }
            return sg_default_appender->MakeLogfileName(_timespan, _prefix, _filepath_vec);
        }
    }
}