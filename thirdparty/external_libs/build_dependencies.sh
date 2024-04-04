#! /bin/bash
handle_error() {
  echo "An error occurred. Exiting..."
  rm -rf $ScriptDir/../bins
  rm -rf $ScriptDir/../include 
  rm -rf $ScriptDir/../libs 
  exit 1
}

trap handle_error ERR

echo -e "\n============================================================================"
echo -e "Creating directories"
echo -e "============================================================================\n"

declare ScriptDir=$(dirname "$0")

mkdir -p $ScriptDir/../bins
mkdir -p $ScriptDir/../include
mkdir -p $ScriptDir/../include/stb
mkdir -p $ScriptDir/../include/imgui
mkdir -p $ScriptDir/../libs

echo -e "\n============================================================================"
echo -e "Start Building Assimp"
echo -e "============================================================================\n"

unzip -o $ScriptDir/assimp-5.3.1.zip -d $ScriptDir/temp > /dev/null

cmake $ScriptDir/temp/assimp-5.3.1 -B$ScriptDir/temp/assimp-5.3.1/build \
  -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=OFF  \
  -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=OFF \
  -DASSIMP_BUILD_ASSIMP_TOOLS=OFF \
  -DASSIMP_BUILD_TESTS=OFF \
  -DASSIMP_BUILD_FBX_IMPORTER=ON \
  -DASSIMP_BUILD_OBJ_IMPORTER=ON \
  -DASSIMP_BUILD_ZLIB=ON \
  -DASSIMP_LIBRARY_SUFFIX="" \
  -DASSIMP_INJECT_DEBUG_POSTFIX=OFF \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_DEBUG_POSTFIX=""

cmake --build $ScriptDir/temp/assimp-5.3.1/build

cp -r $ScriptDir/temp/assimp-5.3.1/include/ $ScriptDir/../include
cp -r $ScriptDir/temp/assimp-5.3.1/build/include/ $ScriptDir/../include
find $ScriptDir/temp/assimp-5.3.1/build \( -name '*.lib' -o -name '*.dylib' \) -type f -exec cp '{}' $ScriptDir/../libs \;
find $ScriptDir/temp/assimp-5.3.1/build \( -name '*.pdb' -o -name '*.a' \) -type f -exec cp '{}' $ScriptDir/../bins \;

echo -e "\n============================================================================"
echo -e "Finish Building Assimp"
echo -e "============================================================================\n"

echo -e "\n============================================================================"
echo -e "Start Building SDL2"
echo -e "============================================================================\n"

unzip -o $ScriptDir/SDL2-2.28.5.zip -d $ScriptDir/temp > /dev/null

cmake $ScriptDir/temp/SDL2-2.28.5 -B$ScriptDir/temp/SDL2-2.28.5/build \
  -DSDL2_DISABLE_INSTALL=ON \
  -DSDL2_DISABLE_UNINSTALL=ON \
  -DSDL_TEST=OFF \
  -DSDL_SHARED=ON \
  -DSDL_STATIC=OFF \
  -DCMAKE_DEBUG_POSTFIX=""

cmake --build $ScriptDir/temp/SDL2-2.28.5/build --config Release

cp -r $ScriptDir/temp/SDL2-2.28.5/build/include/ $ScriptDir/../include
cp -r $ScriptDir/temp/SDL2-2.28.5/build/include-config-/ $ScriptDir/../include
find $ScriptDir/temp/SDL2-2.28.5/build \( -name '*.lib' -o -name '*.dylib' \) -type f -exec cp '{}' $ScriptDir/../libs \;
find $ScriptDir/temp/SDL2-2.28.5/build \( -name '*.pdb' -o -name '*.a' \) -type f -exec cp '{}' $ScriptDir/../bins \;

echo -e "\n============================================================================"
echo -e "Finish Building SDL2"
echo -e "============================================================================\n"

