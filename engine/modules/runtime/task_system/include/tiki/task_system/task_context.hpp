#pragma once

namespace tiki
{
	class Thread;

	struct TaskContext
	{
		TaskContext( const Thread& _thread, void* _pTaskData )
			: thread( _thread )
		{
			pTaskData = _pTaskData;
		}

		const Thread&	thread;
		void*			pTaskData;
	};
}
