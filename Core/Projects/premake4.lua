-- Options

-- Use NedMalloc memory allocator
kUseNedmalloc = false

-- Fbx Sdk Version; ex. "2017.1"
kFbxSdkVersion = "2017.1"

-- Windows Fbx Sdk Path; ex. "C:/Program Files/Autodesk/FBX/FBX SDK/"
kFbxSdkPathWindows = "../External/FBX/Windows/"

-- macOS Fbx Sdk Path; ex. Unknown
kFbxSdkPathMacOS = "../External/FBX/macOS/"

-- Build Test
kBuildTests = false

-- Build Exporters
kBuildExporters = true

---------------------------------------------------------------------------------------------------------------------------------------------

function makeCProject(_type, _name, _depend)
  project (_type .. _name)
    uuid(os.uuid(_type .. _name))
    kind            "ConsoleApp"
    platforms       { "x64" }
    language        "C"
    objdir          "BuildTemp"
    includedirs     { "../../Build" }
    files           { path.join("../Source", _name .. "*.*")  }
    links           { _depend }

    includedirs     { "../External" }
    
    configuration   { "vs*" }
              flags  { "ExtraWarnings", "FatalWarnings" }
end

function makeApiProject(_type, _name)
  project (_type .. _name)
    uuid(os.uuid(_type .. _name))
    kind            "ConsoleApp"
    platforms       { "x64" }
    language        "C++"
    objdir          "BuildTemp"
    includedirs     { "../../Build" }
    files           { path.join("../Source", _name .. "*.*")  }
    links           {  }

    includedirs     { "../External" }

    configuration   { "vs*" }
              flags  { "ExtraWarnings", "FatalWarnings" }
end

function makeTest(_type, _name, _depend)
  project (_type .. _name)
    uuid(os.uuid(_type .. _name))
    kind            "ConsoleApp"
    platforms       { "x64" }
    language        "C++"
    objdir          "BuildTemp"
    includedirs     { "../../Build" }
    files           { path.join("../../Tests/Unit/", _type .. _name .. "*.*"), path.join("../../Tests/Unit/TestCommon.h")  }
    links           { _depend }

    includedirs     { "../External", "../External/catch/single_include"  }

    defines         {
                      "_HAS_EXCEPTIONS=0",
                      "_ITERATOR_DEBUG_LEVEL=0",
                      "_CRT_SECURE_NO_WARNINGS"
    }
    configuration   { "vs*" }
              flags  { "ExtraWarnings", "FatalWarnings" }
end

function makeFbxExportTest(_type, _name, _depend)
  project (_type .. _name)
    uuid(os.uuid(_type .. _name))
    kind            "ConsoleApp"
    platforms       { "x64" }
    language        "C++"
    objdir          "BuildTemp"
    includedirs     { "../../Build" }
    files           { 
                      path.join("../../Tests/Unit/", _type .. _name .. "*.*"), 
                      path.join("../../Tests/Unit/TestCommon.h"),  
                      path.join("../../Tests/Unit/TestFbxMockUnity*.*"),
                      path.join("../../Tests/Unit/TestFbxMesh*.*")   
                    }
    links           { "SceneTrack"}

    includedirs     { "../External", "../External/catch/single_include"  }

    defines         {
                      "_HAS_EXCEPTIONS=0",
                      "_ITERATOR_DEBUG_LEVEL=0",
                      "_CRT_SECURE_NO_WARNINGS"
    }
    configuration   { "vs*" }
              flags  { "ExtraWarnings", "FatalWarnings" }
end

function makeFbxImportTest(_type, _name, _depend)
  project (_type .. _name)
    uuid(os.uuid(_type .. _name))
    kind            "ConsoleApp"
    platforms       { "x64" }
    language        "C++"
    objdir          "BuildTemp"
    includedirs     { "../../Build" }
    files           { 
                      path.join("../../Tests/Unit/", _type .. _name .. "*.*"), 
                      path.join("../../Tests/Unit/TestCommon.h")
                    }
    links           { "SceneTrack", "SceneTrackFbx" }

    includedirs     { "../External", "../External/catch/single_include"  }

    defines         {
                      "_HAS_EXCEPTIONS=0",
                      "_ITERATOR_DEBUG_LEVEL=0",
                      "_CRT_SECURE_NO_WARNINGS"
    }
    configuration   { "vs*" }
              flags  { "ExtraWarnings", "FatalWarnings" }
