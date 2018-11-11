#include "tiki/ui/uisystemscript.hpp"

#include "tiki/script/scriptcontext.hpp"
#include "tiki/ui/uisystem.hpp"

namespace tiki
{
	struct UiSystemScriptData
	{
		UiSystem*			pUiSystem;
		UiElementScript*	pUiElementScript;
	};

	UiSystemScript::UiSystemScript()
	{
		m_pUiSystem			= nullptr;
		m_pUiElementScript	= nullptr;
	}

	UiSystemScript::~UiSystemScript()
	{
		TIKI_ASSERT( m_pUiSystem == nullptr );
		TIKI_ASSERT( m_pUiElementScript == nullptr );
	}

	bool UiSystemScript::create( ScriptContext& scriptContext, UiSystem& uiSystem, UiElementScript& elementScript )
	{
		m_pUiSystem			= &uiSystem;
		m_pUiElementScript	= &elementScript;

		static const ScriptMethod s_aMethods[] =
		{
			{ "addElement", &ScriptContext::scriptWrapperFunction< UiSystemScript::addElement > },
			{ "removeElement", &ScriptContext::scriptWrapperFunction< UiSystemScript::removeElement > },
		};

		if( !ScriptClass::create( scriptContext, "UiSystem", s_aMethods, TIKI_COUNT( s_aMethods ) ) )
		{
			return false;
		}

		UiSystemScriptData scriptData;
		scriptData.pUiSystem		= &uiSystem;
		scriptData.pUiElementScript	= &elementScript;

		ScriptValue value = registerInstance( &scriptData, sizeof( UiSystemScriptData ) );
		scriptContext.setGlobalValue( "uiSystem", value );
		
		return true;
	}

	void UiSystemScript::dispose()
	{
		ScriptClass::dispose();

		m_pUiSystem			= nullptr;
		m_pUiElementScript	= nullptr;
	}

	void UiSystemScript::addElement( const ScriptCall& call )
	{
		const UiSystemScriptData& scriptData = *(const UiSystemScriptData*)call.getInstanceData();

		const ScriptValue parentValue = call.getArgument( 0u );
		call.setReturnValue( scriptData.pUiElementScript->createScriptElement( parentValue ) );
	}

	void UiSystemScript::removeElement( const ScriptCall& call )
	{
		const UiSystemScriptData& scriptData = *(const UiSystemScriptData*)call.getInstanceData();

		const ScriptValue value = call.getArgument( 0u );
		scriptData.pUiElementScript->disposeScriptElement( value );
	}
}