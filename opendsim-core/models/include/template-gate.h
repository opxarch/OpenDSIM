/** @file
 * model-lib Digital gate Model
 *
 * required macro definitions:
 *  D_GATE_NAME   generate the symbol of gate functions
 *  D_GATE_OPERATOR operator code
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

#include "mdel-lib-internal.h"

#ifndef D_GATE_NAME
#define D_GATE_NAME(name) or_ ## name
#endif
#ifndef D_GATE_OPERATOR
#define D_GATE_OPERATOR(inputs, sum) (inputs > 0)
#endif

#define FNN(name) D_GATE_NAME(name)

typedef struct
{
  int inputs_count;
  bool reversed;
} d_gate_param_t;

static int
LIB_FUNC(FNN(create))( circ_element_t *element )
{
  DEFINE_PARAM(param, element, d_gate_param_t);

  circ_element_set_digital_pin( element, 0, circ_element_get_pin_count(element)-1 );
  param->inputs_count = 2;
  param->reversed = false;
  return 0;
}

static int
LIB_FUNC(FNN(init))( circ_element_t *element )
{
  DEFINE_PARAM(param, element, d_gate_param_t);
  int rc = 0;

  circ_element_set_digital_pin( element, 0, param->inputs_count );

  for( int i=0; i < param->inputs_count; i++ )
    {
      if( element->pin_vector[i]->connected )
        if( (rc = circ_node_add_logic( PINNODE(element, i), element )) )
          return rc;
    }

  return 0;
}

static int
LIB_FUNC(FNN(event))( circ_element_t *element )
{
  int inputs = 0;
  logic_state_t state;
  DEFINE_PARAM(param, element, d_gate_param_t);

  for( int i=0; i < param->inputs_count; i++ )
    {
      state = GET_STATE( element->pin_vector[i] );
      if( logic_high( state ) ) inputs++;
    }
  if( param->reversed )
    state = D_GATE_OPERATOR(inputs, param->inputs_count) ? SIG_LOW : SIG_HIGH;
  else
    state = D_GATE_OPERATOR(inputs, param->inputs_count) ? SIG_HIGH : SIG_LOW;
  return circ_pin_set_state( element->pin_vector[param->inputs_count], state );
}

/* configuration procedure */
static int
LIB_FUNC(FNN(config))( circ_element_t *element, int op, ... )
{
  int rc = 0;
  DEFINE_PARAM(param, element, d_gate_param_t);

  va_list vlist;
  va_start( vlist, op );

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT:
      {
        int *count = va_arg( vlist, int* );
        *count = 2;
      }
      break;

    case ELM_CONFIG_LIST_PROP:
      {
        const model_variable_prop_t **prop = va_arg( vlist, const model_variable_prop_t ** );
        static model_variable_prop_t prop_list[] =
            {
              { "inc", "Number of Input ports", MDEL_VAR_INTEGER },
              { "rev", "Reversed output", MDEL_VAR_BOOL }
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: *(va_arg( vlist, int* )) = param->inputs_count; break;
          case 1: *(va_arg( vlist, int* )) = (int)param->reversed; break;
          default:
            rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: /* Input pins count */
            {
              param->inputs_count = va_arg( vlist, int );
              if( (rc = circ_element_set_pins( element, param->inputs_count+1 )) )
                break;
              circ_element_set_digital_pin( element, 0, param->inputs_count );
              break;
            }
          case 1: /* Reversed output */
            {
              param->reversed = (bool)(va_arg( vlist, int ));
              break;
            }
          default:
            rc = -DS_FAULT;
        }
      }
      break;

    default:
      rc = -DS_UNIMPLEMENTED;
  }

  va_end( vlist );
  return rc;
}

static void
LIB_FUNC(FNN(uninit))( circ_element_t *element )
{
  (void)element;
}
