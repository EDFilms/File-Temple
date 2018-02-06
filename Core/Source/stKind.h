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

#ifndef ST_KIND_H
#define ST_KIND_H

#include"stPlatform.h"

ST_PUBLIC_ENUM(Named="None",                      Value=0, For="Kind")
#define ST_KIND_NONE                              0

// Generic
ST_PUBLIC_ENUM(Named="Length",                    Value=100, For="Kind")
#define ST_KIND_LENGTH                            100
ST_PUBLIC_ENUM(Named="Minimum",                   Value=101, For="Kind")
#define ST_KIND_MINIMUM                           101
ST_PUBLIC_ENUM(Named="Maximum",                   Value=102, For="Kind")
#define ST_KIND_MAXIMUM                           102
ST_PUBLIC_ENUM(Named="Intensity",                 Value=103, For="Kind")
#define ST_KIND_INTENSITY                         103
ST_PUBLIC_ENUM(Named="Named",                     Value=104, For="Kind")
#define ST_KIND_NAMED                             104
ST_PUBLIC_ENUM(Named="Type",                      Value=105, For="Kind")
#define ST_KIND_TYPE                              105
ST_PUBLIC_ENUM(Named="Repeat",                    Value=106, For="Kind")
#define ST_KIND_REPEAT                            106

// References
ST_PUBLIC_ENUM(Named="Relationship",              Value=201, For="Kind")
#define ST_KIND_RELATIONSHIP                      201
ST_PUBLIC_ENUM(Named="Spatial",                   Value=202, For="Kind")
#define ST_KIND_SPATIAL                           202
ST_PUBLIC_ENUM(Named="Geometry",                  Value=203, For="Kind")
#define ST_KIND_GEOMETRY                          203
ST_PUBLIC_ENUM(Named="Audio",                     Value=204, For="Kind")
#define ST_KIND_AUDIO                             204
ST_PUBLIC_ENUM(Named="Video",                     Value=205, For="Kind")
#define ST_KIND_VIDEO                             205
ST_PUBLIC_ENUM(Named="Event",                     Value=206, For="Kind")
#define ST_KIND_EVENT                             206
ST_PUBLIC_ENUM(Named="Surface",                   Value=207, For="Kind")
#define ST_KIND_SURFACE                           207

// Relationships
ST_PUBLIC_ENUM(Named="Layer",                     Value=300, For="Kind")
#define ST_KIND_LAYER                             300
ST_PUBLIC_ENUM(Named="Tag",                       Value=301, For="Kind")
#define ST_KIND_TAG                               301
ST_PUBLIC_ENUM(Named="Parent",                    Value=302, For="Kind")
#define ST_KIND_PARENT                            302
ST_PUBLIC_ENUM(Named="Child",                     Value=304, For="Kind")
#define ST_KIND_CHILD                             304
ST_PUBLIC_ENUM(Named="Next",                      Value=305, For="Kind")
#define ST_KIND_NEXT                              305
ST_PUBLIC_ENUM(Named="Previous",                  Value=306, For="Kind")
#define ST_KIND_PREVIOUS                          306

// Spatial
ST_PUBLIC_ENUM(Named="Size",                      Value=400, For="Kind")
#define ST_KIND_SIZE                              400
ST_PUBLIC_ENUM(Named="Width",                     Value=401, For="Kind")
#define ST_KIND_WIDTH                             401
ST_PUBLIC_ENUM(Named="Height",                    Value=402, For="Kind")
#define ST_KIND_HEIGHT                            402
ST_PUBLIC_ENUM(Named="Depth",                     Value=403, For="Kind")
#define ST_KIND_DEPTH                             403
ST_PUBLIC_ENUM(Named="X",                         Value=404, For="Kind")
#define ST_KIND_X                                 404
ST_PUBLIC_ENUM(Named="Y",                         Value=405, For="Kind")
#define ST_KIND_Y                                 405
ST_PUBLIC_ENUM(Named="Z",                         Value=406, For="Kind")
#define ST_KIND_Z                                 406
ST_PUBLIC_ENUM(Named="Yaw",                       Value=407, For="Kind")
#define ST_KIND_YAW                               407
ST_PUBLIC_ENUM(Named="Pitch",                     Value=408, For="Kind")
#define ST_KIND_PITCH                             408
ST_PUBLIC_ENUM(Named="Roll",                      Value=409, For="Kind")
#define ST_KIND_ROLL                              409
ST_PUBLIC_ENUM(Named="Position",                  Value=410, For="Kind")
#define ST_KIND_POSITION                          410
ST_PUBLIC_ENUM(Named="Rotation",                  Value=411, For="Kind")
#define ST_KIND_ROTATION                          411
ST_PUBLIC_ENUM(Named="Scale",                     Value=412, For="Kind")
#define ST_KIND_SCALE                             412
ST_PUBLIC_ENUM(Named="Transform",                 Value=413, For="Kind")
#define ST_KIND_TRANSFORM                         413
ST_PUBLIC_ENUM(Named="Linear Velocity",           Value=414, For="Kind")
#define ST_KIND_LINEAR_VELOCITY                   414
ST_PUBLIC_ENUM(Named="Linear Acceleration",       Value=415, For="Kind")
#define ST_KIND_LINEAR_ACCELERATION               415
ST_PUBLIC_ENUM(Named="Angular Velocity",          Value=416, For="Kind")
#define ST_KIND_ANGULAR_VELOCITY                  416
ST_PUBLIC_ENUM(Named="Angular Acceleration",      Value=417, For="Kind")
#define ST_KIND_ANGULAR_ACCELERATION              417
ST_PUBLIC_ENUM(Named="Offset",                    Value=418, For="Kind")
#define ST_KIND_OFFSET                            418

