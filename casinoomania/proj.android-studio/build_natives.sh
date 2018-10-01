#! /bin/sh

APPNAME="Dovemobi Casino"

clear

source ~/.bash_profile

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

variant="$1"

if  [ "$1" = "" ]
then
variant="Debug"
fi

echo $variant

# paths

if [ -z "${NDK_ROOT+aaa}" ];then
echo "please define NDK_ROOT"
exit 1
fi

# ... use paths relative to current directory
COCOS2DX_ROOT="$DIR/.."
APP_ROOT="$DIR/.."
APP_ANDROID_ROOT="$DIR/app"


echo ""
echo "===== Paths ====="
echo "NDK_ROOT = $NDK_ROOT"
echo "COCOS2DX_ROOT = $COCOS2DX_ROOT"
echo "APP_ROOT = $APP_ROOT"
echo "APP_ANDROID_ROOT = $APP_ANDROID_ROOT"
echo ""
echo "BUILD MODE: $variant"
echo ""
echo "===== Build ====="

# make sure assets are changed
scriptDir=$DIR
resourceDir="$scriptDir/../Resources"


# make sure assets is exist
assets=0
if [ -d "$APP_ANDROID_ROOT"/src/main/assets ]; then
assets=1
fi


echo "Removing all old assets…"
rm -rf "$APP_ANDROID_ROOT"/src/main/assets

mkdir "$APP_ANDROID_ROOT"/src/main/assets

# copy resources
echo "Copy new assets…"

for file in "$APP_ROOT"/Resources/*
do
if [ -d "$file" ]; then
cp -rf "$file" "$APP_ANDROID_ROOT"/src/main/assets
fi

if [ -f "$file" ]; then
cp "$file" "$APP_ANDROID_ROOT"/src/main/assets
fi
done

echo "Removing HDR assets…"
rm -rf "$APP_ANDROID_ROOT"/src/main/assets/HDR

# run ndk-build
echo "Build native code"
if [ "$1" = "Release" ]
then
"$NDK_ROOT"/ndk-build -C "$DIR/app" NDK_DEBUG=0
else
"$NDK_ROOT"/ndk-build -C "$DIR/app" NDK_DEBUG=1
fi

OUT=$?
if [ $OUT -eq 0 ]; then
echo "Compilation Successful."
else
echo "***ERROR*** Error occurred during compilation."
exit 1
fi


echo "Moving .so libraries"
if [ -d "$APP_ANDROID_ROOT/src/main/jniLibs" ]; then
rm -rf "$APP_ANDROID_ROOT/src/main/jniLibs"
fi

mkdir "$APP_ANDROID_ROOT/src/main/jniLibs"

for file in $APP_ANDROID_ROOT/libs/**
do
if [ -d "$file" ]; then
cp -rf "$file" "$APP_ANDROID_ROOT/src/main/jniLibs"
fi
done
