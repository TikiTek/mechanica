#pragma once
#ifndef TIKI_GENERICDATAVALUETAGEDITOR_HPP_INCLUDED
#define TIKI_GENERICDATAVALUETAGEDITOR_HPP_INCLUDED

#include <QWindow>

namespace tiki
{
	class GenericDataTag;

	class GenericDataTagEditor : public QWindow
	{
		Q_OBJECT

	public:

		GenericDataTagEditor( QWindow* pParent, GenericDataTag* pTag );
		~GenericDataTagEditor();

	private:

		GenericDataTag*		m_pTag;
	};
}

#endif // TIKI_GENERICDATAVALUETAGEDITOR_HPP_INCLUDED
