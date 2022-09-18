//
// Created by Charles on 2022/9/18.
//

#include "ScopedJstring.h"
#include <stddef.h>
#include <string.h>

#include "cpplang.hpp"

ScopedJstring::ScopedJstring(JNIEnv *_env, jstring _jsr) : env_(_env), jstr_((jstring) env_->NewLocalRef(_jsr)), char_(nullptr), jstr2char_(true) {
    assert(env_);
    if (nullptr == env_ || nullptr == jstr_) {
        return;
    }
    if (env_->ExceptionOccurred()) {
        return;
    }
    char_ = env_->GetStringUTFChars(jstr_, nullptr);
}


ScopedJstring::ScopedJstring(JNIEnv *_env, const char *_char) : env_(_env), jstr_(nullptr), char_(_char), jstr2char_(false) {
    assert(env_);
    if (nullptr == env_ || nullptr == _char) {
        return;
    }

    if (env_->ExceptionOccurred()) {
        return;
    }

    jclass strClass = env_->FindClass("java/lang/String");
    jmethodID ctorID = env_->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");

    jbyteArray bytes = env_->NewByteArray((jsize) strlen(char_));
    env_->SetByteArrayRegion(bytes, 0, (jsize) strlen(char_), (jbyte *) char_);
    jstring encoding = env_->NewStringUTF("utf-8");

    jstr_ = (jstring) env_->NewObject(strClass, ctorID, bytes, encoding);

    env_->DeleteLocalRef(bytes);
    env_->DeleteLocalRef(encoding);
    env_->DeleteLocalRef(strClass);
}

ScopedJstring::~ScopedJstring() {
    if (nullptr == env_ || nullptr == jstr_ || nullptr == char_) {
        return;
    }
    if (env_->ExceptionOccurred()) {
        return;
    }

    if (jstr2char_) {
        env_->ReleaseStringUTFChars(jstr_, char_);
    }
    env_->DeleteLocalRef(jstr_);
}

const char *ScopedJstring::GetChar() const {
    if (env_->ExceptionOccurred()) {
        return nullptr;
    }

    return char_;
}



