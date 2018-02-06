#ifndef MOCK_UNITY_H
#define MOCK_UNITY_H

#include "TestCommon.h"
#include "SceneTrack.h"

#include <map>
#include <memory>
#include <vector>

#define ARRAY_COUNT(X) (sizeof(X) / sizeof(X[0]))

namespace Unity
{

  class GameObject;
  class MonoBehaviour;
  class Transform;
  class SkinnedMeshRenderer;
  class StandardMeshRenderer;
  class Bone;
  class Mesh;
  class Material;
  class SubMesh;

  typedef std::shared_ptr<GameObject>            GameObjectPtr;
  typedef std::shared_ptr<MonoBehaviour>         MonoBehaviourPtr;
  typedef std::shared_ptr<Transform>             TransformPtr;
  typedef std::shared_ptr<Mesh>                  MeshPtr;
  typedef std::shared_ptr<Material>              MaterialPtr;
  typedef std::shared_ptr<SubMesh>               SubMeshPtr;
  typedef std::shared_ptr<SkinnedMeshRenderer>   SkinnedMeshRendererPtr;
  typedef std::shared_ptr<StandardMeshRenderer>  StandardMeshRendererPtr;
  typedef std::shared_ptr<Bone>                  BonePtr;

  struct Vector
  {
    f32 x, y, z, w;

    Vector() {}

    Vector(f32 X, f32 Y, f32 Z, f32 W = 1.0f) : x(X), y(Y), z(Z), w(W) 
    {}

    static Vector AddVelocity(Vector p, Vector q, float t)
    {
      Vector s;
      s.x = p.x + q.x * t;
      s.y = p.y + q.y * t;
      s.z = p.z + q.z * t;
      return s;
    }

    static Vector Lerp(Vector p, Vector q, float t)
    {
      Vector s;
      s.x = (1 - t) * p.x + t * q.x;
      s.y = (1 - t) * p.y + t * q.y;
      s.z = (1 - t) * p.z + t * q.z;
      return s;
    }
  };

  struct Uint8Vector
  {
    u8 x, y, z, w;
  };

  struct Quaternion
  {
    f32 x, y, z, w;

    Quaternion() {}
    Quaternion(f32 X, f32 Y, f32 Z, f32 W = 1.0f) : x(X), y(Y), z(Z), w(W) 
    {}

  };

  struct Color
  {
    f32 r, g, b, a;
  };

  struct TexCoord
  {
    f32 u, v;
  };

  struct Bounds
  {
    Vector min, max;
  };

  void CreateUnitySchema();

  GameObjectPtr            CreateGameObject(TransformPtr parent = nullptr);

  GameObjectPtr            CreateGameObject(Vector pos, Vector rot = Vector(0,0,0), Vector sca = Vector(1,1,1,1), TransformPtr parent = nullptr);

  StandardMeshRendererPtr  CreateStandardMeshRenderer(MeshPtr mesh, GameObjectPtr gameObject);

  StandardMeshRendererPtr CreateStandardMeshRenderer(MeshPtr mesh, Vector position, Vector rotation, Vector scale);

  SkinnedMeshRendererPtr  CreateSkinnedMeshRenderer(MeshPtr mesh, GameObjectPtr gameObject);

  SkinnedMeshRendererPtr  CreateSkinnedMeshRenderer(MeshPtr mesh, Vector position, Vector rotation, Vector scale);

  BonePtr                 CreateBone(BonePtr parent = nullptr);


  std::pair<MeshPtr, SubMeshPtr> CreateMesh1(float* vertices, float* normals, float* tangents, float* uv, float* colors, int* indexes1, u32 nbVertices, u32 nbIndexes);

  std::pair<MeshPtr, SubMeshPtr> CreateAnimatedMesh1(float* vertices, float* normals, float* tangents, float* uv, float* colors, int* indexes1, float* boneWeightsWeight, u8* boneWeightIndex, u32 nbVertices, u32 nbIndexes, float* bindPoses, u32 nbBindPoses);

  class GameObject
  {
  public:

