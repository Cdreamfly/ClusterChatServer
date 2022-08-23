//
// Created by Cmf on 2022/5/23.
//

#pragma once

#include "base/noncopyable.hpp"
#include "base/Timestamp.hpp"
#include <memory>
#include <chrono>
#include <thread>
#include <sstream>
#include <fstream>
#include <mutex>
#include <iostream>

/*
 * 日志级别
 */
enum class LogLevel : uint8_t {
    NONE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5
};

class Logger;

/*
 * 日志事件
 */
class LogEvent {
public:
    using ptr = std::shared_ptr<LogEvent>;

    template<typename ...Args>
    LogEvent(LogLevel level,
             const std::string &fileName,
             const uint32_t line,
             const std::string &format,
             const Args &...args)
            : _level(level),
              _fileName(fileName),
              _line(line),
              _time(GetCurrentSystemTime()),
              _msg(LogFormat(format, args...)),
              _threadId(std::this_thread::get_id()) {
    }

    const std::string GetCurrentSystemTime() {
        return std::move(Timestamp::Now().ToString());
    }

    static const char *ToString(LogLevel level) noexcept {
        switch (level) {
            case LogLevel::DEBUG:
                return "DEBUG";
                break;
            case LogLevel::INFO:
                return "INFO";
                break;
            case LogLevel::WARN:
                return "WARN";
                break;
            case LogLevel::ERROR:
                return "ERROR";
                break;
            case LogLevel::FATAL:
                return "FATAL";
                break;
            default:
                return "NONE";
        }
    }

    template<typename ... Args>
    std::string LogFormat(const std::string &format, Args &... args) {
        size_t size = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
        std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
        std::snprintf(buf.get(), size, format.c_str(), args ...);
        return std::string(buf.get(), buf.get() + size - 1);
    }

    static LogLevel FromString(const std::string &str) noexcept {
        if (str == "DEBUG") {
            return LogLevel::DEBUG;
        } else if (str == "INFO") {
            return LogLevel::INFO;
        } else if (str == "WARN") {
            return LogLevel::WARN;
        } else if (str == "ERROR") {
            return LogLevel::ERROR;
        } else if (str == "FATAL") {
            return LogLevel::FATAL;
        } else {
            return LogLevel::NONE;
        }
    }

    LogLevel GetLevel() const {
        return _level;
    }

    std::thread::id GetThreadId() const {
        return _threadId;
    }

    std::string GetFileName() const {
        return _fileName;
    }

    uint32_t GetLine() const {
        return _line;
    }

    std::string GetMsg() const {
        return _msg;
    }

    std::string GetTime() const {
        return _time;
    }

private:
    uint32_t _line = 0;
    std::thread::id _threadId;
    std::string _fileName;
    std::string _time;
    std::string _msg;
    LogLevel _level;
};

class LogAppender {
public:
    using ptr = std::shared_ptr<LogAppender>;

    LogAppender() = default;

    virtual ~LogAppender() noexcept {}

    virtual void Log(const LogEvent::ptr event) = 0;
};

/*
 * 输出到控制台
 */
class StdoutLogAppender : public LogAppender {
public:
    using ptr = std::shared_ptr<StdoutLogAppender>;

    void Log(const LogEvent::ptr event) override {
        std::stringstream _ss;
        _ss << event->GetTime() << " "
            << event->ToString(event->GetLevel()) << " "
            << event->GetThreadId() << " "
            << event->GetFileName() << " "
            << event->GetLine() << " "
            << event->GetMsg() << std::endl;
        std::cout << _ss.str();
    }
};

/*
 * 输出到文件
 */
class FileLogAppender : public LogAppender {
public:
    using ptr = std::shared_ptr<FileLogAppender>;

    FileLogAppender(const std::string &fileName) : _fileName(fileName) {
        if (_ofs.is_open()) {
            _ofs.close();
        }
        _ofs.open(_fileName.c_str(), std::ios::out | std::ios::app | std::ios::binary);
    }

    ~FileLogAppender() {
        if (_ofs.is_open()) {
            _ofs.flush();
            _ofs.close();
        }
    }

    virtual void Log(const LogEvent::ptr event) override {
        if (_ofs.is_open()) {
            _ofs << event->GetTime() << " "
                 << event->ToString(event->GetLevel()) << " "
                 << event->GetThreadId() << " "
                 << event->GetFileName() << " "
                 << event->GetLine() << " "
                 << event->GetMsg() << " " << std::endl;
        }
    }

private:
    std::string _fileName;
    std::ofstream _ofs;

};

/*
 * 日志器
 */
class Logger : private noncopyable {
public:
    using ptr = std::shared_ptr<Logger>;

    static Logger &GetInstance() {
        static Logger logger;
        return logger;
    }

    LogAppender::ptr StdoutLog() {
        _logAppender = std::make_shared<StdoutLogAppender>();
        return _logAppender;
    }

    LogAppender::ptr FileLog(const std::string path) {
        _logAppender = std::make_shared<FileLogAppender>(path);
        return _logAppender;
    }

private:
    LogAppender::ptr _logAppender;
};

#define LOG_BASE(level, fmt, ...) \
Logger::GetInstance().StdoutLog()->Log(std::make_shared<LogEvent>(level,__FILE__,__LINE__,fmt,##__VA_ARGS__));

#define LOG_DEBUG(fmt, ...) LOG_BASE(LogLevel::DEBUG,fmt,##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG_BASE(LogLevel::INFO,fmt,##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG_BASE(LogLevel::WARN,fmt,##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG_BASE(LogLevel::ERROR,fmt,##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG_BASE(LogLevel::FATAL,fmt,##__VA_ARGS__)