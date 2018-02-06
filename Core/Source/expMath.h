// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "expRequired.h"
#include "float.h"
#include "math.h"

namespace EXP_NAMESPACE
{

  enum class Node
  {
    STEXP_PUBLIC_ENUM(Named="Transform",   Value=0, For="Node")
    Transform,
    STEXP_PUBLIC_ENUM(Named="Bone",        Value=1, For="Node")
    Bone,
    STEXP_PUBLIC_ENUM(Named="Mesh",        Value=2, For="Node")
    Mesh,
    COUNT
  };

  enum class TRS
  {
    Translation,
    Rotation,
    Scale,
    COUNT
  };

  // Mask versions of above, because the user functions support setting T,R or S at the same time.

  STEXP_PUBLIC_ENUM(Named="Translation",   Value=1, For="NodeProperty")
#define STEXP_NODE_PROPERTY_TRANSLATION_MASK 1
  STEXP_PUBLIC_ENUM(Named="Rotation",      Value=2, For="NodeProperty")
#define STEXP_NODE_PROPERTY_ROTATION_MASK 2
  STEXP_PUBLIC_ENUM(Named="Scale",         Value=4, For="NodeProperty")
#define STEXP_NODE_PROPERTY_SCALE_MASK 4
  STEXP_PUBLIC_ENUM(Named="Vertex",        Value=1, For="NodeProperty")
#define STEXP_NODE_PROPERTY_TRANSLATION_MASK 1
  STEXP_PUBLIC_ENUM(Named="Normal",        Value=2, For="NodeProperty")
#define STEXP_NODE_PROPERTY_NORMAL_MASK 2

  constexpr size_t kNodePropertyNodeTranslation = 0;
  constexpr size_t kNodePropertyNodeRotation    = 1;
  constexpr size_t kNodePropertyNodeScale       = 2;
  constexpr size_t kNodePropertyMeshVertex      = 0;
  constexpr size_t kNodePropertyMeshNormal      = 1;

  constexpr size_t kNodePropertyCount           = 3;

  enum class Axis
  {
    STEXP_PUBLIC_ENUM(Named="X",     Value=0, For="Axis")
    X     = 0,
    STEXP_PUBLIC_ENUM(Named="Y",     Value=1, For="Axis")
    Y     = 1,
    STEXP_PUBLIC_ENUM(Named="Z",     Value=2, For="Axis")
    Z     = 2,
    STEXP_PUBLIC_ENUM(Named="W",     Value=3, For="Axis")
    W     = 3,
    COUNT
  };

  enum class AxisOrder
  {
    STEXP_PUBLIC_ENUM(Named="XYZ",     Value=0, For="AxisOrder")
    XYZ = 0,
    STEXP_PUBLIC_ENUM(Named="XZY",     Value=1, For="AxisOrder")
    XZY = 1,
    STEXP_PUBLIC_ENUM(Named="YXZ",     Value=2, For="AxisOrder")
    YXZ = 2,
    STEXP_PUBLIC_ENUM(Named="YZX",     Value=3, For="AxisOrder")
    YZX = 3,
    STEXP_PUBLIC_ENUM(Named="ZXY",     Value=4, For="AxisOrder")
    ZXY = 4,
    STEXP_PUBLIC_ENUM(Named="ZYX",     Value=5, For="AxisOrder")
    ZYX = 5,
    COUNT
  };

  enum class Operator
  {
    STEXP_PUBLIC_ENUM(Named="SetZero",     Value=0, For="Operator", Text="=0")
    Zero     = 0,
    STEXP_PUBLIC_ENUM(Named="SetOne",      Value=1, For="Operator", Text="=1")
    One      = 1,
    STEXP_PUBLIC_ENUM(Named="Value",       Value=2, For="Operator", Text="=v")
    Value    = 2,
    STEXP_PUBLIC_ENUM(Named="Constant",    Value=3, For="Operator", Text="=k")
    Constant = 3,
    STEXP_PUBLIC_ENUM(Named="Add",         Value=4, For="Operator", Text="=v + k")
    Add      = 4,
    STEXP_PUBLIC_ENUM(Named="Subtract",    Value=5, For="Operator", Text="=v - k")
    Subtract = 5,
    STEXP_PUBLIC_ENUM(Named="Multiply",    Value=6, For="Operator", Text="=v * k")
    Multiply = 6,
    STEXP_PUBLIC_ENUM(Named="Divide",      Value=7, For="Operator", Text="=v / k")
    Divide   = 7,
    STEXP_PUBLIC_ENUM(Named="Negate",      Value=8, For="Operator", Text="=-v")
    Negate   = 8,
    COUNT
  };

