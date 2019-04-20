#pragma once

#include "tiki/converter_base/converter_base.hpp"

#include "tiki/base/platform.hpp"
#include "tiki/graphics/shader_type.hpp"

namespace tiki
{
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
		virtual const char*	getOutputType() const TIKI_OVERRIDE_FINAL;

		virtual bool		initializeConverter() TIKI_OVERRIDE_FINAL;
		virtual void		disposeConverter() TIKI_OVERRIDE_FINAL;

		virtual bool		startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const TIKI_OVERRIDE_FINAL;

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

		List< string >		m_includePathes;

		mutable Mutex		m_openGlMutex;

		bool				compilePlatformShader( Array< uint8 >& targetData, const ShaderArguments& args, ShaderIncludeHandler& includeHandler, GraphicsApi targetApi ) const;
		bool				compileD3dShader( Array< uint8 >& targetData, const ShaderArguments& args, ShaderIncludeHandler& includeHandler ) const;
		bool				compileVulkanShader( Array< uint8 >& targetData, const ShaderArguments& args, ShaderIncludeHandler& includeHandler ) const;
	};
}
