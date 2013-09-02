#ifndef TIKI_DYNAMICBUFFER_HPP__
#define TIKI_DYNAMICBUFFER_HPP__

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"

#include "graphicstypes.hpp"

namespace tiki
{
	template<typename T, uint binding>
	class DynamicBuffer
	{
		friend class GpuContext;
		TIKI_NONCOPYABLE_CLASS( DynamicBuffer );
	public:
					DynamicBuffer( void );
		virtual		~DynamicBuffer( void );

		void		dispose( void );

		uint		getStride( void ) const;
		uint		getType( void ) const;
		uint		getCount( void ) const;
			
		T*			map( const uint count );
		void		unmap( void );

	private:

		TGBuffer*	m_pBuffer;
		uint		m_count;
	};

	template<typename T, uint binding>
	tiki::DynamicBuffer<T, binding>::DynamicBuffer( void ) :
		m_pBuffer( nullptr ), m_count( 0u )
	{

	}

	template<typename T, uint binding>
	tiki::DynamicBuffer<T, binding>::~DynamicBuffer( void )
	{
		TIKI_ASSERT( m_pBuffer == nullptr );
	}

	template<typename T, uint binding>
	void tiki::DynamicBuffer<T, binding>::dispose( void )
	{
		safeRelease( &m_pBuffer );
		m_count = 0u;
	}

	template<typename T, uint binding>
	T* tiki::DynamicBuffer<T, binding>::map( const uint count )
	{
		TIKI_ASSERT( count > 0u );

		GraphicsHandles* pHandles = getHandles( framework::getGraphicsSystem() );
		TIKI_ASSERT( pHandles );

		if( count > m_count )
		{
			m_count = count;
			safeRelease( &m_pBuffer );

			TGDevice* pDevice = pHandles->pDevice;
			TIKI_ASSERT( pDevice );

			D3D11_BUFFER_DESC desc;
			desc.Usage					= D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
			desc.StructureByteStride	= 0;
			desc.BindFlags				= binding;
			desc.ByteWidth				= sizeof( T ) * m_count;
			desc.MiscFlags				= 0;

			TIKI_VERIFY0( pDevice->CreateBuffer( &desc, 0, &m_pBuffer ) );
		}

		D3D11_MAPPED_SUBRESOURCE mapped;
		TGContext* context = pHandles->pContext;

		TIKI_VERIFY0( context->Map( m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped ) );

		return (T*)mapped.pData;
	}

	template<typename T, uint binding>
	void tiki::DynamicBuffer<T, binding>::unmap( void )
	{
		if( m_pBuffer )
		{
			TGContext* pContext = getHandles( framework::getGraphicsSystem() )->pContext;
			TIKI_ASSERT( pContext );
			pContext->Unmap( m_pBuffer, 0 );
		}
	}

	template<typename T, uint binding>
	uint tiki::DynamicBuffer<T, binding>::getCount( void ) const
	{
		return m_count;
	}

	template<typename T, uint binding>
	uint tiki::DynamicBuffer<T, binding>::getType( void ) const
	{
		return binding;
	}

	template<typename T, uint binding>
	uint tiki::DynamicBuffer<T, binding>::getStride( void ) const
	{
		return sizeof( T );
	}

	template< typename T >
	class DVertexBuffer : public DynamicBuffer< T, 1u > { };

	template< typename T >
	class DIndexBuffer : public DynamicBuffer< T, 2u > { };

	template<typename T>
	class DConstantBuffer : public DynamicBuffer< T, 4u > { };
}

#endif // TIKI_DYNAMICBUFFER_HPP__
