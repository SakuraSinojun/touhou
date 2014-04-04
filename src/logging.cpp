

#include "logging.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <iomanip>

#if defined(ANDROID)
#include <android/log.h>
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "logging"
#endif

#ifdef ANDROID
#define GETTID()    gettid()
#else
#define GETTID()    syscall(SYS_gettid)
#endif

/*
static unsigned int GetTickCount(void)
{
    struct sysinfo  si;
    sysinfo(&si);
    return si.uptime;
}
*/

namespace logging {
LogMessage::LogMessage(const char * file, const char * func, int line, bool fatal)
    : file_(file)
    , func_(func)
    , line_(line)
    , fatal_(fatal)
{
}

LogMessage::LogMessage()
    : fatal_(false)
{
}

LogMessage::~LogMessage()
{

    std::ostringstream   oss;
    oss << "[" << GETTID() << "] ";

    // time_t  t = time(NULL);
    struct tm local_time;
    struct timeval tv;
    memset(&local_time, 0, sizeof(local_time));
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &local_time);

    oss << '[';
    oss << std::setfill('0')
        << std::setw(2) << local_time.tm_hour
        << ':'
        << std::setw(2) << local_time.tm_min
        << ':'
        << std::setw(2) << local_time.tm_sec
        << ':'
        << std::setw(6) << tv.tv_usec
        << "] ";

    if (!file_.empty()) {
        size_t  pos = file_.find_last_of("/\\");
        if (pos != std::string::npos)
            file_ = file_.substr(pos + 1);
        oss << '[' << file_ << ':' << func_ << ':' << line_ << "] ";
    }

    std::string st = stream_.str();
    size_t pos = st.find_last_not_of("\r\n ");
    if (pos != std::string::npos)
        st = st.substr(0, pos + 1);
    oss << ' ' << st;
    oss << TERMC_NONE;
    oss << std::endl;
#if defined(ANDROID)
    __android_log_write(ANDROID_LOG_ERROR, LOG_TAG, oss.str().c_str());
#else
    printf("%s",  oss.str().c_str());
    fflush(stdout);
#endif
    if (fatal_) {
        abort();
    }
}
} // namespace

extern "C" void logging_LogPrintfC(const char * file, const char * func, int line, const char * fmt, ...)
{
    char temp[4096];
    va_list args; 
    va_start(args, fmt);
    vsnprintf(temp, sizeof(temp), fmt, args);
    va_end(args);
    ::logging::LogMessage(file, func, line).stream() << temp;
}

std::ostream& operator<<(std::ostream& o, cocos2d::CCPoint& pt)
{
    o << "(" << pt.x << ", " << pt.y << ")";
    return o;
}

std::ostream& operator<<(std::ostream& o, cocos2d::CCSize& sz)
{
    o << "(" << sz.width << ", " << sz.height << ")";
    return o;
}