end

function makeMidiTest(_type, _name)
  project (_type .. _name)
    uuid(os.uuid(_type .. _name))
    kind            "ConsoleApp"
    platforms       { "x64" }
    language        "C++"
    objdir          "BuildTemp"
    includedirs     { "../../Build" }
    files           { 
                      path.join("../../Tests/Unit/", _type .. _name .. "*.*"), 
                      path.join("../../Tests/Unit/TestCommon.h")
                    }
    links           { "SceneTrack", "SceneTrackMidi" }

    includedirs     { "../External", "../External/catch/single_include"  }

    defines         {
                      "_HAS_EXCEPTIONS=0",
                      "_ITERATOR_DEBUG_LEVEL=0",
                      "_CRT_SECURE_NO_WARNINGS"
    }
    configuration   { "vs*" }
              flags  { "ExtraWarnings", "FatalWarnings" }
end

function makeStProject(_name, _kind)
      project (_name)
          kind            (_kind)
          platforms       { "x64" }
          language        "C"
          objdir          "BuildTemp"
          flags           { 
                            "FatalWarnings",
                            "NoExceptions",
                            "NoRTTI",
                            "NoPCH",
                            "NoEditAndContinue"
          }
          
          includedirs     { "../External", "../../", "../External/bx/include/", "../External/nedmalloc/", }
                
          files           { 
                            "../Source/st*.c", 
                            "../Source/st*.h",
                            "../Generated/st*.c", 
                            "../Generated/st*.h",
                            "../../Config.inc"
          }

          if (kUseNedmalloc) then
            files { "../External/nedmalloc/nedmalloc.c", "../External/nedmalloc/nedmalloc.h" }
          end
          
          libdirs         { }    

          configuration   { "vs*" }
              flags  {
                "ExtraWarnings", "FatalWarnings"
              }
              defines         {
                                "_HAS_EXCEPTIONS=0",
                                "_ITERATOR_DEBUG_LEVEL=0",
                                "_CRT_SECURE_NO_WARNINGS"
              }

              configuration  { "x32", "vs*" }
                defines      { "WIN32" }
                prebuildcommands          { "genie st-version-bump"}
                postbuildcommands         { "genie install-win32" }

              configuration  { "x64", "vs*" }
                defines      { "WIN32", "WIN64" }
                prebuildcommands          { "genie st-version-bump" }
                postbuildcommands         { "genie install-win64" }

              configuration  { "macosx" }
                prebuildcommands          { "${SRCROOT}/genie st-version-bump"}
                postbuildcommands         { "${SRCROOT}/genie install-osx" }

end

