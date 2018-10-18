#include "tiki/base/ascii.hpp"
#include "tiki/base/debug.hpp"
#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"
#include "tiki/io/file.hpp"

namespace tiki
{
	DynamicString toCamelCase( DynamicString name, bool startUp = true )
	{
		DynamicString result;

		bool up = startUp;
		for( char c : name )
		{
			if( c == ' ' || c == '-' || c == '_' )
			{
				up = true;
			}
			else if( up )
			{
				result += ascii::toUpper( c );
				up = false;
			}
			else
			{
				result += c;
			}
		}

		return result;
	}
}

int tiki::mainEntryPoint()
{
	int retValue = 0;

	//debug::breakOnAlloc( 1449 );
	{
		List< const char* > arguments( platform::getArguments().getBegin() + 1u, platform::getArguments().getCount() - 1u );

		Path outputDir;
		const char* pValue = nullptr;
		if( platform::findArgumentValue( &pValue, "--output-dir", true ) )
		{
			outputDir.setCompletePath( pValue );
			arguments.removeSortedByValue( pValue );
		}
		else
		{
			TIKI_TRACE_ERROR( "No output directory specified.\n" );
			return 1;
		}

		DynamicString outputName;
		if( platform::findArgumentValue( &pValue, "--output-name", true ) )
		{
			outputName = pValue;
			arguments.removeSortedByValue( pValue );
		}
		else
		{
			TIKI_TRACE_ERROR( "No output name specified.\n" );
			return 1;
		}

		Path headerFilepath = outputDir;
		DynamicString headerFilename = "res_" + outputName + ".hpp";
		headerFilepath.push( headerFilename.cStr() );

		Path sourceFilepath = outputDir;
		DynamicString sourceFilename = "res_" + outputName + ".cpp";
		sourceFilepath.push( sourceFilename.cStr() );

		const DynamicString name = toCamelCase( outputName );
		const DynamicString enumName = name + "Resources";

		DynamicString sourceContent;
		sourceContent += "#include \"" + headerFilename + "\"\n\n";
		sourceContent += "\n";
		sourceContent += "namespace tiki\n";
		sourceContent += "{\n";

		DynamicString enumNames;
		DynamicString resourceValues;
		for( const char* pFile : arguments )
		{
			if( doesStringStartWith( pFile, "-" ) )
			{
				continue;
			}

			if( !file::exists( pFile ) )
			{
				TIKI_TRACE_ERROR( "'%s' not found.\n", pFile );
				return 1;
			}

			Array< uint8 > fileData;
			if( !file::readAllBytes( pFile, fileData ) )
			{
				TIKI_TRACE_ERROR( "Could not load '%s'.\n", pFile );
				return 1;
			}

			const Path filePath( pFile );
			const DynamicString variabelName = "s_" + toCamelCase( filePath.getFilename(), false );
			const DynamicString enumValueName = enumName + "_" + toCamelCase( filePath.getFilename() );

			enumNames += "\t\t" + enumValueName + ",\n";
			resourceValues += "\t\t\t{ " + variabelName + ", sizeof( " + variabelName + " ) },\n";

			sourceContent += "\tstatic const uint8 " + variabelName + "[] =\n";
			sourceContent += "\t{\n";
			sourceContent += "\t\t";

			uint index = 0;
			for( uint8 b : fileData )
			{
				if( index != 0u && index % 32u == 0u )
				{
					sourceContent += "\n\t\t";
				}
				index++;

				sourceContent += formatDynamicString( "0x%02x, ", b );
			}

			sourceContent += "\n";
			sourceContent += "\t};\n";
			sourceContent += "\n";

			fileData.dispose();
		}

		sourceContent += "\tstatic const ConstMemoryBlock s_resources[] =\n";
		sourceContent += "\t{\n";
		sourceContent += resourceValues;
		sourceContent += "\t};\n";
		sourceContent += "\n";
		sourceContent += "\tConstMemoryBlock get" + name + "Resource( " + enumName + " name )\n";
		sourceContent += "\t{\n";
		sourceContent += "\t\tTIKI_ASSERT( name < " + enumName + "_Count );\n";
		sourceContent += "\t\treturn s_resources[ name ];\n";
		sourceContent += "\t}\n";
		sourceContent += "}\n";

		DynamicString headerContent;
		headerContent += "#pragma once\n";
		headerContent += "\n";
		headerContent += "#include \"tiki/base/memory.hpp\"\n";
		headerContent += "\n";
		headerContent += "namespace tiki\n";
		headerContent += "{\n";
		headerContent += "\tenum " + enumName + " : uint8\n";
		headerContent += "\t{\n";
		headerContent += enumNames;
		headerContent += "\n";
		headerContent += "\t\t" + enumName + "_Count\n";
		headerContent += "\t};\n";
		headerContent += "\n";
		headerContent += "\tConstMemoryBlock get" + name + "Resource( " + enumName + " name );\n";
		headerContent += "}\n";

		file::writeToFileIfNotEquals( headerFilepath, headerContent );
		file::writeToFileIfNotEquals( sourceFilepath, sourceContent );
	}

	return retValue;
}