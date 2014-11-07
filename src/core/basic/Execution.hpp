#pragma once
#ifndef _VGR_CORE_BASIC_EXECUTION_HPP
#define _VGR_CORE_BASIC_EXECUTION_HPP

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QVector>
#include <common/global.hpp>



class Execution;
class ExecutionThread;
class ThreadManager;



class Execution {

public:
	virtual bool runTask(int task) = 0;
	virtual bool preTask() { return (true); }
	virtual bool postTask() { return (true); }
};



class ExecutionThread : public QThread {

private:
	int									m_serial;
	Execution							*m_executor;
	std::vector<int>					m_tasks;

public:
	ExecutionThread(int serial = 0) : QThread(), m_serial(serial), m_executor(NULL), m_tasks() { }

public:
	int serial() const { return (this->m_serial); }
	const Execution *executor() const { return (this->m_executor); }
	Execution *executor() { return (this->m_executor); }
	void setExecution(Execution *value) { this->m_executor = value; }

public:
	void clearTasks() { this->m_tasks.clear(); }
	void addTask(int task) { this->m_tasks.push_back(task); }
	size_t tasksCount() const { return (this->m_tasks.size()); }

protected:
	virtual void run();
};



class ThreadManager {

private:
	QVector<ExecutionThread *>			m_threads;

public:
	static ThreadManager &instance();

private:
	ThreadManager();
	~ThreadManager();

public:
	int threadsCount() const { return (this->m_threads.size()); }
	const ExecutionThread &thread(int index) const { return (*this->m_threads[index]); }
	ExecutionThread &thread(int index) { return (*this->m_threads[index]); }
	void assignTasks(int tasksCount);
	void start();
	void exec();
};



#endif /* _VGR_CORE_BASIC_EXECUTION_HPP */
