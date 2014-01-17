#pragma once
#ifndef TIKI_TOOLMODELANIMATION_HPP
#define TIKI_TOOLMODELANIMATION_HPP

#include "tiki/base/array.hpp"
#include "tiki/math/matrix.hpp"

struct _XmlElement;

namespace tiki
{
	class TikiXml;
	class ToolModelHierarchy;
	struct ToolModelJoint;

	struct ToolModelKey
	{
		float		time;
		Matrix44	transform;
	};

	struct ToolModelAnimationJoint
	{
		const ToolModelJoint*	pJoint;

		bool					used;

		Array< ToolModelKey >	keys;
		Array< Matrix44 >		samples;
	};

	class ToolModelAnimation
	{
	public:

		void								create( ToolModelHierarchy& hierarchy, const TikiXml* pXml, const _XmlElement* pNode );
		void								dispose();

		bool								isCreated() const { return m_isCreated; }

		uint								getFrameCount() const { return m_frameCount; }
		
		size_t								getJointCount() const { return m_joints.getCount(); }
		const ToolModelAnimationJoint&		getJointByIndex( size_t index ) const { return m_joints[ index ]; }

	private:

		bool								m_isCreated;

		uint								m_frameCount;

		Array< ToolModelAnimationJoint >	m_joints;

		size_t								createJoint( const TikiXml* pXml, const _XmlElement* pNode, const ToolModelHierarchy& hierarchy );

	};
}

#endif // TIKI_TOOLMODELANIMATION_HPP
