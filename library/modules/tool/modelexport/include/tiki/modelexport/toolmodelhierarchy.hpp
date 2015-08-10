#pragma once
#ifndef TIKI_TOOLMODELHIERARCHY_HPP
#define TIKI_TOOLMODELHIERARCHY_HPP

#include "tiki/base/array.hpp"
#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/math/matrix.hpp"

struct _XmlElement;

namespace tiki
{
	class XmlReader;
	template<typename T>
	class List;

	struct ToolModelJoint
	{
		const _XmlElement*	pNode;

		uint32				index;
		string				name;
		crc32				crc;

		bool				used;
		uint32				finalIndex;
		uint32				finalParentIndex;

		const _XmlElement*	pParentNode;
		uint32				parentIndex;

		Matrix44			defaultPose;
		Matrix44			skinToBone;
	};

	struct ToolModelGeometryInstance
	{
		const _XmlElement*	pNode;
		string				geometryId;

		Matrix44			worldTransform;
	};

	class ToolModelHierarchy
	{
	public:

		ToolModelHierarchy();

		void								create( const XmlReader* pXml, const _XmlElement* pHierarchyNode, const _XmlElement* pGeometriesNode, float scale );
		void								dispose();

		bool								isCreated() const { return m_pXml != nullptr; }

		const Array< ToolModelJoint >&		getJointData() const						{ return m_joints; }
		uint								getJointCount() const						{ return m_joints.getCount(); }
		const ToolModelJoint&				getJointByIndex( uint index ) const		{ return m_joints[ index ]; }
		const ToolModelJoint*				getJointByName( const string& name ) const;

		const ToolModelGeometryInstance&	getGeometryInstanceByIndex( uint index ) const	{ return m_instances[ index ]; }
		uint								getGeometryInstanceCount() const				{ return m_instances.getCount(); }

		void								markJointAsUsed( const ToolModelJoint& joint );
		void								setBindMatrix( const ToolModelJoint& joint, const Matrix44& matrix );

		void								calculateFinalIndices();

	private:

		const XmlReader*						m_pXml;

		Array< ToolModelJoint >				m_joints;
		Array< ToolModelGeometryInstance >	m_instances;

		uint								m_finalJointCount;
		bool								m_hasFinalIndices;

		void								searchNodes( List< ToolModelJoint >& targetList, const _XmlElement* pNode );

	};
}

#endif // TIKI_TOOLMODELHIERARCHY_HPP