function makeCppFbxExporterProject(_name, _depend, _kind)

  winSdkPath = kFbxSdkPathWindows
  macSdkPath = kFbxSdkPathMacOS

  project (_name)
    uuid(os.uuid(_name))
    language        "C++"
    kind            (_kind)
    objdir          "BuildTemp"
    platforms       { "x64" }
    includedirs     { "../External", "../../Build" }
    files           { path.join("../Source", "fbx*.*"), path.join("../Source", "exp*.*")  }
    links           { _depend }
    defines         { "EXP_NAMESPACE=SceneTrackFbx"}

    configuration   { "vs*" }
            flags       { "ExtraWarnings", "FatalWarnings" }
            includedirs { path.join(winSdkPath, "include") }

            configuration  { "x32", "vs*", "debug" }
                    postbuildcommands   { "genie fbx-version-bump", "genie install-fbx-sdk-win32-debug"}
                    libdirs             { path.join(winSdkPath, "lib/debug") }
                    links               { "libfbxsdk-md" }

            configuration { "x32", "vs*", "release" }
                    postbuildcommands   { "genie fbx-version-bump", "genie install-fbx-sdk-win32-release"}
                    links               { "libfbxsdk-md" }
                    libdirs             { path.join(winSdkPath, "lib/release") }

            configuration  { "x64", "vs*", "debug" }
                    postbuildcommands   { "genie fbx-version-bump", "genie install-fbx-sdk-win64-debug"}
                    links               { "libfbxsdk-md" }
                    libdirs             { path.join(winSdkPath, "lib/debug") }

            configuration  { "x64", "vs*", "release" }
                    postbuildcommands   { "genie fbx-version-bump", "genie install-fbx-sdk-win64-release"}
                    links               { "libfbxsdk-md" }
                    libdirs             { path.join(winSdkPath, "lib/release") }

    configuration  { "macosx" }
            defines         { "FBXSDK_SHARED" }
            premake.xcode.parameters = {'CLANG_CXX_LANGUAGE_STANDARD = "c++11"'}
            includedirs               { path.join(macSdkPath, "include") }
            links                     { "libiconv.tbd", "Cocoa.framework", "SceneTrackStatic", "fbxsdk" }
            libdirs                   { path.join(macSdkPath, "lib") }
            postbuildcommands         "${SRCROOT}/genie install-fbx-sdk-macos"
    
end

function makeCppMidiExporterProject(_name, _depend, _kind)
  project (_name)
    uuid(os.uuid(_name))
    language        "C++"
    kind            (_kind)
    objdir          "BuildTemp"
    platforms       { "x64" }
    includedirs     { "../External", "../../Build" }
    files           { path.join("../Source", "midi*.*"), path.join("../Source", "exp*.*")  }
    links           { _depend }
    defines         { "EXP_NAMESPACE=SceneTrackMidi"}

    configuration   { "vs*" }
            defines         { "_CRT_SECURE_NO_WARNINGS" }
            flags       { "ExtraWarnings", "FatalWarnings" }
            configuration { "x32", "vs*", "debug" }
                    postbuildcommands   { "genie midi-version-bump", "genie install-midi-sdk-win32-debug"}

            configuration { "x32", "vs*", "release" }
                    postbuildcommands   { "genie midi-version-bump", "genie install-midi-sdk-win32-release"}

            configuration { "x64", "vs*", "debug" }
                    postbuildcommands   { "genie midi-version-bump", "genie install-midi-sdk-win64-debug"}

            configuration { "x64", "vs*", "release" }
                    postbuildcommands   { "genie midi-version-bump", "genie install-midi-sdk-win64-release"}

    configuration  { "macosx" }
            premake.xcode.parameters = {'CLANG_CXX_LANGUAGE_STANDARD = "c++11"'}
            postbuildcommands         "${SRCROOT}/genie install-midi-sdk-macos"
            includedirs               { path.join(macSdkPath, "include") }
            links                     { "SceneTrackStatic" }
            libdirs                   { path.join(macSdkPath, "lib") }
    
end

function makeCppVideoExporterProject(_name, _depend, _kind)
  project (_name)
    uuid(os.uuid(_name))
    language        "C++"
    kind            (_kind)
    objdir          "BuildTemp"
    platforms       { "x64" }
    includedirs     { "../External", "../../Build" }
    files           { path.join("../Source", "video*.*"), path.join("../Source", "exp*.*"), path.join("../External", "lodepng*.*"), path.join("../External", "tiny_jpeg*.*"), path.join("../External", "jo_mpeg*.*")  }
    links           { _depend }
    defines         { "EXP_NAMESPACE=SceneTrackVideo"}

    configuration   { "vs*" }
            flags       { "ExtraWarnings", "FatalWarnings" }
            configuration { "x32", "vs*", "debug" }
                    postbuildcommands   { "genie video-version-bump", "genie install-video-sdk-win32-debug"}

            configuration { "x32", "vs*", "release" }
                    postbuildcommands   { "genie video-version-bump", "genie install-video-sdk-win32-release"}

            configuration { "x64", "vs*", "debug" }
                    postbuildcommands   { "genie video-version-bump", "genie install-video-sdk-win64-debug"}

            configuration { "x64", "vs*", "release" }
                    postbuildcommands   { "genie video-version-bump", "genie install-video-sdk-win64-release"}

    configuration  { "macosx" }
            premake.xcode.parameters = {'CLANG_CXX_LANGUAGE_STANDARD = "c++11"'}
            postbuildcommands         "${SRCROOT}/genie install-video-sdk-macos"
            includedirs               { path.join(macSdkPath, "include") }
            links                     { "SceneTrackStatic" }
            libdirs                   { path.join(macSdkPath, "lib") }
    
