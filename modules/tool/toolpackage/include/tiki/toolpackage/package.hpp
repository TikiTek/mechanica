#pragma once
#ifndef TIKI_PACKAGE_HPP_INCLUDED
#define TIKI_PACKAGE_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/container/list.hpp"

namespace tiki
{
	class PackageManager;

	class Package : public LinkedItem< Package >
	{
		friend class PackageManager;

	public:

		const string&			getName() const { return m_name; }
		const Path&				getFilename() const { return m_filename; }
		const Path&				getBasepath() const { return m_basepath; }

		const string&			getDescription() const { return m_description; }
		void					setDescription( const string& value ) { m_description = value; }
		const string&			getAuthor() const { return m_author; }
		void					setAuthor( const string& value ) { m_author = value; }

		const List< Package* >&	getDependencies() const { return m_dependencies; }
		void					addDependencies( Package* pPackage );
		void					removeDependencies( Package* pPackage );

		const Path&				getGenericDataTypesPath() const { return m_genericDataTypesPath; }
		void					setGenericDataTypesPath( const Path& value ) { m_genericDataTypesPath = value; }
		const Path&				getAssetTemplatesPath() const { return m_assetTemplatesPath; }
		void					setAssetTemplatesPath( const Path& value ) { m_assetTemplatesPath = value; }

		void					findGenericDataTypeFiles( List< Path >& targetList ) const;
		void					findAssetTemplateFiles( List< Path >& targetList ) const;

	private: // friend

		bool					create( const Path& filePath );
		void					create( const Path& contentPath, const string& packageName );

		bool					writeToFile();

	private:

		string					m_name;
		Path					m_filename;
		Path					m_basepath;

		string					m_description;
		string					m_author;

		List< Package* >		m_dependencies;

		Path					m_genericDataTypesPath;
		Path					m_assetTemplatesPath;
	};
}

#endif // TIKI_PACKAGE_HPP_INCLUDED
