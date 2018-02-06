// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	1.0.0
 */

#include "expRequired.h"
#include "expTrs.h"

namespace EXP_NAMESPACE
{
  namespace Trs
  {
    static const char kAxisChar[4] = { 'X', 'Y', 'Z', 'W' };

    static void PrintAxisFn1(AxisElement<4>& axis, AxisTransformFunction<4>& fn, size_t idx)
    {
      char axisChar = kAxisChar[(size_t) axis.axis[idx]];
      const char* sign = axis.sign[idx] < 0.0f ? " -" : " ";

      switch(fn.op[idx])
      {
        case Operator::Zero:      STEXP_PRINT_INFO_PART(" 0");  break;
        case Operator::One:       STEXP_PRINT_INFOF_PART("%s1", sign);  break;
        case Operator::Value:     STEXP_PRINT_INFOF_PART("%s%c", sign, axisChar);  break;
        case Operator::Constant:  STEXP_PRINT_INFOF_PART("%s%0.0f", sign, fn.k[idx]);  break;
        case Operator::Add:       STEXP_PRINT_INFOF_PART("%s(%c + %0.0f)", sign, axisChar, fn.k[idx]);  break;
        case Operator::Subtract:  STEXP_PRINT_INFOF_PART("%s(%c - %0.0f)", sign, axisChar, fn.k[idx]);  break;
        case Operator::Multiply:  STEXP_PRINT_INFOF_PART("%s(%c * %0.0f)", sign, axisChar, fn.k[idx]);  break;
        case Operator::Divide:    STEXP_PRINT_INFOF_PART("%s(%c / %0.0f)", sign, axisChar, fn.k[idx]);  break;
        case Operator::Negate:    STEXP_PRINT_INFOF_PART("%s(-%c)", sign, axisChar);  break;
      }
    }

    static void PrintAxisFn3(const char* name, const char* axisName, AxisElement<4>& axis, AxisTransformFunction<4>& fn)
    {
      STEXP_PRINT_INFOF_BEGIN();

      STEXP_PRINT_INFOF_PART("Settings.%s.%s = ", name, axisName);
      PrintAxisFn1(axis, fn, 0);
      STEXP_PRINT_INFO_PART(",");
      PrintAxisFn1(axis, fn, 1);
      STEXP_PRINT_INFO_PART(",");
      PrintAxisFn1(axis, fn, 2);
      STEXP_PRINT_INFOF_END();
    }

    static void PrintAxisFn4(const char* name, const char* axisName, AxisElement<4>& axis, AxisTransformFunction<4>& fn)
    {
      STEXP_PRINT_INFOF_BEGIN();

      STEXP_PRINT_INFOF_PART("Settings.%s.%s = ", name, axisName);
      PrintAxisFn1(axis, fn, 0);
      STEXP_PRINT_INFO_PART(",");
      PrintAxisFn1(axis, fn, 1);
      STEXP_PRINT_INFO_PART(",");
      PrintAxisFn1(axis, fn, 2);
      STEXP_PRINT_INFO_PART(",");
      PrintAxisFn1(axis, fn, 3);
      STEXP_PRINT_INFOF_END();
    }
    void TrsSystem::Print(Node node)
    {
      switch(node)
      {
        case Node::Transform:
        {
          PrintAxisFn3("Transform", "Translation", axis[0], fn[0]);
          PrintAxisFn4("Transform", "Rotational", axis[1], fn[1]);
          PrintAxisFn3("Transform", "Scale", axis[2], fn[2]);
        }
        break;
        case Node::Mesh:
        {
          PrintAxisFn3("Mesh", "Vertex", axis[0], fn[0]);
          PrintAxisFn3("Mesh", "Normal", axis[1], fn[1]);
        }
        break;
      }


    }
  }
}
