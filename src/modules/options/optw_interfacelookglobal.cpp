//
//   File : optw_interfacelookglobal.cpp
//   Creation date : Sun Jun 3 2001 20:50:13 CEST by Szymon Stefanek
//
//   This file is part of the KVirc irc client distribution
//   Copyright (C) 2001 Szymon Stefanek (pragma at kvirc dot net)
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your opinion) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc. ,59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include "optw_interfacelookglobal.h"

#include "kvi_settings.h"
#include "kvi_options.h"
#include "kvi_locale.h"

#include <QLayout>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

KviThemeGeneralOptionsWidget::KviThemeGeneralOptionsWidget(QWidget * parent)
: KviOptionsWidget(parent)
{
	createLayout();

	KviBoolSelector * b = addBoolSelector(0,1,1,1,__tr2qs_ctx("Use global application font","options"),KviOption_boolUseGlobalApplicationFont);
	KviBoolSelector * g = addBoolSelector(0,0,1,0,__tr2qs_ctx("Hide icons in Popup","options"),KviOption_boolDisablePopupIcons);
	KviFontSelector * f = addFontSelector(0,2,1,2,__tr2qs_ctx("Global application font:","options"),KviOption_fontApplication,
			KVI_OPTION_BOOL(KviOption_boolUseGlobalApplicationFont));

	connect(b,SIGNAL(toggled(bool)),f,SLOT(setEnabled(bool)));

	addRowSpacer(0,3,1,3);
}

KviThemeGeneralOptionsWidget::~KviThemeGeneralOptionsWidget()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


