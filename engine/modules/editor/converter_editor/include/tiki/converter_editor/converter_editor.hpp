#pragma once
#ifndef TIKI_CONVERTER_EDITOR_HPP_INCLUDED
#define TIKI_CONVERTER_EDITOR_HPP_INCLUDED

#include "tiki/editor_interface/ieditor.hpp"

#include <QObject>

namespace tiki
{
	class IAssetConverter;
	class IEditorInterface;
	class QtAssetConverterBuildWidget;
	class QtAssetConverterWidget;
	class QtRibbonButton;

	class ConverterEditor : public QObject, public IEditor
	{
		Q_OBJECT
		TIKI_NONCOPYABLE_CLASS( ConverterEditor );

	public:

						ConverterEditor( IEditorInterface* pInterface, IAssetConverter* pAssetConverter );
		virtual			~ConverterEditor();

		virtual bool	saveEditable( IEditable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void	closeEditable( IEditable* pEditable ) TIKI_OVERRIDE_FINAL;

		virtual QIcon	getEditableIcon() const TIKI_OVERRIDE_FINAL;

	private	slots:

		void			convertAllClicked();
		void			showConverterClicked();
		void			showLastResultClicked();

	private:

		IEditorInterface*				m_pInterface;
		IAssetConverter*				m_pConverter;

		QIcon							m_icon;

		QtRibbonTab*					m_pRibbon;
		QtRibbonButton*					m_pConvertAllButton;
		QtRibbonButton*					m_pShowConverterButton;
		QtRibbonButton*					m_pShowLastResultButton;

		QtAssetConverterWidget*			m_pConverterWidget;
		QtAssetConverterBuildWidget*	m_pAssetBuildWidget;
	};
}

#endif // TIKI_CONVERTER_EDITOR_HPP_INCLUDED
