#pragma once

#include "tiki/base/platform.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"
#include "tiki/container/queue.hpp"
#include "tiki/task_system/task.hpp"
#include "tiki/threading/event.hpp"
#include "tiki/threading/mutex.hpp"
#include "tiki/threading/semaphore.hpp"
#include "tiki/threading/thread.hpp"

namespace tiki
{
	struct Task;
	class TaskQueue;

	struct TaskSystemParameters
	{
		TaskSystemParameters()
		{
			maxTaskCount	= 128u;

			threadCount		= platform::getProcessorCount();
			threadStackSize	= 1u * 1024u * 1024u;
		}

		uint	maxTaskCount;

		uint	threadCount;
		uint	threadStackSize;
	};

	class TaskSystem
	{
		TIKI_NONCOPYABLE_CLASS( TaskSystem );

	public:

		TaskSystem();
		~TaskSystem();

		bool	create( const TaskSystemParameters& parameters );
		void	dispose();

		TaskId	queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId = InvalidTaskId );
		void	waitForTask( TaskId taskId );
		void	waitForAllTasks();

	private:

		struct ThreadContext
		{
			TaskSystem*	pTaskSystem;

			Thread		thread;
			Event		workingEvent;
		};

		Mutex					m_globalMutex;
		Semaphore				m_taskCountSemaphore;

		TaskId					m_nextTaskId;

		Array< ThreadContext >	m_threads;
		Queue< Task >			m_tasks;

		static int				staticThreadEntryPoint( const Thread& thread );
		void					threadEntryPoint( const Thread& thread, ThreadContext& context );
		void					threadExecuteTask( const Thread& thread, const Task& task );
		bool					threadDispatchTask( Task& targetTask );
	};
}