KviThemeTransparencyOptionsWidget::KviThemeTransparencyOptionsWidget(QWidget * parent)
: KviOptionsWidget(parent)
{
#ifdef COMPILE_PSEUDO_TRANSPARENCY

	#ifdef COMPILE_KDE_SUPPORT
		createLayout();
	#else
		createLayout();
	#endif

	m_pUseTransparencyBoolSelector = addBoolSelector(0,0,1,0,__tr2qs_ctx("Enable fake transparency","options"),KviOption_boolUseGlobalPseudoTransparency);
	#ifdef COMPILE_KDE_SUPPORT
		mergeTip(m_pUseTransparencyBoolSelector,
			__tr2qs_ctx("<center>This option makes all KVIrc windows look " \
				"transparent.<br>You must choose a blending " \
				"background image to below or check the " \
				"\"Use KDE desktop for transparency\" option.</center>","options"));
	#else
		mergeTip(m_pUseTransparencyBoolSelector,
			__tr2qs_ctx("<center>This option makes all KVIrc windows look " \
				"like transparent.<br>You must choose a blending " \
				"background image to below.</center>","options"));
	#endif

	KviUIntSelector * u = addUIntSelector(0,1,1,1,__tr2qs_ctx("Child window opacity:","options"),KviOption_uintGlobalTransparencyChildFadeFactor,
			0,100,35,KVI_OPTION_BOOL(KviOption_boolUseGlobalPseudoTransparency));
	connect(m_pUseTransparencyBoolSelector,SIGNAL(toggled(bool)),u,SLOT(setEnabled(bool)));

	u = addUIntSelector(0,2,1,2,__tr2qs_ctx("Parent window opacity:","options"),KviOption_uintGlobalTransparencyParentFadeFactor,
			0,100,10,KVI_OPTION_BOOL(KviOption_boolUseGlobalPseudoTransparency));
	connect(m_pUseTransparencyBoolSelector,SIGNAL(toggled(bool)),u,SLOT(setEnabled(bool)));

	KviColorSelector * c = addColorSelector(0,3,1,3,__tr2qs_ctx("Blend color:","options"),KviOption_colorGlobalTransparencyFade,
			KVI_OPTION_BOOL(KviOption_boolUseGlobalPseudoTransparency));
	connect(m_pUseTransparencyBoolSelector,SIGNAL(toggled(bool)),c,SLOT(setEnabled(bool)));

//	addRowSpacer(0,13,0,13);
#ifdef COMPILE_KDE_SUPPORT
	m_pObtainBackgroundFromKdeBoolSelector = addBoolSelector(0,4,1,4,__tr2qs_ctx("Use KDE desktop for transparency","options"),KviOption_boolObtainGlobalBackgroundFromKde,
			KVI_OPTION_BOOL(KviOption_boolUseGlobalPseudoTransparency));
	connect(m_pUseTransparencyBoolSelector,SIGNAL(toggled(bool)),m_pObtainBackgroundFromKdeBoolSelector,SLOT(setEnabled(bool)));
	connect(m_pObtainBackgroundFromKdeBoolSelector,SIGNAL(toggled(bool)),this,SLOT(enableUpdateKdeBackgroundBoolSelector(bool)));

	m_pUpdateKdeBackgroundOnChangeBoolSelector = addBoolSelector(0,5,1,5,__tr2qs_ctx("Keep in sync with KDE background changes","options"),KviOption_boolUpdateKdeBackgroundOnChange,
			KVI_OPTION_BOOL(KviOption_boolUseGlobalPseudoTransparency) && KVI_OPTION_BOOL(KviOption_boolObtainGlobalBackgroundFromKde));

	m_pGlobalBackgroundPixmapSelector = addPixmapSelector(0,6,1,6,__tr2qs_ctx("Transparency blend image:","options"),KviOption_pixmapGlobalTransparencyBackground,
			KVI_OPTION_BOOL(KviOption_boolUseGlobalPseudoTransparency) && !KVI_OPTION_BOOL(KviOption_boolObtainGlobalBackgroundFromKde));
	layout()->setRowStretch(6,1);
	connect(m_pObtainBackgroundFromKdeBoolSelector,SIGNAL(toggled(bool)),this,SLOT(enableGlobalBackgroundPixmapSelector(bool)));

#else //!COMPILE_KDE_SUPPORT
	m_pGlobalBackgroundPixmapSelector = addPixmapSelector(0,4,1,4,__tr2qs_ctx("Transparency blend image:","options"),KviOption_pixmapGlobalTransparencyBackground,
			KVI_OPTION_BOOL(KviOption_boolUseGlobalPseudoTransparency));
	layout()->setRowStretch(4,1);
#endif //!COMPILE_KDE_SUPPORT
	connect(m_pUseTransparencyBoolSelector,SIGNAL(toggled(bool)),this,SLOT(enableGlobalBackgroundPixmapSelector(bool)));
	connect(m_pUseTransparencyBoolSelector,SIGNAL(toggled(bool)),this,SLOT(enableUpdateKdeBackgroundBoolSelector(bool)));
#else
	createLayout();
	addRowSpacer(0,0,0,0);
#endif
}

KviThemeTransparencyOptionsWidget::~KviThemeTransparencyOptionsWidget()
{
}

void KviThemeTransparencyOptionsWidget::enableGlobalBackgroundPixmapSelector(bool)
{
#ifdef COMPILE_PSEUDO_TRANSPARENCY
	#ifdef COMPILE_KDE_SUPPORT
		m_pGlobalBackgroundPixmapSelector->setEnabled(m_pUseTransparencyBoolSelector->isChecked() && !(m_pObtainBackgroundFromKdeBoolSelector->isChecked()));
	#else
		m_pGlobalBackgroundPixmapSelector->setEnabled(m_pUseTransparencyBoolSelector->isChecked());
	#endif
#endif
}

