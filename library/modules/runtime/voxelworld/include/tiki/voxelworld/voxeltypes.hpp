#pragma once
#ifndef TIKI_VOXELTYPES_HPP_INCLUDED
#define TIKI_VOXELTYPES_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/math/box.hpp"
#include "tiki/math/sphere.hpp"

namespace tiki
{
	enum VoxelChilds : uint8
	{
		VoxelChilds_TopLeftFront,
		VoxelChilds_TopRightFront,
		VoxelChilds_TopLeftBack,
		VoxelChilds_TopRightBack,
		VoxelChilds_BottomLeftFront,
		VoxelChilds_BottomRightFront,
		VoxelChilds_BottomLeftBack,
		VoxelChilds_BottomRightBack,

		VoxelChilds_Count
	};

	struct Voxel
	{
		Voxel()
		{
			pParent = nullptr;

			for( uint i = 0u; i < TIKI_COUNT( apChilds ); ++i )
			{
				apChilds[ i ] = nullptr;
			}

			isFull		= true;
			voxelType	= 0u;
		}

		Voxel*	pParent;
		Voxel*	apChilds[ VoxelChilds_Count ];

		bool	isFull;
		uint8	voxelType;
	};

	enum VoxelWorldTransformTypes
	{
		VoxelWorldTransformTypes_CutOutBox,
		VoxelWorldTransformTypes_CutOutSphere,
		VoxelWorldTransformTypes_FillBox,
		VoxelWorldTransformTypes_FillSphere,

		VoxelWorldTransformTypes_Count
	};

	struct VoxelWorldTranformCommand
	{
		VoxelWorldTransformTypes	type;
		union 
		{
			Box						box;
			Sphere					sphere;
		} data;
	};
}

#endif // TIKI_VOXELTYPES_HPP_INCLUDED
