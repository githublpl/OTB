/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef mvdWrapperQtWidgetView_h
#define mvdWrapperQtWidgetView_h

//
// Configuration include.
//// Included at first position before any other ones.
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "ConfigureMonteverdi.h"
#endif //tag=QT4-boost-compatibility

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>
#include <QShortcut>

//
// OTB includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperQtWidgetView.h"
#include "otbWrapperQtWidgetModel.h"
#endif //tag=QT4-boost-compatibility
#include "OTBMonteverdiGUIExport.h"

//
// Monteverdi includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdTypes.h"
#endif //tag=QT4-boost-compatibility

namespace mvd
{
namespace Wrapper
{

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ApplicationLauncher
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */

class OTBMonteverdiGUI_EXPORT QtWidgetView :
    public otb::Wrapper::QtWidgetView
{
  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /**
   */
  static char const * const OBJECT_NAME;

  /** \brief Constructor. */
  QtWidgetView( const otb::Wrapper::Application::Pointer & otbApp,
		QWidget* p =0,
		Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  ~QtWidgetView() override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  void OTBApplicationOutputImageChanged( const QString &, const QString &);

  void ExecutionDone( int nbOutputs );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  QWidget* CreateInputWidgets() override;

protected slots:

  /** extend the behaviour of base class OnExecButtonClicked */
  void OnExecButtonClicked();

  /** modify the behaviour of base class OnExceptionRaised
   */
  void OnExceptionRaised( QString what );

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  QtWidgetView(const QtWidgetView&); //purposely not implemented
  void operator=(const QtWidgetView&); //purposely not implemented

  /**
   */
  void SetupParameterWidgets( QWidget* widget );

  /**
   */
  void SetupFileSelectionWidget( QWidget * );

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:

  // slot in charge of emitting a signal to the catalogue mainWindow.
  // when received, the main application need to get the output
  // image filename{s} set by the user in this OTB application (if any).
  void OnApplicationExecutionDone( int );

  /**
   */
  inline void OnFileSelectionWidgetAdded0( QWidget * );

  /**
   */
  inline void OnFileSelectionWidgetAdded1( QWidget * );
};

} // end namespace 'Wrapper'

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

namespace Wrapper
{

/*******************************************************************************/
inline
void
QtWidgetView
::OnFileSelectionWidgetAdded0( QWidget * widget )
{
  SetupFileSelectionWidget( widget );
}

/*******************************************************************************/
inline
void
QtWidgetView
::OnFileSelectionWidgetAdded1( QWidget * widget )
{
  SetupFileSelectionWidget( widget );
}

} // end namespace 'Wrapper'

} // end namespace 'mvd'

#endif
