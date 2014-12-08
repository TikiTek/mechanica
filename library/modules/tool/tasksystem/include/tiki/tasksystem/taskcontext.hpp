#pragma once
#ifndef TIKI_TASKCONTEXT_HPP_INCLUDED__
#define TIKI_TASKCONTEXT_HPP_INCLUDED__

namespace tiki
{
	struct TaskContext
	{
		TaskContext( void* _pTaskData )
		{
			pTaskData = _pTaskData;
		}

		void*			pTaskData;
	};
}

#endif // TIKI_TASKCONTEXT_HPP_INCLUDED__
