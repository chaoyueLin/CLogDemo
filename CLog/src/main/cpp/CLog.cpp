#include <jni.h>
#include <string>
#include "cpplang.hpp"
#include "ScopedJstring.h"
#include "xloggerbase.h"
#include "appender.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_clog_CLog_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_clog_CLog_logWrite(JNIEnv *env, jclass clazz, jobject log_info, jstring log) {
    // TODO: implement logWrite()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_clog_CLog_logWrite2(JNIEnv *env, jclass clazz, jint level, jstring tag, jstring filename, jstring funcname, jint line, jint pid,
                                     jlong tid, jlong maintid, jstring log) {
    // TODO: implement logWrite2()
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_clog_CLog_getLogLevel(JNIEnv *env, jobject thiz) {
    // TODO: implement getLogLevel()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_clog_CLog_setLogLevel(JNIEnv *env, jclass clazz, jint log_level) {
    // TODO: implement setLogLevel()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_clog_CLog_setAppenderMode(JNIEnv *env, jclass clazz, jint mode) {
    // TODO: implement setAppenderMode()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_clog_CLog_setConsoleLogOpen(JNIEnv *env, jclass clazz, jboolean is_open) {
    // TODO: implement setConsoleLogOpen()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_clog_CLog_setErrLogOpen(JNIEnv *env, jclass clazz, jboolean is_open) {
    // TODO: implement setErrLogOpen()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_clog_CLog_appenderOpen(JNIEnv *env, jclass clazz, jint level, jint mode, jstring _cache_dir, jstring _log_dir, jstring _nameprefix) {
    if (NULL == _log_dir || NULL == _nameprefix) {
        return;
    }

    std::string cache_dir;
    if (NULL != _cache_dir) {
        ScopedJstring cache_dir_jstr(env, _cache_dir);
        cache_dir = cache_dir_jstr.GetChar();
    }

    ScopedJstring log_dir_jstr(env, _log_dir);
    ScopedJstring nameprefix_jstr(env, _nameprefix);
    appender_open_with_cache((TAppenderMode)mode, cache_dir.c_str(), log_dir_jstr.GetChar(), nameprefix_jstr.GetChar());
    xlogger_SetLevel((TLogLevel)level);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_clog_CLog_appenderClose(JNIEnv *env, jobject thiz) {
    // TODO: implement appenderClose()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_clog_CLog_appenderFlush(JNIEnv *env, jobject thiz, jboolean is_sync) {
    // TODO: implement appenderFlush()
}