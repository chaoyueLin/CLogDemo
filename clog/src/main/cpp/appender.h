// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in 
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.


/*
 * appender.h
 *
 *  Created on: 2013-3-7
 *      Author: yerungui
 */

#ifndef APPENDER_H_
#define APPENDER_H_

#include <string>
#include <vector>
#include <stdint.h>
#include "autobuffer.h"
#include "ptrbuffer.h"
#include "xloggerbase.h"

#include "log_base_buffer.h"
#include "mapped_file.h"
#include <thread>
#include <mutex>


namespace mars {
    namespace xlog {

        enum TAppenderMode {
            kAppenderAsync,
            kAppenderSync,
        };

        enum TCompressMode {
            kZlib,
            kZstd,
        };

        struct XLogConfig {
            TAppenderMode mode_ = kAppenderAsync;
            std::string logdir_;
            std::string nameprefix_;
            std::string pub_key_;
            TCompressMode compress_mode_ = kZlib;
            int compress_level_ = 6;
            std::string cachedir_;
            int cache_days_ = 0;
        };

        class XloggerAppender {
        public:
            static XloggerAppender* NewInstance(const XLogConfig& _config, uint64_t _max_byte_size);
            static void DelayRelease(XloggerAppender* _appender);
            static void Release(XloggerAppender*& _appender);

            void Open(const XLogConfig& _config);

            void Write(const XLoggerInfo* _info, const char* _log);
            void SetMode(TAppenderMode _mode);
            void Flush();
            void FlushSync();
            void Close();
            void WriteTips2File(const char* _tips_format, ...);
            const char* Dump(const void* _dumpbuffer, size_t _len);

            bool GetCurrentLogPath(char* _log_path, unsigned int _len);
            bool GetCurrentLogCachePath(char* _logPath, unsigned int _len);
            void SetConsoleLog(bool _is_open);
            void SetMaxFileSize(uint64_t _max_byte_size);
            void SetMaxAliveDuration(long _max_time);
            bool GetfilepathFromTimespan(int _timespan, const char* _prefix,
                                         std::vector<std::string>& _filepath_vec);
            bool MakeLogfileName(int _timespan, const char* _prefix,
                                 std::vector<std::string>& _filepath_vec);

        private:
            XloggerAppender(const XLogConfig& _config, uint64_t _max_byte_size);



            std::string __MakeLogFileNamePrefix(const timeval& _tv, const char* _prefix);
            void __GetFileNamesByPrefix(const std::string& _logdir,
                                        const std::string& _fileprefix,
                                        const std::string& _fileext,
                                        std::vector<std::string>& _filename_vec);
            void __GetFilePathsFromTimeval(const timeval& _tv,
                                           const std::string& _logdir,
                                           const char* _prefix,
                                           const std::string& _fileext,
                                           std::vector<std::string>& _filepath_vec);
            long __GetNextFileIndex(const std::string& _fileprefix, const std::string& _fileext);
            void __MakeLogFileName(const timeval& _tv,
                                   const std::string& _logdir,
                                   const char* _prefix,
                                   const std::string& _fileext,
                                   char* _filepath, unsigned int _len);

            void __GetMarkInfo(char* _info, size_t _info_len);
            void __WriteTips2Console(const char* _tips_format, ...);
            bool __WriteFile(const void* _data, size_t _len, FILE* _file);
            bool __OpenLogFile(const std::string& _log_dir);
            void __CloseLogFile();
            bool __CacheLogs();
            void __Log2File(const void* _data, size_t _len, bool _move_file);
            void __AsyncLogThread();
            void __WriteSync(const XLoggerInfo* _info, const char* _log);
            void __WriteAsync(const XLoggerInfo* _info, const char* _log);
            void __DelTimeoutFile(const std::string& _log_path);
            bool __AppendFile(const std::string& _src_file, const std::string& _dst_file);
            void __MoveOldFiles(const std::string& _src_path, const std::string& _dest_path,
                                const std::string& _nameprefix);

        private:
            XLogConfig config_;
            LogBaseBuffer* log_buff_ = nullptr;
            mapped_file mmap_file_;
            std::thread thread_async_;
            std::mutex mutex_buffer_async_;
            std::mutex mutex_log_file_;
            FILE* logfile_ = nullptr;
            time_t openfiletime_ = 0;
#ifdef DEBUG
            bool consolelog_open_ = true;
#else
            bool consolelog_open_ = false;
#endif
            bool log_close_ = true;
            std::condition_variable cond_buffer_async_;
            uint64_t max_file_size_ = 0; // 0, will not split log file.
            long max_alive_time_ = 10 * 24 * 60 * 60;    // 10 days in second

            time_t last_time_ = 0;
            uint64_t last_tick_ = 0;
            char last_file_path_[1024] = {0};
        };

        void appender_open(const XLogConfig &_config);

        void appender_flush();

        void appender_flush_sync();

        void appender_close();

        void appender_setmode(TAppenderMode _mode);

        bool appender_getfilepath_from_timespan(int _timespan, const char *_prefix, std::vector<std::string> &_filepath_vec);

        bool appender_make_logfile_name(int _timespan, const char *_prefix, std::vector<std::string> &_filepath_vec);

        bool appender_get_current_log_path(char *_log_path, unsigned int _len);

        bool appender_get_current_log_cache_path(char *_logPath, unsigned int _len);

        void appender_set_console_log(bool _is_open);


        void appender_set_max_file_size(uint64_t _max_byte_size);

        void appender_set_max_alive_duration(long _max_time);

    }
}


#endif
