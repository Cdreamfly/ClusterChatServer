#pragma once

#include "cm/base/NonCopyable.hpp"
#include "cm/base/Timestamp.hpp"

#include <memory>
#include <thread>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>

namespace cm {
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
                 std::string fileName,
                 std::string funcName,
                 const uint32_t line,
                 const std::string &format,
                 const Args &...args)
                : _line(line),
                  _threadId(std::this_thread::get_id()),
                  _fileName(std::move(fileName)),
                  _funcName(std::move(funcName)),
                  _time(GetCurrentSystemTime()),
                  _msg(LogFormat(format, args...)),
                  _level(level) {
        }

        static std::string GetCurrentSystemTime() {
            return Timestamp::now().toString();
        }

        template<typename ... Args>
        std::string LogFormat(const std::string &format, Args &... args) {
            size_t size = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
            std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
            std::snprintf(buf.get(), size, format.c_str(), args ...);
            return std::string{buf.get(), buf.get() + size - 1};
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

        std::string GetFuncName() const {
            return _funcName;
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

        std::unordered_map<LogLevel, std::string> GetLogLevelMap() const {
            return _logLevelMap;
        }

    private:
        std::unordered_map<LogLevel, std::string> _logLevelMap{
                {LogLevel::NONE,  "NONE"},
                {LogLevel::INFO,  "INFO"},
                {LogLevel::DEBUG, "DEBUG"},
                {LogLevel::ERROR, "ERROR"},
                {LogLevel::FATAL, "FATAL"},
                {LogLevel::WARN,  "WARN"},
        };

    private:
        uint32_t _line = 0;
        std::thread::id _threadId;
        std::string _fileName;
        std::string _funcName;
        std::string _time;
        std::string _msg;
        LogLevel _level;
    };

    class LogAppender {
    public:
        using ptr = std::shared_ptr<LogAppender>;

        LogAppender() = default;

        virtual ~LogAppender() noexcept = default;

        virtual void Log(LogEvent::ptr event) = 0;
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
                << event->GetLogLevelMap()[event->GetLevel()] << " "
                << event->GetThreadId() << " "
                << event->GetFileName() << " "
                << event->GetFuncName() << " "
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

        explicit FileLogAppender(std::string fileName) : _fileName(std::move(fileName)) {
            if (_ofs.is_open()) {
                _ofs.close();
            }
            _ofs.open(_fileName.c_str(), std::ios::out | std::ios::app | std::ios::binary);
        }

        ~FileLogAppender() override {
            if (_ofs.is_open()) {
                _ofs.flush();
                _ofs.close();
            }
        }

        void Log(const LogEvent::ptr event) override {
            if (_ofs.is_open()) {
                _ofs << event->GetTime() << " "
                     << event->GetLogLevelMap()[event->GetLevel()] << " "
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
    class Logger : private cm::NonCopyable {
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

        LogAppender::ptr FileLog(const std::string &path) {
            _logAppender = std::make_shared<FileLogAppender>(path);
            return _logAppender;
        }

    private:
        LogAppender::ptr _logAppender;
    };
}
#define LOG_BASE(level, fmt, ...) \
cm::Logger::GetInstance().StdoutLog()->Log(std::make_shared<cm::LogEvent>(level,__FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__));
//cm::Logger::GetInstance().FileLog("../file.log")->Log(std::make_shared<cm::LogEvent>(level,__FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)) \

#define LOG_DEBUG(fmt, ...) LOG_BASE(cm::LogLevel::DEBUG,fmt,##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG_BASE(cm::LogLevel::INFO,fmt,##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG_BASE(cm::LogLevel::WARN,fmt,##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG_BASE(cm::LogLevel::ERROR,fmt,##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG_BASE(cm::LogLevel::FATAL,fmt,##__VA_ARGS__)
