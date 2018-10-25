#include "generic_data_file.hpp"

#include "tiki/toolgenericdata/generic_data_array.hpp"
#include "tiki/toolgenericdata/generic_data_object.hpp"

#include <imgui.h>

namespace tiki
{
	GenericDataFile::GenericDataFile( const Path& fileName, GenericDataEditor* pEditor )
		: EditableFile( fileName, pEditor )
		, m_document( pEditor->getTypeCollection() )
	{
	}

	GenericDataFile::~GenericDataFile()
	{
	}

	bool GenericDataFile::load()
	{
		return m_document.importFromFile( getFileName().getCompletePath() );
	}

	bool GenericDataFile::save()
	{
		return m_document.exportToFile( getFileName().getCompletePath() );
	}

	void GenericDataFile::doUi()
	{
		ImGui::Columns( 2 );
		ImGui::Separator();

		GenericDataObject* pObject = m_document.getObject();
		doObjectUi( pObject );

		ImGui::Columns( 1 );
		ImGui::Separator();
	}

	void GenericDataFile::doObjectUi( GenericDataObject* pObject )
	{
		for( uint i = 0u; i < pObject->getFieldCount(); ++i )
		{
			doElementUi( pObject->getFieldName( i ), pObject->getFieldValue( i ) );
		}
	}

	void GenericDataFile::doArrayUi( GenericDataArray* pArray )
	{
		for( uint i = 0u; i < pArray->getCount(); ++i )
		{
			const string name = formatDynamicString( "%d", i );
			doElementUi( name, pArray->getElement( i ) );
		}
	}

	void GenericDataFile::doElementUi( const string& name, GenericDataValue* pValue )
	{
		const GenericDataType* pType = pValue->getType();
		const bool canBeOpen = pType->getType() == GenericDataTypeType_Array || pType->getType() == GenericDataTypeType_Struct || pType->getType() == GenericDataTypeType_Pointer;

		ImGui::PushID( pValue );
		ImGui::AlignTextToFramePadding();
		bool nodeOpen = false;
		if( canBeOpen )
		{
			nodeOpen = ImGui::TreeNode( name.cStr() );
		}
		else
		{
			ImGui::TreeNodeEx( name.cStr(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet );
		}
		ImGui::NextColumn();

		ImGui::AlignTextToFramePadding();
		if( canBeOpen )
		{
			ImGui::Text( " " );
		}
		else
		{
			doValueUi( pValue );
		}
		ImGui::NextColumn();

		if( nodeOpen )
		{
			if( pType->getType() == GenericDataTypeType_Array )
			{
				GenericDataArray* pArray = nullptr;
				if( pValue->getArray( pArray ) )
				{
					doArrayUi( pArray );
				}
			}
			else if( pType->getType() == GenericDataTypeType_Struct || pType->getType() == GenericDataTypeType_Pointer )
			{
				GenericDataObject* pSubObject = nullptr;
				if( pValue->getObject( pSubObject ) )
				{
					doObjectUi( pSubObject );
				}
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	void GenericDataFile::doValueUi( GenericDataValue* pValue )
	{
		const GenericDataType* pType = pValue->getType();
		switch( pType->getType() )
		{
		case GenericDataTypeType_Enum:
			break;

		case GenericDataTypeType_Struct:
			break;

		case GenericDataTypeType_ValueType:
			break;

		case GenericDataTypeType_Resource:
			break;

		case GenericDataTypeType_Array:
			break;

		case GenericDataTypeType_Reference:
			break;

		case GenericDataTypeType_Pointer:
			break;

		default:
			break;
		}

		ImGui::Text( " " );
	}
}
