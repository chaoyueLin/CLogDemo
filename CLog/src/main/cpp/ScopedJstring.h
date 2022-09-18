//
// Created by Charles on 2022/9/18.
//

#ifndef CLOGDEMO_SCOPEDJSTRING_H
#define CLOGDEMO_SCOPEDJSTRING_H

#include <jni.h>

class ScopedJstring final {
    JNIEnv* env_;
    jstring jstr_;
    const char* char_;
    bool jstr2char_;
public:
    ScopedJstring(JNIEnv* _env,jstring _jsr);
    ScopedJstring(JNIEnv* _env,const char* _char);

    ~ScopedJstring();
    const char* GetChar() const;
    jstring GetJstr() const;

    ScopedJstring() =delete;
    ScopedJstring(const ScopedJstring&) =delete;
    ScopedJstring& operator=(const ScopedJstring&) =delete;
};


#endif //CLOGDEMO_SCOPEDJSTRING_H
