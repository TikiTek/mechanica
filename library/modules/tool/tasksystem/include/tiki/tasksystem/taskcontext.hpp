#pragma once
#ifndef TIKI_TASKCONTEXT_HPP_INCLUDED__
#define TIKI_TASKCONTEXT_HPP_INCLUDED__

namespace tiki
{
	class Thread;

	struct TaskContext
	{
		TaskContext(const Thread& _thread, void* _pTaskData )
			: thread( _thread )
			, pTaskData( _pTaskData )
		{
		}

		const Thread&	thread;
		void*			pTaskData;
	};
}

#endif // TIKI_TASKCONTEXT_HPP_INCLUDED__
