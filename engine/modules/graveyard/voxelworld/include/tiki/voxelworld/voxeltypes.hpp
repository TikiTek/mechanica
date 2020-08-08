#pragma once
#ifndef TIKI_VOXELTYPES_HPP_INCLUDED
#define TIKI_VOXELTYPES_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/fixedarray.hpp"
#include "tiki/math/axisalignedbox.hpp"
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

		VoxelChilds_Count,
		VoxelChilds_Invalid = 0xffu
	};

	struct Voxel
	{
		Voxel()
		{
			pParent = nullptr;

			for( uint i = 0u; i < children.getCount(); ++i )
			{
				children[ i ] = nullptr;
			}

			isFull			= false;
			childCount		= 0u;
			voxelType		= 0u;
			voxelChildType	= VoxelChilds_Invalid;
		}

		typedef FixedArray< Voxel*, VoxelChilds_Count > ChildrenArray;

		Voxel*			pParent;
		ChildrenArray	children;

		bool			isFull;
		uint8			childCount;
		uint8			voxelType;
		VoxelChilds		voxelChildType;

		AxisAlignedBox	boundingBox;
	};

	enum VoxelWorldTransformTypes
	{
		VoxelWorldTransformTypes_Clear,
		VoxelWorldTransformTypes_CutOutBox,
		VoxelWorldTransformTypes_CutOutSphere,
		VoxelWorldTransformTypes_FillBox,
		VoxelWorldTransformTypes_FillSphere,

		VoxelWorldTransformTypes_Count
	};

	union VoxelWorldTranformCommandData
	{
		VoxelWorldTranformCommandData()
		{
		}

		Box		box;
		Sphere	sphere;
	};

	struct VoxelWorldTranformCommand
	{

		VoxelWorldTransformTypes		type;
		VoxelWorldTranformCommandData	data;
	};
}

#endif // TIKI_VOXELTYPES_HPP_INCLUDED
