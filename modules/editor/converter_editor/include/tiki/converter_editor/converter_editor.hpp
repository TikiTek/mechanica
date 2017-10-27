#pragma once
#ifndef TIKI_CONVERTER_EDITOR_HPP_INCLUDED
#define TIKI_CONVERTER_EDITOR_HPP_INCLUDED

#include "tiki/editor_interface/ieditorextension.hpp"

#include <QObject>

namespace tiki
{
	class IEditorInterface;
	class QtRibbonButton;

	class ConverterEditor : public QObject, public IEditorExtension
	{
		Q_OBJECT
		TIKI_NONCOPYABLE_CLASS( ConverterEditor );

	public:

												ConverterEditor();
		virtual									~ConverterEditor();

	private	slots:

		void									convertAllClicked();
		void									showConverterClicked();
		void									showLastResultClicked();

	private:

		QtRibbonTab*				m_pRibbon;
		QtRibbonButton*				m_pConvertAllButton;
		QtRibbonButton*				m_pShowConverterButton;
		QtRibbonButton*				m_pShowLastResultButton;
	};
}

#endif // TIKI_CONVERTER_EDITOR_HPP_INCLUDED
