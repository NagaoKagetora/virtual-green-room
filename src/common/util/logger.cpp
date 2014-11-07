#include "logger.hpp"

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



_VGR_BEGIN
_VGR_UTIL_BEGIN



void DefaultLogManager::append(const Item &item) {
	switch (item.level()) {
	case Item::Level::Message:
		this->m_ostream << (boost::format("<MSG> [%1%] %2%") % item.sender() % item.content()).str() << std::endl;
		return;
	case Item::Level::Warning:
		this->m_ostream << (boost::format("<WRN> [%1%] %2%") % item.sender() % item.content()).str() << std::endl;
		return;
	case Item::Level::Error:
		this->m_ostream << (boost::format("<ERR> [%1%] %2%") % item.sender() % item.content()).str() << std::endl;
		return;
	}
}



Logger &Logger::instance() {
	static Logger Instance;
	return (Instance);
}

void Logger::message(const std::string &content) {
	Logger::message("System", content);
}

void Logger::message(const boost::format &format) {
	Logger::message("System", format.str());
}

void Logger::message(const std::string &sender, const std::string &content) {
	Logger::instance().getManager().append(LogManagerItem(LogManagerItem::Message, sender, content));
}

void Logger::message(const std::string &sender, const boost::format &format) {
	Logger::message(sender, format.str());
}

void Logger::warning(const std::string &content) {
	Logger::warning("System", content);
}

void Logger::warning(const boost::format &format) {
	Logger::warning("System", format.str());
}

void Logger::warning(const std::string &sender, const std::string &content) {
	Logger::instance().getManager().append(LogManagerItem(LogManagerItem::Warning, sender, content));
}

void Logger::warning(const std::string &sender, const boost::format &format) {
	Logger::warning(sender, format.str());
}

void Logger::error(const std::string &content) {
	Logger::error("System", content);
}

void Logger::error(const boost::format &format) {
	Logger::error("System", format.str());
}

void Logger::error(const std::string &sender, const std::string &content) {
	Logger::instance().getManager().append(LogManagerItem(LogManagerItem::Error, sender, content));
}

void Logger::error(const std::string &sender, const boost::format &format) {
	Logger::error(sender, format.str());
}

void Logger::beginProgress() {
	Logger::instance().getManager().beginProgress();
}

void Logger::beginProgress(int thread) {
	Logger::instance().getManager().beginProgress(thread);
}

void Logger::endProgress() {
	Logger::instance().getManager().endProgress();
}

void Logger::endProgress(int thread) {
	Logger::instance().getManager().endProgress(thread);
}

void Logger::updateProgress(int completed, int total) {
	Logger::instance().getManager().updateProgress(completed, total);
}

void Logger::updateProgress(int thread, int completed, int total) {
	Logger::instance().getManager().updateProgress(thread, completed, total);
}

void Logger::busyProgress() {
	Logger::instance().getManager().busyProgress();
}

void Logger::busyProgress(int thread) {
	Logger::instance().getManager().busyProgress(thread);
}



_VGR_UTIL_END
_VGR_END



#pragma warning (pop)
