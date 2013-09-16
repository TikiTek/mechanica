#pragma once
#ifndef TIKI_SHADER_HPP
#define TIKI_SHADER_HPP

#include "tiki/base/types.hpp"
#include "tiki/resource/resource.hpp"

#include "graphicstypes.hpp"

namespace tiki
{
	class ShaderFactory;
	enum ShaderType;

	class Shader : public Resource
	{
		TIKI_DEFINE_RESOURCE( ShaderFactory, TIKI_FOURCC( 'T', 'G', 'F', 'X' ) );
		TIKI_NONCOPYABLE_CLASS( Shader );

	public:

		//fourcc					getType() const { return  }

		ShaderType					getShaderType() const { return m_type; }
		void*						getShaderObject() const { return m_pShaderObject; }

		TGInputLayout*				createInputLayout( TGInputElementDesc* desc, const int count ) const;

	protected:

		ShaderType					m_type;
		void*						m_pShaderObject;
		TGBlob*						m_pBlob;
				
									Shader();
		virtual						~Shader();

		void						initialize( ShaderType type, void* pShaderObject, TGBlob* pBlob );
	};
}

#endif // TIKI_SHADER_HPP