# echo -e "\n============================================================================"
# echo -e "Start Building GLFW"
# echo -e "============================================================================\n"

# unzip -o $ScriptDir/glfw-3.3.8.zip -d $ScriptDir/temp > /dev/null

# cmake $ScriptDir/temp/glfw-3.3.8 -B$ScriptDir/temp/glfw-3.3.8/build \
#   -DGLFW_BUILD_EXAMPLES=OFF \
#   -DGLFW_BUILD_TESTS=OFF \
#   -DGLFW_BUILD_DOCS=OFF \
#   -DGLFW_INSTALL=OFF \
#   -DGLFW_VULKAN_STATIC=OFF \
#   -DBUILD_SHARED_LIBS=OFF \
#   -DCMAKE_DEBUG_POSTFIX=""

# cmake --build $ScriptDir/temp/glfw-3.3.8/build --config Release

# cp -r $ScriptDir/temp/glfw-3.3.8/include/ $ScriptDir/../include
# find $ScriptDir/temp/glfw-3.3.8/build \( -name '*.lib' -o -name '*.dylib' \) -type f -exec cp '{}' $ScriptDir/../libs \;
# find $ScriptDir/temp/glfw-3.3.8/build \( -name '*.pdb' -o -name '*.a' \) -type f -exec cp '{}' $ScriptDir/../bins \;

# echo -e "\n============================================================================"
# echo -e "Finish Building GLFW"
# echo -e "============================================================================\n"

echo -e "\n============================================================================"
echo -e "Start Building GLEW"
echo -e "============================================================================\n"

unzip -o $ScriptDir/glew-2.2.0.zip -d $ScriptDir/temp > /dev/null

cmake $ScriptDir/temp/glew-2.2.0/build/cmake -B$ScriptDir/temp/glew-2.2.0/new_build \
    -DBUILD_UTILS=OFF \
    -DBUILD_SHARED_LIBS=OFF

cmake --build $ScriptDir/temp/glew-2.2.0/new_build --config Release

cp -r $ScriptDir/temp/glew-2.2.0/include/ $ScriptDir/../include
find $ScriptDir/temp/glew-2.2.0/new_build \( -name '*.lib' -o -name '*.dylib' \) -type f -exec cp '{}' $ScriptDir/../libs \;
find $ScriptDir/temp/glew-2.2.0/new_build \( -name '*.pdb' -o -name '*.a' \) -type f -exec cp '{}' $ScriptDir/../bins \;

echo -e "\n============================================================================"
echo -e "Finish Building GLEW"
echo -e "============================================================================\n"

echo -e "\n============================================================================"
echo -e "Unpacking GLM"
echo -e "============================================================================\n"

unzip -o $ScriptDir/glm-0.9.9.8.zip -d $ScriptDir/temp > /dev/null
cp -r $ScriptDir/temp/glm/glm $ScriptDir/../include

echo -e "\n============================================================================"
echo -e "Unpacking STB"
echo -e "============================================================================\n"

unzip -o $ScriptDir/stb.zip -d $ScriptDir/temp > /dev/null
cp -r $ScriptDir/temp/stb $ScriptDir/../include

echo -e "\n============================================================================"
echo -e "Unpacking ImGui"
echo -e "============================================================================\n"

unzip -o $ScriptDir/imgui-1.90.1.zip -d $ScriptDir/temp > /dev/null

find $ScriptDir/temp/imgui-1.90.1 \( -name '*.c*' -o -name '*.h*' \) -maxdepth 1 -type f -exec cp '{}' $ScriptDir/../include/imgui \;
cp -r $ScriptDir/temp/imgui-1.90.1/backends $ScriptDir/../include/imgui

echo -e "\n============================================================================"
echo -e "Cleaning"
echo -e "============================================================================\n"
rm -rf $ScriptDir/temp
echo -e "\n============================================================================"
echo -e "Finished"
echo -e "============================================================================"
