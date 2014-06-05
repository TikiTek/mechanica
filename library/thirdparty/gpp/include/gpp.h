#ifndef __TIKI_GPP_H_INCLUDED__
#define __TIKI_GPP_H_INCLUDED__

#define FLAG_META 0
#define FLAG_USER 1
#define FLAG_TEXT 2

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _ReadBuffer
{
	const char*	pData;
	int			dataSize;
} ReadBuffer;

typedef struct _WriteBuffer
{
	char*	pData;
	int		dataCapacity;
} WriteBuffer;

typedef int (*OpenFileFunc)( ReadBuffer*, const char* );
typedef void (*CloseFileFunc)( const ReadBuffer* );

typedef struct _PreprocessParameters
{
	ReadBuffer		inputData;
	WriteBuffer		outputData;

	OpenFileFunc	pOpenFileFunc;
	CloseFileFunc	pCloseFileFunc;
} PreprocessParameters;

int preprocessText( const PreprocessParameters* pParameters );

#ifdef __cplusplus
};
#endif

#endif // __TIKI_GPP_H_INCLUDED__
