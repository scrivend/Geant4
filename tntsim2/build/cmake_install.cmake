# Install script for directory: /home/scrivend/Geant4/tntsim2

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./include" TYPE FILE FILES
    "/home/scrivend/Geant4/tntsim2/include/LXeScintSD.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeUserEventInformation.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeSteppingMessenger.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeEMPhysics.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeRecorderBase.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeStackingAction.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXePMTSD.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeWLSFiber.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXePhysicsList.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeSteppingVerbose.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeEventMessenger.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeEventAction.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeSteppingAction.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeMuonPhysics.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeDetectorConstruction.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeActionInitialization.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXePrimaryGeneratorAction.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeScintHit.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeMainVolume.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeGeneralPhysics.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeDataRecordTree.hh"
    "/home/scrivend/Geant4/tntsim2/include/menate_R.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeRunAction.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeUserTrackInformation.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeTrajectory.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeNuclearPhysics.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXePMTHit.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeTrackingAction.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeDetectorMessenger.hh"
    "/home/scrivend/Geant4/tntsim2/include/LXeWLSSlab.hh"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/scrivend/Geant4/tntsim2/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
