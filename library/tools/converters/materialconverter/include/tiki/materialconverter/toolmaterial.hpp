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

		bool			create( const string& fileName );
		void			dispose();

		ReferenceKey	writeResource( ResourceWriter& writer ) const;

	private:

		struct MaterialField
		{
			string	name;
			string	type;
			string	value;
		};
		typedef SortedSizedMap< const reflection::FieldMember*, MaterialField > MaterialFieldMap;

		const reflection::StructType*	m_pDataType;
		MaterialFieldMap				m_effectData;

	};
}

#endif // TIKI_TOOLMATERIAL_HPP__
