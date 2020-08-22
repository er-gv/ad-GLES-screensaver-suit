//
// Created by nathan on 05/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_LOGGER_H
#define ANDROIDOPENGLESLESSONSCPP_LOGGER_H

#define log(severity, tag, ...) __android_log_print(severity, tag, __VA_ARGS__);
#define  LOG_I(tag, ...)  log(ANDROID_LOG_INFO, tag, __VA_ARGS__)
#define  LOG_W(tag, ...)  log(ANDROID_LOG_WARN, tag, __VA_ARGS__)
#define  LOG_D(tag, ...)  log(ANDROID_LOG_DEBUG, tag, __VA_ARGS__)
#define  LOG_V(tag, ...)  log(ANDROID_LOG_VERBOSE, tag, __VA_ARGS__)
#define  LOG_E(tag, ...)  log(ANDROID_LOG_ERROR, tag, __VA_ARGS__)

#include <android/log.h>
#include <GLES2/gl2.h>
#include <cstring>
#include <cstdlib>

class Logger {

    char *log_tag;

public:

    Logger(const char *tag) {
        log_tag = strdup(tag);
    }

    ~Logger() {
        free(log_tag);
    }

    static enum SEVERITY {
        INFO = ANDROID_LOG_INFO,
        WARNS = ANDROID_LOG_WARN,
        DEBUG = ANDROID_LOG_DEBUG,
        ERROR = ANDROID_LOG_ERROR,
        FATAL = ANDROID_LOG_FATAL,
        VERBOSE = ANDROID_LOG_VERBOSE
    };



    void printGLString(const char *name, GLenum s) {
        const char *v = (const char *) glGetString(s);
        LOG_I("GL %s = %s \n", name, v);
    }

    void checkGlError(const char *op) {
        for (GLint error = glGetError(); error; error = glGetError()) {
            LOG_I("after %s() glError (0x%x)\n", op, error);
        }
    }
};
#endif //ANDROIDOPENGLESLESSONSCPP_LOGGER_H
