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

#include "d-lcd8544.h"

static inline void
inc_gdram_ptrs( lcd8544_param_t *param )
{
  if( param->bV )
    {
      param->addrY++;
      if( param->addrY >= 6 )
        {
          param->addrY = 0;
          param->addrX++;
        }
      if( param->addrX >= 84 )
        {
          param->addrX = 0;
        }
    }
  else
    {
      param->addrX++;
      if( param->addrX >= 84 )
        {
          param->addrX = 0;
          param->addrY++;
        }
      if( param->addrY >= 6 )
        {
          param->addrY = 0;
        }
    }
}

int
LIB_FUNC(lcd8544_event)( circ_element_t *element )
{
  logic_state_t state;
  DEFINE_PARAM(param, element, lcd8544_param_t);

  if( SIG_LOW == GET_STATE( element->pin_vector[0] ) ) /* ~RST */
    {
      lcd8544_reset( element );
      return 0;
    }
  if( SIG_HIGH == GET_STATE( element->pin_vector[1] ) ) /* CS disabled */
    {
      param->cinBuf = 0; /* Initialize serial buffer */
      param->inBit  = 0;
      return 0;
    }
  param->dc = ( GET_STATE( element->pin_vector[2] ) == SIG_HIGH ); /* D/C */

  state = GET_STATE( element->pin_vector[4] );
  if( state == SIG_LOW || state == SIG_FALL )   /* an sclk falling edge */
    {
      param->lastScl = false;
      return 0;
    }
  else if( param->lastScl ) return 0;           /* not a rising edge */
  param->lastScl = true;

  if( SIG_HIGH == GET_STATE( element->pin_vector[3] )  ) /* SDIN */
    param->cinBuf |= 1;
  else
    param->cinBuf &= ~1;

  if( param->inBit == 7 )
    {
      if( param->dc )                               /* Write Data */
        {
          param->gdram[param->addrY][param->addrX] = param->cinBuf;
          inc_gdram_ptrs( param );
        }
      else                                          /* Write Command */
        {
          //if(param->cinBuf == 0) { //(NOP) }

          if((param->cinBuf & 0xF8) == 0x20)        /* Function set */
            {
              param->bH  = ((param->cinBuf & 1) == 1);
              param->bV  = ((param->cinBuf & 2) == 2);
              param->bPD = ((param->cinBuf & 4) == 4);
            }
          else
            {
              if(param->bH)
                {
                  /* reserved for extended instruction set */
                }
              else                            /* Basic instruction set */
                {
                  if((param->cinBuf & 0xFA) == 0x08)     /* Display control */
                    {
                      param->bD = ((param->cinBuf & 0x04) == 0x04);
                      param->bE =  (param->cinBuf & 0x01);
                      mdel_logtrace( MDEL_DEBUG, ("LCD 8544 Write Command: D=%d E=%d.", param->bD, param->bE) );
                    }
                  else if((param->cinBuf & 0xF8) == 0x40) /* Set Y RAM address */
                    {
                      int addrY = param->cinBuf & 0x07;
                      if( addrY<6 ) param->addrY = addrY;
                    }
                  else if((param->cinBuf & 0x80) == 0x80) /* Set X RAM address */
                    {
                      int addrX = param->cinBuf & 0x7F;
                      if( addrX<84 ) param->addrX = addrX;
                    }
                }
            }
        }
      param->inBit = 0;
    }
  else
    {
      param->cinBuf <<= 1;
      param->inBit++;
    }

  return 0;
}
