Installing the Autodesk FBX SDK
----

The Scenetrack FBX exporter is built with the **Autodesk® FBX® SDK 2017.1** 

It must be installed correctly for the Scenetrack FBX to compile correctly and to operate.

The Autodesk FBX installer for Microsoft Windows and Apple macOS can be downloaded [from the Autodesk FBX website](http://usa.autodesk.com/adsk/servlet/pc/item?siteID=123112&id=26012646).

Microsoft Windows
--
You should download the ``FBX SDK 2017.1 VS2015 (exe - 90276Kb)``. Double click the installer and let it install to the default paths; with no reference or not into the SceneTrack SDK folder.

For the Scenetrack FBX to compile you need to copy the ``include`` folder and the correct `.lib` files into the ``External\Windows\`` folders;

The ``include`` folder:-

1. Copy ``C:\Program Files\Autodesk\FBX\FBX SDK\2017.0.1\include\`` to ``Scenetrack\Core\Source\External\FBX\Windows\include``

The ``.lib`` files:-

1. Make a folder called ``Scenetrack\Core\Source\External\FBX\Windows\lib`` and then in there make two folders ``debug`` and ``release``.
2. Copy ``C:\Program Files\Autodesk\FBX\FBX SDK\2017.0.1\lib\vs2015\x64\debug\libfbxsdk-md.lib`` to ``Scenetrack\Core\Source\External\FBX\Windows\lib\debug\``
2. Copy ``C:\Program Files\Autodesk\FBX\FBX SDK\2017.0.1\lib\vs2015\x64\release\libfbxsdk-md.lib`` to ``Scenetrack\Core\Source\External\FBX\Windows\lib\release\``

Apple macOS
---
You should download the ``FBX SDK 2017.1 Clang (tgz - 122646Kb)``.

Install the Autodesk FBX SDK to it's normal folder, with no reference or not into the Scenetrack SDK folder.

Next you need to copy the Autodesk FBX SDK libraries and headers folders into the ``External`` folder for the Scenetrack FBX exporter to compile.

For the Scenetrack FBX to compile you need to copy the ``include`` folder and the correct `.a` file into the ``External\FBX\macOS\`` folders;

The ``include`` folder contents:-

1. Copy ``/Applications/Autodesk/FBXSDK/2017.1/include`` to ``Scenetrack/Core/Source/External/FBX/macOS/include``

The ``.a`` file:-

1. Make a folder called ``Scenetrack/Core/Source/External/FBX/macOS/lib/``
2. Copy ``/Applications/Autodesk/FBDSDK/2017.1/lib/clang/release/libfbxsdk.a`` to ``Scenetrack/Core/Source/External/FBX/macOS/lib/``