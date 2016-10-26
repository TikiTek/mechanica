-- library/thirdparty/freetype

local module = Module:new( "freetype" );

module.module_type = ModuleTypes.FilesModule;

module:add_files( "freetype.lua" );
module:add_files( "zerror.c" );
module:set_base_path( "blobs/freetype" );

module:add_files( "include/*.h" );
module:add_include_dir( "include" );

module:add_files( "src/autofit/autofit.c" );
module:add_files( "src/base/ftbase.c" );
module:add_files( "src/base/ftbbox.c" );
module:add_files( "src/base/ftbitmap.c" );
module:add_files( "src/base/ftfstype.c" );
module:add_files( "src/base/ftgasp.c" );
module:add_files( "src/base/ftglyph.c" );
module:add_files( "src/base/ftgxval.c" );
module:add_files( "src/base/ftinit.c" );
module:add_files( "src/base/ftlcdfil.c" );
module:add_files( "src/base/ftmm.c" );
module:add_files( "src/base/ftotval.c" );
module:add_files( "src/base/ftpatent.c" );
module:add_files( "src/base/ftpfr.c" );
module:add_files( "src/base/ftstroke.c" );
module:add_files( "src/base/ftsynth.c" );
module:add_files( "src/base/ftsystem.c" );
module:add_files( "src/base/fttype1.c" );
module:add_files( "src/base/ftwinfnt.c" );
module:add_files( "src/bdf/bdf.c" );
module:add_files( "src/cache/ftcache.c" );
module:add_files( "src/cff/cff.c" );
module:add_files( "src/cid/type1cid.c" );
module:add_files( "src/gzip/ftgzip.c" );
module:add_files( "src/lzw/ftlzw.c" );
module:add_files( "src/pcf/pcf.c" );
module:add_files( "src/pfr/pfr.c" );
module:add_files( "src/psaux/psaux.c" );
module:add_files( "src/pshinter/pshinter.c" );
module:add_files( "src/psnames/psmodule.c" );
module:add_files( "src/raster/raster.c" );
module:add_files( "src/sfnt/sfnt.c" );
module:add_files( "src/smooth/smooth.c" );
module:add_files( "src/truetype/truetype.c" );
module:add_files( "src/type1/type1.c" );
module:add_files( "src/type42/type42.c" );
module:add_files( "src/winfonts/winfnt.c" );

if is_windows then
	module:add_files( "builds/windows/ftdebug.c" );
end

module:set_define( "FT2_BUILD_LIBRARY", "1" );
module:set_define( "FT_DEBUG_LEVEL_ERROR", "1" );
module:set_define( "FT_DEBUG_LEVEL_TRACE", "1" );
module:set_define( "_CRT_SECURE_NO_WARNINGS" );

