#pragma once

#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"
#include "tiki/container/list.hpp"
#include "tiki/io/xml_reader.hpp"
#include "tiki/model_export/tool_model_animation.hpp"
#include "tiki/model_export/tool_model_geometrie.hpp"
#include "tiki/model_export/tool_model_hierarchy.hpp"

namespace tiki
{
	class ToolModel
	{
	public:
										ToolModel();

		bool							create( const string& fileName, float scale );
		void							dispose();

		bool							parseGeometies( bool calculateTangents );
		bool							parseAnimations();

		const ToolModelHierarchy&		getHierarchy() const { return m_hierarchy; }
		const ToolModelAnimation&		getAnimation() const { return m_animation; }

		const ToolModelGeometrie&		getGeometryByIndex( size_t index ) const { return m_geometries[ index ]; }
		size_t							getGeometyCount() const { return m_geometries.getCount(); }

		TIKI_INLINE float				getScale() const { return m_scale; }

	private:

		XmlReader						m_xml;

		ToolModelHierarchy				m_hierarchy;
		ToolModelAnimation				m_animation;
		List< ToolModelGeometrie >		m_geometries;

		float							m_scale;
	};
}
