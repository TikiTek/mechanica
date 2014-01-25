
#ifdef DEBUG
int z_verbose;
void z_error(char *m)
{
	__debugbreak();
}
#endif