#pragma once
#ifndef TIKI_PACKAGE_MANAGER_HPP_INCLUDED
#define TIKI_PACKAGE_MANAGER_HPP_INCLUDED

#include "tiki/base/path.hpp"
#include "tiki/container/chunkedpool.hpp"
#include "tiki/container/linkedlist.hpp"

namespace tiki
{
	class Package;

	class PackageManager
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( PackageManager );

	public:

		void							create( const Path& contentPath );
		void							dispose();

		const Path&						getContentPath() const { return m_contentPath; }

		Package*						addPackage( const string& packageName );
		Package*						findPackage( const string& packageName );
		const Package*					findPackage( const string& packageName ) const;
		const LinkedList< Package >&	getPackages() const;

		bool							writeToFile();

	private:

		Path					m_contentPath;

		ChunkedPool< Package >	m_packagePool;
		LinkedList< Package >	m_packages;
	};
}

#endif // TIKI_PACKAGE_MANAGER_HPP_INCLUDED
