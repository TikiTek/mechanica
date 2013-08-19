#ifndef TIKI_MODEL_HPP__
#define TIKI_MODEL_HPP__

#include "tiki/resource/resource.hpp"

#include "tiki/base/array.hpp"
#include "tiki/graphics/modelhierarchy.hpp"
#include "tiki/graphics/modelgeometry.hpp"

namespace tiki
{
	class GraphicsSystem;
	class ModelFactory;

	class Model : public Resource
	{
		TIKI_DEFINE_RESOURCE( ModelFactory, TIKI_FOURCC( 'M', 'O', 'D', 'L' ) );

	public:

											Model();
		virtual								~Model();

		const Material*						getMaterial() const { return m_pMaterial; }
		const ModelHierarchy*				getHierarchy() const { return m_pHierarchy; }

		void								setMaterial( Material* pMaterial );

		const ModelGeometry&				getGeometryByIndex( size_t index ) const { return m_geometries[ index ]; }
		size_t								getGeometryCount() const { return m_geometries.getCount(); }

	private:

		const Material*						m_pMaterial;
		ModelHierarchy*						m_pHierarchy;

		Array< ModelGeometry >				m_geometries;

		bool								initialize( GraphicsSystem* pGraphicsSystem, const void* pData );
		void								dispose();
		
	};
}

#endif // TIKI_MODEL_HPP__
