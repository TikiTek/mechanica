#ifndef TIKI_VERTEXFORMAT_HPP
#define TIKI_VERTEXFORMAT_HPP

#include "tiki/base/types.hpp"

#include "tiki/graphicsbase/vertexattribute.hpp"

#include "graphicstypes.hpp"

namespace tiki
{
	class Shader;

	struct VertexFormatParameters
	{
		const VertexAttribute*	pAttributes;
		size_t					attributeCount;

		const Shader*			pShader;
	};

	class VertexFormat
	{
		TIKI_NONCOPYABLE_CLASS( VertexFormat );
		friend class GraphicsContext;
		friend struct VertexFormatInfo;

	public:

		static void					initializeSystem();
		static void					disposeSystem();

		static const VertexFormat*	getVertexFormat( const VertexFormatParameters& params );
		static void					releaseVertexFormat( const VertexFormat* pVertexFormat );

		size_t						getVertexStride( const size_t streamIndex ) const { return m_vertexStride[ streamIndex ]; }

	private:

		enum
		{
			MaxVertexFormatCount	= 64u,
			MaxFormatDescCount		= 16u,
		};

									VertexFormat();
									~VertexFormat();

		size_t						m_vertexStride[ GraphicsSystemLimits_MaxInputStreams ];
		TGInputLayout*				m_pInputLayout;
		VertexAttribute				m_attributes[ MaxFormatDescCount ];

	};
}

#endif // TIKI_VERTEXFORMAT_HPP