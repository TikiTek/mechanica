#pragma once
#ifndef TIKI_VOXELMESHBUILDER_HPP_INCLUDED
#define TIKI_VOXELMESHBUILDER_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/graphics/color.hpp"

#include "base.hpp"

namespace tiki
{
	struct Voxel;

	struct VoxelVertex
	{
		float3	position;
		float3	normal;
		float4	tangent;
		float2	texCoord;
		float4	material;
		Color	color;
	};

	struct VoxelMesh
	{
		const VoxelVertex*	pVertices;
		uint				vertexCount;

		const uint16*		pIndicies;
		uint				indexCount;
	};

	TIKI_DEFINE_HANLE( VoxelMeshHandle );

	class VoxelMeshBuilder
	{
		TIKI_NONCOPYABLE_CLASS( VoxelMeshBuilder );

	public:

		bool				create();
		void				dispose();

		VoxelMeshHandle		startGenerateMesh( const Voxel& voxel );

		const VoxelMesh*	getMesh( VoxelMeshHandle handle ) const;

		void				disposeMesh( VoxelMeshHandle handle );

	private:



	};
}

#endif // TIKI_VOXELMESHBUILDER_HPP_INCLUDED
