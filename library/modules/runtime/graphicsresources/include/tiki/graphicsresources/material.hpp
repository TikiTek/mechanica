#ifndef TIKI_MATERIAL_HPP__
#define TIKI_MATERIAL_HPP__

#include "tiki/resource/resource.hpp"

namespace tiki
{
	class Material : public Resource
	{
		friend class GraphicsContext;
		TIKI_DEFINE_RESOURCE( Material, TIKI_FOURCC( 'M', 'A', 'T', 'E' ) );
		TIKI_NONCOPYABLE_CLASS( Material );

	public:

		static void			registerResourceType( ResourceManager& resourceManager );
		static void			unregisterResourceType( ResourceManager& resourceManager );

		//const Shader*		getVertexShader() const { return m_pVertexShader; }

	protected:

		virtual bool		createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext );
		virtual void		disposeInternal( const FactoryContext& factoryContext );

	private:

							Material();
		virtual				~Material();

	};
}

#endif // TIKI_MATERIAL_HPP__
