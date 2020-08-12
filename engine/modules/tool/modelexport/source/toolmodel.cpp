
#include "tiki/modelexport/toolmodel.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	ToolModel::ToolModel()
		: m_scale( 0.0f )
	{

	}

	bool ToolModel::create( const string& fileName, float scale )
	{
		m_xml.create( fileName.cStr() );

		m_scale		= scale;

		const XmlElement* pGeometries = m_xml.findNodeByName( "library_geometries" );
		if ( !pGeometries )
		{
			TIKI_TRACE_ERROR( "ToolModel: geometries node not found.\n" );
			return false;
		}

		const XmlElement* pHierarchies = m_xml.findNodeByName( "library_visual_scenes" );
		const XmlElement* pHierarchy = nullptr;
		if ( pHierarchies != nullptr )
		{
			pHierarchy = m_xml.findFirstChild( "visual_scene", pHierarchies );
		}

		m_hierarchy.create( &m_xml, pHierarchy, pGeometries, m_scale );

		return true;
	}

	bool ToolModel::parseGeometies( bool calculateTangents )
	{
		// Geometries
		bool isSkinned = false;
		
		for (size_t i = 0u; i < m_hierarchy.getGeometryInstanceCount(); ++i)
		{
			ToolModelGeometrie& geometry = m_geometries.add();
			geometry.create(
				&m_xml,
				m_hierarchy.getGeometryInstanceByIndex( i ),
				m_scale
			);
		}

		// Skinning
		const XmlElement* pControllers = m_xml.findNodeByName( "library_controllers" );

		if ( pControllers != nullptr && pControllers->elements != nullptr )
		{
			const XmlElement* pController = m_xml.findFirstChild( "controller", pControllers );

			while ( pController )
			{
				const XmlElement* pSkinNode = m_xml.findFirstChild( "skin", pController );

				if ( pSkinNode )
				{
					const XmlAttribute* pIdAtt = m_xml.findAttributeByName( "source", pSkinNode );
					TIKI_ASSERT( pIdAtt );

					const string id = string( pIdAtt->content ).subString( 1u );

					bool found = false;
					for (size_t i = 0u; i < m_geometries.getCount(); ++i)
					{
						if ( m_geometries[ i ].getDesc().id == id )
						{
							found = true;
							isSkinned = true;
							m_geometries[ i ].applySkinning( m_hierarchy, &m_xml, pSkinNode );
						}
					}

					if ( found == false )
					{
						TIKI_TRACE_WARNING( "can't find geometry with name: %s\n", id.cStr() );
					}
				}

				pController = m_xml.findNext( pController );
			}
		}

		// transform
		for (uint i = 0u; i < m_geometries.getCount(); ++i)
		{
			m_geometries[ i ].transformToInstance();

			if ( calculateTangents )
			{
				if ( !m_geometries[ i ].calculateTangents() )
				{
					TIKI_TRACE_ERROR( "[modelconverter] can't calculate tangents because this geometry has no normals.\n" );
				}
			}
		} 

		if ( isSkinned == false && m_hierarchy.isCreated() )
		{
			m_hierarchy.dispose();
		}

		return true;
	}

	bool ToolModel::parseAnimations()
	{
		//////////////////////////////////////////////////////////////////////////
		// Animations
		const XmlElement* pAnimations = m_xml.findNodeByName( "library_animations" );

		if ( pAnimations != nullptr )
		{
			m_animation.create( m_hierarchy, &m_xml, pAnimations );

			return true;
		}

		return false;
	}

	void ToolModel::dispose()
	{
		if ( m_geometries.getCount() > 0u )
		{
			for (size_t i = 0u; i < m_geometries.getCount(); ++i)
			{
				m_geometries[ i ].dispose();
			}
			m_geometries.dispose();
		}

		if ( m_animation.isCreated() )
		{
			m_animation.dispose();
		}

		if ( m_hierarchy.isCreated() )
		{
			m_hierarchy.dispose();
		}

		m_xml.dispose();
	}

}
