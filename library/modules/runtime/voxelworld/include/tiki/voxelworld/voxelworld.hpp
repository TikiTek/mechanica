#pragma once
#ifndef TIKI_VOXELWORLD_HPP_INCLUDED
#define TIKI_VOXELWORLD_HPP_INCLUDED

#include "tiki/container/chunkedpool.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/threading/mutex.hpp"
#include "tiki/voxelworld/voxeltypes.hpp"

namespace tiki
{
	class TaskSystem;

	struct VoxelWorldParameters
	{
		Vector3		worldDimentions;
		uint		maxTreeDepth;

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

		const Voxel*			getRootVoxel() const { return m_pRootVoxel; }
		const Voxel*			findVoxelAt( const Vector3& position );

		void					update();

	private:

		ChunkedPool< Voxel >	m_voxels;

		Voxel*					m_pRootVoxel;

	};
}

#endif // TIKI_VOXELWORLD_HPP_INCLUDED
