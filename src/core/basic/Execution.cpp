#include "Execution.hpp"
#include <common/util/logger.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



void ExecutionThread::run() {
	if (!this->m_executor) return;

	_VGR_THREAD_BEG_PROG(this->m_serial);
	_VGR_THREAD_BSY_PROG(this->m_serial);
	if (!this->m_executor->preTask()) return;
	for (size_t i = 0; i < this->m_tasks.size(); ++i) {
		_VGR_THREAD_UPD_PROG(this->m_serial, i, this->m_tasks.size());
		if (!this->m_executor->runTask(this->m_tasks[i])) break;
	}
	_VGR_THREAD_BSY_PROG(this->m_serial);
	this->m_executor->postTask();
	_VGR_THREAD_END_PROG(this->m_serial);
}



ThreadManager::ThreadManager() {
	this->m_threads.resize(std::max<int>(QThread::idealThreadCount() - 4, 2));
	for (int i = 0; i < this->m_threads.size(); ++i) this->m_threads[i] = new ExecutionThread(i);
}

ThreadManager::~ThreadManager() {
	for (int i = 0; i < this->m_threads.size(); ++i) delete this->m_threads[i];
	this->m_threads.clear();
}

ThreadManager &ThreadManager::instance() {
	static ThreadManager Instance;
	return (Instance);
}

void ThreadManager::assignTasks(int tasksCount) {
	for (int i = 0; i < this->m_threads.size(); ++i) this->m_threads[i]->clearTasks();
	for (int i = 0; i < tasksCount; ++i) {
		this->m_threads[i % this->m_threads.size()]->addTask(i);
	}
}

void ThreadManager::start() {
	for (int i = 0; i < this->m_threads.size(); ++i) this->m_threads[i]->start();
}

void ThreadManager::exec() {
	this->start();
	while (true) {
		bool result = true;
		for (int i = 0; i < this->m_threads.size(); ++i) if (!(result &= this->m_threads[i]->isFinished())) break;
		if (result) break;
	}
}



#pragma warning (pop)
