#pragma once
#ifndef _VGR_COMMON_UTIL_LOGGER_HPP
#define _VGR_COMMON_UTIL_LOGGER_HPP

#include <common/global.hpp>
#include <common/3rdparty/boost.hpp>
#include <common/util/util.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



_VGR_BEGIN
_VGR_UTIL_BEGIN



class LogManagerItem;
class LogManager;
class DefaultLogManager;
class Logger;



class LogManagerItem {

public:
	enum Level {
		Uncategorized,
		Message,
		Warning,
		Error
	};

private:
	int									m_tid;
	Level								m_level;
	std::string							m_sender;
	std::string							m_content;

public:
	inline LogManagerItem() : m_tid(-1), m_level(Level::Uncategorized), m_sender(), m_content() { }
	inline LogManagerItem(Level level, const std::string &sender, const std::string &content) :
			m_level(level), m_sender(sender), m_content(content) { }
	inline LogManagerItem(Level level, const std::string &sender, const boost::format &format) :
			m_level(level), m_sender(sender), m_content(format.str()) { }
	inline LogManagerItem(int tid, Level level, const std::string &sender, const std::string &content) :
			m_tid(tid), m_level(level), m_sender(sender), m_content(content) { }
	inline LogManagerItem(int tid, Level level, const std::string &sender, const boost::format &format) :
			m_tid(tid), m_level(level), m_sender(sender), m_content(format.str()) { }

public:
	inline int tid() const { return (this->m_tid); }
	inline Level level() const { return (this->m_level); }
	inline const std::string &sender() const { return (this->m_sender); }
	inline const std::string &content() const { return (this->m_content); }
};



class LogManager {

public:
	typedef	LogManagerItem				Item;

public:
	LogManager() { }
	virtual ~LogManager() { }

private:
	LogManager(const LogManager &logger) { }

private:
	LogManager &operator =(const LogManager &logger) { return (*this); }

public:
	virtual void append(const Item &item) { }
	virtual void append(int thread, const Item &item) { }
	virtual void beginProgress() { }
	virtual void beginProgress(int thread) { }
	virtual void endProgress() { }
	virtual void endProgress(int thread) { }
	virtual void updateProgress(int completed, int total) { }
	virtual void updateProgress(int thread, int completed, int total) { }
	virtual void busyProgress() { }
	virtual void busyProgress(int thread) { }
};



class DefaultLogManager : public LogManager {

private:
	std::ostream						&m_ostream;
	std::istream						&m_istream;

public:
	DefaultLogManager() : m_ostream(std::cout), m_istream(std::cin) { }
	DefaultLogManager(std::ostream &ostream, std::istream &istream) : m_ostream(ostream), m_istream(istream) { }
	virtual ~DefaultLogManager() { }

private:
	DefaultLogManager(const DefaultLogManager &logger) : m_ostream(logger.m_ostream), m_istream(logger.m_istream) { }

private:
	DefaultLogManager &operator =(const DefaultLogManager &logger) { return (*this); }

public:
	virtual void append(const Item &item);
	virtual void append(int thread, const Item &item) {
		this->append(item);
	}
};



class Logger {

private:
	LogManager							*m_log_manager;

public:
	static Logger &instance();
	static void message(const std::string &content);
	static void message(const boost::format &format);
	static void message(const std::string &sender, const std::string &content);
	static void message(const std::string &sender, const boost::format &format);
	static void warning(const std::string &content);
	static void warning(const boost::format &format);
	static void warning(const std::string &sender, const std::string &content);
	static void warning(const std::string &sender, const boost::format &format);
	static void error(const std::string &content);
	static void error(const boost::format &format);
	static void error(const std::string &sender, const std::string &content);
	static void error(const std::string &sender, const boost::format &format);
	static void beginProgress();
	static void beginProgress(int thread);
	static void endProgress();
	static void endProgress(int thread);
	static void updateProgress(int completed, int total);
	static void updateProgress(int thread, int completed, int total);
	static void busyProgress();
	static void busyProgress(int thread);

private:
	inline Logger() : m_log_manager(new DefaultLogManager()) { }
	inline ~Logger() {
		if (this->m_log_manager) delete this->m_log_manager;
	}

public:
	inline LogManager *logManager() { return (this->m_log_manager); }
	inline const LogManager *logManager() const { return (this->m_log_manager); }
	inline void setLogManager(LogManager *log_manager) {
		if (this->m_log_manager) delete this->m_log_manager;
		this->m_log_manager = log_manager;
	}

public:
	inline LogManager &getManager() { return (*(this->m_log_manager)); }
};



#ifndef _VGR_LOGGER_DEFINED
#define _VGR_LOGGER_DEFINED
#  define _VGR_LOGGER_MSG(_S, _M)				vgr::util::Logger::message(_S, _M);
#  define _VGR_LOGGER_WRN(_S, _W)				vgr::util::Logger::warning(_S, _W);
#  define _VGR_LOGGER_ERR(_S, _E)				vgr::util::Logger::error(_S, _E);
#  define _VGR_SYSTEM_MSG(_M)					vgr::util::Logger::message(_M)
#  define _VGR_SYSTEM_WRN(_W)					vgr::util::Logger::warning(_W)
#  define _VGR_SYSTEM_ERR(_E)					vgr::util::Logger::error(_E)
#  define _VGR_LOGGER_BEG_PROG					vgr::util::Logger::beginProgress();
#  define _VGR_THREAD_BEG_PROG(_I)				vgr::util::Logger::beginProgress(_I);
#  define _VGR_LOGGER_END_PROG					vgr::util::Logger::endProgress();
#  define _VGR_THREAD_END_PROG(_I)				vgr::util::Logger::endProgress(_I);
#  define _VGR_LOGGER_UPD_PROG(_C, _T)			vgr::util::Logger::updateProgress(_C, _T);
#  define _VGR_THREAD_UPD_PROG(_I, _C, _T)		vgr::util::Logger::updateProgress(_I, _C, _T);
#  define _VGR_LOGGER_BSY_PROG					vgr::util::Logger::busyProgress();
#  define _VGR_THREAD_BSY_PROG(_I)				vgr::util::Logger::busyProgress(_I);
#endif /* _GT_LOGGER_DEFINED */



_VGR_UTIL_END
_VGR_END



#pragma warning (pop)

#endif /* _VGR_COMMON_UTIL_LOGGER_HPP */
