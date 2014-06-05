#ifndef __TIKI_GPP_H_INCLUDED__
#define __TIKI_GPP_H_INCLUDED__

#define FLAG_META 0
#define FLAG_USER 1
#define FLAG_TEXT 2

#ifdef __cplusplus
extern "C"
{
#endif

static void getDirname(const char *fname, char *dirname);

#ifdef __cplusplus
};
#endif

#endif // __TIKI_GPP_H_INCLUDED__
