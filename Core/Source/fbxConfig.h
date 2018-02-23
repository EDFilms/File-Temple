// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxConfig.h
 *
 * Compile time configuration for the SceneTrack FBX Exporter
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

/**
 * Enable materials
 * @default 1
 */
#define STFBX_CONFIG_MATERIALS_ENABLED 1

/**
 * Enable textures
 * @default 1
 */
#define STFBX_CONFIG_TEXTURES_ENABLED 1

/**
 * Place a dummy reference node on Nodes with Skinned Mesh Renderers
 * that are away from the origin or other objects.
 *
 * This is a workaround for unintended behaviour or a bug with the
 * FBX SDK
 *
 * @default 1
 */
#define STFBX_CONFIG_DUMMY_PRECISION_NODE_ON_SKINNED_MESH_RENDERERS 1