  template<typename T>
  inline T TransformValue(Operator op, const T& a, const T& b)
  {
    switch(op)
    {
      default:
      case Operator::Zero:      return static_cast<T>(0);
      case Operator::One:       return static_cast<T>(1);
      case Operator::Value:     return a;
      case Operator::Constant:  return b;
      case Operator::Add:       return a + b;
      case Operator::Subtract:  return a - b;
      case Operator::Multiply:  return a * b;
      case Operator::Divide:    return a / b;
      case Operator::Negate:    return -a;
    }
  }

  template<typename T>
  struct Unit {};

  template<>
  struct Unit<f32>
  {
    inline static f32 Abs(f32 v) { return fabsf(v); }
    inline static f32 Epsilon()  { return FLT_EPSILON; }
  };


  template<typename T> 
  struct Vector2T;

  typedef Vector2T<Operator> Operator2;
  typedef Vector2T<s32>      Vector2i;
  typedef Vector2T<u32>      Vector2u;
  typedef Vector2T<f32>      Vector2f;

  template<typename T>
  struct Vector2T
  {
    static const size_t kNbElements = 2;

    typedef T kType;

    T x, y;

    Vector2T() {}

    Vector2T(const T& X, const T& Y) 
      : x(X), y(Y)
    {
    }

    static inline Vector2T One(const T& s)
    {
      return Vector2T(s, s);
    }

    inline T AxisValue(Axis axis) const
    {
      return *(&x + (size_t) axis);
    }

    static inline Vector2T Swizzle(const Vector2T& v, Axis e1 = Axis::X, Axis e2 = Axis::X)
    {
      return Vector3T(v.AxisValue(e1), v.AxisValue(e2));
    }

    static inline Vector2T Transform(const Vector2T& v, const Vector2T& t = Vector2T::One(1.0f), const Operator2& op = Operator2::One(Operator::Multiply))
    {
      return Vector3T(TransformValue(op.x, v.x, t.x), TransformValue(op.y, v.y, t.y));
    }
  };

  template<typename K> class Type<Vector2T<K>>   { public: static constexpr u32 kType = Type<K>::kType; static constexpr u32 kNbElements = 2;  }; 

  template<typename T> 
  struct Vector3T;

  typedef Vector3T<Operator> Operator3;
  typedef Vector3T<s32>      Vector3i;
  typedef Vector3T<u32>      Vector3u;
  typedef Vector3T<f32>      Vector3f;

  template<typename T>
  struct Vector3T
  {
    static const size_t kNbElements = 3;

    T x, y, z;

    typedef T kType;

    Vector3T() {}

    Vector3T(const T& X, const T& Y, const T& Z) 
      : x(X), y(Y), z(Z)
    {
    }

    inline f32* ptr()
    {
      return &x;
    }

    static inline Vector3T One(const T& s)
    {
      return Vector3T(s, s, s);
    }

    inline T& operator[](size_t axis)
    {
      return *(&x + axis);
    }

    inline const T& operator[](size_t axis) const
    {
      return *(&x + axis);
    }

    inline T& operator[](Axis axis)
    {
      return *(&x + (size_t) axis);
    }

    inline const T& operator[](Axis axis) const
    {
      return *(&x + (size_t) axis);
    }

    inline T AxisValue(Axis axis) const
    {
      return *(&x + (size_t) axis);
    }

    static inline Vector3T Swizzle(const Vector3T& v, Axis e1 = Axis::X, Axis e2 = Axis::X, Axis e3 = Axis::X)
    {
      return Vector3T(v.AxisValue(e1), v.AxisValue(e2), v.AxisValue(e3));
    }

    static inline Vector3T Transform(const Vector3T& v, const Vector3T& t = Vector3T::One(1.0f), const Operator3& op = Operator3::One(Operator::Multiply))
    {
      return Vector3T(TransformValue(op.x, v.x, t.x), TransformValue(op.y, v.y, t.y), TransformValue(op.z, v.z, t.z));
    }