// Geometry
ST_PUBLIC_ENUM(Named="Vertex",                     Value=500, For="Kind")
#define ST_KIND_VERTEX                             500
ST_PUBLIC_ENUM(Named="Index",                      Value=501, For="Kind")
#define ST_KIND_INDEX                              501
ST_PUBLIC_ENUM(Named="Normal",                     Value=502, For="Kind")
#define ST_KIND_NORMAL                             502
ST_PUBLIC_ENUM(Named="Tangent",                    Value=503, For="Kind")
#define ST_KIND_TANGENT                            503
ST_PUBLIC_ENUM(Named="Color",                      Value=504, For="Kind")
#define ST_KIND_COLOR                              504
ST_PUBLIC_ENUM(Named="UV 0",                       Value=505, For="Kind")
#define ST_KIND_UV_0                               505
ST_PUBLIC_ENUM(Named="UV 1",                       Value=506, For="Kind")
#define ST_KIND_UV_1                               506
ST_PUBLIC_ENUM(Named="UV 2",                       Value=507, For="Kind")
#define ST_KIND_UV_2                               507
ST_PUBLIC_ENUM(Named="UV 3",                       Value=508, For="Kind")
#define ST_KIND_UV_3                               508
ST_PUBLIC_ENUM(Named="UV 4",                       Value=509, For="Kind")
#define ST_KIND_UV_4                               509
ST_PUBLIC_ENUM(Named="UV 5",                       Value=510, For="Kind")
#define ST_KIND_UV_5                               510
ST_PUBLIC_ENUM(Named="UV 6",                       Value=511, For="Kind")
#define ST_KIND_UV_6                               511
ST_PUBLIC_ENUM(Named="UV 7",                       Value=512, For="Kind")
#define ST_KIND_UV_7                               512
ST_PUBLIC_ENUM(Named="Bone Begin",                 Value=513, For="Kind")
#define ST_KIND_BONE_BEGIN                         513
ST_PUBLIC_ENUM(Named="Bone End",                   Value=514, For="Kind")
#define ST_KIND_BONE_END                           514
ST_PUBLIC_ENUM(Named="Bone Length",                Value=515, For="Kind")
#define ST_KIND_BONE_LENGTH                        515
ST_PUBLIC_ENUM(Named="Bone Weight",                Value=516, For="Kind")
#define ST_KIND_BONE_WEIGHT                        516
ST_PUBLIC_ENUM(Named="Bone Index",                 Value=517, For="Kind")
#define ST_KIND_BONE_INDEX                         517
ST_PUBLIC_ENUM(Named="Bone",                       Value=518, For="Kind")
#define ST_KIND_BONE                               518
ST_PUBLIC_ENUM(Named="Pose",                       Value=519, For="Kind")
#define ST_KIND_POSE                               519


// Audio
ST_PUBLIC_ENUM(Named="PCM",                        Value=600, For="Kind")
#define ST_KIND_PCM                                600

// Video
ST_PUBLIC_ENUM(Named="Image",                      Value=700, For="Kind")
#define ST_KIND_IMAGE                              700
ST_PUBLIC_ENUM(Named="R",                          Value=701, For="Kind")
#define ST_KIND_R                                  701
ST_PUBLIC_ENUM(Named="G",                          Value=702, For="Kind")
#define ST_KIND_G                                  702
ST_PUBLIC_ENUM(Named="B",                          Value=703, For="Kind")
#define ST_KIND_B                                  703
ST_PUBLIC_ENUM(Named="A",                          Value=704, For="Kind")
#define ST_KIND_A                                  704

// Surface Properties
ST_PUBLIC_ENUM(Named="Ambient",                    Value=800, For="Kind")
#define ST_KIND_AMBIENT                            800
ST_PUBLIC_ENUM(Named="Specular",                   Value=801, For="Kind")
#define ST_KIND_SPECULAR                           801
ST_PUBLIC_ENUM(Named="Emissive",                   Value=802, For="Kind")
#define ST_KIND_EMMISIVE                           802
ST_PUBLIC_ENUM(Named="Refraction",                 Value=803, For="Kind")
#define ST_KIND_REFRACTION                         803
ST_PUBLIC_ENUM(Named="Roughness",                  Value=804, For="Kind")
#define ST_KIND_ROUGHNESS                          804
ST_PUBLIC_ENUM(Named="Reflection",                 Value=806, For="Kind")
#define ST_KIND_REFLECTION                         806
ST_PUBLIC_ENUM(Named="Transparency",               Value=807, For="Kind")
#define ST_KIND_TRANSPARENCY                       807

// User
#define ST_KIND_USER                              16384



ST_PUBLIC_FUNCTION(Named="stCreateKind", Text="Create User Kind", Category="Object", ThreadSafe)
ST_PUBLIC u32 stCreateKind(stCStr name);


#endif
