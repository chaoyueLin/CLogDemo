//
// Created by Charles on 2022/9/18.
//

#ifndef CLOGDEMO_LOGBUFFER_H
#define CLOGDEMO_LOGBUFFER_H


#include <zlib.h>
#include <string>
#include <stdint.h>

#include "ptrbuffer.h"
#include "autobuffer.h"

class LogCrypt;

class LogBuffer {
public:
    LogBuffer(void* _pbuffer, size_t _len, bool _is_compress);
    ~LogBuffer();

public:
    static bool GetPeriodLogs(const char* _log_path, int _begin_hour, int _end_hour, unsigned long& _begin_pos, unsigned long& _end_pos, std::string& _err_msg);
    static bool Write(const void* _data, size_t _inputlen, AutoBuffer& _out_buff);

public:
    PtrBuffer& GetData();


    void Flush(AutoBuffer& _buff);
    bool Write(const void* _data, size_t _length);

private:

    bool __Reset();
    void __Flush();
    void __Clear();

    void __Fix();

private:
    PtrBuffer buff_;
    bool is_compress_;
    z_stream cstream_;

    static class LogCrypt* s_log_crypt;

};



#endif //CLOGDEMO_LOGBUFFER_H
