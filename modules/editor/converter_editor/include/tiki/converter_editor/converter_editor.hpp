#pragma once
#ifndef TIKI_CONVERTER_EDITOR_HPP_INCLUDED
#define TIKI_CONVERTER_EDITOR_HPP_INCLUDED

#include "tiki/editor_interface/ieditor.hpp"

#include <QObject>

namespace tiki
{
	class AssetConverterInterface;
	class IEditorInterface;
	class QtAssetConverterBuildWidget;
	class QtAssetConverterStatusWidget;
	class QtRibbonButton;

	class ConverterEditor : public QObject, public IEditor
	{
		Q_OBJECT
		TIKI_NONCOPYABLE_CLASS( ConverterEditor );

	public:

						ConverterEditor( IEditorInterface* pInterface, AssetConverterInterface* pAssetConverter );
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
		AssetConverterInterface*		m_pConverter;

		QIcon							m_icon;

		QtRibbonTab*					m_pRibbon;
		QtRibbonButton*					m_pConvertAllButton;
		QtRibbonButton*					m_pShowConverterButton;
		QtRibbonButton*					m_pShowLastResultButton;

		QtAssetConverterBuildWidget*	m_pBuildWidget;
		QtAssetConverterStatusWidget*	m_pStatusWidget;
	};
}

#endif // TIKI_CONVERTER_EDITOR_HPP_INCLUDED
