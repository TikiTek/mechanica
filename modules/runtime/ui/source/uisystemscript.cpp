#include "tiki/ui/uisystemscript.hpp"

#include "tiki/script/scriptcontext.hpp"
#include "tiki/ui/uisystem.hpp"

namespace tiki
{
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

		ScriptValue value = registerInstance( this );
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
		UiSystemScript& uiSystem = *(UiSystemScript*)call.getInstance();

		UiElement* pElement = uiSystem.m_pUiSystem->addElement();
		call.setReturnValue( uiSystem.m_pUiElementScript->registerInstance( pElement ) );
	}

	void UiSystemScript::removeElement( const ScriptCall& call )
	{
		UiSystemScript& uiSystem = *(UiSystemScript*)call.getInstance();

		const ScriptValue value = call.getArgument( 0u );
		UiElement* pElement = (UiElement*)value.getObjectInstance();

		uiSystem.m_pUiSystem->removeElement( pElement );
	}
}