# $Id: GNUmakefile 68752 2013-04-05 10:23:47Z gcosmo $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := OpNovice
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

#include $(G4INSTALL)/config/binmake.gmk
include $(G4INSTALL)/share/Geant4-10.1.2/geant4make/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
