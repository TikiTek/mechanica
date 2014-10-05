
#ifdef DEBUG

#define TIKI_ENABLED( value ) ( ( value 0 ) == 2 )

int z_verbose;
void z_error(char *m)
{
#if TIKI_ENABLED( TIKI_BUILD_MSVC )
	__debugbreak();
#endif
}
#endif
