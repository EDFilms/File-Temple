call "clean_vs2015.bat"
call "make_vs2015.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"

msbuild SceneTrack.sln /t:ToolApiBuilder /p:Configuration="Release" /p:Platform="x64" /p:BuildProjectReferences=false

cd ..
cd ..
cd Build

call "make_api_vs2015.bat"


cd ..
cd Core
cd Projects

msbuild SceneTrack.sln /t:SceneTrack /p:Configuration="Release" /p:Platform="x64" /p:BuildProjectReferences=false
msbuild SceneTrack.sln /t:SceneTrackFbx /p:Configuration="Release" /p:Platform="x64" /p:BuildProjectReferences=false
msbuild SceneTrack.sln /t:SceneTrackMidi /p:Configuration="Release" /p:Platform="x64" /p:BuildProjectReferences=false
msbuild SceneTrack.sln /t:SceneTrackVideo /p:Configuration="Release" /p:Platform="x64" /p:BuildProjectReferences=false
