#!/bin/csh

# ENV for running GEANT on the cdaql3 

source /data1/cdaq/geant4.tmp/geant4.9.6-p2/geant4-build/geant4make.csh
setenv G4INSTALL  /data1/cdaq/geant4.tmp/geant4.9.6-p2/geant4-install
cd $G4INSTALL/bin ; source geant4.csh
setenv G4WORKDIR ~/geant4work/examples/advanced/shms_geant
cd $G4WORKDIR
setenv G4LIB_USE_GDML 1
setenv G4UI_USE 1
setenv G4VIS_USE 1
setenv GEANT4_USE_QT 1
