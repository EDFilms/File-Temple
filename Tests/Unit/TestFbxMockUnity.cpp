#include "TestFbxMockUnity.h"

namespace Unity
{

  u32 GameObject::kType                = 0;
  u32 GameObject::kName                = 0;
  u32 GameObject::kLayer               = 0;
  u32 GameObject::kTransform           = 0;
  u32 GameObject::kComponents          = 0;
  u32 GameObject::kVisibility          = 0;

  u32 Transform::kType                 = 0;
  u32 Transform::kLocalPosition        = 0;
  u32 Transform::kLocalRotation        = 0;
  u32 Transform::kLocalScale           = 0;
  u32 Transform::kParent               = 0;

  u32 StandardMeshRenderer::kType      = 0;
  u32 StandardMeshRenderer::kMaterials = 0;
  u32 StandardMeshRenderer::kMesh      = 0;
  u32 StandardMeshRenderer::kParent    = 0;

  u32 SkinnedMeshRenderer::kType       = 0;
  u32 SkinnedMeshRenderer::kMaterials  = 0;
  u32 SkinnedMeshRenderer::kMesh       = 0;
  u32 SkinnedMeshRenderer::kBones      = 0;
  u32 SkinnedMeshRenderer::kParent     = 0;
  u32 SkinnedMeshRenderer::kBoneTransform = 0;

  u32 Bone::kType                      = 0;
  u32 Bone::kParent                    = 0;
  u32 Bone::kLocalPosition             = 0;
  u32 Bone::kLocalRotation             = 0;
  u32 Bone::kLocalScale                = 0;
  u32 Bone::kId         = 0;

  u32 Mesh::kType                      = 0;
  u32 Mesh::kName                      = 0;
  u32 Mesh::kVertices                  = 0;
  u32 Mesh::kNormals                   = 0;
  u32 Mesh::kTangents                  = 0;
  u32 Mesh::kColors                    = 0;
  u32 Mesh::kUv                        = 0;
  u32 Mesh::kUv1                       = 0;
  u32 Mesh::kUv2                       = 0;
  u32 Mesh::kUv3                       = 0;
  u32 Mesh::kUv4                       = 0;
  u32 Mesh::kBoneWeightWeight          = 0;
  u32 Mesh::kBoneWeightIndex           = 0;
  u32 Mesh::kPoses                     = 0;
  u32 Mesh::kBounds                    = 0;
  u32 Mesh::kSubMesh                   = 0;

  u32 SubMesh::kType                   = 0;
  u32 SubMesh::kIndexes                = 0;

  u32 Material::kType                  = 0;
  u32 Material::kName                  = 0;
  u32 Material::kShader                = 0;
  u32 Material::kImage                 = 0;

  void CreateUnitySchema()
  {
    GameObject::Register();
    Transform::Register();
    StandardMeshRenderer::Register();
    SkinnedMeshRenderer::Register();
    Bone::Register();
    Mesh::Register();
    SubMesh::Register();
    Material::Register();
  }

  BonePtr                  CreateBone(BonePtr parent)
  {
    BonePtr bone = std::make_shared<Bone>();

    if (parent != nullptr)
    {
      bone->setParent(parent);
    }

    return bone;
  }

  GameObjectPtr            CreateGameObject(TransformPtr parent)
  {
    TransformPtr transform = std::make_shared<Transform>();

    if (parent != nullptr)
    {
      transform->setParent(parent);
    }

    GameObjectPtr gameObject = std::make_shared<GameObject>();
    gameObject->setTransform(transform);

    return gameObject;
  }

  GameObjectPtr            CreateGameObject(Vector pos, Vector rot, Vector sca, TransformPtr parent)
  {
    TransformPtr transform = std::make_shared<Transform>();

    if (parent != nullptr)
    {
      transform->setParent(parent);
    }

    transform->setLocalPosition(pos);
    transform->setLocalRotation(rot);
    transform->setLocalScale(sca);

    GameObjectPtr gameObject = std::make_shared<GameObject>();
    gameObject->setTransform(transform);

    return gameObject;
  }

  StandardMeshRendererPtr CreateStandardMeshRenderer(MeshPtr mesh, GameObjectPtr gameObject)
  {
    StandardMeshRendererPtr smr = std::make_shared<StandardMeshRenderer>();
    smr->setParent(gameObject->getTransform());
    smr->setMesh(mesh);

    return smr;
  }

  StandardMeshRendererPtr CreateStandardMeshRenderer(MeshPtr mesh, Vector position, Vector rotation, Vector scale)
  {
    
    TransformPtr transform = std::make_shared<Transform>();
    transform->setLocalPosition(position);
    transform->setLocalRotation(rotation);
    transform->setLocalScale(scale);

    GameObjectPtr gameObject = std::make_shared<GameObject>();
    gameObject->setTransform(transform);

    return CreateStandardMeshRenderer(mesh, gameObject);
  }

  SkinnedMeshRendererPtr CreateSkinnedMeshRenderer(MeshPtr mesh, GameObjectPtr gameObject)
  {
    SkinnedMeshRendererPtr smr = std::make_shared<SkinnedMeshRenderer>();
    smr->setParent(gameObject->getTransform());
    smr->setMesh(mesh);

    return smr;
  }

