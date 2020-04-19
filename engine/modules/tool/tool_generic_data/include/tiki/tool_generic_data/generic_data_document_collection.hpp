#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/map.hpp"

namespace tiki
{
	class GenericDataDocument;
	class GenericDataTypeCollection;
	class Package;
	class Project;

	class GenericDataDocumentCollection : NonCopyable
	{
	public:

									GenericDataDocumentCollection( GenericDataTypeCollection& typeCollection );
									~GenericDataDocumentCollection();

		void						create( const Project* pProject );
		void						dispose();

		GenericDataTypeCollection&	getTypeCollection() { return m_typeCollection; }

		GenericDataDocument*		loadDocument( const char* pFilename );

	private:

		struct Document
		{
			const Package*			pPackage;
			GenericDataDocument*	pDocument;
		};

		using DocumentMap = Map< DynamicString, Document >;

		GenericDataTypeCollection&	m_typeCollection;
		const Project*				m_pProject			= nullptr;

		DocumentMap					m_documents;
	};
}