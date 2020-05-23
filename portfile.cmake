include(vcpkg_common_functions)

set(XLL12_VERSION 12.0.0)
set(XLL12_HASH 1)

#architecture detection
if(VCPKG_TARGET_ARCHITECTURE STREQUAL "x86")
   set(XLL12_ARCH Win32)
   set(XLL12_CONFIGURATION _x86)
elseif(VCPKG_TARGET_ARCHITECTURE STREQUAL "x64")
   set(XLL12_ARCH x64)
   set(XLL12_CONFIGURATION _x86)
else()
   message(FATAL_ERROR "unsupported architecture")
endif()

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO xll12
    REF master
    SHA512 1
    HEAD_REF master
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
)

vcpkg_build_msbuild(
    PROJECT_PATH ${SOURCE_PATH}/xll12.vcxproj
)