void KviThemeTransparencyOptionsWidget::enableUpdateKdeBackgroundBoolSelector(bool)
{
#ifdef COMPILE_PSEUDO_TRANSPARENCY
	#ifdef COMPILE_KDE_SUPPORT
		m_pUpdateKdeBackgroundOnChangeBoolSelector->setEnabled(m_pUseTransparencyBoolSelector->isChecked() && m_pObtainBackgroundFromKdeBoolSelector->isChecked());
	#endif
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


KviThemeMdiCaptionsOptionsWidget::KviThemeMdiCaptionsOptionsWidget(QWidget * parent)
: KviOptionsWidget(parent)
{
	createLayout();

	addColorSelector(0,0,0,0,__tr2qs_ctx("Active Background","options"),KviOption_colorMdiCaptionActive);
	addColorSelector(0,1,0,1,__tr2qs_ctx("Inactive Background","options"),KviOption_colorMdiCaptionInactive);
	addColorSelector(0,2,0,2,__tr2qs_ctx("Active Text (Primary)","options"),KviOption_colorCaptionTextActive);
	addColorSelector(0,3,0,3,__tr2qs_ctx("Active Text (Secondary)","options"),KviOption_colorCaptionTextActive2);
	addColorSelector(0,4,0,4,__tr2qs_ctx("Inactive Text (Primary)","options"),KviOption_colorCaptionTextInactive);
	addColorSelector(0,5,0,5,__tr2qs_ctx("Inactive Text (Secondary)","options"),KviOption_colorCaptionTextInactive2);

	addRowSpacer(0,6,0,6);
}

KviThemeMdiCaptionsOptionsWidget::~KviThemeMdiCaptionsOptionsWidget()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


KviThemeMdiAreaOptionsWidget::KviThemeMdiAreaOptionsWidget(QWidget * parent)
: KviOptionsWidget(parent)
{
	createLayout();

	addColorSelector(0,0,0,0,__tr2qs_ctx("Background","options"),KviOption_colorMdiBackground);
	addPixmapSelector(0,1,0,1,__tr2qs_ctx("Background Image","options"),KviOption_pixmapMdiBackground);

	layout()->setRowStretch(1,1);
}

KviThemeMdiAreaOptionsWidget::~KviThemeMdiAreaOptionsWidget()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


KviThemeToolBarAppletsOptionsWidget::KviThemeToolBarAppletsOptionsWidget(QWidget * parent)
: KviOptionsWidget(parent)
{
	createLayout();
	
	addFontSelector(0,0,0,0,__tr2qs_ctx("Font","options"),KviOption_fontIrcToolBarApplet);
	
	addColorSelector(0,1,0,1,__tr2qs_ctx("Background","options"),KviOption_colorIrcToolBarAppletBackground);
	addColorSelector(0,2,0,2,__tr2qs_ctx("Low-Contrast Foreground","options"),KviOption_colorIrcToolBarAppletForegroundLowContrast);
	addColorSelector(0,3,0,3,__tr2qs_ctx("Mid-Contrast Foreground","options"),KviOption_colorIrcToolBarAppletForegroundMidContrast);
	addColorSelector(0,4,0,4,__tr2qs_ctx("Active High-Contrast Foreground (Primary)","options"),KviOption_colorIrcToolBarAppletForegroundHighContrastActive1);
	addColorSelector(0,5,0,5,__tr2qs_ctx("Active High-Contrast Foreground (Secondary)","options"),KviOption_colorIrcToolBarAppletForegroundHighContrastActive2);
	addColorSelector(0,6,0,6,__tr2qs_ctx("Inactive High-Contrast Foreground (Primary)","options"),KviOption_colorIrcToolBarAppletForegroundHighContrastInactive1);
	addColorSelector(0,7,0,7,__tr2qs_ctx("Inactive High-Contrast Foreground (Secondary)","options"),KviOption_colorIrcToolBarAppletForegroundHighContrastInactive2);

	addPixmapSelector(0,8,0,8,__tr2qs_ctx("Background Image","options"),KviOption_pixmapIrcToolBarAppletBackground);

	layout()->setRowStretch(8,1);
}

KviThemeToolBarAppletsOptionsWidget::~KviThemeToolBarAppletsOptionsWidget()
{
}


#ifndef COMPILE_USE_STANDALONE_MOC_SOURCES
#include "m_optw_interfacelookglobal.moc"
#endif //!COMPILE_USE_STANDALONE_MOC_SOURCES
