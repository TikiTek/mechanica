#pragma once
#ifndef TIKI_TOOLMODELANIMATION_HPP
#define TIKI_TOOLMODELANIMATION_HPP

#include "tiki/container/array.hpp"
#include "tiki/math/matrix.hpp"

struct _XmlElement;

namespace tiki
{
	class XmlReader;
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

		void								create( ToolModelHierarchy& hierarchy, const XmlReader* pXml, const _XmlElement* pNode );
		void								dispose();

		bool								isCreated() const { return m_isCreated; }

		uint								getFrameCount() const { return m_frameCount; }
		
		uint								getJointCount() const { return m_joints.getCount(); }
		const ToolModelAnimationJoint&		getJointByIndex( uint index ) const { return m_joints[ index ]; }

	private:

		bool								m_isCreated;

		uint								m_frameCount;

		Array< ToolModelAnimationJoint >	m_joints;

		uint								createJoint( const XmlReader* pXml, const _XmlElement* pNode, const ToolModelHierarchy& hierarchy );

	};
}

#endif // TIKI_TOOLMODELANIMATION_HPP
