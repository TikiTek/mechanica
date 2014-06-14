#pragma once
#ifndef TIKI_SHADERCONVERTER_HPP
#define TIKI_SHADERCONVERTER_HPP

#include "tiki/converterbase/converterbase.hpp"

namespace tiki
{
	enum GraphicsApi;
	enum ShaderType;
	class ShaderIncludeHandler;
	
	class ShaderConverter : public ConverterBase
	{
	public:

		virtual					~ShaderConverter() {}

	protected:

		virtual uint16			getConverterRevision() const { return 4u; }

		virtual crc32			getInputType() const;
		virtual crc32			getOutputType() const;
		virtual void			getDependingType( List< crc32 >& types ) const;

		virtual bool			initializeConverter();
		virtual void			disposeConverter();

		virtual bool			startConversionJob( const ConversionParameters& params ) const;

	private:

		struct ShaderArguments
		{
			ShaderType		type;

			string			fileName;
			string			outputName;

			string			version;
			string			entryPoint;
			string			defineCode;

			bool			debugMode;
		};
		
		string					m_baseSourceCode;
		ShaderIncludeHandler*	m_pIncludeHandler;

		bool					compilePlatformShader( Array< uint8 >& targetData, const ShaderArguments& args, GraphicsApi targetApi ) const;

		bool					compileD3d11Shader( Array< uint8 >& targetData, const ShaderArguments& args ) const;
		bool					compileOpenGl4Shader( Array< uint8 >& targetData, const ShaderArguments& args ) const;

	};
}

#endif // TIKI_SHADERCONVERTER_HPP
