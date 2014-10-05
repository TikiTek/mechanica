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
	class ResourceManager;

	class Model : public Resource
	{
		TIKI_DEFINE_RESOURCE( Model, TIKI_FOURCC( 'M', 'O', 'D', 'L' ) );

	public:

		static void							registerResourceType( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem );
		static void							unregisterResourceType( ResourceManager& resourceManager );

		const Material*						getMaterial() const { return m_pMaterial; }
		const ModelHierarchy*				getHierarchy() const { return m_pHierarchy; }

		void								setMaterial( Material* pMaterial );

		const ModelGeometry&				getGeometryByIndex( size_t index ) const { return m_geometries[ index ]; }
		size_t								getGeometryCount() const { return m_geometries.getCount(); }

	protected:

		virtual bool						createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext );
		virtual void						disposeInternal( const FactoryContext& factoryContext );

	private:

											Model();
		virtual								~Model();

		const Material*						m_pMaterial;
		ModelHierarchy*						m_pHierarchy;

		Array< ModelGeometry >				m_geometries;

	};
}

#endif // TIKI_MODEL_HPP__
