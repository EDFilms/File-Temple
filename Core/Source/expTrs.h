// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * expTrs.h
 *
 * Functions and classes to transform cartesian coordinate systems into other kinds
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once
#include "expMath.h"

namespace EXP_NAMESPACE
{
  namespace Trs
  {
    /**
     * A system to transform a default coordinate system into a different kind
     */
    struct TrsSystem
    {
      AxisElement<4>            axis[kNodePropertyCount];
      AxisTransformFunction<4>  fn[kNodePropertyCount];

      TrsSystem()
      {
        for(size_t i=0;i < kNodePropertyCount;i++)
        {
          axis[i].Set(Axis::X, Axis::Y, Axis::Z, Axis::Z, 1.0f, 1.0f, 1.0f, 1.0f);
          fn[i].Set(Operator::Value, 0.0f, Operator::Value, 0.0f, Operator::Value, 0.0f, Operator::Value, 0.0f);
        }
      }

      /**
       * Print out to stdout the transform information for a Node i.e. Transform, Bone or Mesh
       */
      void Print(Node node);

      /**
       * Configure the swizzle of a node property
       */
      void ConfigureSwizzle(size_t nodeProperty, Axis src, Axis dst, s32 sign_)
      {
        axis[nodeProperty].axis[(size_t) src] = dst;
        axis[nodeProperty].sign[(size_t) src] = sign_ >= 0 ? 1.0f : -1.0f;
      }

      /***
       * Configure the swizzle of multiple node properties through a bitmask.
       * Where:
       *    1 - Translation,Vertex
       *    2 - Rotation, Normal
       *    3 - Scale
       */
      void ConfigureSwizzleFromMask(stEnum32 trsMask, Axis srcAxis, Axis dstAxis, s32 sign)
      {
        if ((trsMask & 1) != 0) // Translation, Vertex
          ConfigureSwizzle(0, srcAxis, dstAxis, sign);
        if ((trsMask & 2) != 0) // Rotation, Normal
          ConfigureSwizzle(1, srcAxis, dstAxis, sign);
        if ((trsMask & 4) != 0) // Scale
          ConfigureSwizzle(2, srcAxis, dstAxis, sign);
      }

      /**
       * Configure function operation for a node property
       */
      void ConfigureFunction(size_t nodeProperty, Axis dstAxis, Operator op, f32 constant)
      {
        fn[nodeProperty].op[(size_t) dstAxis] = op;
        fn[nodeProperty].k[(size_t) dstAxis]  = constant;
      }

      /**
       * Configure function operation for multiple node properties through a bitmask
       * Where:
       *    1 - Translation,Vertex
       *    2 - Rotation, Normal
       *    3 - Scale
       */
      void ConfigureFunctionFromMask(stEnum32 trsMask, Axis dstAxis, Operator op, f32 constant)
      {
        if ((trsMask & 1) != 0) // Translation, Vertex
          ConfigureFunction(0, dstAxis, op, constant);
        if ((trsMask & 2) != 0) // Rotation, Normal
          ConfigureFunction(1, dstAxis, op, constant);
        if ((trsMask & 4) != 0) // Scale
          ConfigureFunction(2, dstAxis, op, constant);
      }

      /**
       * Transform a 2-component node property from a source TrsSystem to a destination TrsSystem
       * Where:
       *  Vdst = [            e0,            e1 ]  Vsrc
       *       = [            ei,            ej ]  Swizzle
       *       = [          � ei,          � ej ]  Maybe Negate
       *       = [ fn0(� ei, k0), fn1(� ej, k1) ]  Apply Functions
       */
      template<size_t NODE_PROPERTY, typename TDST, typename TSRC>
      void Transform2(TDST& dst, const TSRC& src)
      {
        dst.x = TransformValue(fn[NODE_PROPERTY].op[0], src.AxisValue(axis[NODE_PROPERTY].axis[0]) * axis[NODE_PROPERTY].sign[0], fn[NODE_PROPERTY].k[0]);
        dst.y = TransformValue(fn[NODE_PROPERTY].op[1], src.AxisValue(axis[NODE_PROPERTY].axis[1]) * axis[NODE_PROPERTY].sign[1], fn[NODE_PROPERTY].k[1]);
      }
      /**
       * Transform a 3-component node property from a source TrsSystem to a destination TrsSystem
       * Where:
       *  Vdst = [            e0,            e1,            e2 ]  Vsrc
       *       = [            ei,            ej,            ek ]  Swizzle
       *       = [          � ei,          � ej,          � ek ]  Maybe Negate
       *       = [ fn0(� ei, k0), fn1(� ej, k1), fn2(� ek, k2) ]  Apply Functions
       */
      template<size_t NODE_PROPERTY, typename TDST, typename TSRC>
      void Transform3(TDST& dst, const TSRC& src)
      {
        dst.x = TransformValue(fn[NODE_PROPERTY].op[0], src[(size_t) axis[NODE_PROPERTY].axis[0]] * axis[NODE_PROPERTY].sign[0], fn[NODE_PROPERTY].k[0]);
        dst.y = TransformValue(fn[NODE_PROPERTY].op[1], src[(size_t) axis[NODE_PROPERTY].axis[1]] * axis[NODE_PROPERTY].sign[1], fn[NODE_PROPERTY].k[1]);
        dst.z = TransformValue(fn[NODE_PROPERTY].op[2], src[(size_t) axis[NODE_PROPERTY].axis[2]] * axis[NODE_PROPERTY].sign[2], fn[NODE_PROPERTY].k[2]);
      }

      /**
       * Transform a 4-component node property from a source TrsSystem to a destination TrsSystem
       * Where:
       *  Vdst = [            e0,            e1,            e2,            e3 ]  Vsrc
       *       = [            ei,            ej,            ek,            el ]  Swizzle
       *       = [          � ei,          � ej,          � ek,          � e3 ]  Maybe Negate
       *       = [ fn0(� ei, k0), fn1(� ej, k1), fn2(� ek, k2), fn3(� el, k3) ]  Apply Functions
       */
      template<size_t NODE_PROPERTY, typename TDST, typename TSRC>
      void Transform4(TDST& dst, const TSRC& src)
      {
        dst.x = TransformValue(fn[NODE_PROPERTY].op[0], src.AxisValue(axis[NODE_PROPERTY].axis[0]) * axis[NODE_PROPERTY].sign[0], fn[NODE_PROPERTY].k[0]);
        dst.y = TransformValue(fn[NODE_PROPERTY].op[1], src.AxisValue(axis[NODE_PROPERTY].axis[1]) * axis[NODE_PROPERTY].sign[1], fn[NODE_PROPERTY].k[1]);
        dst.z = TransformValue(fn[NODE_PROPERTY].op[2], src.AxisValue(axis[NODE_PROPERTY].axis[2]) * axis[NODE_PROPERTY].sign[2], fn[NODE_PROPERTY].k[2]);
        dst.w = TransformValue(fn[NODE_PROPERTY].op[3], src.AxisValue(axis[NODE_PROPERTY].axis[3]) * axis[NODE_PROPERTY].sign[3], fn[NODE_PROPERTY].k[3]);
      }

    };

  }
}
