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

#ifndef SCHEMAWIDGET_H_
#define SCHEMAWIDGET_H_

#include <QVBoxLayout>
#include <QWidget>

#include "schemaview.h"

namespace dsim
{

class SchemaScene;
class SchemaSheet;
class LispDataset;
class DomItem;

class SchemaWidget : public QWidget
{
  Q_OBJECT

public:
  SchemaWidget( QWidget *parent, SchemaSheet *sheet, LispDataset *dataset );
  ~SchemaWidget();

  void clear();
  void setMode( DrawMode mode );

public:
  inline SchemaView *view() const { return m_view; }

private:
  QVBoxLayout  *m_verticalLayout;
  SchemaView  *m_view;
  SchemaSheet *m_sheet;
  LispDataset  *m_dom;
};

} // namespace dsim

#endif
