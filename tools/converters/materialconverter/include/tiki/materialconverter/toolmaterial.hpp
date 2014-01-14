#ifndef TIKI_TOOLMATERIAL_HPP__
#define TIKI_TOOLMATERIAL_HPP__

#include "tiki/base/sortedsizedmap.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class ResourceWriter;

	namespace reflection
	{
		class FieldMember;
		class StructType;
	}

	class ToolMaterial
	{
	public:

					ToolMaterial();
					~ToolMaterial();

		bool		create( const string& fileName );
		void		dispose();

		bool		writeResource( ResourceWriter& writer ) const;

	private:

		const reflection::StructType*							m_pDataType;
		SortedSizedMap< const reflection::FieldMember*, int >	m_effectData;

	};
}

#endif // TIKI_TOOLMATERIAL_HPP__
