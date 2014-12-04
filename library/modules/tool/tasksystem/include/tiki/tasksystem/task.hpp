#pragma once
#ifndef TIKI_TASK_HPP_INCLUDED__
#define TIKI_TASK_HPP_INCLUDED__

namespace tiki
{
	struct TaskContext;

	typedef void (*TaskFunc)(const TaskContext& context);

	typedef uint32 TaskId;

	enum
	{
		InvalidTaskId = 0xffffffffu
	};

	struct Task
	{
		Task()
		{
			id				= InvalidTaskId;
			dependingTaskId	= InvalidTaskId;
			pFunc			= nullptr;
			pData			= nullptr;
		}

		Task(TaskId _id, TaskId _dependingTaskId, TaskFunc _pFunc, void* _pData)
		{
			id				= id;
			dependingTaskId	= _dependingTaskId;
			pFunc			= _pFunc;
			pData			= _pData;
		}

		TaskId		id;
		TaskId		dependingTaskId;
		TaskFunc	pFunc;
		void*		pData;
	};
}

#endif // TIKI_TASK_HPP_INCLUDED__
