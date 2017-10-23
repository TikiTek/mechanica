#pragma once
#ifndef TIKI_PACKAGE_MANAGER_HPP_INCLUDED
#define TIKI_PACKAGE_MANAGER_HPP_INCLUDED

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

		bool							writeToFile();

		Package*						addPackage( const string& packageName );
		Package*						findPackage( const string& packageName );
		const Package*					findPackage( const string& packageName ) const;
		const LinkedList< Package >&	getPackages() const;

	private:

		Path					m_contentPath;

		ChunkedPool< Package >	m_packagePool;
		LinkedList< Package >	m_packages;
	};
}

#endif // TIKI_PACKAGE_MANAGER_HPP_INCLUDED
