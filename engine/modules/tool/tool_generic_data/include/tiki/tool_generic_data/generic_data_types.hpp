#pragma once

namespace tiki
{
	class GenericDataDocumentCollection;
	class GenericDataTypeCollection;

	struct GenericDataLoadContext
	{
		GenericDataTypeCollection*		pTypeCollection		= nullptr;
		GenericDataDocumentCollection*	pDocumentCollection	= nullptr;
		bool							isType				= false;
	};
}