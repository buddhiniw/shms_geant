# --------------------------------------------------------------
# GNUmakefile for SHMS geant4 simulation package. 
# B.Waidyawansa 10-11-2013.
# --------------------------------------------------------------

name := shms_geant
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

