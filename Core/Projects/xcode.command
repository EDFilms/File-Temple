#!/bin/bash
cd "$(dirname "$0")"

rm -R *.xcodeproj
rm -R *.xcworkspace
cd ..
cd ..
cd Build
cd macOS
rm *.dylib
rm *.a
rm -R *.bundle
rm *.dll
rm *.lib
rm *.h
rm *.cs
rm Test*
rm Tool*
rm *.st
cd ..
cd ..
cd Core
cd Projects

./genie xcode4

xcodebuild -project ToolApiBuilder.xcodeproj

../../Build/make_api_xcode.command

xcodebuild -project SceneTrack.xcodeproj
xcodebuild -project SceneTrackStatic.xcodeproj
xcodebuild -project SceneTrackFbx.xcodeproj
xcodebuild -project SceneTrackMidi.xcodeproj
xcodebuild -project SceneTrackVideo.xcodeproj

