# Install script for directory: /home/gacgroup/shuyaota/Geant4/tntsim3

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/gacgroup/shuyaota/install")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./include" TYPE FILE FILES
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXePrimaryGeneratorAction.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXePMTSD.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeMuonPhysics.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeWLSFiber.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeDetectorConstruction.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeRecorderBase.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/menate_R.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeEventMessenger.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeSteppingMessenger.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeUserTrackInformation.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeStackingAction.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeTrackingAction.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeUserEventInformation.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeEMPhysics.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeGeneralPhysics.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXePhysicsList.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeEventAction.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeScintSD.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeDataRecordTree.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeTrajectory.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeSteppingAction.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeActionInitialization.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeNuclearPhysics.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeScintHit.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeSteppingVerbose.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeDetectorMessenger.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeRunAction.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeWLSSlab.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXeMainVolume.hh"
    "/home/gacgroup/shuyaota/Geant4/tntsim3/include/LXePMTHit.hh"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/gacgroup/shuyaota/Geant4/tntsim3/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/gacgroup/shuyaota/Geant4/tntsim3/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
