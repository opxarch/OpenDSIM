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

#include <cstring>

#include "schemagraph.h"
#include "elementgraphitem.h"
#include "componentgraphitem.h"
#include "elementline.h"
#include "elementtext.h"
#include "elementpin.h"
#include "elementrect.h"
#include "elementellipse.h"
#include "propertiescustomdialog.h"

#include "schemaview.h"


namespace dsim
{

void SchemaView::createContextMenus()
{
  m_actionEditProperties = new QAction( tr("Edit properties..."), this );

  connect( m_actionEditProperties, SIGNAL(triggered(bool)), this, SLOT(onEditProperties(bool)) );
}

void SchemaView::contextMenuEvent( QContextMenuEvent* event )
{
  QTransform transform;
  m_selectedElements = elementbase_cast( m_schemaGraph->itemAt( mapToScene( event->pos() ), transform ) );

  if( m_selectedElements )
    {
      QMenu *contextMenu = new QMenu( this );
      if( 0==std::strcmp( m_selectedElements->classname(), "line" ) )
        {
        }
      else if( 0==std::strcmp( m_selectedElements->classname(), "rect" ) )
        {
        }
      if( 0==std::strcmp( m_selectedElements->classname(), "ellipse" ) )
        {
        }
      else if( 0==std::strcmp( m_selectedElements->classname(), "text" ) )
        {
        }
      contextMenu->addAction( m_actionEditProperties ); // general menu part
      contextMenu->exec( event->globalPos() );

      contextMenu->deleteLater();
    }

  m_selectedElements = 0l;
}

template <typename base>
  static void configLineFillText( PropertiesCustomDialog *customDialog, base *element )
  {
    customDialog->loadTemplate( element->style(), element->customLine(), element->customFill(), element->customText() );

    if( customDialog->exec() == QDialog::Accepted )
      {
        element->setCustomLine( customDialog->lineCustom() );
        element->setCustomFill( customDialog->fillCustom() );
        element->setCustomText( customDialog->textCustom() );
      }
  }

template <typename base>
  static void configText( PropertiesCustomDialog *customDialog, base *element )
  {
    customDialog->loadTemplate( element->style(), 0l, 0l, element->customText() );

    if( customDialog->exec() == QDialog::Accepted )
      {
        element->setCustomText( customDialog->textCustom() );
      }
  }

// ------------------------------------------------------------------ //
// General actions for property edition
// ------------------------------------------------------------------ //

void SchemaView::onEditProperties( bool checked )
{
  Q_UNUSED( checked );
  PropertiesCustomDialog customDialog;

  if( 0==std::strcmp( m_selectedElements->classname(), "line" ) )
    {
      configLineFillText( &customDialog, static_cast<ElementLine*>(m_selectedElements) );
    }
  else if( 0==std::strcmp( m_selectedElements->classname(), "rect" ) )
    {
      configLineFillText( &customDialog, static_cast<ElementRect*>(m_selectedElements) );
    }
  else if( 0==std::strcmp( m_selectedElements->classname(), "ellipse" ) )
    {
      configLineFillText( &customDialog, static_cast<ElementEllipse*>(m_selectedElements) );
    }
  else if( 0==std::strcmp( m_selectedElements->classname(), "text" ) )
    {
      configText( &customDialog, static_cast<ElementText*>(m_selectedElements) );
    }
}

}