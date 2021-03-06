/** @file
 * OpenDSIM - MainWindow
 */

/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#define TRACE_UNIT "mainwnd"
#include <dsim/logtrace.h>
#include <frontend/error.h>
#include <fstream>
#include <sstream>
#include <string>

#include <QObject>
#include <QString>
#include <QtGui>
#include <QApplication>
#include <QtWidgets>
#include <frontend/error.h>
#include <dsim/version.h>

#include "lispdataset.h"
#include "schemawidget.h"
#include "schemaeditorform.h"
#include "netlistexplorerdialog.h"
#include "templatestyle.h"

#include "mainwindow.h"

namespace dsim
{

MainWindow *MainWindow::m_pinstance = 0;

MainWindow::MainWindow()
          : m_template( new Templatestyle )
{
  setWindowTitle( appBanner );
  setWindowState( Qt::WindowMaximized );

  createActions();
  createMenuBar();
  createToolBars();
  createStatusBar();
  createWorkspace();

  readSettings();

  setSchemaEditState( SCHEDIT_INPUT );

  trace_assert( 0l==m_pinstance );
  m_pinstance = this;
}

MainWindow::~MainWindow()
{
  m_pinstance = 0;
}

void MainWindow::readSettings()
{
  restoreGeometry( settings()->value("geometry").toByteArray());
  restoreState( settings()->value("windowState").toByteArray());
}

void MainWindow::writeSettings()
{
  settings()->setValue( "geometry", saveGeometry() );
  //settings()->setValue( "windowState", saveState() );
}

void MainWindow::closeEvent( QCloseEvent *event )
{
  writeSettings();

  event->accept();
}

QSize MainWindow::sizeHint () const
{
  return QSize( 1087, 572 );
}

void MainWindow::setSchemaEditState( SchemaEditState state )
{
  m_schemaEditState = state;
  onUpdateMenus();
}

/***************************************************
  *****  Create workspace view                 *****
  ***************************************************/

void MainWindow::createActions()
{
  /*
   * File actions
   */
  fileNewProject = new QAction(QIcon((":/bitmaps/filenew.png")), tr("&New project"), this);
  fileNewProject->setShortcut(Qt::CTRL+Qt::Key_N);
  fileNewProject->setStatusTip(tr("Create a new project."));
  connect(fileNewProject, SIGNAL(triggered()), SLOT(onFileNewProject()));

  fileNewSchema = new QAction(tr("&New Schema file"), this);
  fileNewSchema->setStatusTip(tr("Create a new schema file."));
  connect(fileNewSchema, SIGNAL(triggered()), SLOT(onFileNewSchema()));

  fileNewSchemaSymbol = new QAction(tr("&New Schema Symbol file"), this);
  fileNewSchemaSymbol->setStatusTip(tr("Create a new schema symbol file."));
  connect(fileNewSchemaSymbol, SIGNAL(triggered()), SLOT(onFileNewSchemaSymbol()));

  fileOpen = new QAction(QIcon((":/bitmaps/fileopen.png")), tr("&Open"), this);
  fileOpen->setStatusTip(tr("Open a file."));
  connect(fileOpen, SIGNAL(triggered()), SLOT(onFileOpen()));

  fileSave = new QAction(QIcon((":/bitmaps/filesave.png")), tr("&Save"), this);
  fileSave->setStatusTip(tr("Save the current file."));
  connect(fileSave, SIGNAL(triggered()), SLOT(onFileSave()));

  fileQuit = new QAction(tr("&Quit"), this);
  fileQuit->setStatusTip(tr("Quit the dsim."));
  connect(fileQuit, SIGNAL(triggered()), SLOT(onFileQuit()));

  /*
   * Design actions
   */
  designViewNetlist = new QAction(tr("&View netlist..."), this);
  designViewNetlist->setStatusTip( tr("View schematic netlist...") );
  connect(designViewNetlist, SIGNAL(triggered()), SLOT(onDesignViewNetlist()));

  /*
   * Simulation actions
   */
  debugStep = new QAction(tr("&Step"), this);
  debugStep->setStatusTip( tr("Step...") );
  connect(debugStep, SIGNAL(triggered()), SLOT(onDebugStep()));

  debugRun = new QAction(tr("&Run"), this);
  debugRun->setStatusTip( tr("Run simulation...") );
  debugRun->setEnabled( false );
  connect(debugRun, SIGNAL(triggered()), SLOT(onDebugRun()));

  debugEnd = new QAction(tr("&End"), this);
  debugEnd->setStatusTip( tr("Step...") );
  debugEnd->setEnabled( false );
  connect(debugEnd, SIGNAL(triggered()), SLOT(onDebugEnd()));
}

void MainWindow::createMenuBar()
{
  /*
   * Menu entry 'file'.
   */
  fileMenu = new QMenu( tr("&File") );
  fileMenu->addAction(fileNewProject);

  QMenu *fileNewfilesMenu = new QMenu(tr("New..."));
  fileNewfilesMenu->addAction( fileNewSchema );
  fileNewfilesMenu->addAction( fileNewSchemaSymbol );
  fileMenu->addMenu(fileNewfilesMenu);
  fileMenu->addAction(fileOpen);
  fileMenu->addAction(fileSave);
  fileMenu->addSeparator();
  fileMenu->addAction(fileQuit);

  menuBar()->addMenu(fileMenu);

  /*
   * Menu entry 'design'
   */
  designMenu = new QMenu( tr("&Design") );
  designMenu->addAction( designViewNetlist );

  menuBar()->addMenu(designMenu);

  /*
   * Menu entry 'debug'
   */
  debugMenu = new QMenu( tr("&Debug") );
  debugMenu->addAction( debugRun );
  debugMenu->addAction( debugStep );
  debugMenu->addAction( debugEnd );

  menuBar()->addMenu(debugMenu);
}

void MainWindow::createToolBars()
{
  /*
   * ToolBar file.
   */
  fileToolBar = new QToolBar( tr("File") ); // toolBar file
  fileToolBar->addAction(fileNewProject);
  fileToolBar->addAction(fileOpen);
  fileToolBar->addAction(fileSave);
  fileToolBar->setIconSize(QSize(16, 16));
  this->addToolBar(fileToolBar);
}

void MainWindow::createWorkspace()
{
  /*
   * Workspace
   */
  workspace = new QMdiArea( this );
  setCentralWidget( workspace );

  connect( workspace, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onUpdateMenus()) );
  onUpdateMenus();
}

