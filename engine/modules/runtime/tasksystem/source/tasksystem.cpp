
#include "tiki/tasksystem/tasksystem.hpp"

#include "tiki/base/basicstring.hpp"
#include "tiki/tasksystem/taskcontext.hpp"

namespace tiki
{
	TaskSystem::TaskSystem()
	{
		m_nextTaskId = 0u;
	}

	TaskSystem::~TaskSystem()
	{
	}

	bool TaskSystem::create( const TaskSystemParameters& parameters )
	{
		m_nextTaskId = 0u;

		m_globalMutex.create();
		
		if ( !m_taskCountSemaphore.create() )
		{
			dispose();
			return false;
		}

		if ( !m_tasks.create( parameters.maxTaskCount ) )
		{
			dispose();
			return false;
		}

		if ( !m_threads.create( parameters.threadCount ) )
		{
			dispose();
			return false;
		}

		for (uint i = 0u; i < m_threads.getCount(); ++i)
		{
			ThreadContext& context = m_threads[ i ];
			context.pTaskSystem	= this;
			context.workingEvent.create();

			const string threadName = formatString( "TaskSystem_%u", i );
			if ( !context.thread.create( staticThreadEntryPoint, &context, parameters.threadStackSize, threadName.cStr() ) )
			{
				dispose();
				return false;
			}
		}

		return true;
	}

	void TaskSystem::dispose()
	{
		for (uint i = 0u; i < m_threads.getCount(); ++i)
		{
			m_threads[ i ].thread.requestExit();
		}

		for (uint i = 0u; i < m_threads.getCount(); ++i)
		{
			ThreadContext& context = m_threads[ i ];

			context.thread.waitForExit();
			context.thread.dispose();

			context.workingEvent.dispose();
		}

		m_threads.dispose();
		m_tasks.dispose();	

		m_taskCountSemaphore.dispose();
		m_globalMutex.dispose();
	}

	TaskId TaskSystem::queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId /* = InvalidTaskId */ )
	{
		Task task(
			m_nextTaskId++,
			dependingTaskId,
			pFunc,
			pData
		);

		m_globalMutex.lock();
		m_tasks.push( task );
		m_taskCountSemaphore.incement();
		m_globalMutex.unlock();

		return task.id;
	}

	void TaskSystem::waitForTask( TaskId taskId )
	{
		const Thread& thread = Thread::getCurrentThread();

		Task task;
		do
		{
			task = Task();

			if ( threadDispatchTask( task ) )
			{
				threadExecuteTask( thread, task );
			}
		}
		while ( task.id <= taskId );
	}
	
	void TaskSystem::waitForAllTasks()
	{
		const Thread& thread = Thread::getCurrentThread();

		Task task;
		do
		{
			task = Task();

			if ( threadDispatchTask( task ) )
			{
				threadExecuteTask( thread, task );
			}
		}
		while ( task.id != InvalidTaskId );

		for (uint i = 0u; i < m_threads.getCount(); ++i)
		{
			m_threads[ i ].workingEvent.waitForSignal();
		}
	}

	/*static*/ int TaskSystem::staticThreadEntryPoint( const Thread& thread )
	{
		void* pArgument = thread.getArgument();
		if ( pArgument == nullptr )
		{
			return -1;
		}

		ThreadContext& context = *static_cast< ThreadContext* >( pArgument );
		TIKI_ASSERT( &context.thread == &thread );

		context.pTaskSystem->threadEntryPoint( thread, context );

		return 0;
	}

	void TaskSystem::threadEntryPoint( const Thread& thread, ThreadContext& context )
	{
		Task task;
		while ( !thread.isExitRequested() )
		{
			context.workingEvent.reset();

			if ( threadDispatchTask( task ) )
			{
				threadExecuteTask( thread, task );
			}

			context.workingEvent.signal();
		}
	}

	void TaskSystem::threadExecuteTask( const Thread& thread, const Task& task )
	{
		if ( task.dependingTaskId != InvalidTaskId )
		{
			waitForTask( task.dependingTaskId );
		}

		TaskContext context( thread, task.pData );
		task.pFunc( context );
	}

	bool TaskSystem::threadDispatchTask( Task& targetTask )
	{
		if ( m_taskCountSemaphore.tryDecrement( 10u ) )
		{
			bool result = false;

			m_globalMutex.lock();
			TIKI_VERIFY( result = m_tasks.pop( targetTask ) );
			m_globalMutex.unlock();

			return result;
		}

		return false;
	}
}