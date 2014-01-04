#ifndef TIKI_TOOLMATERIAL_HPP__
#define TIKI_TOOLMATERIAL_HPP__

#include "tiki/base/assert.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/list.hpp"
#include "tiki/toolbase/tikixml.hpp"

namespace tiki
{
	class ToolMaterial
	{
	public:

					ToolMaterial();
					~ToolMaterial();

		bool		create( const string& fileName );
		void		dispose();

	private:

		TikiXml		m_xml;



	};
}

#endif // TIKI_TOOLMATERIAL_HPP__
