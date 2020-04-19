local module = Module:new();

module.module_type = ModuleTypes.FilesModule;

module:add_files( "*.lua" );
module:add_files( "*.h" );
module:add_files( "*.c" );
module:add_include_dir( "." );

module:set_base_path( "blobs/pcre2" );
module:add_files( "src/*.h" );
module:add_files( "src/*.c" );

module:add_files( "src/dftables.c", { exclude = true } );
module:add_files( "src/pcre2demo.c", { exclude = true } );
module:add_files( "src/pcre2test.c", { exclude = true } );
module:add_files( "src/pcre2posix.c", { exclude = true } );
module:add_files( "src/pcre2_jit_match.c", { exclude = true } );
module:add_files( "src/pcre2_jit_misc.c", { exclude = true } );
module:add_files( "src/pcre2_jit_test.c", { exclude = true } );
module:add_files( "src/pcre2_printint.c", { exclude = true } );
module:add_files( "src/pcre2_fuzzsupport.c", { exclude = true } );

module:add_include_dir( "src" );

module:set_define( "HAVE_CONFIG_H" );
module:set_define( "PCRE2_STATIC" );
module:set_define( "PCRE2_CODE_UNIT_WIDTH", "8" );
