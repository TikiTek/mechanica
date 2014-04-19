#ifndef TIKI_TOOLMATERIAL_HPP__
#define TIKI_TOOLMATERIAL_HPP__

#include "tiki/base/sortedsizedmap.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/converterbase/resourcewriter.hpp"

namespace tiki
{
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

		ReferenceKey	writeResource( ResourceWriter& writer );

	private:

		enum MaterialFieldType
		{
			MaterialFieldType_Invalid = -1,

			MaterialFieldType_Reference,
			MaterialFieldType_Integer,
			MaterialFieldType_Float
		};

		struct MaterialField
		{
			string				name;
			string				value;
			MaterialFieldType	type;

			union
			{
				ReferenceKey	referenceKey;
				sint64			integerValue;
				double			floatValue;
			} data;
		};
		typedef SortedSizedMap< const reflection::FieldMember*, MaterialField > MaterialFieldMap;

		const reflection::StructType*	m_pDataType;
		MaterialFieldMap				m_effectData;

	};
}

#endif // TIKI_TOOLMATERIAL_HPP__