    static u32 kType;
    static u32 kName, kLayer, kTransform, kComponents, kVisibility;

    static void Register()
    {
      kType       = stCreateObjectTypeEx(ST_FREQUENCY_DYNAMIC, 100);
      kName       = stAddObjectTypeComponentEx2(kType, ST_KIND_NAMED, ST_TYPE_CSTRING, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kLayer      = stAddObjectTypeComponentEx2(kType, ST_KIND_LAYER, ST_TYPE_INT32, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kTransform  = stAddObjectTypeComponentEx2(kType, ST_KIND_SPATIAL, ST_TYPE_UINT32, 1, 1,  ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kComponents = stAddObjectTypeComponentEx2(kType, ST_KIND_CHILD, ST_TYPE_UINT32, 1, INT_MAX, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kVisibility = stAddObjectTypeComponentEx2(kType, ST_KIND_INTENSITY, ST_TYPE_UINT8, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
    }

  private:

    u32                                         handle;
    std::string                                 name;
    TransformPtr                                transform;
    std::vector<std::shared_ptr<MonoBehaviour>> children;

  public:

    GameObject()
    {
      handle = stCreateObject(kType);
    }

    u32 getHandle() const { return handle; }

    std::string getName() const { return name; }

    void setName(const std::string& name_)
    {
      name = name_;
      stSetValue_string(handle, kName, name.c_str(), (u32) name.length());
    }

    void setTransform(std::shared_ptr<Transform> transform_);

    inline TransformPtr getTransform() const
    {
      return transform;
    }

    void addComponent(std::shared_ptr<MonoBehaviour> child_);

    void addComponent_Apply();

  };

  class MonoBehaviour
  {
  public:
    virtual ~MonoBehaviour()
    {
    }

    virtual u32 getHandle() const = 0;
  };

  class Transform : public MonoBehaviour
  {
  public:

    static u32 kType;
    static u32 kLocalPosition, kLocalRotation, kLocalScale, kParent;

    static void Register()
    {
      kType          = stCreateObjectTypeEx(ST_FREQUENCY_DYNAMIC, 101);
      kLocalPosition = stAddObjectTypeComponentEx2(kType, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_LOCAL);
      kLocalRotation = stAddObjectTypeComponentEx2(kType, ST_KIND_ROTATION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_DEGREE, ST_REFERENCE_LOCAL);
      kLocalScale    = stAddObjectTypeComponentEx2(kType, ST_KIND_SCALE,    ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_LOCAL);
      kParent        = stAddObjectTypeComponentEx2(kType, ST_KIND_PARENT,   ST_TYPE_UINT32,  1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
    }
  private:

    u32                            handle;
    Vector                         localPosition;
    Vector                         localRotation;
    Vector                         localScale;
    std::shared_ptr<Transform>     parent;

  public:

    Transform()
      : localPosition(0,0,0), localRotation(0,0,0), localScale(1,1,1)
    {
      handle = stCreateObject(kType);
    }

    u32 getHandle() const override { return handle; }

    void setLocalPosition(const Vector& localPosition_)
    {
      localPosition = localPosition_;
      stSetValue_3_float32(handle, kLocalPosition, localPosition.x, localPosition.y, localPosition.z);
    }

    void setLocalRotation(const Vector& localRotation_)
    {
      localRotation = localRotation_;
      stSetValue_3_float32(handle, kLocalRotation, localRotation.x, localRotation.y, localRotation.z);
    }

    void setLocalScale(const Vector& localScale_)
    {
      localScale = localScale_;
      stSetValue_3_float32(handle, kLocalScale, localScale.x, localScale.y, localScale.z);
    }

    void setParent(std::shared_ptr<Transform> parent_)
    {
      parent = parent_;
      stSetValue_uint32(handle, kParent, parent->getHandle());
    }

    Vector getLocalPosition() const
    {
      return localPosition;
    }

    Vector getLocalRotation() const
    {
      return localRotation;
    }

    Vector getLocalScale() const
    {
      return localScale;
    }

  };


  class Bone : public MonoBehaviour
  {
  public:

    static u32 kType;
    static u32 kParent, kLocalPosition, kLocalRotation, kLocalScale, kId;

    static void Register()
    {
      kType          = stCreateObjectTypeEx(ST_FREQUENCY_DYNAMIC, 104);
      kParent        = stAddObjectTypeComponentEx2(kType, ST_KIND_PARENT,   ST_TYPE_UINT32,  1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kLocalPosition = stAddObjectTypeComponentEx2(kType, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_LOCAL);
      kLocalRotation = stAddObjectTypeComponentEx2(kType, ST_KIND_ROTATION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_DEGREE, ST_REFERENCE_LOCAL);
      kLocalScale    = stAddObjectTypeComponentEx2(kType, ST_KIND_SCALE, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_LOCAL);
      kId            = stAddObjectTypeComponentEx2(kType, ST_KIND_NAMED, ST_TYPE_UINT8, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_LOCAL);
    }
  private:

    u32                            handle;
    Vector                         localPosition;
    Vector                         localRotation;
    Vector                         localScale;
    u8                             id;
    BonePtr                        parent;

  public:

    Bone()
    {
      handle = stCreateObject(kType);
    }

    u32 getHandle() const override { return handle; }

    u8 getId() const { return id; }

    void setId(u8 id_)
    {
      id = id_;
      stSetValue_uint8(handle, kId, id);
    }

    void setLocalPosition(const Vector& localPosition_)
    {
      localPosition = localPosition_;
      stSetValue_3_float32(handle, kLocalPosition, localPosition.x, localPosition.y, localPosition.z);
    }

    void setLocalRotation(const Vector& localRotation_)
    {
      localRotation = localRotation_;
      stSetValue_3_float32(handle, kLocalRotation, localRotation.x, localRotation.y, localRotation.z);
    }

    void setLocalScale(const Vector& localScale_)
    {
      localScale = localScale_;
      stSetValue_3_float32(handle, kLocalScale, localScale.x, localScale.y, localScale.z);
    }

    void setParent(BonePtr parent_)
    {
      parent = parent_;
      stSetValue_uint32(handle, kParent, parent->getHandle());
    }

    Vector getLocalPosition() const
    {
      return localPosition;
    }

  };

  class StandardMeshRenderer : public MonoBehaviour
  {
  public:

    static u32 kType;
    static u32 kMaterials, kMesh, kParent;

    static void Register()
    {
      kType          = stCreateObjectTypeEx(ST_FREQUENCY_DYNAMIC, 102);
      kMaterials     = stAddObjectTypeComponentEx2(kType, ST_KIND_SURFACE,  ST_TYPE_UINT32, 1, INT_MAX, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kMesh          = stAddObjectTypeComponentEx2(kType, ST_KIND_GEOMETRY, ST_TYPE_UINT32, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kParent        = stAddObjectTypeComponentEx2(kType, ST_KIND_PARENT,   ST_TYPE_UINT32, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
    }

  private:

    u32                                     handle;
    std::vector<MaterialPtr>                materials;
    MeshPtr                                 mesh;
    TransformPtr                            parent;

  public:

    StandardMeshRenderer()
    {
      handle = stCreateObject(kType);
    }

    u32 getHandle() const override { return handle; }

    void setMesh(std::shared_ptr<Mesh> mesh);

    TransformPtr getParent() const { return parent; }

    void setParent(std::shared_ptr<Transform> transform_)
    {
      parent = transform_;
      stSetValue_uint32(handle, kParent, parent->getHandle());
    }

    void addMaterial(std::shared_ptr<Material> material_)
    {
      materials.push_back(material_);
    }
    
    void addMaterial_Apply();
  };

  class SkinnedMeshRenderer : public MonoBehaviour
  {
  public:

    static u32 kType;
    static u32 kMaterials, kMesh, kBones, kParent, kBoneTransform;

    static void Register()
    {
      kType          = stCreateObjectTypeEx(ST_FREQUENCY_DYNAMIC, 103);
      kMaterials     = stAddObjectTypeComponentEx2(kType, ST_KIND_SURFACE,  ST_TYPE_UINT32, 1, INT_MAX, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kMesh          = stAddObjectTypeComponentEx2(kType, ST_KIND_GEOMETRY, ST_TYPE_UINT32, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kBones         = stAddObjectTypeComponentEx2(kType, ST_KIND_BONE,     ST_TYPE_UINT32, 1, INT_MAX, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kParent        = stAddObjectTypeComponentEx2(kType, ST_KIND_PARENT,   ST_TYPE_UINT32, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kBoneTransform = stAddObjectTypeComponentEx2(kType, ST_KIND_BONE_BEGIN, ST_TYPE_UINT32, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
    }

  public:

    u32                                     handle;
    std::vector<MaterialPtr>                materials;
    std::vector<BonePtr>                    bones;
    MeshPtr                                 mesh;
    TransformPtr                            parent;
    TransformPtr                            boneTransform;


    SkinnedMeshRenderer()
    {
      handle = stCreateObject(kType);
    }

    u32 getHandle() const override { return handle; }

    void setMesh(std::shared_ptr<Mesh> mesh);

    TransformPtr getParent() const { return parent; }

    void setParent(std::shared_ptr<Transform> transform_)
    {
      parent = transform_;
      stSetValue_uint32(handle, kParent, parent->getHandle());
    }

    void setBoneTransform(std::shared_ptr<Transform> boneTransform_)
    {
      boneTransform = boneTransform_;
      stSetValue_uint32(handle, kBoneTransform, boneTransform->getHandle());
    }

    void addMaterial(std::shared_ptr<Material> material_)
    {
      materials.push_back(material_);
    }

    void addMaterial_Apply();


    void addBone(std::shared_ptr<Bone> bone_)
    {
      bones.push_back(bone_);
    }

    void addBone_Apply()
    {
      u32* ids = new u32[bones.size()];

      for(u32 i=0;i < bones.size();i++)
      {
        ids[i] = bones[i]->getHandle();
      }

      stSetValue_p_uint32(handle, kBones, ids, (u32) bones.size(), stCalculateStride1(ST_TYPE_UINT32, 1));

      delete[] ids;
    }

  };

  class Mesh
  {

  public:

    static u32 kType;
    static u32 kName, kVertices, kNormals, kTangents, kColors, kUv, kUv1, kUv2, kUv3, kUv4, kBoneWeightWeight, kBoneWeightIndex, kBounds, kPoses, kSubMesh;

    static void Register()
    {
      kType             = stCreateObjectTypeEx(ST_FREQUENCY_DYNAMIC, 201);
      kName             = stAddObjectTypeComponentEx2(kType, ST_KIND_NAMED,  ST_TYPE_CSTRING, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kVertices         = stAddObjectTypeComponentEx2(kType, ST_KIND_VERTEX, ST_TYPE_FLOAT32, 3, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_LOCAL);
      kNormals          = stAddObjectTypeComponentEx2(kType, ST_KIND_NORMAL, ST_TYPE_FLOAT32, 3, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kTangents         = stAddObjectTypeComponentEx2(kType, ST_KIND_TANGENT, ST_TYPE_FLOAT32, 4, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kColors           = stAddObjectTypeComponentEx2(kType, ST_KIND_COLOR, ST_TYPE_FLOAT32, 4, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kUv               = stAddObjectTypeComponentEx2(kType, ST_KIND_UV_0, ST_TYPE_FLOAT32, 2, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kUv2              = stAddObjectTypeComponentEx2(kType, ST_KIND_UV_1, ST_TYPE_FLOAT32, 2, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kUv3              = stAddObjectTypeComponentEx2(kType, ST_KIND_UV_2, ST_TYPE_FLOAT32, 2, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kUv4              = stAddObjectTypeComponentEx2(kType, ST_KIND_UV_3, ST_TYPE_FLOAT32, 2, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kBoneWeightIndex  = stAddObjectTypeComponentEx2(kType, ST_KIND_BONE_INDEX, ST_TYPE_UINT8, 4, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kBoneWeightWeight = stAddObjectTypeComponentEx2(kType, ST_KIND_BONE_WEIGHT, ST_TYPE_FLOAT32, 4, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kBounds           = stAddObjectTypeComponentEx2(kType, ST_KIND_SIZE, ST_TYPE_FLOAT32, 3, 2, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kPoses            = stAddObjectTypeComponentEx2(kType, ST_KIND_POSE, ST_TYPE_FLOAT32, 16, INT_MAX, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kSubMesh          = stAddObjectTypeComponentEx2(kType, ST_KIND_GEOMETRY, ST_TYPE_UINT32, 1, 8, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
    }

  private:

    u32                                     handle;
    std::string                             name;
    std::vector<f32>                        vertices;
    std::vector<f32>                        normals;
    std::vector<f32>                        tangents;
    std::vector<f32>                        colors;
    std::vector<f32>                        uv;
    std::vector<f32>                        uv2;
    std::vector<f32>                        uv3;
    std::vector<f32>                        uv4;
    std::vector<f32>                        boneWeightWeight;
    std::vector<u8>                         boneWeightIndex;
    std::vector<f32>                        bindPoses;
    Bounds                                  bounds;
    std::vector<SubMeshPtr>                 subMeshes;


  public:

    Mesh()
    {
      handle = stCreateObject(kType);
    }

    u32 getHandle() const { return handle; }

    void setName(const std::string& name_)
    {
      name = name_;
      stSetValue_string(handle, kName, name.c_str(), (u32) name.length());
    }

    void addVertex(const Vector& vertex_)
    {
      vertices.push_back(vertex_.x);
      vertices.push_back(vertex_.y);
      vertices.push_back(vertex_.z);
    }

    void addVertex_Apply()
    {
      stSetValue_p_float32(handle, kVertices, &vertices[0], (u32) vertices.size() / 3, stCalculateStride1(ST_TYPE_FLOAT32, 3));
    }

    void addNormal(const Vector& vertex_)
    {
      normals.push_back(vertex_.x);
      normals.push_back(vertex_.y);
      normals.push_back(vertex_.z);
    }

    void addNormal_Apply()
    {
      stSetValue_p_float32(handle, kNormals, &normals[0], (u32) normals.size() / 3, stCalculateStride1(ST_TYPE_FLOAT32, 3));
    }

    void addTangent(const Vector& vertex_)
    {
      tangents.push_back(vertex_.x);
      tangents.push_back(vertex_.y);
      tangents.push_back(vertex_.z);
      tangents.push_back(vertex_.w);
    }

    void addTangent_Apply()
    {
      stSetValue_p_float32(handle, kTangents, &tangents[0], (u32) tangents.size() / 4, stCalculateStride1(ST_TYPE_FLOAT32, 4));
    }

    void addColor(const Color& color_)
    {
      colors.push_back(color_.r);
      colors.push_back(color_.g);
      colors.push_back(color_.b);
      colors.push_back(color_.a);
    }

    void addColor_Apply()
    {
      stSetValue_p_float32(handle, kColors, &colors[0], (u32) colors.size() / 4, stCalculateStride1(ST_TYPE_FLOAT32, 4));
    }

    void addUv(const TexCoord& uv_)
    {
      uv.push_back(uv_.u);
      uv.push_back(uv_.v);
    }

    void addUv_Apply()
    {
      stSetValue_p_float32(handle, kUv, &uv[0], (u32) uv.size() / 2, stCalculateStride1(ST_TYPE_FLOAT32, 2));
    }

    void addUv2(const TexCoord& uv_)
    {
      uv2.push_back(uv_.u);
      uv2.push_back(uv_.v);
    }

    void addUv2_Apply()
    {
      stSetValue_p_float32(handle, kUv2, &uv2[0], (u32) uv2.size() / 2, stCalculateStride1(ST_TYPE_FLOAT32, 2));
    }

    void addUv3(const TexCoord& uv_)
    {
      uv3.push_back(uv_.u);
      uv3.push_back(uv_.v);
    }

    void addUv3_Apply()
    {
      stSetValue_p_float32(handle, kUv3, &uv3[0], (u32) uv3.size() / 2, stCalculateStride1(ST_TYPE_FLOAT32, 2));
    }

    void addUv4(const TexCoord& uv_)
    {
      uv4.push_back(uv_.u);
      uv4.push_back(uv_.v);
    }

    void addUv4_Apply()
    {
      stSetValue_p_float32(handle, kUv4, &uv4[0], (u32) uv4.size() / 2, stCalculateStride1(ST_TYPE_FLOAT32, 2));
    }

    void addBoneWeights(const Vector& boneWeight, const Uint8Vector& boneIndex)
    {
      boneWeightWeight.push_back(boneWeight.x);
      boneWeightWeight.push_back(boneWeight.y);
      boneWeightWeight.push_back(boneWeight.z);
      boneWeightWeight.push_back(boneWeight.w);

      boneWeightIndex.push_back(boneIndex.x);
      boneWeightIndex.push_back(boneIndex.y);
      boneWeightIndex.push_back(boneIndex.z);
      boneWeightIndex.push_back(boneIndex.w);
    }

    void addBoneWeights(float* boneWeights, u8* boneIndexes, u32 vertexCount)
    {
      u32 size = vertexCount * 4;
      boneWeightWeight.reserve(boneWeightWeight.size() + size);
      boneWeightIndex.reserve(boneWeightIndex.size() + size);
      for(u32 i=0;i < size;i++)
      {
        boneWeightWeight.push_back(*boneWeights++);
        boneWeightIndex.push_back(*boneIndexes++);
      }
    }

    void addBoneWeights_Apply()
    {
      stSetValue_p_float32(handle, kBoneWeightWeight, &boneWeightWeight[0], (u32) boneWeightWeight.size() / 4, stCalculateStride1(ST_TYPE_FLOAT32, 4));
      stSetValue_p_uint8(handle,   kBoneWeightIndex,  &boneWeightIndex[0], (u32) boneWeightIndex.size() / 4, stCalculateStride1(ST_TYPE_UINT8, 4));
    }

    void setBounds(const Bounds& bounds_)
    {
      bounds = bounds_;
      stSetValue_p_float32(handle, kBounds, (f32*) &bounds, 2, stCalculateStride1(ST_TYPE_FLOAT32, 3));
    }

    void addSubMesh(std::shared_ptr<SubMesh> subMesh)
    {
      subMeshes.push_back(subMesh);
    }

    void addSubMesh_Apply();

    void addVertices(float* verticesPtr, u32 vertexCount)
    {
      u32 size = vertexCount * 3;
      vertices.reserve(vertices.size() + size);
      for(u32 i=0;i < size;i++)
      {
        vertices.push_back(*verticesPtr++);
      }
    }

    void addNormals(float* normalsPtr, u32 vertexCount)
    {
      u32 size = vertexCount * 3;
      normals.reserve(normals.size() + size);
      for(u32 i=0;i < size;i++)
      {
        normals.push_back(*normalsPtr++);
      }
    }

    void addTangents(float* tangentsPtr, u32 vertexCount)
    {
      u32 size = vertexCount * 4;
      tangents.reserve(tangents.size() + size);
      for(u32 i=0;i < size;i++)
      {
        tangents.push_back(*tangentsPtr++);
      }
    }

    void addColors(float* colorsPtr, u32 vertexCount)
    {
      u32 size = vertexCount * 4;
      colors.reserve(colors.size() + size);
      for(u32 i=0;i < size;i++)
      {
        colors.push_back(*colorsPtr++);
      }
    }

    void addUv(float* uvPtr, u32 vertexCount)
    {
      u32 size = vertexCount * 2;
      uv.reserve(uv.size() + size);
      for(u32 i=0;i < size;i++)
      {
        uv.push_back(*uvPtr++);
      }
    }

    void addUv2(float* uvPtr, u32 vertexCount)
    {
      u32 size = vertexCount * 2;
      uv2.reserve(uv2.size() + size);
      for(u32 i=0;i < size;i++)
      {
        uv2.push_back(*uvPtr++);
      }
    }

    void addUv3(float* uvPtr, u32 vertexCount)
    {
      u32 size = vertexCount * 2;
      uv3.reserve(uv3.size() + size);
      for(u32 i=0;i < size;i++)
      {
        uv3.push_back(*uvPtr++);
      }
    }

    void addUv4(float* uvPtr, u32 vertexCount)
    {
      u32 size = vertexCount * 2;
      uv4.reserve(uv4.size() + size);
      for(u32 i=0;i < size;i++)
      {
        uv4.push_back(*uvPtr++);
      }
    }

    void addBindPoses(float* bindPosePtr, u32 boneCount)
    {
      u32 size = boneCount * 16;
      bindPoses.reserve(bindPoses.size() + size);
      for(u32 i=0;i < size;i++)
      {
        bindPoses.push_back(*bindPosePtr++);
      }
    }

    void addBindPoses_Apply()
    {
      stSetValue_p_float32(handle, kPoses, &bindPoses[0], (u32) bindPoses.size() / 16, stCalculateStride1(ST_TYPE_FLOAT32, 16));
    }

  };


  class SubMesh
  {

  public:

    static u32 kType;
    static u32 kIndexes;

    static void Register()
    {
      kType             = stCreateObjectTypeEx(ST_FREQUENCY_DYNAMIC, 202);
      kIndexes          = stAddObjectTypeComponentEx2(kType, ST_KIND_INDEX, ST_TYPE_UINT32, 1, 1 << 16, ST_UNITS_UNSPECIFIED, ST_REFERENCE_LOCAL);
    }

  private:

    u32                                     handle;
    std::vector<u32>                        indexes;


  public:

    SubMesh()
    {
      handle = stCreateObject(kType);
    }

    u32 getHandle() const { return handle; }

    void addTriangle(int a, int b, int c)
    {
      indexes.push_back(a);
      indexes.push_back(b);
      indexes.push_back(c);
    }

    void addIndexes(int* indexesPtr, u32 nbIndexes)
    {
      indexes.reserve(indexes.size() + nbIndexes);
      for(u32 i=0;i < nbIndexes;i++)
      {
        indexes.push_back((u32) (*indexesPtr++));
      }
    }

    void addTriangle_Apply()
    {
      stSetValue_p_uint32(handle, kIndexes, &indexes[0], (u32) indexes.size(), stCalculateStride1(ST_TYPE_UINT32, 1));
    }

  };

  class Material
  {

  public:

    static u32 kType;
    static u32 kName, kShader, kImage;

    static void Register()
    {
      kType             = stCreateObjectTypeEx(ST_FREQUENCY_STATIC, 203);
      kName             = stAddObjectTypeComponentEx2(kType, ST_KIND_NAMED,    ST_TYPE_CSTRING, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kShader           = stAddObjectTypeComponentEx2(kType, ST_KIND_SURFACE,  ST_TYPE_CSTRING, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
      kImage            = stAddObjectTypeComponentEx2(kType, ST_KIND_IMAGE,    ST_TYPE_CSTRING, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
    }

  private:

    u32                                     handle;
    std::string                             name;
    std::string                             shader;
    std::string                             image;


  public:

    Material()
    {
      handle = stCreateObject(kType);
    }

    u32 getHandle() const { return handle; }

    void setName(const std::string& name_)
    {
      name = name_;
      stSetValue_string(handle, kName, name.c_str(), (u32) name.length());
    }

    void setShader(const std::string& shader_)
    {
      shader = shader_;
      stSetValue_string(handle, kShader, shader.c_str(), (u32) shader.length());
    }

    void setImage(const std::string& image_)
    {
      image = image_;
      stSetValue_string(handle, kImage, image.c_str(), (u32) image.length());
    }

  };

}

#endif
