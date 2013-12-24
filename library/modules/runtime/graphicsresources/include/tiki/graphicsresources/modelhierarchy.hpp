#pragma once
#ifndef TIKI_MODELHIERARCHY_HPP
#define TIKI_MODELHIERARCHY_HPP

#include "tiki/base/inline.hpp"
#include "tiki/base/simd.hpp"
#include "tiki/base/staticarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/math/matrix.hpp"

namespace tiki
{
	struct ModelHierarchyInitData;

	struct ModelHierarchy
	{
		friend class Model;

	public:
		
		enum
		{
			InvalidBoneIndex = 0xffff
		};

		TIKI_FORCE_INLINE uint16		getJointCount() const						{ return m_jointCount; }
		TIKI_FORCE_INLINE uint16		getAlignedJointCount() const				{ return m_alignedJointCount; }

		TIKI_FORCE_INLINE crc32			getJointNameByIndex( size_t index ) const	{ return m_jointNames[ index ]; }
		TIKI_FORCE_INLINE uint16		getParentByIndex( size_t index ) const		{ return m_parentIndices[ index ]; }

		TIKI_FORCE_INLINE const vf32*	getDefaultPoseOrientation() const			{ return m_defaultPose.getData(); }
		TIKI_FORCE_INLINE const vf32*	getDefaultPoseTranslation() const			{ return &m_defaultPose[ m_alignedJointCount ]; }
		TIKI_FORCE_INLINE const vf32*	getDefaultPoseScale() const					{ return &m_defaultPose[ 2 * m_alignedJointCount ]; }

		TIKI_FORCE_INLINE const Matrix&	getSkinToBoneMatrix( size_t index ) const	{ return m_skinToBoneMatrices[ index ]; }
		
	private:
		
		uint					m_jointCount;
		uint					m_alignedJointCount;

		StaticArray< crc32 >	m_jointNames;
		StaticArray< uint16 >	m_parentIndices;
		StaticArray< vf32 >		m_defaultPose;
		StaticArray< Matrix >	m_skinToBoneMatrices;

		bool					initialize( const ModelHierarchyInitData& initData );
		void					dispose();

	};
}

#endif // TIKI_MODELHIERARCHY_HPP
