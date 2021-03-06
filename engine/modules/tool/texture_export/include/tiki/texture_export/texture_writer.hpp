#pragma once

#include "tiki/base/types.hpp"
#include "tiki/converter_base/resource_writer.hpp"
#include "tiki/graphics/texture_description.hpp"

namespace tiki
{
	class HdrImage;
	class ResourceWriter;

	struct TextureWriterParameters
	{
		struct Texture1dData
		{
		};

		struct Texture2dData
		{
		};

		struct Texture3dData
		{
			uint sliceWidth;
			uint sliceHeight;
		};

		struct TextureCubeData
		{
		};

		TextureWriterParameters()
		{
			targetType		= TextureType_Invalid;
			targetFormat	= PixelFormat_Invalid;

			mipMapCount		= 0u;
		}

		TextureType	targetType;
		PixelFormat	targetFormat;

		uint		mipMapCount;

		union
		{
			Texture1dData	texture1d;
			Texture2dData	texture2d;
			Texture3dData	texture3d;
			TextureCubeData	textureCube;
		} data;
	};

	class TextureWriter
	{
		TIKI_NONCOPYABLE_CLASS( TextureWriter );

	public:

		TextureWriter();
		~TextureWriter();

		bool						create( const HdrImage& image, const TextureWriterParameters& parameters );
		void						dispose();

		const TextureDescription&	getDescription() const { return m_description; }

		ReferenceKey				writeTextureData( ResourceWriter& writer, GraphicsApi graphicsApi );

	private:

		TextureWriterParameters	m_parameters;
		TextureDescription		m_description;

		const HdrImage*			m_pImage;
	};
}
