
#include "tiki/graphics/shader.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicsystem.hpp"
#include "graphicshandles.hpp"

#include <dxgi.h>
#include <d3d11.h>

namespace tiki
{
	Shader::Shader()
	{
		m_pShaderObject	= nullptr;
	}

	Shader::~Shader()
	{
		
	}

	void Shader::initialize( ShaderTypes type, void* pShaderObject, TGBlob* pBlob )
	{
		TIKI_ASSERT( pShaderObject );

		m_type			= type;
		m_pShaderObject	= pShaderObject;

		m_pBlob			= pBlob;
	}

	TGInputLayout* Shader::createInputLayout( TGInputElementDesc* desc, const int count ) const
	{
		TGInputLayout* pLayout = nullptr;

		TGDevice* pDevice = getHandles( framework::getGraphicsSystem() )->pDevice;

		const void* pData	= m_pBlob->GetBufferPointer();
		const size_t size	= m_pBlob->GetBufferSize();

		const HRESULT result = pDevice->CreateInputLayout( desc, count, pData, size, &pLayout );
		TIKI_ASSERT( result == S_OK );

		return pLayout;
	}

}