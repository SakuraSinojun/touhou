
#pragma once

#define TERMC_RED     "\033[1;31;40m"
#define TERMC_GREEN   "\033[1;32;40m"
#define TERMC_YELLOW  "\033[1;33;40m"
#define TERMC_BLUE    "\033[1;34;40m"
#define TERMC_PINK    "\033[1;35;40m"
#define TERMC_NONE    "\033[m"

#ifdef __cplusplus

#include "cocos2d.h"

#include <sstream>
#include <string>
#include <sys/time.h>

namespace logging {

class LogMessageVoidify {
public:
    LogMessageVoidify() {}
    void operator&(std::ostream&) {}
};

class LogMessage {
public:
    LogMessage(const char * file, const char * func, int line, bool fatal = false);
    LogMessage();
    ~LogMessage();

    std::ostream&   stream() { return stream_; }

private:
    std::ostringstream  stream_;
    std::string file_;
    std::string func_;
    int         line_;
    bool        fatal_;
};

class ScopeTracer {
public:
    ScopeTracer(const char * func, const char * name, int line) : name_(name), func_(func), line_(line) {
        LogMessage().stream() << TERMC_RED << "Into scope: " << name << " (" << func << ':' << line << ")" << TERMC_NONE;
        gettimeofday(&start_, NULL);

    }
    ~ScopeTracer() {
        struct timeval  tv;
        gettimeofday(&tv, NULL);
        long long tu = ((long long)tv.tv_sec * 1000000 + tv.tv_usec) - ((long long)start_.tv_sec * 1000000 + start_.tv_usec);
        LogMessage().stream() << TERMC_GREEN << "Leave scope: " << name_ << " (" << func_ << ':' << line_ << ")" << TERMC_BLUE << " Time costs: " << tu << " us." << TERMC_NONE;
    }
private:
    const char * name_;
    const char * func_;
    int          line_;
    struct timeval  start_;
};

class TimerScopeTracer {
public:
    TimerScopeTracer(const char * func, const char * name, int line, long long dt) : name_(name), func_(func), line_(line), difftime_(dt) {
        // LogMessage().stream() << TERMC_RED << "Into scope: " << name << " (" << func << ':' << line << ")" << TERMC_NONE;
        gettimeofday(&start_, NULL);
    }
    ~TimerScopeTracer() {
        struct timeval  tv;
        gettimeofday(&tv, NULL);
        long long tu = ((long long)tv.tv_sec * 1000000 + tv.tv_usec) - ((long long)start_.tv_sec * 1000000 + start_.tv_usec);
        if (tu >= difftime_) {
            LogMessage().stream() << TERMC_PINK << "Leave scope: " << name_ << " (" << func_ << ':' << line_ << ")" << TERMC_BLUE << " Time costs: " << tu << " us." << TERMC_NONE;
        }
    }
private:
    const char * name_;
    const char * func_;
    int          line_;
    struct timeval  start_;
    long long    difftime_;
};

} // namespace logging

std::ostream& operator<<(std::ostream& o, cocos2d::CCPoint& pt);

#if 1
#if defined(__GNUC__)
#   define __FUNC__     ((const char *) (__PRETTY_FUNCTION__))
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 19901L
#   define __FUNC__     ((const char *) (__func__))
#else
#   define __FUNC__     ((const char *) (__FUNCTION__))
#endif

#define ILOG()   ::logging::LogMessageVoidify() & ::logging::LogMessage(__FILE__, __FUNC__, __LINE__).stream()
#define FATAL()  ::logging::LogMessageVoidify() & ::logging::LogMessage(__FILE__, __FUNC__, __LINE__, true).stream() << TERMC_RED << "FATAL: "
#define RUN_HERE()  ILOG() << TERMC_YELLOW << "Run here! "
#define ALERT()  ILOG() << TERMC_RED << "Alert! " << TERMC_NONE
#define ScopeTrace(x)   ::logging::ScopeTracer x ## __LINE__(__FUNC__, #x, __LINE__)
#define DCHECK(condition)   if (!(condition)) FATAL() << "Check failed. (" << #condition << ") "
#define TimerScopeTrace(x, t)   ::logging::TimerScopeTracer x ## __LINE__(__FUNC__, #x, __LINE__, t)
#else
#define ScopeTrace(x)
#define TimerScopeTrace(x, t)
#endif
extern "C" void logging_LogPrintfC(const char * file, const char * func, int line, const char * fmt, ...);
#define LOGPRINTF(fmt, ...) logging_LogPrintfC(__FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
void logging_LogPrintfC(const char * file, const char * func, int line, const char * fmt, ...);
#define LOGPRINTF(fmt, ...) logging_LogPrintfC(__FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define RUN_HERE() LOGPRINTF(TERMC_YELLOW"___________________ Run here!"TERMC_NONE"\n");
#define FATAL() RUN_HERE();abort()
#endif



