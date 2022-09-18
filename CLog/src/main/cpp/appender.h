//
// Created by Charles on 2022/9/18.
//

#ifndef CLOGDEMO_APPENDER_H
#define CLOGDEMO_APPENDER_H


#ifndef APPENDER_H_
#define APPENDER_H_

#include <string>
#include <vector>
#include <stdint.h>

enum TAppenderMode
{
    kAppednerAsync,
    kAppednerSync,
};

void appender_open(TAppenderMode _mode, const char* _dir, const char* _nameprefix);
void appender_open_with_cache(TAppenderMode _mode, const std::string& _cachedir, const std::string& _logdir, const char* _nameprefix);
void appender_flush();
void appender_flush_sync();
void appender_close();
void appender_setmode(TAppenderMode _mode);
bool appender_getfilepath_from_timespan(int _timespan, const char* _prefix, std::vector<std::string>& _filepath_vec);
bool appender_make_logfile_name(int _timespan, const char* _prefix, std::vector<std::string>& _filepath_vec);
bool appender_get_current_log_path(char* _log_path, unsigned int _len);
bool appender_get_current_log_cache_path(char* _logPath, unsigned int _len);
void appender_set_console_log(bool _is_open);


void appender_set_max_file_size(uint64_t _max_byte_size);

#endif

#endif //CLOGDEMO_APPENDER_H
