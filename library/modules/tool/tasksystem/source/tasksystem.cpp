
#include "tiki/tasksystem/tasksystem.hpp"

#include "tiki/base/string.hpp"
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
		m_taskCountSemaphore.create();

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
			const string threadName = formatString( "TaskSystem_%u", i );
			m_threads[ i ].create( staticThreadEntryPoint, parameters.threadStackSize, threadName.cStr() );
			m_threads[ i ].start( this );
		}

		return true;
	}

	void TaskSystem::dispose()
	{
		for (uint i = 0u; i < m_threads.getCount(); ++i)
		{
			m_threads[ i ].requestExit();
		}

		for (uint i = 0u; i < m_threads.getCount(); ++i)
		{
			m_threads[ i ].waitForExit();
			m_threads[ i ].dispose();
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
		const Thread& thread = *(const Thread*)nullptr;

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

	/*static*/ int TaskSystem::staticThreadEntryPoint( const Thread& thread )
	{
		TaskSystem* pTaskSystem = static_cast< TaskSystem* >( thread.getArgument() );
		if ( pTaskSystem != nullptr )
		{
			pTaskSystem->threadEntryPoint( thread );
		}

		return 0;
	}

	void TaskSystem::threadEntryPoint( const Thread& thread )
	{
		Task task;
		while ( !thread.isExitRequested() )
		{
			if ( threadDispatchTask( task ) )
			{
				threadExecuteTask( thread, task );
			}
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
		if ( m_taskCountSemaphore.tryDecrement( 1u, 10u ) )
		{
			bool result = false;

			m_globalMutex.lock();
			result = m_tasks.pop( targetTask );
			m_globalMutex.unlock();

			return result;
		}

		return false;
	}
}