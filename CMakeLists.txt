cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

OPTION(DEFINE_DEBUG
  "Build the project using debugging code"
  OFF)
IF(DEFINE_DEBUG)
  MESSAGE("Adding Debug flag...")
  ADD_DEFINITIONS(-DDEFINE_DEBUG)
  #SET(CMAKE_BUILD_TYPE Debug)
  MESSAGE("Build type is " ${CMAKE_BUILD_TYPE})
ENDIF(DEFINE_DEBUG)


set(SHORT_NAME FNaF)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Five Nights at Freddys")
set(VITA_TITLEID  "FNAF00001")
set(VITA_VERSION  "02.50")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=gnu14 -Wall ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall -Wno-psabi ")


find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(
  ../common
)

add_executable(${SHORT_NAME}.elf
src/game.cpp
src/menu.cpp
src/main.cpp
)

target_link_libraries(${SHORT_NAME}.elf


  debugnet
  vita2d
  soloud
  pthread
  ScePgf_stub
  ScePvf_stub
  SceGxm_stub
  SceCtrl_stub
  SceTouch_stub
  SceAppUtil_stub
  SceDisplay_stub
  SceLibKernel_stub
  SceSysmodule_stub
  SceSysmem_stub
  ScePower_stub
  SceCommonDialog_stub
  SceRegistryMgr_stub
  SceHid_stub
  freetype
  png
  jpeg
  SceAudio_stub
  SceDisplay_stub
  SceIofilemgr_stub
  SceSysmodule_stub
  SceNet_stub
  SceNetCtl_stub
  SceHttp_stub
  SceSsl_stub
  jansson
  ${CURL_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  c
  m
  z
)


vita_create_self(${SHORT_NAME}.self ${SHORT_NAME}.elf)

vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE assets/font/FNaF.ttf font/FNaF.ttf
  FILE assets/images/menu0.png images/menu0.png
  FILE assets/images/menu1.png images/menu1.png
  FILE assets/images/menu2.png images/menu2.png
  FILE assets/images/menu3.png images/menu3.png
  FILE assets/images/static0.png images/static0.png
  FILE assets/images/static1.png images/static1.png
  FILE assets/images/static2.png images/static2.png
  FILE assets/images/static3.png images/static3.png
  FILE assets/images/static4.png images/static4.png
  FILE assets/images/static5.png images/static5.png
  FILE assets/images/static6.png images/static6.png
  FILE assets/images/static7.png images/static7.png
  FILE assets/images/gfreddy.png images/gfreddy.png
  FILE assets/images/bear.png images/bear.png
  FILE assets/images/bunny.png images/bunny.png
  FILE assets/images/bird.png images/bird.png
  FILE assets/images/fox.png images/fox.png
  FILE assets/images/paper.png images/paper.png
  FILE assets/images/clock.png images/clock.png
  FILE assets/images/office_on.png images/office_on.png
  FILE assets/images/office_onL.png images/office_onL.png
  FILE assets/images/office_onR.png images/office_onR.png
  FILE assets/images/fan1.png images/fan1.png
  FILE assets/images/fan2.png images/fan2.png
  FILE assets/images/fan3.png images/fan3.png
  FILE assets/images/ldoor1.png images/ldoor1.png
  FILE assets/images/ldoor2.png images/ldoor2.png
  FILE assets/images/ldoor3.png images/ldoor3.png
  FILE assets/images/ldoor4.png images/ldoor4.png
  FILE assets/images/ldoor5.png images/ldoor5.png
  FILE assets/images/ldoor6.png images/ldoor6.png
  FILE assets/images/ldoor7.png images/ldoor7.png
  FILE assets/images/ldoor8.png images/ldoor8.png
  FILE assets/images/ldoor9.png images/ldoor9.png
  FILE assets/images/ldoor10.png images/ldoor10.png
  FILE assets/images/ldoor11.png images/ldoor11.png
  FILE assets/images/ldoor12.png images/ldoor12.png
  FILE assets/images/ldoor13.png images/ldoor13.png
  FILE assets/images/rdoor1.png images/rdoor1.png
  FILE assets/images/rdoor2.png images/rdoor2.png
  FILE assets/images/rdoor3.png images/rdoor3.png
  FILE assets/images/rdoor4.png images/rdoor4.png
  FILE assets/images/rdoor5.png images/rdoor5.png
  FILE assets/images/rdoor6.png images/rdoor6.png
  FILE assets/images/rdoor7.png images/rdoor7.png
  FILE assets/images/rdoor8.png images/rdoor8.png
  FILE assets/images/rdoor9.png images/rdoor9.png
  FILE assets/images/rdoor10.png images/rdoor10.png
  FILE assets/images/rdoor11.png images/rdoor11.png
  FILE assets/images/rdoor12.png images/rdoor12.png
  FILE assets/images/rdoor13.png images/rdoor13.png
  FILE assets/images/fredscare1.png images/fredscare1.png
  FILE assets/images/fredscare2.png images/fredscare2.png
  FILE assets/images/fredscare3.png images/fredscare3.png
  FILE assets/images/fredscare4.png images/fredscare4.png
  FILE assets/images/fredscare5.png images/fredscare5.png
  FILE assets/images/fredscare6.png images/fredscare6.png
  FILE assets/images/fredscare7.png images/fredscare7.png
  FILE assets/images/fredscare8.png images/fredscare8.png
  FILE assets/images/fredscare9.png images/fredscare9.png
  FILE assets/images/fredscare10.png images/fredscare10.png
  FILE assets/images/fredscare11.png images/fredscare11.png
  FILE assets/images/fredscare12.png images/fredscare12.png
  FILE assets/images/fredscare13.png images/fredscare13.png
  FILE assets/images/fredscare14.png images/fredscare14.png
  FILE assets/images/fredscare15.png images/fredscare15.png
  FILE assets/images/fredscare16.png images/fredscare16.png
  FILE assets/images/fredscare17.png images/fredscare17.png
  FILE assets/images/fredscare18.png images/fredscare18.png
  FILE assets/images/fredscare19.png images/fredscare19.png
  FILE assets/images/fredscare20.png images/fredscare20.png
  FILE assets/images/fredscare21.png images/fredscare21.png
  FILE assets/images/fredscare22.png images/fredscare22.png
  FILE assets/images/fredscare23.png images/fredscare23.png
  FILE assets/images/fredscare24.png images/fredscare24.png
  FILE assets/images/fredscare25.png images/fredscare25.png
  FILE assets/images/fredscare26.png images/fredscare26.png
  FILE assets/images/fredscare27.png images/fredscare27.png
  FILE assets/images/fredscare28.png images/fredscare28.png
  FILE assets/images/fredscare29.png images/fredscare29.png
  FILE assets/images/fredscare30.png images/fredscare30.png
  FILE assets/sounds/menu.wav sounds/menu.wav
  FILE assets/sounds/static2.wav sounds/static2.wav
  FILE assets/sounds/XSCREAM.wav sounds/XSCREAM.wav
  FILE assets/sounds/XSCREAM2.wav sounds/XSCREAM2.wav
  FILE assets/sounds/blip.wav sounds/blip.wav
  FILE assets/sounds/fanbuzz.wav sounds/fanbuzz.wav
  FILE assets/sounds/lightbuzz.wav sounds/lightbuzz.wav
  FILE assets/sounds/doorshut.wav sounds/doorshut.wav
  FILE assets/sounds/voiceover1.wav sounds/voiceover1.wav
  FILE assets/sounds/voiceover2.wav sounds/voiceover2.wav
  FILE assets/sounds/voiceover3.wav sounds/voiceover3.wav
  FILE assets/sounds/voiceover4.wav sounds/voiceover4.wav
  FILE assets/sounds/voiceover5.wav sounds/voiceover5.wav
  #FILE images/theme/default/gary.png images/theme/default/gary.png
)
