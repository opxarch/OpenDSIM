/** @file
 * device-lib 1N4148 Diode
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

#include "1n4148.h"

DeviceLibraryEntry *dev_1n4148::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "1n4148",
      /* category */        "Diodes",
      /* sub_category */    "Switching",
      /* description */     "1N4148 Diode",
      /* manufacturer */    "Diodes",
      dev_1n4148::construct,
      0l);
}

IDevice *dev_1n4148::construct( const char *reference, int id, circuit_t *circuit, void *reserved )
{
  return new dev_1n4148( reference, id, circuit, reserved );
}

dev_1n4148::dev_1n4148( const char *reference, int id, circuit_t *circuit, void *reserved )
  : IDevice( reference, id, circuit, reserved )
{
}

int dev_1n4148::create()
{
  return IDevice::baseinit( "diode" );
}

int dev_1n4148::init()
{
  return circ_element_config( (m_mdel, ELM_CONFIG_SET, /*vz*/2, 0));
}

struct IRECT dev_1n4148::get_bound()
{
  return IRECT( -12, -8, 24, 16 );
}

int dev_1n4148::render_frame( IDeviceGraph *graph )
{
  graph->setStyle("component");

  graph->line( 7, 0, -8, -7 );
  graph->line( -8, -7, -8, 7 );
  graph->line( -8, 7, 7, 0 );

  graph->setPenWidth( 3 );

  graph->line( 7, -6, 7, 6 );
  return 0;
}

int dev_1n4148::uninit()
{
  return 0;
}
