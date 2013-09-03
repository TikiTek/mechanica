
#include "tiki/graphicsresources/shaderfactory.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/graphicsresources/shader.hpp"

#include "graphicshandles.hpp"

namespace tiki
{
	ShaderFactory::ShaderFactory()
	{
		m_pGraphicsSystem	= nullptr;
	}

	ShaderFactory::~ShaderFactory()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
	}

	void ShaderFactory::create(  GraphicsSystem* pSystem )
	{
		TIKI_ASSERT( pSystem );
		m_pGraphicsSystem	= pSystem;
		m_pGraphicsHandles	= getHandles( *m_pGraphicsSystem );
	}

	void ShaderFactory::dispose()
	{
		TIKI_ASSERT( m_pGraphicsSystem );
		m_pGraphicsSystem	= nullptr;
		m_pGraphicsHandles	= nullptr;
	}

	fourcc ShaderFactory::getType() const
	{
		return TIKI_FOURCC( 'T', 'G', 'F', 'X' );
	}

	Resource* ShaderFactory::createResource() const
	{
		return TIKI_NEW Shader();
	}

	void ShaderFactory::destroyResource( Resource* pResource ) const
	{
		TIKI_ASSERT( pResource );

		Shader* pShaderResource	= (Shader*)pResource;
		void* pShaderObject		= pShaderResource->m_pShaderObject;
		ID3D10Blob* pBlob		= pShaderResource->m_pBlob;

		if ( pBlob )
		{
			pBlob->Release();
		}

		if ( pShaderObject )
		{
			switch ( pShaderResource->getShaderType() )
			{
			case ShaderTypes_ComputeShader:
				{
					ID3D11ComputeShader* pComputeShader = (ID3D11ComputeShader*)pShaderObject;
					pComputeShader->Release();
				}
				break;
			case ShaderTypes_DomainShader:
				{
					ID3D11DomainShader* pDomainShader = (ID3D11DomainShader*)pShaderObject;
					pDomainShader->Release();
				}
				break;
			case ShaderTypes_GeometrieShader:
				{
					ID3D11GeometryShader* pGeometryShader = (ID3D11GeometryShader*)pShaderObject;
					pGeometryShader->Release();
				}
				break;
			case ShaderTypes_HullShader:
				{
					ID3D11HullShader* pHullShader = (ID3D11HullShader*)pShaderObject;
					pHullShader->Release();
				}
				break;
			case ShaderTypes_PixelShader:
				{
					ID3D11PixelShader* pPixelShader = (ID3D11PixelShader*)pShaderObject;
					pPixelShader->Release();
				}
				break;
			case ShaderTypes_VertexShader:
				{
					ID3D11VertexShader* pVertexShader = (ID3D11VertexShader*)pShaderObject;
					pVertexShader->Release();
				}
				break;
			}
		}



		delete (Shader*)pResource;
	}

	bool ShaderFactory::initializeResource( Resource* pResource, const void* pInitData ) const
	{
		TIKI_ASSERT( pInitData );

		const uint32* pInfo		= (const uint32*)pInitData;
		const ShaderTypes type	= (ShaderTypes)pInfo[ 0u ];
		const uint32 dataLength	= pInfo[ 1u ];
		pInfo += 2u;

		void* pData				= (void*)pInfo;
		void* pShader			= nullptr;

		switch ( type )
		{
		case ShaderTypes_ComputeShader:
			{
				ID3D11ComputeShader* pComputeShader = nullptr;
				TIKI_VERIFY0( m_pGraphicsHandles->pDevice->CreateComputeShader( pData, dataLength, nullptr, &pComputeShader ) );

				pShader = pComputeShader;
			}
			break;
		case ShaderTypes_DomainShader:
			{
				ID3D11DomainShader* pDomainShader = nullptr;
				TIKI_VERIFY0( m_pGraphicsHandles->pDevice->CreateDomainShader( pData, dataLength, nullptr, &pDomainShader ) );

				pShader = pDomainShader;
			}
			break;
		case ShaderTypes_GeometrieShader:
			{
				ID3D11GeometryShader* pGeometryShader = nullptr;
				TIKI_VERIFY0( m_pGraphicsHandles->pDevice->CreateGeometryShader( pData, dataLength, nullptr, &pGeometryShader ) );

				pShader = pGeometryShader;
			}
			break;
		case ShaderTypes_HullShader:
			{
				ID3D11HullShader* pHullShader = nullptr;
				TIKI_VERIFY0( m_pGraphicsHandles->pDevice->CreateHullShader( pData, dataLength, nullptr, &pHullShader ) );

				pShader = pHullShader;
			}
			break;
		case ShaderTypes_PixelShader:
			{
				ID3D11PixelShader* pPixelShader = nullptr;
				TIKI_VERIFY0( m_pGraphicsHandles->pDevice->CreatePixelShader( pData, dataLength, nullptr, &pPixelShader ) );

				pShader = pPixelShader;
			}
			break;
		case ShaderTypes_VertexShader:
			{
				ID3D11VertexShader* pVertexShader = nullptr;
				TIKI_VERIFY0( m_pGraphicsHandles->pDevice->CreateVertexShader( pData, dataLength, nullptr, &pVertexShader ) );

				pShader = pVertexShader;
			}
			break;
		}

		ID3D10Blob* pBlob = nullptr;
		D3D10CreateBlob( dataLength, &pBlob );

		if ( pBlob )
		{
			memory::copy( pBlob->GetBufferPointer(), pData, dataLength );
		}
		else
		{
			return false;
		}
				
		Shader* pShaderResource = (Shader*)pResource;
		pShaderResource->initialize( type, pShader, pBlob );

		return true;
	}
}