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