  SkinnedMeshRendererPtr CreateSkinnedMeshRenderer(MeshPtr mesh, Vector position, Vector rotation, Vector scale)
  {

    TransformPtr transform = std::make_shared<Transform>();
    transform->setLocalPosition(position);
    transform->setLocalRotation(rotation);
    transform->setLocalScale(scale);

    GameObjectPtr gameObject = std::make_shared<GameObject>();
    gameObject->setTransform(transform);

    return CreateSkinnedMeshRenderer(mesh, gameObject);
  }

  std::pair<MeshPtr, SubMeshPtr> CreateMesh1(float* vertices, float* normals, float* tangents, float* uv, float* colors, int* indexes1, u32 nbVertices, u32 nbIndexes)
  {
    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->addVertices(vertices, nbVertices);
    mesh->addVertex_Apply();

    if (normals != nullptr)
    {
      mesh->addNormals(normals, nbVertices);
      mesh->addNormal_Apply();
    }

    if (tangents != nullptr)
    {
      mesh->addTangents(tangents, nbVertices);
      mesh->addTangent_Apply();
    }

    if (uv != nullptr)
    {
      mesh->addUv(uv, nbVertices);
      mesh->addUv_Apply();
    }

    if (colors != nullptr)
    {
      mesh->addColors(colors, nbVertices);
      mesh->addColor_Apply();
    }


    SubMeshPtr subMesh = std::make_shared<SubMesh>();
    subMesh->addIndexes(indexes1, nbIndexes);
    subMesh->addTriangle_Apply();

    mesh->addSubMesh(subMesh);
    mesh->addSubMesh_Apply();

    return std::make_pair(mesh, subMesh);
  }

  std::pair<MeshPtr, SubMeshPtr> CreateAnimatedMesh1(float* vertices, float* normals, float* tangents, float* uv, float* colors, int* indexes1, float* boneWeightsWeight, u8* boneWeightIndex, u32 nbVertices, u32 nbIndexes, float* bindPoses, u32 nbBindPoses)
  {

    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->addVertices(vertices, nbVertices);
    mesh->addVertex_Apply();

    if (normals != nullptr)
    {
      mesh->addNormals(normals, nbVertices);
      mesh->addNormal_Apply();
    }

    if (tangents != nullptr)
    {
      mesh->addTangents(tangents, nbVertices);
      mesh->addTangent_Apply();
    }

    if (uv != nullptr)
    {
      mesh->addUv(uv, nbVertices);
      mesh->addUv_Apply();
    }

    if (colors != nullptr)
    {
      mesh->addColors(colors, nbVertices);
      mesh->addColor_Apply();
    }

    if (boneWeightsWeight != nullptr && boneWeightIndex != nullptr)
    {
      mesh->addBoneWeights(boneWeightsWeight, boneWeightIndex, nbVertices);
      mesh->addBoneWeights_Apply();
    }

    if (bindPoses != nullptr && nbBindPoses > 0)
    {
      mesh->addBindPoses(bindPoses, nbBindPoses);
      mesh->addBindPoses_Apply();
    }

    SubMeshPtr subMesh = std::make_shared<SubMesh>();
    subMesh->addIndexes(indexes1, nbIndexes);
    subMesh->addTriangle_Apply();

    mesh->addSubMesh(subMesh);
    mesh->addSubMesh_Apply();

    return std::make_pair(mesh, subMesh);
  }

  void GameObject::setTransform(std::shared_ptr<Transform> transform_)
  {
    transform = transform_;
    stSetValue_uint32(handle, kTransform, transform->getHandle());
  }

  void GameObject::addComponent(std::shared_ptr<MonoBehaviour> child_)
  {
    children.push_back(child_);
  }

  void GameObject::addComponent_Apply()
  {
    u32* ids = new u32[children.size()];

    for(u32 i=0;i < children.size();i++)
    {
      ids[i] = children[i]->getHandle();
    }

    stSetValue_p_uint32(handle, kComponents, ids, (u32) children.size(), stCalculateStride1(ST_TYPE_UINT32, 1));

    delete[] ids;
  }

  void StandardMeshRenderer::setMesh(std::shared_ptr<Mesh> mesh_)
  {
    mesh = mesh_;

    stSetValue_uint32(handle, kMesh, mesh->getHandle());
  }

  void StandardMeshRenderer::addMaterial_Apply()
  {
    u32* ids = new u32[materials.size()];

    for(u32 i=0;i < materials.size();i++)
    {
      ids[i] = materials[i]->getHandle();
    }

    stSetValue_p_uint32(handle, kMaterials, ids, (u32) materials.size(), stCalculateStride1(ST_TYPE_UINT32, 1));

    delete[] ids;
  }

  void Mesh::addSubMesh_Apply()
  {
    u32* ids = new u32[subMeshes.size()];

    for(u32 i=0;i < subMeshes.size();i++)
    {
      ids[i] = subMeshes[i]->getHandle();
    }

    stSetValue_p_uint32(handle, kSubMesh, ids, (u32) subMeshes.size(), stCalculateStride1(ST_TYPE_UINT32, 1));

    delete[] ids;
  }

  void SkinnedMeshRenderer::setMesh(std::shared_ptr<Mesh> mesh_)
  {
    mesh = mesh_;

    stSetValue_uint32(handle, kMesh, mesh->getHandle());
  }

  void SkinnedMeshRenderer::addMaterial_Apply()
  {
    u32* ids = new u32[materials.size()];

    for(u32 i=0;i < materials.size();i++)
    {
      ids[i] = materials[i]->getHandle();
    }

    stSetValue_p_uint32(handle, kMaterials, ids, (u32) materials.size(), stCalculateStride1(ST_TYPE_UINT32, 1));

    delete[] ids;
  }

}
