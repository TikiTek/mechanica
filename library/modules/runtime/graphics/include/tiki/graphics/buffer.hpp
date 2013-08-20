#ifndef TIKI_BUFFER_HPP__
#define TIKI_BUFFER_HPP__

#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

#include "graphicstypes.hpp"

namespace tiki
{
	class BaseBuffer
	{
		friend class GpuContext;

	protected:

		ID3D11Buffer*			m_pBuffer;

		void					create( const size_t size, const uint binding );
		void					dispose();

		void*					map();
		void					unmap();

		void					apply( const size_t index, const size_t stride, const size_t size, const uint binding ) const;

	};

	template<typename T, uint binding >
	class Buffer : public BaseBuffer
	{
		TIKI_NONCOPYABLE_CLASS( Buffer );

	public:
								Buffer( void );
								~Buffer( void );

		TIKI_FORCE_INLINE void	create( void );
		TIKI_FORCE_INLINE void	dispose( void );

		TIKI_FORCE_INLINE uint	getStride() const	{ return sizeof( T ); }
		TIKI_FORCE_INLINE uint	getCount() const	{ return m_count; }

		TIKI_FORCE_INLINE T*	map( const uint count );
		TIKI_FORCE_INLINE void	unmap( void );

		TIKI_FORCE_INLINE void	apply( const uint index = 0 );
				
	private:

		uint					m_count;

	};
}

#include "buffer.inl"

#endif // TIKI_BUFFER_HPP__
