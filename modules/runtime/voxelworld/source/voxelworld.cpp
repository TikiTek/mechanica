#include "tiki/voxelworld/voxelworld.hpp"

#include "tiki/tasksystem/task.hpp"
#include "tiki/tasksystem/tasksystem.hpp"

namespace tiki
{
	VoxelWorld::VoxelWorld()
	{
		m_pTaskSystem	= nullptr;
		m_pRootVoxel	= nullptr;
	}

	VoxelWorld::~VoxelWorld()
	{
		TIKI_ASSERT( m_pTaskSystem == nullptr );
		TIKI_ASSERT( m_pRootVoxel == nullptr );
	}

	bool VoxelWorld::create( const VoxelWorldParameters& parameters )
	{
		m_pTaskSystem = parameters.pTaskSystem;

		m_worldSize = parameters.worldSize;
		m_maxTreeDepth = parameters.maxTreeDepth;

		if( !m_voxels.create( parameters.poolGrow ) ||
			!m_commands.create( parameters.maxCommandCount ) )
		{
			dispose();
			return false;
		}

		m_pRootVoxel = m_voxels.push();
		if( m_pRootVoxel == nullptr )
		{
			dispose();
			return false;
		}

		m_pRootVoxel->boundingBox.createFromCenterExtends( Vector3::zero, m_worldSize );

		return true;
	}

	void VoxelWorld::dispose()
	{
		removeVoxelRecursive( m_pRootVoxel );
		m_pRootVoxel = nullptr;

		m_voxels.dispose();

		for( uint i = 0u; i < m_commands.getCount(); ++i )
		{
			m_commands[ i ].changes.dispose();
		}
		m_commands.dispose();

		m_pTaskSystem = nullptr;
	}

	const Voxel* VoxelWorld::findVoxelAt( const Vector3& position )
	{
		return findVoxelAt( position, m_pRootVoxel );
	}

	void VoxelWorld::replaceVoxelRecursive( Voxel* pOldVoxel, Voxel* pNewVoxel )
	{
		VoxelChilds childType = pOldVoxel->voxelChildType;
		Voxel* pParent = pOldVoxel;

		removeVoxelRecursive( pOldVoxel );

		if( pNewVoxel != nullptr )
		{
			pParent->children[ childType ] = pNewVoxel;

			pNewVoxel->pParent = pParent;
			pNewVoxel->voxelChildType = childType;
		}
	}

	void VoxelWorld::removeVoxelRecursive( Voxel* pVoxel )
	{
		Voxel* pParent = pVoxel->pParent;
		if( pParent != nullptr )
		{
			TIKI_ASSERT( pParent->childCount > 0u );

			pParent->children[ pVoxel->voxelChildType ] = nullptr;
			pParent->isFull = false;
			pParent->childCount--;
		}

		for( uint i = 0u; i < pVoxel->children.getCount(); ++i )
		{
			Voxel* pChild = pVoxel->children[ i ];

			if( pChild != nullptr )
			{
				removeVoxelRecursive( pChild );
			}
		}

		m_voxels.removeUnsortedByValue( *pVoxel );
	}

	void VoxelWorld::update()
	{
		for( uint i = 0u; i < m_commands.getCount(); ++i )
		{
			startCommandTask( m_commands[ i ] );
			finalizeCommand( m_commands[ i ] );
		}
		m_commands.clear();
	}

	bool VoxelWorld::queueCommand( const VoxelWorldTranformCommand& command )
	{
		if( m_commands.isFull() )
		{
			return false;
		}

		Command& newCommand	= m_commands.push();
		newCommand.status	= CommandStatus_Queued;
		newCommand.taskId	= InvalidTaskId;
		newCommand.command	= command;

		return true;
	}

	const Voxel* VoxelWorld::findVoxelAt( const Vector3& position, const Voxel* pCurrentVoxel )
	{
		TIKI_ASSERT( pCurrentVoxel != nullptr );

		if( pCurrentVoxel->isFull )
		{
			return pCurrentVoxel;
		}

		for( uint i = 0u; i < pCurrentVoxel->children.getCount(); ++i )
		{
			const Voxel* pChildVoxel = pCurrentVoxel->children[ i ];

			if( pChildVoxel && pChildVoxel->boundingBox.contains( position ) )
			{
				return findVoxelAt( position, pChildVoxel );
			}
		}

		return nullptr;
	}

	void VoxelWorld::startCommandTask( Command& command )
	{
		switch( command.command.type )
		{
		case VoxelWorldTransformTypes_Clear:
			executeClearCommand( command );
			break;

		case VoxelWorldTransformTypes_CutOutBox:
			executeCutOutBoxCommand( command );
			break;

		case VoxelWorldTransformTypes_CutOutSphere:
			executeCutOutSphereCommand( command );
			break;

		case VoxelWorldTransformTypes_FillBox:
			executeFillBoxCommand( command );
			break;

		case VoxelWorldTransformTypes_FillSphere:
			executeFillSphereCommand( command );
			break;

		default:
			TIKI_BREAK( "case unsupported" );
			break;
		}
	}

	void VoxelWorld::executeClearCommand( Command& command )
	{
		uint changeIndex = 0u;
		command.changes.create( m_pRootVoxel->childCount );

		for( uint i = 0u; i < m_pRootVoxel->children.getCount(); ++i )
		{
			Voxel* pVoxel = m_pRootVoxel->children[ i ];

			if( pVoxel != nullptr )
			{
				CommandChange change;
				change.pOldVoxel = pVoxel;
				change.pNewVoxel = nullptr;

				command.changes[ changeIndex ] = change;
				changeIndex++;
			}
		}
	}

	void VoxelWorld::executeCutOutBoxCommand( Command& command )
	{

	}

	void VoxelWorld::executeCutOutSphereCommand( Command& command )
	{

	}

	void VoxelWorld::executeFillBoxCommand( Command& command )
	{

	}

	void VoxelWorld::executeFillSphereCommand( Command& command )
	{

	}

	void VoxelWorld::finalizeCommand( Command& command )
	{
		for( uint i = 0u; i < command.changes.getCount(); ++i )
		{
			const CommandChange& change = command.changes[ i ];
			replaceVoxelRecursive( change.pOldVoxel, change.pNewVoxel );
		}
		command.changes.dispose();
	}
}