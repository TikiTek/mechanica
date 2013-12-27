
#include "tiki/modelexport/toolmodel.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	ToolModel::ToolModel() : m_scale( 1.0f ), m_material( "" )
	{

	}

	bool ToolModel::create( const string& fileName, float scale, const string& material /*= ""*/)
	{
		m_xml.create( fileName );

		m_scale		= scale;
		m_material	= material;

		//////////////////////////////////////////////////////////////////////////
		// Hierarchy
		const XmlElement* pHierarchies = m_xml.findNodeByName( "library_visual_scenes" );

		if ( pHierarchies != nullptr )
		{
			const XmlElement* pHierarchy = m_xml.findFirstChild( "visual_scene", pHierarchies );

			if ( pHierarchy != nullptr )
			{
				m_hierarchy.create( &m_xml, pHierarchy, m_scale );
			}
		}

		return true;
	}

	bool ToolModel::parseGeometies( bool calculateTangents )
	{
		//////////////////////////////////////////////////////////////////////////
		// Geometries
		bool isSkinned = false;
		const XmlElement* pGeometries = m_xml.findNodeByName( "library_geometries" );

		if ( !pGeometries )
		{
			TIKI_TRACE_ERROR( "ToolModel: library_geometries node not found" );
			return false;
		}

		List< const XmlElement* > geometires;

		const XmlElement* pGeometrieNode = m_xml.findFirstChild( "geometry", pGeometries );
		while ( pGeometrieNode )
		{
			geometires.add( pGeometrieNode );
			pGeometrieNode = m_xml.findNext( "geometry", pGeometrieNode );
		}

		m_geometries.create( geometires.getCount() );

		for (size_t i = 0u; i < m_geometries.getCount(); ++i)
		{
			m_geometries[ i ].create( &m_xml, geometires[ i ], m_scale );

			if ( calculateTangents )
			{
				if ( m_geometries[ i ].calculateTangents() == false )
				{
					TIKI_TRACE_ERROR( "[modelconverter] can't calculate tangents because this geometry has no normals.\n" );
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
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

					const string id = string( pIdAtt->content ).substring( 1u );

					ToolModelGeometrie* pGeometry = nullptr;
					for (size_t i = 0u; i < m_geometries.getCount(); ++i)
					{
						if ( m_geometries[ i ].getDesc().id == id )
						{
							pGeometry = &m_geometries[ i ];
							break;
						}
					}

					if ( pGeometry != nullptr )
					{
						isSkinned = true;
						pGeometry->applySkinning( m_hierarchy, &m_xml, pSkinNode );
					}
					else
					{
						TIKI_TRACE_WARNING( "can't find geometry with name: %s\n", id.cStr() );
					}
				}

				pController = m_xml.findNext( pController );
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
