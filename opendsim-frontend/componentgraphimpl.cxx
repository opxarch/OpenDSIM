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

#include <QPainter>
#include <QPixmap>

#include "templatestyle.h"
#include "elementgraphitem.h"
#include "elementpainter.h"
#include "componentgraphimpl.h"

namespace dsim
{

ComponentGraphImpl::ComponentGraphImpl()
              : m_pixBuffer( 0l )
              , m_painter( new QPainter )
              , m_elementPainter( 0l )
              , m_selected( false )
              , m_width( 0 )
              , m_height( 0 )
              , m_format( BITMAP_FORMAT_DEFAULT )
{
  createPixBuffer( m_format );
}

ComponentGraphImpl::~ComponentGraphImpl()
{
  delete m_pixBuffer;
  delete m_painter;
}

void ComponentGraphImpl::init( ElementPainter *elementPainter )
{ m_elementPainter = elementPainter; m_tokenId = elementPainter->tokenId(); }

void ComponentGraphImpl::setStyle( const char *style )
{
  Templatestyle::apply( m_painter, /*customLine*/0l, /*customFill*/0l, style, m_selected );
}

void ComponentGraphImpl::setBrushColor( int r, int g, int b )
{
  QBrush brush = m_painter->brush();
  brush.setColor( QColor(r, g, b) );
  m_painter->setBrush( brush );
}

void ComponentGraphImpl::setBrushStyle( BrushStyle style )
{
  QBrush brush = m_painter->brush();

  brush.setStyle( Templatestyle::toQtBrushStyle( style ) );
  m_painter->setBrush( brush );
}

void ComponentGraphImpl::setPenColor( int r, int g, int b )
{
  QPen pen = m_painter->pen();
  if( r<0 || g<0 || b<0 ) return;
  pen.setColor( QColor(r, g, b) );
  m_painter->setPen( pen );
}

void ComponentGraphImpl::setPenWidth( float w )
{
  QPen pen = m_painter->pen();
  if( w<0 ) return;
  pen.setWidthF( w );
  m_painter->setPen( pen );
}

void ComponentGraphImpl::setPenStyle( LineStyle style )
{
  QPen pen = m_painter->pen();

  switch( style )
  {
    case LINE_NONE: pen.setStyle( Qt::NoPen ); break;
    case LINE_SOLID: pen.setStyle( Qt::SolidLine ); break;
    case LINE_DASH: pen.setStyle( Qt::DashLine ); break;
    case LINE_DOT: pen.setStyle( Qt::DotLine ); break;
    case LINE_DASH_DOT: pen.setStyle( Qt::DashDotLine ); break;
    case LINE_DASH_DOTDOT: pen.setStyle( Qt::DashDotDotLine ); break;
    default: return;
  }

  m_painter->setPen( pen );
}

static inline QImage::Format mapBitmapFormat( BitmapFormat format )
{
  switch( format )
  {
    case BITMAP_FORMAT_MONO:        return QImage::Format_Mono;
    case BITMAP_FORMAT_MONOLSB:     return QImage::Format_MonoLSB;
    case BITMAP_FORMAT_RGB32:       return QImage::Format_RGB32;
    case BITMAP_FORMAT_ARGB32:      return QImage::Format_ARGB32;
    case BITMAP_FORMAT_RGB16:       return QImage::Format_RGB16;
    case BITMAP_FORMAT_RGB666:      return QImage::Format_RGB666;
    case BITMAP_FORMAT_RGB555:      return QImage::Format_RGB555;
    case BITMAP_FORMAT_RGB888:      return QImage::Format_RGB888;
    case BITMAP_FORMAT_DEFAULT:     return QImage::Format_RGB32;
    case BITMAP_FORMAT_INVALID:
    default:                        return QImage::Format_Invalid;
  }
}

void ComponentGraphImpl::createPixBuffer( BitmapFormat format )
{
  delete m_pixBuffer;
  m_pixBuffer = new QImage( m_width, m_height, mapBitmapFormat( format ) );

  m_format = format;
  if( format == BITMAP_FORMAT_DEFAULT )
    m_pixBuffer->fill( Qt::transparent );
  else
    m_pixBuffer->fill( 0 );
}

void ComponentGraphImpl::begin( BitmapFormat format /* = BITMAP_FORMAT_DEFAULT */ )
{
  if( m_format != format )
    createPixBuffer( format );
  m_painter->begin( m_pixBuffer );
}

void ComponentGraphImpl::point( int x, int y )
{
  m_painter->drawPoint( x, y );
}

void ComponentGraphImpl::line( int x1, int y1, int x2, int y2 )
{
  m_painter->drawLine( x1, y1, x2, y2 );
}

void ComponentGraphImpl::rect( int x, int y, int w, int h )
{
  m_painter->drawRect( x, y, w, h );
}

void ComponentGraphImpl::roundRect( int x, int y, int w, int h, int xRnd, int yRnd )
{
  m_painter->drawRoundRect( x, y, w, h, xRnd, yRnd );
}

void ComponentGraphImpl::ellipse( int x, int y, int w, int h )
{
  m_painter->drawEllipse( x, y, w, h );
}

void ComponentGraphImpl::arc( int x, int y, int w, int h, int a, int alen )
{
  m_painter->drawArc( x, y, w, h, a, alen );
}

int ComponentGraphImpl::text( const char *text, int x, int y, int r, int g, int b, bool bold, bool italic, int *height )
{
  QPen lastPen = m_painter->pen();
  if( r>0 && g>0 && b>0 )
    m_painter->setPen( QColor( r, g, b ) );
  QFont lastFont = m_painter->font();
  QFont nfont = lastFont;
  nfont.setBold( bold );
  nfont.setItalic( italic );
  m_painter->setFont( nfont );
  m_painter->drawText( QPoint(x, y), text );
  if( height )
    {
      *height = m_painter->fontMetrics().height();
    }
  m_painter->setPen( lastPen );
  m_painter->setFont( lastFont );
  return 0;
}

int ComponentGraphImpl::text( const char *text, int x, int y, int r, int g, int b, int pixsize, bool bold, bool italic, int *height )
{
  QPen lastPen = m_painter->pen();
  if( r>0 && g>0 && b>0 )
    m_painter->setPen( QColor( r, g, b ) );
  QFont lastFont = m_painter->font();
  QFont nfont = lastFont;
  nfont.setBold( bold );
  nfont.setItalic( italic );
  if( pixsize > 0 )
    nfont.setPixelSize( pixsize );
  m_painter->setFont( nfont );
  m_painter->drawText( QPoint(x, y), text );
  if( height )
    {
      *height = m_painter->fontMetrics().height();
    }
  m_painter->setPen( lastPen );
  m_painter->setFont( lastFont );
  return 0;
}

void ComponentGraphImpl::fill( int r, int g, int b )
{
  m_pixBuffer->fill( QColor( r, g, b ) );
}

void ComponentGraphImpl::setColor( int index, int r, int g, int b )
{
  m_pixBuffer->setColor( index, qRgb( r, g, b ) );
}

void ComponentGraphImpl::setPixel( int col, int row, unsigned int color )
{
  m_pixBuffer->setPixel( col, row, color );
}

void ComponentGraphImpl::end()
{
  m_painter->end();
  m_elementPainter->setPixBuffer( m_pixBuffer );
  m_elementPainter->update();
}

} // namespace dsim