end

function install(_type)

  if _type == "Win32" then
	  if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Unity Plugin Base")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library")
      os.copyfile("../../Build/SceneTrack.cs", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/SceneTrack.cs")
      
      printf("Installing Windows32 Plugin to Unity Plugin")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows")
      os.copyfile("../../Build/Win32/SceneTrack.dll", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows/SceneTrack.dll")
    end

  elseif _type == "Win64" then
	  if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Unity Plugin Base")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library")
      os.copyfile("../../Build/SceneTrack.cs", "../../../GCAP.Unity/Assets/Assets/Plugins/SceneTrack/Library/SceneTrack.cs")
      
      printf("Installing Windows32 Plugin to Unity Plugin")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows")
      os.copyfile("../../Build/Win64/SceneTrack.dll", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows/SceneTrack.dll")
    end

  elseif _type == "macOS" then
    if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Unity Plugin Base")
      os.outputof("mkdir -p ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/")
      os.outputof("cp -R ../../Build/SceneTrack.cs ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/")

      printf("Installing Mac Plugin to Unity Plugin")
      os.outputof("mkdir -p ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Mac")
      os.outputof("cp -R ../../Build/macOS/SceneTrack.bundle ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Mac")
    end
  end

end

function installFbxSdk(_type, _isDebug)
  src = ""
  dst = ""

  -- SceneTrackFbx Library

  if _type == "Win32" then
	  if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Fbx Unity Plugin Base")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library")
      os.copyfile("../../Build/SceneTrackFbx.cs", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/SceneTrackFbx.cs")
      
      printf("Installing Fbx Windows32 Plugin to Unity Plugin")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows")
      os.copyfile("../../Build/Win32/SceneTrackFbx.dll", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows/SceneTrackFbx.dll")
    end

  elseif _type == "Win64" then
	  if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Fbx Unity Plugin Base")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library")
      os.copyfile("../../Build/SceneTrackFbx.cs", "../../../GCAP.Unity/Assets/Assets/Plugins/SceneTrack/Library/SceneTrackFbx.cs")
      
      printf("Installing Fbx Windows64 Plugin to Unity Plugin")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows")
      os.copyfile("../../Build/Win64/SceneTrackFbx.dll", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows/SceneTrackFbx.dll")
    end

  elseif _type == "macOS" then
    if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Fbx Unity Plugin Base")
      os.outputof("mkdir -p ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/")
      os.outputof("cp -R ../../Build/SceneTrackFbx.cs ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/")

      printf("Installing Fbx Mac Plugin to Unity Plugin")
      os.outputof("mkdir -p ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Mac")
      os.outputof("cp -R ../../Build/macOS/SceneTrackFbx.bundle ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Mac")
    end
  end
end

function installMidiSdk(_type, _isDebug)
  src = ""
  dst = ""

  if _type == "Win32" then
	  if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Midi Unity Plugin Base")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library")
      os.copyfile("../../Build/SceneTrackMidi.cs", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/SceneTrackMidi.cs")
      
      printf("Installing Midi Windows32 Plugin to Unity Plugin")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows")
      os.copyfile("../../Build/Win32/SceneTrackMidi.dll", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows/SceneTrackMidi.dll")
    end

  elseif _type == "Win64" then
	  if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Midi Unity Plugin Base")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library")
      os.copyfile("../../Build/SceneTrackMidi.cs", "../../../GCAP.Unity/Assets/Assets/Plugins/SceneTrack/Library/SceneTrackMidi.cs")
      
      printf("Installing Midi Windows64 Plugin to Unity Plugin")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows")
      os.copyfile("../../Build/Win64/SceneTrackMidi.dll", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows/SceneTrackMidi.dll")
    end

  elseif _type == "macOS" then
    if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Midi Unity Plugin Base")
      os.outputof("mkdir -p ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/")
      os.outputof("cp -R ../../Build/SceneTrackMidi.cs ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/")

      printf("Installing Midi Mac Plugin to Unity Plugin")
      os.outputof("mkdir -p ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Mac")
      os.outputof("cp -R ../../Build/macOS/SceneTrackMidi.bundle ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Mac")
    end
  end
end



function installVideoSdk(_type, _isDebug)
  src = ""
  dst = ""

  -- SceneTrackVideo Library

  if _type == "Win32" then
	  if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Video Unity Plugin Base")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library")
      os.copyfile("../../Build/SceneTrackVideo.cs", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/SceneTrackVideo.cs")
      
      printf("Installing Video Windows32 Plugin to Unity Plugin")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows")
      os.copyfile("../../Build/Win32/SceneTrackVideo.dll", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows/SceneTrackVideo.dll")
    end

  elseif _type == "Win64" then
	  if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Video Unity Plugin Base")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library")
      os.copyfile("../../Build/SceneTrackVideo.cs", "../../../GCAP.Unity/Assets/Assets/Plugins/SceneTrack/Library/SceneTrackVideo.cs")
      
      printf("Installing Video Windows64 Plugin to Unity Plugin")
      os.mkdir("../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows")
      os.copyfile("../../Build/Win64/SceneTrackVideo.dll", "../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Windows/SceneTrackVideo.dll")
    end

  elseif _type == "macOS" then
    if (os.isdir("../../../GCAP.Unity")) then
      printf("Installing Video Unity Plugin Base")
      os.outputof("mkdir -p ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/")
      os.outputof("cp -R ../../Build/SceneTrackVideo.cs ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/")

      printf("Installing Video Mac Plugin to Unity Plugin")
      os.outputof("mkdir -p ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Mac")
      os.outputof("cp -R ../../Build/macOS/SceneTrackVideo.bundle ../../../GCAP.Unity/Assets/Plugins/SceneTrack/Library/Mac")
    end
  end
end


function securityCheck()
  f = io.open("../Source/stSecurity.h", "w")
  f:write("#define ST_EXPIRES (" ..  (os.time() + 1209600) .. ")\n")
  f:close()
end
function print_r(arr, indentLevel)
    local str = ""
    local indentStr = "#"

    if(indentLevel == nil) then
        print(print_r(arr, 0))
        return
    end

    for i = 0, indentLevel do
        indentStr = indentStr.."\t"
    end

    for index,value in pairs(arr) do
        if type(value) == "table" then
            str = str..indentStr..index..": \n"..print_r(value, (indentLevel + 1))
        else 
            str = str..indentStr..index..": "..value.."\n"
        end
    end
    return str
end

function versionBump(version_h, name)
  f = io.open("../Source/" .. version_h, "r")
  local vers_data_str = f:read("*all")
  f:close()

  local vstr = string.sub(vers_data_str, 3, string.find(vers_data_str, '\n'))
  local version_info = string.explode(vstr, ',')

  version_info[4] = tonumber(version_info[4]) + 1
 name_upper = string.upper(name)

  f = io.open("../Source/" .. version_h, "w")
  f:write("//" .. version_info[1] .. "," .. version_info[2] .. "," .. version_info[3] .. "," .. version_info[4] .. "\n")
  f:write("#ifndef " .. name_upper .. "_VERSION_H\n")
  f:write("#define " .. name_upper .. "_VERSION_H\n")
  f:write("#define " .. name_upper .. "_VERSION_NAME \""  .. version_info[1] .. "\"\n")
  f:write("#define " .. name_upper .. "_VERSION_MAJOR "   .. version_info[2] .. "\n")
  f:write("#define " .. name_upper .. "_VERSION_MINOR "   .. version_info[3] .. "\n")
  f:write("#define " .. name_upper .. "_VERSION_BUILD "   .. version_info[4] .. "\n")
  
  f:write("#define " .. name_upper .. "_VERSION_STR \""  .. version_info[1] .. " " .. version_info[2] .. "." .. version_info[3] .. "." .. version_info[4] .. "\"\n")

  f:write("#endif\n")
  f:close()

end

newoption {
    trigger     = "tests",
    description = "Include Tests"
}

newaction {
  trigger = "install-win32",
  description = "Install the library into Integrations",
  execute = function()
    install("Win32")
  end
}

newaction {
  trigger = "install-win64",
  description = "Install the library into Integrations",
  execute = function()
    install("Win64")
  end
}

newaction {
  trigger = "install-osx",
  description = "Install the library into Integrations",
  execute = function()
    install("macOS")
  end
}

newaction {
  trigger = "install-fbx-sdk-win32-debug",
  description = "Install the AutoDesk Fbx SDK library into the build folder for Win32-Debug",
  execute = function()
    installFbxSdk("Win32", true)
  end
}

newaction {
  trigger = "install-fbx-sdk-win32-release",
  description = "Install the AutoDesk Fbx SDK library into the build folder for Win32-Release",
  execute = function()
    installFbxSdk("Win32", false)
  end
}

newaction {
  trigger = "install-fbx-sdk-win64-debug",
  description = "Install the AutoDesk Fbx SDK library into the build folder for Win32-Debug",
  execute = function()
    installFbxSdk("Win64", true)
  end
}

newaction {
  trigger = "install-fbx-sdk-win64-release",
  description = "Install the AutoDesk Fbx SDK library into the build folder for Win32-Release",
  execute = function()
    installFbxSdk("Win64", false)
  end
}

newaction {
  trigger = "install-fbx-sdk-macos",
  description = "Install the AutoDesk Fbx SDK library into the build folder for macOS",
  execute = function()
    installFbxSdk("macOS", false)
  end
}


newaction {
  trigger = "install-midi-sdk-win32-debug",
  description = "Install the Midi library into the build folder for Win32-Debug",
  execute = function()
    installMidiSdk("Win32", true)
  end
}

newaction {
  trigger = "install-midi-sdk-win32-release",
  description = "Install the Midi SDK library into the build folder for Win32-Release",
  execute = function()
    installMidiSdk("Win32", false)
  end
}

newaction {
  trigger = "install-midi-sdk-win64-debug",
  description = "Install the Midi SDK library into the build folder for Win32-Debug",
  execute = function()
    installMidiSdk("Win64", true)
  end
}

newaction {
  trigger = "install-midi-sdk-win64-release",
  description = "Install the Midi SDK library into the build folder for Win32-Release",
  execute = function()
    installMidiSdk("Win64", false)
  end
}

newaction {
  trigger = "install-midi-sdk-macos",
  description = "Install the Midi SDK library into the build folder for macOS",
  execute = function()
    installMidiSdk("macOS", false)
  end
}


newaction {
  trigger = "install-video-sdk-win32-debug",
  description = "Install the Video library into the build folder for Win32-Debug",
  execute = function()
    installVideoSdk("Win32", true)
  end
}

newaction {
  trigger = "install-video-sdk-win32-release",
  description = "Install the Video SDK library into the build folder for Win32-Release",
  execute = function()
    installVideoSdk("Win32", false)
  end
}

newaction {
  trigger = "install-video-sdk-win64-debug",
  description = "Install the Video SDK library into the build folder for Win32-Debug",
  execute = function()
    installVideoSdk("Win64", true)
  end
}

newaction {
  trigger = "install-video-sdk-win64-release",
  description = "Install the Video SDK library into the build folder for Win32-Release",
  execute = function()
    installVideoSdk("Win64", false)
  end
}

newaction {
  trigger = "install-video-sdk-macos",
  description = "Install the Video SDK library into the build folder for macOS",
  execute = function()
    installVideoSdk("macOS", false)
  end
}

newaction {
  trigger = "security",
  description = "Security",
  execute = function()
    securityCheck();
  end
}

newaction {
  trigger = "st-version-bump",
  description = "Write stVersion and bump build number",
  execute = function()
    versionBump('stVersion.h', 'SceneTrack')
  end
}

newaction {
  trigger = "fbx-version-bump",
  description = "Write fbxVersion and bump build number",
  execute = function()
    versionBump('fbxVersion.h', 'SceneTrackFbx')
  end
}

newaction {
  trigger = "midi-version-bump",
  description = "Write midiVersion and bump build number",
  execute = function()
    versionBump('midiVersion.h', 'SceneTrackMidi')
  end
}

newaction {
  trigger = "video-version-bump",
  description = "Write videoVersion and bump build number",
  execute = function()
    versionBump('videoVersion.h', 'SceneTrackVideo')
  end
}


function makeSolution(_name, _tests, _exporters)

  solution(_name)

      configurations      { "Debug", "Release" }
      platforms           { "x64" }

      targetdir           "../../Build"
      debugdir            "../../Build"

      configuration "Debug"
        defines           { "DEBUG"   }
        flags             { "Symbols" }

      configuration "Release"
        defines           { "NDEBUG"  }
        flags             { "Optimize" }

      configuration  { "x32", "vs*" }
        targetdir         "../../Build/Win32"
        defines      { }
      configuration  { "x64", "vs*" }
        targetdir         "../../Build/Win64"
      configuration  { "macosx"}
        targetdir         "../../Build/macOS"

      libraryFile = "UNKNOWN"
      libraryKind = "UNKNOWN"
      unityFile = "UNKNOWN"
      unityKind = "UNKNOWN"

      if (os.get() == "windows") then
        libraryFile = "SceneTrackStatic"
        libraryKind = "StaticLib" 
        unityFile = "SceneTrack"
        unityKind = "SharedLib"     
      else -- OSX
        libraryFile = "SceneTrackStatic"
        libraryKind = "StaticLib"   -- For Internal Builds Only
        unityFile = "SceneTrack"
        unityKind = "Bundle"
      end

      makeApiProject("Tool", "ApiBuilder")

      if (_tests) then

        makeTest("Test", "Basics", libraryFile)
        makeTest("Test", "BigData", libraryFile)
        makeTest("Test", "Memory", libraryFile)
        makeTest("Test", "File", libraryFile)
        makeTest("Test", "ErrorWriting", libraryFile)
        makeTest("Test", "BadMemory", libraryFile)
        makeTest("Test", "ObjectHeaders", libraryFile)
        makeTest("Test", "Milestone1", libraryFile)
        makeTest("Test", "Milestone2", libraryFile)
        makeTest("Test", "Milestone3", libraryFile)
        makeTest("Test", "Validation", libraryFile)
        makeTest("Test", "Array", libraryFile)
      end

      --------------------------------------------------------------------------

      if (unityFile == libraryFile) then
        makeStProject(unityFile, unityKind)
      else
        makeStProject(unityFile, unityKind)
        makeStProject(libraryFile, libraryKind)
      end

      --------------------------------------------------------------------------

      if (_exporters) then
        makeCppFbxExporterProject("SceneTrackFbx", unityFile, unityKind)
        makeCppMidiExporterProject("SceneTrackMidi", unityFile, unityKind)
        makeCppVideoExporterProject("SceneTrackVideo", unityFile, unityKind)

        if (_tests) then
          makeFbxExportTest("Test", "FbxExportCubes")
          makeFbxImportTest("Test", "FbxImportCubes")
          makeMidiTest("Test", "MidiSave")
        end
      end
      --------------------------------------------------------------------------

      startproject ("SceneTrack")

      --------------------------------------------------------------------------

end

if _OPTIONS["tests"] then
  kBuildTests = true
end

makeSolution("SceneTrack", kBuildTests, kBuildExporters)
