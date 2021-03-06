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

#include "lispdataset.h"

#ifndef ELEMENTGRAPHITEM_H_
#define ELEMENTGRAPHITEM_H_

#include <QtWidgets>
#include <string>

#include "elementbase.h"
#include "templatecustom.h"

namespace dsim
{

template <class T>
  class ElementGraphItem : public ElementBase, public T
  {
  public:
    ElementGraphItem( int id, SchemaScene *scene, bool editable, QGraphicsItem *parent = 0l );

    virtual void move( QPointF delta );
    virtual void setVisible( bool visible );
    virtual void setSelected( bool selected );
    virtual void setStyle( const char *style );
    virtual void setDirect( ElemDirect direct );
    virtual void setLayout();

    virtual QRectF boundingRect() const;

    virtual int serialize( LispDataset *dataset );
    virtual int deserialize( LispDataset *dataset );

    void paintBound( QPainter *painter );
    void addToScene( QGraphicsScene *scene );
    void removeFromScene( QGraphicsScene *scene );

    inline const char *style() const { return m_style.c_str(); }
    inline TemplateCustom *customLine() { return &m_customLine; }
    inline TemplateCustom *customFill() { return &m_customFill; }
    inline TemplateCustom *customText() { return &m_customText; }
    inline bool editable() const { return m_editable; }
    inline ElemDirect direct() const { return m_direct; }
    inline void setFineturningEnabled( bool enable ) { m_fineturningEnabled = enable; }

    inline void setCustomLine( const TemplateCustom &custom ) { m_customLine = custom; }
    inline void setCustomFill( const TemplateCustom &custom ) { m_customFill = custom; }
    inline void setCustomText( const TemplateCustom &custom ) { m_customText = custom; }

  protected:
    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

  protected:
    bool                      m_editable;
    bool                      m_fineturningEnabled;
    ElemDirect                m_direct;
    std::string               m_style;
    TemplateCustom            m_customLine;
    TemplateCustom            m_customFill;
    TemplateCustom            m_customText;
  };

}

#include "elementgraphitemfunctions.hxx"

#endif