    static bool Equals(const Vector3T& lhs, const Vector3T& rhs)
    {
      if (Unit<T>::Abs(lhs.x - rhs.x) > Unit<T>::Epsilon())
        return false;
      if (Unit<T>::Abs(lhs.y - rhs.y) > Unit<T>::Epsilon())
        return false;
      if (Unit<T>::Abs(lhs.z - rhs.z) > Unit<T>::Epsilon())
        return false;
      return true;
    }

    static bool NotEquals(const Vector3T& lhs, const Vector3T& rhs)
    {
      return !Equals(lhs, rhs);
    }

  };

  template<typename T> 
  struct Vector4T;

  typedef Vector4T<Operator> Operator4;
  typedef Vector4T<s32>      Vector4i;
  typedef Vector4T<u32>      Vector4u;
  typedef Vector4T<f32>      Vector4f;
  typedef Vector4T<u8>       Vector4ub;
  template<typename K> class Type<Vector3T<K>>   { public: static constexpr u32 kType = Type<K>::kType;  static constexpr u32 kNbElements = 3; }; 

  template<typename T>
  struct Vector4T
  {
    static const size_t kNbElements = 4;
    typedef T kType;

    T x, y, z, w;

    Vector4T() {}

    Vector4T(const T& X, const T& Y, const T& Z, const T& W) 
      : x(X), y(Y), z(Z), w(W)
    {
    }

    inline T* ptr()
    {
      return &x;
    }

    inline T element(u32 i) const
    {
      return (&x)[i];
    }

    static inline Vector4T One(const T& s)
    {
      return Vector4T(s, s, s, s);
    }

    inline T AxisValue(Axis axis) const
    {
      return *(&x + (size_t) axis);
    }

    static inline Vector4T Swizzle(const Vector4T& v, Axis e1 = Axis::X, Axis e2 = Axis::X, Axis e3 = Axis::X, Axis e4 = Axis::X)
    {
      return Vector4T(v.AxisValue(e1), v.AxisValue(e2), v.AxisValue(e3), v.AxisValue(e4));
    }

    static inline Vector4T Transform(const Vector4T& v, const Vector4T& t = Vector4T::One(1.0f), const Operator4& op = Operator4::One(Operator::Multiply))
    {
      return Vector4T(TransformValue(op.x, v.x, t.x), TransformValue(op.y, v.y, t.y), TransformValue(op.z, v.z, t.z), TransformValue(op.w, v.w, t.w));
    }

    static bool Equals(const Vector4T& lhs, const Vector4T& rhs)
    {
      if (Unit<T>::Abs(lhs.x - rhs.x) > Unit<T>::Epsilon())
        return false;
      if (Unit<T>::Abs(lhs.y - rhs.y) > Unit<T>::Epsilon())
        return false;
      if (Unit<T>::Abs(lhs.z - rhs.z) > Unit<T>::Epsilon())
        return false;
      if (Unit<T>::Abs(lhs.w - rhs.w) > Unit<T>::Epsilon())
        return false;
      return true;
    }

    static bool NotEquals(const Vector4T& lhs, const Vector4T& rhs)
    {
      return !Equals(lhs, rhs);
    }

  };

  template<typename K> struct Type<Vector4T<K>>   { static constexpr u32 kType = Type<K>::kType; static constexpr u32 kNbElements = 4; }; 

  typedef Vector4f Quaternion;
  
