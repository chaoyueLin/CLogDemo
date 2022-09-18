//
// Created by Charles on 2022/9/18.
//

#ifndef CLOGDEMO_LOGCRYPT_H
#define CLOGDEMO_LOGCRYPT_H

#include <stdint.h>
#include <string>

#include "autobuffer.h"


class LogCrypt {
public:
    LogCrypt(): seq_(0) {}
    virtual ~LogCrypt() {}

private:
    LogCrypt(const LogCrypt&);
    LogCrypt& operator=(const LogCrypt&);

public:
    uint32_t GetHeaderLen();
    uint32_t GetTailerLen();

    void SetHeaderInfo(char* _data, bool _is_async);
    void SetTailerInfo(char* _data);

    uint32_t GetLogLen(const char* const _data, size_t _len);
    void UpdateLogLen(char* _data, uint32_t _add_len);

    bool GetLogHour(const char* const _data, size_t _len, int& _begin_hour, int& _end_hour);
    void UpdateLogHour(char* _data);

    bool GetPeriodLogs(const char* const _log_path, int _begin_hour, int _end_hour, unsigned long& _begin_pos, unsigned long& _end_pos, std::string& _err_msg);

    void CryptSyncLog(const char* const _log_data, size_t _input_len, AutoBuffer& _out_buff);
    void CryptAsyncLog(const char* const _log_data, size_t _input_len, AutoBuffer& _out_buff);

    bool Fix(char* _data, size_t _data_len, bool& _is_async, uint32_t& _raw_log_len);

private:
    uint16_t seq_;

};


#endif //CLOGDEMO_LOGCRYPT_H