void MainWindow::createStatusBar()
{
  /*
   * StatusBar
   */
  statusLabel = new QLabel(tr("Ready..."));
  statusBar()->addWidget(statusLabel);
}

SchemaEditorForm *MainWindow::activeSchemaEditor()
{
  if( QMdiSubWindow *activeSubWindow = workspace->activeSubWindow() )
    return qobject_cast<SchemaEditorForm *>( activeSubWindow->widget() );
  return 0l;
}

/***************************************************
  *****  Slot Responding                       *****
  ***************************************************/

void MainWindow::onFileNewProject()
{
}

void MainWindow::onFileNewSchema()
{
  newSchemaDocument( DOM_SCHEMA );
}

void MainWindow::onFileNewSchemaSymbol()
{
  newSchemaDocument( DOM_SCHEMA_SYMBOL );
}

void MainWindow::onFileOpen()
{
  using namespace std;
  QString filename = QFileDialog::getOpenFileName( this, tr("Open a file"), QString(), tr("Schema & Symbol files (*.sch *.ssym)") );
  if( filename.length() )
    {
      QFileInfo file( filename );
      string fn = filename.toStdString();

      SchemaEditorForm *schema = 0l;

      if( file.suffix() == "sch" )
        {
          schema = newSchemaDocument( DOM_SCHEMA );
        }
      else if( file.suffix() == "ssym" )
        {
          schema = newSchemaDocument( DOM_SCHEMA_SYMBOL );
        }
      else
        {
          MainWindow::instance()->processRc( -DS_INVALID_FILE_TYPE );
        }

      if( schema )
        MainWindow::instance()->processRc( schema->open( fn.c_str() ) );
    }
}

void MainWindow::onFileSave()
{
  using namespace std;

  SchemaEditorForm *schemaForm = activeSchemaEditor();

  if( schemaForm )
    {
      QString filename;

      if( schemaForm->dataset()->type() == DOM_SCHEMA )
        {
          filename = QFileDialog::getSaveFileName( this, tr("Save schema..."), QString(), tr("Schema file(*.sch)") );
        }
      else if( schemaForm->dataset()->type() == DOM_SCHEMA_SYMBOL )
        {
          filename = QFileDialog::getSaveFileName( this, tr("Save symbol..."), QString(), tr("Schema Symbol file(*.ssym)") );
        }
      if( filename.length() )
        {
          std::string fn = filename.toStdString();
          MainWindow::instance()->processRc( schemaForm->save( fn.c_str() ) );
        }
    }
}

void MainWindow::onFileQuit()
{
}