  template<typename T> struct Matrix44T
  {
    f32 m[16];

    inline f32* ptr()
    {
      return &m[0];
    }

    Matrix44T() {}

    Matrix44T(float e0, float e1, float e2, float e3,
              float e4, float e5, float e6, float e7,
              float e8, float e9, float e10, float e11,
              float e12, float e13, float e14, float e15
      )
    {
      m[0]  = e0;
      m[1]  = e1;
      m[2]  = e2;
      m[3]  = e3;
      m[4]  = e4;
      m[5]  = e5;
      m[6]  = e6;
      m[7]  = e7;
      m[8]  = e8;
      m[9]  = e9;
      m[10] = e10;
      m[11] = e11;
      m[12] = e12;
      m[13] = e13;
      m[14] = e14;
      m[15] = e15;
    }

    inline void Identity()
    {
      m[0] = static_cast<T>(1);
      m[1] = static_cast<T>(0);
      m[2] = static_cast<T>(0);
      m[3] = static_cast<T>(0);

      m[4] = static_cast<T>(0);
      m[5] = static_cast<T>(1);
      m[6] = static_cast<T>(0);
      m[7] = static_cast<T>(0);

      m[8] = static_cast<T>(0);
      m[9] = static_cast<T>(0);
      m[10] = static_cast<T>(1);
      m[11] = static_cast<T>(0);

      m[12] = static_cast<T>(0);
      m[13] = static_cast<T>(0);
      m[14] = static_cast<T>(0);
      m[15] = static_cast<T>(1);
    }

    Vector3T<T> Position() const
    {
      return Vector3T<T>();
    }

    Vector4T<T> Rotation() const
    {
      return Vector4T<T>();
    }

    Vector3T<T> Scale() const
    {
      return Vector3T<T>();
    }

    static inline Matrix44T TRS(const Vector3f& pos, const Vector4f& rot, const Vector3f& scale)
    {
      STEXP_UNUSED(pos);
      STEXP_UNUSED(rot);
      STEXP_UNUSED(scale);
      return Matrix44T(); // TODO
    }

    static inline Matrix44T Multiply(const Matrix44T& m, const Matrix44T& n)
    {
      return Matrix44T(m[0]*n[0]  + m[4]*n[1]  + m[8]*n[2]  + m[12]*n[3],   m[1]*n[0]  + m[5]*n[1]  + m[9]*n[2]  + m[13]*n[3],   m[2]*n[0]  + m[6]*n[1]  + m[10]*n[2]  + m[14]*n[3],   m[3]*n[0]  + m[7]*n[1]  + m[11]*n[2]  + m[15]*n[3],
        m[0]*n[4]  + m[4]*n[5]  + m[8]*n[6]  + m[12]*n[7],   m[1]*n[4]  + m[5]*n[5]  + m[9]*n[6]  + m[13]*n[7],   m[2]*n[4]  + m[6]*n[5]  + m[10]*n[6]  + m[14]*n[7],   m[3]*n[4]  + m[7]*n[5]  + m[11]*n[6]  + m[15]*n[7],
        m[0]*n[8]  + m[4]*n[9]  + m[8]*n[10] + m[12]*n[11],  m[1]*n[8]  + m[5]*n[9]  + m[9]*n[10] + m[13]*n[11],  m[2]*n[8]  + m[6]*n[9]  + m[10]*n[10] + m[14]*n[11],  m[3]*n[8]  + m[7]*n[9]  + m[11]*n[10] + m[15]*n[11],
        m[0]*n[12] + m[4]*n[13] + m[8]*n[14] + m[12]*n[15],  m[1]*n[12] + m[5]*n[13] + m[9]*n[14] + m[13]*n[15],  m[2]*n[12] + m[6]*n[13] + m[10]*n[14] + m[14]*n[15],  m[3]*n[12] + m[7]*n[13] + m[11]*n[14] + m[15]*n[15]);

      return Matrix44T(); // TODO
    }

  };
  
  template<typename K> struct Type<Matrix44T<K>>   { static constexpr u32 kType = Type<K>::kType; static constexpr u32 kNbElements = 16; }; 

  typedef Matrix44T<f32> Matrix44f;

  struct Bounds
  {
    Bounds(): min(0.0f, 0.0f, 0.0f), max(0.0f, 0.0f, 0.0f)
    {}

    Vector3f min, max;
  };

  template<size_t N>
  struct AxisElement
  {
    enum { kCount = N };
    Axis axis[N];
  };

  template<>
  struct AxisElement<3>
  {
    enum { kCount = 3 };
    Axis  axis[3];
    float sign[3];

    inline AxisElement() {}

    inline AxisElement(Axis e0, Axis e1, Axis e2, float sign0, float sign1, float sign2)
    {
      axis[0] = e0;
      axis[1] = e1;
      axis[2] = e2;
      sign[0] = sign0;
      sign[1] = sign1;
      sign[2] = sign2;
    }

    inline AxisElement(Axis e0, Axis e1, Axis e2)
    {
      axis[0] = e0;
      axis[1] = e1;
      axis[2] = e2;
      sign[0] = 1.0f;
      sign[1] = 1.0f;
      sign[2] = 1.0f;
    }

    inline void Set(Axis e0, Axis e1, Axis e2, float sign0, float sign1, float sign2)
    {
      axis[0] = e0;
      axis[1] = e1;
      axis[2] = e2;
      sign[0] = sign0;
      sign[1] = sign1;
      sign[2] = sign2;
    }

    inline void Set(Axis e0, Axis e1, Axis e2)
    {
      axis[0] = e0;
      axis[1] = e1;
      axis[2] = e2;
      sign[0] = 1.0f;
      sign[1] = 1.0f;
      sign[2] = 1.0f;
    }
  };

