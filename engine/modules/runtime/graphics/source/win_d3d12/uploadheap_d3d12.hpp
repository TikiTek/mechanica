#ifndef TIKI_UPLOADHEAP_HPP__INCLUDED__
#define TIKI_UPLOADHEAP_HPP__INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/queue.hpp"

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	class GraphicsSystem;
	struct GraphicsSystemPlatformData;

	struct UploadHeapAllocationResult
	{
		void*	pData;
		uint	size;
		uint	offset;
	};

	class UploadHeapD3d12
	{
		friend class GraphicsSystem;
		friend struct GraphicsSystemPlatformData;
		TIKI_NONCOPYABLE_CLASS( UploadHeapD3d12 );

	public:

		bool			allocateData( UploadHeapAllocationResult& result, uint size, uint alignment );

		ID3D12Resource*	getBuffer() const { return m_pBuffer; }

	private: // friend

		UploadHeapD3d12();
		~UploadHeapD3d12();

		bool	create( ID3D12Device* pDevice, uint size, uint maxAllocationCount, const char* pDebugName );
		void	dispose();

		void	finalizeFrame( uint currentFrameIndex );

	private:

		struct AllocationData
		{
			uint8*		pData;
			uint		frameIndex;
			uint		size;
		};

		ID3D12Resource*			m_pBuffer;
		uint8*					m_pDataStart;
		uint8*					m_pDataEnd;
		uint					m_size;

		uint					m_currentFrameIndex;
		Queue< AllocationData > m_allocations;

	};
}

#endif // TIKI_UPLOADHEAP_HPP__INCLUDED__
