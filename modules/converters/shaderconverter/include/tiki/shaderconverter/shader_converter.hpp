#pragma once
#ifndef TIKI_SHADERCONVERTER_HPP
#define TIKI_SHADERCONVERTER_HPP

#include "tiki/converterbase/converterbase.hpp"

#include "tiki/base/platform.hpp"
#include "tiki/graphics/shadertype.hpp"

namespace tiki
{
	class ShaderFileStorage;
	class ShaderIncludeHandler;
	enum GraphicsApi;

	class ShaderConverter : public ConverterBase
	{
		TIKI_NONCOPYABLE_CLASS( ShaderConverter );

	public:

							ShaderConverter();
		virtual				~ShaderConverter();

	protected:

		virtual uint32		getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;
		virtual bool		canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;

		virtual void		getInputExtensions( List< string >& extensions ) const TIKI_OVERRIDE_FINAL;
		virtual crc32		getOutputType() const TIKI_OVERRIDE_FINAL;

		virtual bool		initializeConverter() TIKI_OVERRIDE_FINAL;
		virtual void		disposeConverter() TIKI_OVERRIDE_FINAL;

		virtual bool		startConversionJob( ConversionResult& result, const ConversionAsset& asset ) const TIKI_OVERRIDE_FINAL;

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

		const char*			m_pBaseSourceCode;

		List< string >		m_includeDirs;
		ShaderFileStorage*	m_pFileStorage;

		mutable Mutex		m_openGlMutex;

		bool				compilePlatformShader( Array< uint8 >& targetData, const ShaderArguments& args, ShaderIncludeHandler& includeHandler, GraphicsApi targetApi ) const;
		bool				compileD3dShader( Array< uint8 >& targetData, const ShaderArguments& args, ShaderIncludeHandler& includeHandler ) const;
		bool				compileVulkanShader( Array< uint8 >& targetData, const ShaderArguments& args, ShaderIncludeHandler& includeHandler ) const;

	};
}

#endif // TIKI_SHADERCONVERTER_HPP