  template<>
  struct AxisElement<4>
  {
    enum { kCount = 4 };
    Axis  axis[4];
    float sign[4];

    inline AxisElement() {}

    inline AxisElement(Axis e0, Axis e1, Axis e2, Axis e3, float sign0, float sign1, float sign2, float sign3)
    {
      axis[0] = e0;
      axis[1] = e1;
      axis[2] = e2;
      axis[3] = e3;
      sign[0] = sign0;
      sign[1] = sign1;
      sign[2] = sign2;
      sign[3] = sign3;
    }

    inline AxisElement(Axis e0, Axis e1, Axis e2, Axis e3)
    {
      axis[0] = e0;
      axis[1] = e1;
      axis[2] = e2;
      axis[3] = e3;
      sign[0] = 1.0f;
      sign[1] = 1.0f;
      sign[2] = 1.0f;
      sign[3] = 1.0f;
    }

    inline void Set(Axis e0, Axis e1, Axis e2, Axis e3, float sign0, float sign1, float sign2, float sign3)
    {
      axis[0] = e0;
      axis[1] = e1;
      axis[2] = e2;
      axis[3] = e3;
      sign[0] = sign0;
      sign[1] = sign1;
      sign[2] = sign2;
      sign[3] = sign3;
    }

    inline void Set(Axis e0, Axis e1, Axis e2, Axis e3)
    {
      axis[0] = e0;
      axis[1] = e1;
      axis[2] = e2;
      axis[3] = e3;
      sign[0] = 1.0f;
      sign[1] = 1.0f;
      sign[2] = 1.0f;
      sign[3] = 1.0f;
    }

  };

  template<size_t N>
  struct AxisTransformFunction
  {
    enum { kCount = N };
    Operator op[N];
    f64      constant[N];
  };

  template<>
  struct AxisTransformFunction<3>
  {
    enum { kCount = 3 };
    Operator op[3];
    f32      k[3];

    inline AxisTransformFunction() {}

    inline AxisTransformFunction(Operator op0, f32 k0, Operator op1, f32 k1, Operator op2, f32 k2)
    {
      op[0] = op0;
      op[1] = op1;
      op[2] = op2;
      k[0] = k0;
      k[1] = k1;
      k[2] = k2;
    }

    void Set(Operator op0, f32 k0, Operator op1, f32 k1, Operator op2, f32 k2)
    {
      op[0] = op0;
      op[1] = op1;
      op[2] = op2;
      k[0] = k0;
      k[1] = k1;
      k[2] = k2;
    }
  };

  template<>
  struct AxisTransformFunction<4>
  {
    enum { kCount = 4 };
    Operator op[4];
    f32      k[4];

    inline AxisTransformFunction() {}

    inline AxisTransformFunction(Operator op0, f32 k0, Operator op1, f32 k1, Operator op2, f32 k2, Operator op3, f32 k3)
    {
      op[0] = op0;
      op[1] = op1;
      op[2] = op2;
      op[3] = op3;
      k[0] = k0;
      k[1] = k1;
      k[2] = k2;
      k[3] = k3;
    }

    void Set(Operator op0, f32 k0, Operator op1, f32 k1, Operator op2, f32 k2, Operator op3, f32 k3)
    {
      op[0] = op0;
      op[1] = op1;
      op[2] = op2;
      op[3] = op3;
      k[0] = k0;
      k[1] = k1;
      k[2] = k2;
      k[3] = k3;
    }
  };

  inline void AxisOrderToAxisElement(AxisOrder order, AxisElement<3>& axisElement)
  {
    switch(order)
    {
      default:
      case AxisOrder::XYZ: axisElement.Set(Axis::X, Axis::Y, Axis::Z); break;
      case AxisOrder::XZY: axisElement.Set(Axis::X, Axis::Z, Axis::Y); break;
      case AxisOrder::YXZ: axisElement.Set(Axis::Y, Axis::X, Axis::Z); break;
      case AxisOrder::YZX: axisElement.Set(Axis::Y, Axis::Z, Axis::X); break;
      case AxisOrder::ZXY: axisElement.Set(Axis::Z, Axis::X, Axis::Y); break;
      case AxisOrder::ZYX: axisElement.Set(Axis::Z, Axis::Y, Axis::X); break;
    }
  }


}
