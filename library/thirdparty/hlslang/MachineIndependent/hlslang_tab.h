typedef union {
    struct {
        TSourceLoc line;
        union {
            TString *string;
            float f;
            int i;
            bool b;
        };
        TSymbol* symbol;
    } lex;
    struct {
        TSourceLoc line;
        TOperator op;
        union {
            TIntermNode* intermNode;
            TIntermNodePair nodePair;
            TIntermTyped* intermTypedNode;
            TIntermAggregate* intermAggregate;
        };
        union {
            TPublicType type;
            TQualifier qualifier;
            TFunction* function;
            TParameter param;
            TTypeLine typeLine;
            TTypeList* typeList;
	    TAnnotation* ann;
	    TTypeInfo* typeInfo;
        };
    } interm;
} YYSTYPE;
#define	ATTRIBUTE	258
#define	CONST_QUAL	259
#define	BOOL_TYPE	260
#define	FLOAT_TYPE	261
#define	INT_TYPE	262
#define	STRING_TYPE	263
#define	BREAK	264
#define	CONTINUE	265
#define	DO	266
#define	ELSE	267
#define	FOR	268
#define	IF	269
#define	DISCARD	270
#define	RETURN	271
#define	BVEC2	272
#define	BVEC3	273
#define	BVEC4	274
#define	IVEC2	275
#define	IVEC3	276
#define	IVEC4	277
#define	VEC2	278
#define	VEC3	279
#define	VEC4	280
#define	MATRIX2	281
#define	MATRIX3	282
#define	MATRIX4	283
#define	IN_QUAL	284
#define	OUT_QUAL	285
#define	INOUT_QUAL	286
#define	UNIFORM	287
#define	VARYING	288
#define	STRUCT	289
#define	VOID_TYPE	290
#define	WHILE	291
#define	SAMPLER1D	292
#define	SAMPLER2D	293
#define	SAMPLER3D	294
#define	SAMPLERCUBE	295
#define	SAMPLER1DSHADOW	296
#define	SAMPLER2DSHADOW	297
#define	SAMPLERGENERIC	298
#define	VECTOR	299
#define	MATRIX	300
#define	REGISTER	301
#define	TEXTURE	302
#define	SAMPLERSTATE	303
#define	IDENTIFIER	304
#define	TYPE_NAME	305
#define	FLOATCONSTANT	306
#define	INTCONSTANT	307
#define	BOOLCONSTANT	308
#define	STRINGCONSTANT	309
#define	FIELD_SELECTION	310
#define	LEFT_OP	311
#define	RIGHT_OP	312
#define	INC_OP	313
#define	DEC_OP	314
#define	LE_OP	315
#define	GE_OP	316
#define	EQ_OP	317
#define	NE_OP	318
#define	AND_OP	319
#define	OR_OP	320
#define	XOR_OP	321
#define	MUL_ASSIGN	322
#define	DIV_ASSIGN	323
#define	ADD_ASSIGN	324
#define	MOD_ASSIGN	325
#define	LEFT_ASSIGN	326
#define	RIGHT_ASSIGN	327
#define	AND_ASSIGN	328
#define	XOR_ASSIGN	329
#define	OR_ASSIGN	330
#define	SUB_ASSIGN	331
#define	LEFT_PAREN	332
#define	RIGHT_PAREN	333
#define	LEFT_BRACKET	334
#define	RIGHT_BRACKET	335
#define	LEFT_BRACE	336
#define	RIGHT_BRACE	337
#define	DOT	338
#define	COMMA	339
#define	COLON	340
#define	EQUAL	341
#define	SEMICOLON	342
#define	BANG	343
#define	DASH	344
#define	TILDE	345
#define	PLUS	346
#define	STAR	347
#define	SLASH	348
#define	PERCENT	349
#define	LEFT_ANGLE	350
#define	RIGHT_ANGLE	351
#define	VERTICAL_BAR	352
#define	CARET	353
#define	AMPERSAND	354
#define	QUESTION	355

