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

#include "fbxDocument.h"
#define STFBX_INCLUDE_FBX_SDK
#include "fbxDocument_Shared.h"
#include "expTrs.h"
#include "fbxSettings.h"

namespace SceneTrackFbx
{

  FMesh FbxDocumentWriter_t::CreateNonAnimatedMesh(MeshRef mesh)
  {
    FMesh fMesh = std::make_shared<FMesh_t>();

    FbxMesh* m =  FbxMesh::Create(SCENE, mesh->Object->name.c_str());
    fMesh->mesh = m;

    // Vertices
    const auto& vertices = mesh->Object->vertices;
    size_t controlPointCount = vertices.size();
    m->InitControlPoints((u32) controlPointCount);

    FbxVector4* controlPoints = m->GetControlPoints();

    for(u32 i=0;i < controlPointCount;i++)
    {
#if STFBX_SETTINGS_UNITY2FBX == 1
      Vector3f vertex = vertices[i];
      controlPoints[i].Set(-vertex.x, vertex.y, vertex.z); 
#else
      static Vector3f tVertex;
      Vector3f vertex = vertices[i];

      Settings::Trs::sTransform.Transform3<kNodePropertyMeshVertex>(tVertex, vertex);

      controlPoints[i].Set(tVertex.x, tVertex.y, tVertex.z); 
#endif
    }

    STFBX_PRINT_CHATTERF("Wrote %i Vertices to %p", (u32) controlPointCount, controlPoints);

    // Normals
    const auto& normals = mesh->Object->normals;

    if (normals.size() > 0)
    {
      FbxGeometryElementNormal* normalElem = m->CreateElementNormal();
      normalElem->SetMappingMode(FbxGeometryElement::eByControlPoint);
      normalElem->SetReferenceMode(FbxGeometryElement::eDirect);
      auto& normalArray = normalElem->GetDirectArray();

      for(u32 i=0;i < normals.size();i++)
      {
#if STFBX_SETTINGS_UNITY2FBX == 1
        Vector3f normal = normals[i];
        normal.x = -normal.x;
        normalArray.Add(ToFbxVector4(normal));
#else
        static Vector3f tNormal;
        Vector3f normal = normals[i];

        Settings::Trs::sTransform.Transform3<kNodePropertyMeshNormal>(tNormal, normal);

        normalArray.Add(FbxVector4(normal.x, normal.y, normal.z));
#endif
      }

      STFBX_PRINT_CHATTERF("Wrote %i Normals to %p", (u32) normals.size(), normalElem);
    }

    // Tangents
    const auto& tangents = mesh->Object->tangents;

    if (tangents.size() > 0)
    {
      FbxGeometryElementTangent* tangentElem = m->CreateElementTangent();
      tangentElem->SetMappingMode(FbxGeometryElement::eByControlPoint);
      tangentElem->SetReferenceMode(FbxGeometryElement::eDirect);

      auto& tangentArray = tangentElem->GetDirectArray();

      for(u32 i=0;i < tangents.size();i++)
      {
        tangentArray.Add(ToFbxVector4(tangents[i]));
      }

      STFBX_PRINT_CHATTERF("Wrote %i Tangents to %p", (u32) tangents.size(), tangentElem);
    }

    // Uv..Uv4
    const auto& uv = mesh->Object->uv;

    if (uv.size() > 0)
    {
      FbxGeometryElementUV* uvElem = m->CreateElementUV("UVSet1");
      uvElem->SetMappingMode(FbxGeometryElement::eByControlPoint);
      uvElem->SetReferenceMode(FbxGeometryElement::eDirect);

      auto& uvArray = uvElem->GetDirectArray();

      for(u32 i=0;i < uv.size();i++)
      {
        uvArray.Add(ToFbxVector2(uv[i]));
      }

      STFBX_PRINT_CHATTERF("Wrote %i UV to %p", (u32) uv.size(), uvElem);
    }

    const auto& uv2 = mesh->Object->uv2;

    if (uv2.size() > 0)
    {
      FbxGeometryElementUV* uv2Elem = m->CreateElementUV("UVSet2");
      uv2Elem->SetMappingMode(FbxGeometryElement::eByControlPoint);
      uv2Elem->SetReferenceMode(FbxGeometryElement::eDirect);

      auto& uv2Array = uv2Elem->GetDirectArray();

      for(u32 i=0;i < uv2.size();i++)
      {
        uv2Array.Add(ToFbxVector2(uv2[i]));
      }

      STFBX_PRINT_CHATTERF("Wrote %i UV2 to %p", (u32) uv2.size(), uv2Elem);
    }

    const auto& uv3 = mesh->Object->uv3;

    if (uv3.size() > 0)
    {
      FbxGeometryElementUV* uv3Elem = m->CreateElementUV("UVSet3");
      uv3Elem->SetMappingMode(FbxGeometryElement::eByControlPoint);
      uv3Elem->SetReferenceMode(FbxGeometryElement::eDirect);

      auto& uv3Array = uv3Elem->GetDirectArray();

      for(u32 i=0;i < uv3.size();i++)
      {
        uv3Array.Add(ToFbxVector2(uv3[i]));
      }

      STFBX_PRINT_CHATTERF("Wrote %i UV3 to %p", (u32) uv3.size(), uv3Elem);
    }

    const auto& uv4 = mesh->Object->uv4;

    if (uv4.size() > 0)
    {
      FbxGeometryElementUV* uv4Elem = m->CreateElementUV("UVSet4");
      uv4Elem->SetMappingMode(FbxGeometryElement::eByControlPoint);
      uv4Elem->SetReferenceMode(FbxGeometryElement::eDirect);

      auto& uv4array = uv4Elem->GetDirectArray();

      for(u32 i=0;i < uv4.size();i++)
      {
        uv4array.Add(ToFbxVector2(uv4[i]));
      }

      STFBX_PRINT_CHATTERF("Wrote %i UV4 to %p", (u32) uv4.size(), uv4Elem);
    }

    // Colors
    const auto& colors = mesh->Object->colors;

    if (colors.size() > 0)
    {
      FbxGeometryElementVertexColor* colorElem = m->CreateElementVertexColor();
      colorElem->SetMappingMode(FbxGeometryElement::eByControlPoint);
      colorElem->SetReferenceMode(FbxGeometryElement::eDirect);

      auto& colorArray = colorElem->GetDirectArray();

      for(u32 i=0;i < colors.size();i++)
      {
        colorArray.Add(ToFbxColor(colors[i]));
      }

      STFBX_PRINT_CHATTERF("Wrote %i Colors to %p", (u32) colors.size(), colorElem);
    }

    // Indexes....
    s32 materialIndex = -1;
    
#if STFBX_CONFIG_MATERIALS_ENABLED
      FbxGeometryElementMaterial* material = m->CreateElementMaterial();
      material->SetMappingMode(FbxGeometryElement::eByPolygon);
      material->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
#endif

    for(const auto subMesh : mesh->Object->subMeshes)
    {
      const auto& indexes = subMesh->Object->indexes;

#if STFBX_CONFIG_MATERIALS_ENABLED
      ++materialIndex;
      // STFBX_PRINT_INFOF("Starting SubMesh %i for %s", materialIndex, mesh->Object->name.c_str());
#endif

#if (STFBX_SETTINGS_UNITY2FBX == 1)
      for(u32 i=0;i < indexes.size();i+=3)
      {
        u32 a = indexes[i + 0];
        u32 b = indexes[i + 1];
        u32 c = indexes[i + 2];
        m->BeginPolygon(materialIndex); 
        m->AddPolygon(a);
        m->AddPolygon(c);
        m->AddPolygon(b);
        m->EndPolygon(); 
      }
#else
      if (Settings::sTriangleWinding == Settings::TriangleWindingOrder::AsIs)
      {
        for(u32 i=0;i < indexes.size();i+=3)
        {
          u32 a = indexes[i + 0];
          u32 b = indexes[i + 1];
          u32 c = indexes[i + 2];
          m->BeginPolygon(materialIndex);
          m->AddPolygon(a);
          m->AddPolygon(b);
          m->AddPolygon(c);
          m->EndPolygon(); 
        }
      }
      else if (Settings::sTriangleWinding == Settings::TriangleWindingOrder::Reverse)
      {
        for(u32 i=0;i < indexes.size();i+=3)
        {
          u32 a = indexes[i + 0];
          u32 b = indexes[i + 1];
          u32 c = indexes[i + 2];
          m->BeginPolygon(materialIndex); 
          m->AddPolygon(a);
          m->AddPolygon(c);
          m->AddPolygon(b);
          m->EndPolygon(); 
        }
      }
      else if (Settings::sTriangleWinding == Settings::TriangleWindingOrder::Both)
      {
        for(u32 i=0;i < indexes.size();i+=3)
        {
          u32 a = indexes[i + 0];
          u32 b = indexes[i + 1];
          u32 c = indexes[i + 2];
          m->BeginPolygon(materialIndex); 
          m->AddPolygon(a);
          m->AddPolygon(b);
          m->AddPolygon(c);
          m->EndPolygon(); 
          m->BeginPolygon(materialIndex); 
          m->AddPolygon(a);
          m->AddPolygon(c);
          m->AddPolygon(b);
          m->EndPolygon(); 
        }
      }
#endif
      STFBX_PRINT_CHATTERF("Wrote SubMesh %i indexes", (u32) indexes.size());
    }

    mesh->Object->bExportedMesh = true;
    mesh->Object->fMesh = fMesh;
    return fMesh;
  }


}