void MainWindow::onDesignViewNetlist()
{
  SchemaEditorForm *schema = activeSchemaEditor();
  if( schema )
    {
      if( processRc( activeSchemaEditor()->compileNetlist() ) )
        return;
      if( (schema = activeSchemaEditor()) )
        {
          NetlistExplorerDialog exp( schema->schemaSheet()->nodes(), this );
          exp.exec();
        }
    }
}

void MainWindow::onDebugStep()
{
  SchemaEditorForm *schema = activeSchemaEditor();
  if( schema )
    {
      switch( m_schemaEditState )
      {
        case SCHEDIT_INPUT:
        case SCHEDIT_FROZEN:
          if( processRc( schema->debugStep() ) )
            setSchemaEditState( SCHEDIT_INPUT ); // failed
          else
            setSchemaEditState( SCHEDIT_FROZEN );
          break;
        default:
          break;
      }
    }
}

void MainWindow::onDebugRun()
{
  SchemaEditorForm *schema = activeSchemaEditor();
  if( schema )
    {
      switch( m_schemaEditState )
      {
        case SCHEDIT_INPUT:
          if( processRc( schema->debugRun() ) )
            setSchemaEditState( SCHEDIT_INPUT ); // failed
          else
            setSchemaEditState( SCHEDIT_RUNNING );
          break;
        case SCHEDIT_FROZEN:
          {
            if( QMessageBox::warning( this, tr("Restart the debugging"), tr("The circuit is currently running, confirm to restart the debugging ?"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes ) == QMessageBox::Yes )
              {
                schema->debugEnd();
                if( processRc( schema->debugRun() ) )
                  setSchemaEditState( SCHEDIT_INPUT ); // failed
                else
                  setSchemaEditState( SCHEDIT_RUNNING );
              }
            break;
          }
        default:
          break;
      }
    }
}

void MainWindow::onDebugEnd()
{
  SchemaEditorForm *schema = activeSchemaEditor();
  if( schema )
    {
      switch( m_schemaEditState )
      {
        case SCHEDIT_RUNNING:
        case SCHEDIT_FROZEN:
          if( processRc( schema->debugEnd() ) )
            setSchemaEditState( SCHEDIT_FROZEN );
          else
            setSchemaEditState( SCHEDIT_INPUT );
          break;
        default:
          break;
      }
    }
}

void MainWindow::onUpdateMenus()
{
  bool hasActiveSchame = ( 0l != activeSchemaEditor() );
  fileSave->setEnabled( hasActiveSchame );
  designViewNetlist->setEnabled( hasActiveSchame );

  switch( m_schemaEditState )
  {
    case SCHEDIT_INPUT:
      debugStep->setEnabled( hasActiveSchame );
      debugRun->setEnabled( hasActiveSchame );
      debugEnd->setEnabled( false );
      break;

    case SCHEDIT_RUNNING:
      debugStep->setEnabled( false );
      debugRun->setEnabled( false );
      debugEnd->setEnabled( hasActiveSchame );
      break;

    case SCHEDIT_FROZEN:
      debugStep->setEnabled( hasActiveSchame );
      debugRun->setEnabled( hasActiveSchame );
      debugEnd->setEnabled( hasActiveSchame );
      break;
  }
}


/***************************************************
  *****  Logical Functions                     *****
  ***************************************************/

SchemaEditorForm *MainWindow::newSchemaDocument( DomType type )
{
  SchemaEditorForm *schema = new SchemaEditorForm( type, workspace );

  if( !processRc( schema->init() ) )
    {
      workspace->addSubWindow( schema );

      schema->show();
      schema->gotoCenter();
      return schema;
    }
  schema->deleteLater();
  return 0l;
}

QMdiSubWindow *MainWindow::addChild( QWidget *child )
{ return workspace->addSubWindow( child ); }

void MainWindow::removeChild( QMdiSubWindow *child )
{ workspace->removeSubWindow( child ); }

/**
 * Report the error code
 * param rc status code
 */
int MainWindow::processRc( int rc )
{
  if( !rc ) return rc;

  const ds_error_info_t *error = ds_get_error( rc );

  std::stringstream ss;
  ss << "An error has occurred.\n" << "rc = " << rc << " (" << error->msgDefine << ")\nDetails: " << error->msgFull;
  std::string s = ss.str();

  QMessageBox::critical( this, tr("Fault"), s.c_str(), QMessageBox::Abort | QMessageBox::Cancel );
  return rc;
}

void MainWindow::noMemory()
{
  QMessageBox::critical( this, tr("No Memory"),
                       tr("There is no memory left and the program must be terminated unexpectedly.\n"
                          "However, we will try to backup the current document and then crash."),
                       QMessageBox::Ok );
}

} // namespace dsim

#include "mainwindow.moc"
