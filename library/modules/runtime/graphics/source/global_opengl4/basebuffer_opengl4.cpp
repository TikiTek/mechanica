
#include "tiki/graphics/basebuffer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static const GLenum s_aBufferTypeMapping[] =
	{		
		GL_UNIFORM_BUFFER,			// GraphicsBufferType_ConstantBuffer
		GL_ELEMENT_ARRAY_BUFFER,	// GraphicsBufferType_IndexBuffer
		GL_ARRAY_BUFFER				// GraphicsBufferType_VertexBuffer
	};

	BaseBuffer::BaseBuffer()
	{
		m_bufferId = GL_INVALID_ENUM;
	}

	BaseBuffer::~BaseBuffer()
	{
		TIKI_ASSERT( m_bufferId == GL_INVALID_ENUM );
	}

	bool BaseBuffer::create( GraphicsSystem& graphicsSystem, size_t size, bool dynamic, GraphicsBufferType binding, const void* pInitData /*= nullptr*/ )
	{
		TIKI_ASSERT( m_bufferId == GL_INVALID_ENUM );

		glGenBuffers( 1u, &m_bufferId );
		m_bufferType	= s_aBufferTypeMapping[ binding ];
		m_bufferUsage	= ( dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );

		glBindBuffer( m_bufferType, m_bufferId );
		glBufferData( m_bufferType, size, pInitData, m_bufferUsage );
		glBindBuffer( m_bufferType, 0u );

		//// Allocate an OpenGL vertex array object.
		//glGenVertexArrays(1, &m_vertexArrayId);

		//// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		//glBindVertexArray(m_vertexArrayId);

		//// Generate an ID for the vertex buffer.
		//glGenBuffers(1, &m_vertexBufferId);

		//// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
		//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		//glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

		//// Enable the two vertex array attributes.
		//glEnableVertexAttribArray(0);  // Vertex position.
		//glEnableVertexAttribArray(1);  // Vertex color.

		//// Specify the location and format of the position portion of the vertex buffer.
		//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		//glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);

		//// Specify the location and format of the color portion of the vertex buffer.
		//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		//glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float)));

		//// Generate an ID for the index buffer.
		//glGenBuffers(1, &m_indexBufferId);

		//// Bind the index buffer and load the index data into it.
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount* sizeof(unsigned int), indices, GL_STATIC_DRAW);

		return graphics::checkError();
	}

	void BaseBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
		if ( m_bufferId != GL_INVALID_ENUM )
		{
			glDeleteBuffers( 1u, &m_bufferId );
			m_bufferId = GL_INVALID_ENUM;
		}
	}
}