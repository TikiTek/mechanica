#pragma once
#ifndef TIKI_VOXELWORLD_HPP_INCLUDED
#define TIKI_VOXELWORLD_HPP_INCLUDED

#include "tiki/container/array.hpp"
#include "tiki/container/chunkedpool.hpp"
#include "tiki/container/sizedarray.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/threading/mutex.hpp"
#include "tiki/voxelworld/voxeltypes.hpp"

namespace tiki
{
	class TaskSystem;
	typedef uint32 TaskId;

	struct VoxelWorldParameters
	{
		VoxelWorldParameters()
		{
			vector::clear( worldSize );

			maxTreeDepth	= 0u;
			poolGrow		= 1024u;
			maxCommandCount = 16u;

			pTaskSystem		= nullptr;
		}

		Vector3		worldSize;
		uint		maxTreeDepth;
		uint		maxCommandCount;
		uint		poolGrow;

		TaskSystem*	pTaskSystem;
	};

	class VoxelWorld
	{
		TIKI_NONCOPYABLE_CLASS( VoxelWorld );

	public:

								VoxelWorld();
								~VoxelWorld();

		bool					create( const VoxelWorldParameters& parameters );
		void					dispose();

		const Vector3&			getWorldSize() const { return m_worldSize; }

		const Voxel*			getRootVoxel() const { return m_pRootVoxel; }
		const Voxel*			findVoxelAt( const Vector3& position );

		void					update();

		bool					queueCommand( const VoxelWorldTranformCommand& command );

	private:

		enum CommandStatus
		{
			CommandStatus_Queued,
			CommandStatus_Running,
			CommandStatus_Finish,
			CommandStatus_Failed
		};

		struct CommandChange
		{
			Voxel*		pOldVoxel;
			Voxel*		pNewVoxel;
		};

		struct Command
		{
			CommandStatus				status;
			TaskId						taskId;

			VoxelWorldTranformCommand	command;

			Array< CommandChange >		changes;
		};

		TaskSystem*						m_pTaskSystem;

		Vector3							m_worldSize;
		uint							m_maxTreeDepth;

		ChunkedPool< Voxel >			m_voxels;
		Voxel*							m_pRootVoxel;

		SizedArray< Command >			m_commands;

		const Voxel*					findVoxelAt( const Vector3& position, const Voxel* pCurrentVoxel );

		void							replaceVoxelRecursive( Voxel* pOldVoxel, Voxel* pNewVoxel );
		void							removeVoxelRecursive( Voxel* pVoxel );	

		void							startCommandTask( Command& command );
		void							executeClearCommand( Command& command );
		void							executeCutOutBoxCommand( Command& command );
		void							executeCutOutSphereCommand( Command& command );
		void							executeFillBoxCommand( Command& command );
		void							executeFillSphereCommand( Command& command );

		void							finalizeCommand( Command& command );

		void							fillVoxelForSphere( Voxel& voxel, Command& command, uint depth );

		void							calculateBoundingBoxForVoxelChild( AxisAlignedBox& box, const Voxel& voxel, VoxelChilds child, uint depth );
	};
}

#endif // TIKI_VOXELWORLD_HPP_INCLUDED
