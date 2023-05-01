

#include "xlogger_interface.h"

#include <functional>
#include <map>
#include <mutex>
#include "xlogger_category.h"

using namespace mars::comm;
namespace mars {
    namespace xlog {

        static std::mutex& GetGlobalMutex() {
            static std::mutex sg_mutex;
            return sg_mutex;
        }

        static std::map<std::string, XloggerCategory*>& GetGlobalInstanceMap() {
            static std::map<std::string, XloggerCategory*> sg_map;
            return sg_map;
        }

        XloggerCategory* NewXloggerInstance(const XLogConfig& _config, TLogLevel _level) {

            if (_config.logdir_.empty() || _config.nameprefix_.empty()) {
                return nullptr;
            }

            std::lock_guard<std::mutex> guard{GetGlobalMutex()};
            auto it = GetGlobalInstanceMap().find(_config.nameprefix_);
            if (it != GetGlobalInstanceMap().end()) {
                return it->second;
            }

            XloggerAppender* appender = XloggerAppender::NewInstance(_config, 0);

            using namespace std::placeholders;
            XloggerCategory* category = XloggerCategory::NewInstance(reinterpret_cast<uintptr_t>(appender),
                                                                     std::bind(&XloggerAppender::Write, appender, _1, _2));
            category->SetLevel(_level);
            GetGlobalInstanceMap()[_config.nameprefix_] = category;
            return category;
        }

        mars::comm::XloggerCategory* GetXloggerInstance(const char* _nameprefix) {
            if (nullptr == _nameprefix) {
                return nullptr;
            }

            std::lock_guard<std::mutex> guard{GetGlobalMutex()};
            auto it = GetGlobalInstanceMap().find(_nameprefix);
            if (it != GetGlobalInstanceMap().end()) {
                return it->second;
            }

            return nullptr;
        }

        void ReleaseXloggerInstance(const char* _nameprefix) {
            if (nullptr == _nameprefix) {
                return;
            }

            std::lock_guard<std::mutex> guard{GetGlobalMutex()};
            auto it = GetGlobalInstanceMap().find(_nameprefix);
            if (it == GetGlobalInstanceMap().end()) {
                return;
            }

            XloggerCategory* category  = it->second;
            XloggerAppender* appender = reinterpret_cast<XloggerAppender*>(category->GetAppender());
            XloggerAppender::DelayRelease(appender);
            XloggerCategory::DelayRelease(category);
            GetGlobalInstanceMap().erase(it);
        }

        void XloggerWrite(uintptr_t _instance_ptr, const XLoggerInfo* _info, const char* _log) {
            if (0 == _instance_ptr) {
                xlogger_Write(_info, _log);
            } else {
                XloggerCategory* category  = reinterpret_cast<XloggerCategory*>(_instance_ptr);
                category->Write(_info, _log);
            }
        }

        bool IsEnabledFor(uintptr_t _instance_ptr, TLogLevel _level) {
            if (0 == _instance_ptr) {
                return xlogger_IsEnabledFor(_level);
            } else {
                XloggerCategory* category  = reinterpret_cast<XloggerCategory*>(_instance_ptr);
                return category->IsEnabledFor(_level);
            }
        }

        TLogLevel GetLevel(uintptr_t _instance_ptr) {
            if (0 == _instance_ptr) {
                return xlogger_Level();
            } else {
                XloggerCategory* category  = reinterpret_cast<XloggerCategory*>(_instance_ptr);
                TLogLevel level = category->GetLevel();
                return level;
            }
        }

        void SetLevel(uintptr_t _instance_ptr, TLogLevel _level) {
            if (0 == _instance_ptr) {
                xlogger_SetLevel(_level);
            } else {
                XloggerCategory* category  = reinterpret_cast<XloggerCategory*>(_instance_ptr);
                return category->SetLevel(_level);
            }
        }

        void SetAppenderMode(uintptr_t _instance_ptr, TAppenderMode _mode) {
            if (0 == _instance_ptr) {
                appender_setmode(_mode);
            } else {
                XloggerCategory* category  = reinterpret_cast<XloggerCategory*>(_instance_ptr);
                XloggerAppender* appender = reinterpret_cast<XloggerAppender*>(category->GetAppender());
                return appender->SetMode(_mode);
            }
        }

        void Flush(uintptr_t _instance_ptr, bool _is_sync) {
            if (0 == _instance_ptr) {
                _is_sync ? appender_flush_sync() : appender_flush();
            } else {
                XloggerCategory* category  = reinterpret_cast<XloggerCategory*>(_instance_ptr);
                XloggerAppender* appender = reinterpret_cast<XloggerAppender*>(category->GetAppender());
                _is_sync ? appender->FlushSync() : appender->Flush();
            }
        }

        void FlushAll(bool _is_sync) {
            _is_sync ? appender_flush_sync() : appender_flush();
            std::lock_guard<std::mutex> guard{GetGlobalMutex()};
            auto& xmap = GetGlobalInstanceMap();
            // loop through all categories
            for (auto it = xmap.begin(); it != xmap.end(); ++it) {
                XloggerCategory* category  = it->second;
                XloggerAppender* appender = reinterpret_cast<XloggerAppender*>(category->GetAppender());
                _is_sync ? appender->FlushSync() : appender->Flush();
            }
        }

        void SetConsoleLogOpen(uintptr_t _instance_ptr, bool _is_open) {
            if (0 == _instance_ptr) {
                appender_set_console_log(_is_open);
            } else {
                XloggerCategory* category  = reinterpret_cast<XloggerCategory*>(_instance_ptr);
                XloggerAppender* appender = reinterpret_cast<XloggerAppender*>(category->GetAppender());
                return appender->SetConsoleLog(_is_open);
            }
        }

        void SetMaxFileSize(uintptr_t _instance_ptr, long _max_file_size) {
            if (0 == _instance_ptr) {
                appender_set_max_file_size(_max_file_size);
            } else {
                // 目前非默认实例设置最大大小因为时序会有问题
                // 因为设置过来的时候文件已经打开过一次
                XloggerCategory* category  = reinterpret_cast<XloggerCategory*>(_instance_ptr);
                XloggerAppender* appender = reinterpret_cast<XloggerAppender*>(category->GetAppender());
                return appender->SetMaxFileSize(_max_file_size);
            }
        }

        void SetMaxAliveTime(uintptr_t _instance_ptr, long _alive_seconds) {
            if (0 == _instance_ptr) {
                appender_set_max_alive_duration(_alive_seconds);
            } else {
                XloggerCategory* category  = reinterpret_cast<XloggerCategory*>(_instance_ptr);
                XloggerAppender* appender = reinterpret_cast<XloggerAppender*>(category->GetAppender());
                return appender->SetMaxAliveDuration(_alive_seconds);
            }
        }

    }
}
