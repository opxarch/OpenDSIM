#
#  OpenDSIM (A/D mixed circuit simulator)
#  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
#
#  This project is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License(GPL)
#  as published by the Free Software Foundation; either version 2.1
#  of the License, or (at your option) any later version.
#
#  This project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#

SUBDIRS = devices

INCLUDES = -I$(top_builddir)/include      \
           -I$(top_srcdir)/include        \
           -I$(top_builddir)/opendsim-core  \
           -I$(top_builddir)/opendsim-frontend \
           -I./include \
           $(QT_INCLUDES)

bin_PROGRAMS = opendsim

opendsim_SOURCES = \
  opendsim-edit.cxx \
  componentgraphimpl.cxx \
  schematicimpl.cxx \
  lispdataset.cxx \
  domitem.cxx \
  idallocator.cxx \
  schemasheet.cxx \
  templatestyle.cxx \
  templatecustom.cxx \
  transformations.cxx \
  boundingtraits.cxx \
  elementbase.cxx \
  componentgraphitem.cxx \
  elementgraphitem.cxx \
  elementwire.cxx \
  elementpin.cxx \
  elementtext.cxx \
  elementrect.cxx \
  elementline.cxx \
  elementjoint.cxx \
  elementellipse.cxx \
  elementabstractport.cxx \
  staffgraphitem.cxx \
  staffpad.cxx \
  schemaviewactions.cxx \
  schemaviewmenus.cxx \
  mainwindow.cxx \
  schemawidget.cxx \
  schemaview.cxx \
  schemascene.cxx \
  schemaeditorform.cxx \
  componentpickwidget.cxx \
  componentpicktree.cxx \
  componentlibrarydialog.cxx \
  textsettingsdialog.cxx \
  pinsettingsdialog.cxx \
  propertiescustomdialog.cxx \
  presscombobox.cxx \
  resources.cpp
  
MOCHEADERS = \
  mainwindow.h \
  schemawidget.h \
  schemaview.h \
  schemascene.h \
  schemaeditorform.h \
  componentpickwidget.h \
  componentpicktree.h \
  componentlibrarydialog.h \
  textsettingsdialog.h \
  pinsettingsdialog.h \
  propertiescustomdialog.h \
  presscombobox.h

MOCFILES = $(MOCHEADERS:.h=.moc)

resources.cpp: resources.qrc
	$(RCC) -o $@ $<

%.moc: %.h
	$(MOC) -o $@ $<

opendsim_LDFLAGS = $(QT_LDFLAGS)
opendsim_LDADD = devices/libdsimdevices.la $(top_srcdir)/opendsim-core/libdsim.la $(top_srcdir)/opendsim-core/models/libdsimmodels.la $(QT_LIBS)
opendsim_DEPENDENCIES = devices/libdsimdevices.la $(top_srcdir)/opendsim-core/models/libdsimmodels.la

BUILT_SOURCES = $(MOCFILES)
CLEANFILES = $(MOCFILES)