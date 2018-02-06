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

#pragma once

#include "SceneTrackStatic.h"
#include "fbxConfig.h"
#include "expAutoDoc.h"

#define STFBX_PRIVATE_FUNCTION(...)
#define STFBX_PUBLIC_FUNCTION(...)
#define STFBX_PRIVATE_STRUCT(...)
#define STFBX_PUBLIC_STRUCT(...)
#define STFBX_PUBLIC_ENUM(...)
#define STFBX_PRIVATE_ENUM(...)
#define STFBX_TEST(...)
#define STFBX_COMMENT(...)
#define STFBX_TODO(...)
#define STFBX_DEPRECATED(...)
#define STFBX_UNUSED(X) (void)(X)
#define STFBX_INITIALIZER(...)
#define STFBX_SHUTDOWN(...)

#include "expRequired.h"

#define STFBX_DEBUG                   STEXP_DEBUG
#define STFBX_DEBUG_LEVEL             STEXP_DEBUG_LEVEL
#define STFBX_PUBLIC                  STEXP_PUBLIC
#define STFBX_INLINE                  STEXP_INLINE
#define STFBX_ARRAY_COUNT             STEXP_ARRAY_COUNT
#define STFBX_PRINT_INFO              STEXP_PRINT_INFO
#define STFBX_PRINT_INFOF             STEXP_PRINT_INFOF
#define STFBX_PRINT_INFOF_BEGIN       STEXP_PRINT_INFOF_BEGIN
#define STFBX_PRINT_INFOF_END         STEXP_PRINT_INFOF_END
#define STFBX_PRINT_INFOF_PART        STEXP_PRINT_INFOF_PART
#define STFBX_PRINT_INFO_PART         STEXP_PRINT_INFO_PART
#define STFBX_PRINT_ERRORF            STEXP_PRINT_ERRORF
#define STFBX_PRINT_WARNF             STEXP_PRINT_WARNF
#define STFBX_PRINT_CHATTERF          STEXP_PRINT_CHATTERF
#define STFBX_PRINT_REALLY_CHATTERF   STEXP_PRINT_REALLY_CHATTERF
#define STFBX_PRINT_ERROR             STEXP_PRINT_ERROR
#define STFBX_PRINT_WARN              STEXP_PRINT_WARN
#define STFBX_PRINT_CHATTER           STEXP_PRINT_CHATTER
#define STFBX_MARKER                  STEXP_MARKER


namespace SceneTrackFbx
{
  class FbxExporterT;
  class GameObject_t;
  class Transform_t;
  class StandardMeshRenderer_t;
  class SkinnedMeshRenderer_t;
  class Mesh_t;
  class SubMesh_t;
  class Material_t;

  class FNode_t;
  typedef std::shared_ptr<FNode_t> FNode;
  
  class FMesh_t;
  typedef std::shared_ptr<FMesh_t> FMesh;

  class FSkeleton_t;
  typedef std::shared_ptr<FSkeleton_t> FSkeleton;

  class FBone_t;
  typedef std::shared_ptr<FBone_t> FBone;

  class FMaterial_t;
  typedef std::shared_ptr<FMaterial_t> FMaterial;

  class FTexture_t;
  typedef std::shared_ptr<FTexture_t>  FTexture;

  class FbxDocumentWriter_t;
  typedef std::shared_ptr<FbxDocumentWriter_t> FbxDocumentWriter;
}
