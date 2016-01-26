#include "tiki/ui/uisystemscript.hpp"

#include "tiki/script/scriptcontext.hpp"
#include "tiki/ui/uisystem.hpp"

namespace tiki
{

	bool UiSystemScript::create( ScriptContext& scriptContext, UiSystem& uiSystem )
	{
		static const ScriptMethod s_aMethods[] =
		{
			{ "addElement", &ScriptContext::scriptWrapperFunction< UiSystemScript::addElement > }
		};

		if( !ScriptClass::create( scriptContext, "UiSystem", s_aMethods, TIKI_COUNT( s_aMethods ) ) )
		{
			return false;
		}

		ScriptInstance instance = registerInstance( &uiSystem );
		scriptContext.setGlobalValue( "uiSystem", instance );
		
		return true;
	}

	void UiSystemScript::dispose()
	{
		ScriptClass::dispose();
	}

	void UiSystemScript::addElement( ScriptCall& context )
	{
		UiSystem& uiSystem = *(UiSystem*)context.getInstance();
		uiSystem.addElement();
	}
}