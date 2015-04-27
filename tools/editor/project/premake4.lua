-- library/tools/editor/project

include "../../../buildtools/base_scripts"

finalize( "tiki3editor", { find_project( "editor" ), find_project( "editornative" ), find_project( "converterlibrary" ) } );
