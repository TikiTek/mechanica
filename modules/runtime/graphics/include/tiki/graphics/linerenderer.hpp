#ifndef TIKI_LINERENDERER_HPP__
#define TIKI_LINERENDERER_HPP__

#include "tiki/base/array.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/dynamicbuffer.hpp"

namespace tiki
{
	class Shader;
	class VertexFormat;

	class LineRenderer
	{
	public:

		friend class GpuContext;

										~LineRenderer( void );

		bool							create( GpuContext* pContext, const uint maxLines = 150u );

		void							dispose( void );

		void							drawLine( const Vector3& start, const Vector3& end, const Color& color );

		void							flush( void );

	private:
								LineRenderer( void );
		TIKI_NONCOPYABLE_CLASS( LineRenderer );

		Array< VertexColor >			m_lines;
		uint							m_lineCount;
		uint							m_maxLines;

		// TODO: use material
		const Shader*					m_pVertexShader;
		const Shader*					m_pPixelShader;

		const VertexFormat*				m_pVertexFormat;

		GpuContext*						m_pContext;
		DynamicBuffer<VertexColor, 1U>	m_vertexBuffer;	//1U = vertexbuffer
	};
}

#endif // TIKI_LINERENDERER_HPP__
