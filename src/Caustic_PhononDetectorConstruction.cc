/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// 20241024 Israel Hernandez -- IIT, QSC and Fermilab
// 20250101 Michael Kelsey -- Instantiate SD in ConstructSDandField();
//	      remove many unnecessary blank lines.
// 20251117 G4CMP-541 -- For G4 v11, replace ::Invisible w/::GetInvisible()

#include "Caustic_PhononDetectorConstruction.hh"
#include "Caustic_PhononSensitivity.hh"
#include "G4CMPLogicalBorderSurface.hh"
#include "G4CMPPhononElectrode.hh"
#include "G4CMPSurfaceProperty.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GeometryManager.hh"
#include "G4LatticeLogical.hh"
#include "G4LatticeManager.hh"
#include "G4LatticePhysical.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4Tubs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "Caustic_PhononConfigManager.hh"

#include "G4SubtractionSolid.hh"
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Caustic_PhononDetectorConstruction::Caustic_PhononDetectorConstruction()
: fLiquidHelium(0),fBolometer(0),fCrystalMaterial(0),
  fWorldPhys(0), fpSubstrateLV(0), topSurfProp(0), wallSurfProp(0),
  fConstructed(false) {;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Caustic_PhononDetectorConstruction::~Caustic_PhononDetectorConstruction() {
  delete topSurfProp;

  delete wallSurfProp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VPhysicalVolume* Caustic_PhononDetectorConstruction::Construct()
{
  if (fConstructed) {
    if (!G4RunManager::IfGeometryHasBeenDestroyed()) {
      // Run manager hasn't cleaned volume stores. This code shouldn't execute
      G4GeometryManager::GetInstance()->OpenGeometry();
      G4PhysicalVolumeStore::GetInstance()->Clean();
      G4LogicalVolumeStore::GetInstance()->Clean();
      G4SolidStore::GetInstance()->Clean();
    }
    fpSubstrateLV = 0;		// Avoid dangling pointers

    // Have to completely remove all lattices to avoid warning on reconstruction
    G4LatticeManager::GetLatticeManager()->Reset();
    // Clear all LogicalSurfaces
    // NOTE: No need to redefine the G4CMPSurfaceProperties
    G4CMPLogicalBorderSurface::CleanSurfaceTable();
  }

  Caustic_DefineMaterials();
  Caustic_SetupGeometry();
  fConstructed = true;

  return fWorldPhys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Caustic_PhononDetectorConstruction::ConstructSDandField() {
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  Caustic_PhononSensitivity* electrodeSD
    = new Caustic_PhononSensitivity("PhononElectrode");

  SDman->AddNewDetector(electrodeSD);
  fpSubstrateLV->SetSensitiveDetector(electrodeSD);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Caustic_PhononDetectorConstruction::Caustic_DefineMaterials()
{
  G4NistManager* nistManager = G4NistManager::Instance();

  fLiquidHelium = nistManager->FindOrBuildMaterial("G4_AIR"); // to be corrected
  fBolometer = nistManager->FindOrBuildMaterial("G4_Al");

  // Defining the materials for phonon propagation
  fCrystalMaterial = nistManager->FindOrBuildMaterial("G4_Si");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Caustic_PhononDetectorConstruction::Caustic_SetupGeometry()
{
  bool checkOverlaps = true;

  // World
  G4VSolid* worldSolid = new G4Box("World",6.*cm,6.*cm,6.*cm);
  G4LogicalVolume* worldLogical =
    new G4LogicalVolume(worldSolid,fLiquidHelium,"World");
  worldLogical->SetUserLimits(new G4UserLimits(10*mm, DBL_MAX, DBL_MAX, 0, 0));
  fWorldPhys = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"World",0,
                                 false,0);

  // The Substrate material where the phonons are propagated
  // G4VSolid* SubstrateSolid= new G4Box("SubstrateSolid",0.2*cm,0.2*cm,0.2*cm);
  G4double waferHalfThickness = fWaferThickness/2.0;
  G4double flatY = 
    std::sqrt(fWaferRadius*fWaferRadius - fPrimaryFlatLength*fPrimaryFlatLength/4.0);
  
  G4VSolid* WaferDisk =
    new G4Tubs("WaferDisk",0.0,fWaferRadius,waferHalfThickness,0.0,2*M_PI);

  G4double tolerance = 1*mm;
  G4double cutterHalfY = (fWaferRadius - flatY)/2.0 + tolerance;
  G4double cutterCenterY = -(fWaferRadius + flatY)/2.0 - tolerance;

  G4VSolid* FlatCutter = 
    new G4Box("PrimaryFlatCutter",fWaferRadius + tolerance,
      cutterHalfY,waferHalfThickness + tolerance);


  G4VSolid* SubstrateSolid =
    new G4SubtractionSolid("SubstrateSolid", WaferDisk, FlatCutter,
                           0, G4ThreeVector(0, cutterCenterY, 0));

  G4LogicalVolume* SubstrateLogical1 =
    new G4LogicalVolume(SubstrateSolid,fCrystalMaterial,"SubstrateLogical1");
  G4VPhysicalVolume* SubstratePhys =
    new G4PVPlacement(0,G4ThreeVector(),SubstrateLogical1,"SubstratePhys",
                      worldLogical,false,0,checkOverlaps);

  fpSubstrateLV = SubstrateLogical1;	// Store volume for SD attachment

  // Substrate lattice information

  // G4LatticeManager gives physics processes access to lattices by volume
  G4LatticeManager* LM = G4LatticeManager::GetLatticeManager();
  G4LatticeLogical* SubstrateLogical = LM->LoadLattice(fCrystalMaterial, "Si");
  G4LatticePhysical* SubstratePhysical = new G4LatticePhysical(SubstrateLogical);

  SubstratePhysical->SetMillerOrientation(1,0,0,45.*deg);
  LM->RegisterLattice(SubstratePhys, SubstratePhysical);

  // Aluminum . This is where phonon hits are registered (Bolometer)
  // G4VSolid* BolometerSolid= new G4Box("BolometerSolid",0.2*cm,0.2*cm,0.001*cm);
  // G4LogicalVolume* BolometerLogical =
  //   new G4LogicalVolume(BolometerSolid,fBolometer,"BolometerLogical");
  // G4VPhysicalVolume* BolometerPhysical =
  //   new G4PVPlacement(0, G4ThreeVector(0.,0.,0.201*cm), BolometerLogical,
	// 	      "BolometerPhysical",worldLogical,false,0,checkOverlaps);
    
  G4double bolometerHalfThickness = 0.001*cm;
  G4VSolid* BolometerDisk = 
      new G4Tubs("BolometerDisk",0.0,fWaferRadius,bolometerHalfThickness,0.0,2*M_PI);
  // G4VSolid* BolometerFlatCutter = 
  //    new G4Box("BolometerFlatCutter",fWaferRadius,cutterHalfY,bolometerHalfThickness);
  G4VSolid* BolometerSolid =
      new G4SubtractionSolid("BolometerSolid", BolometerDisk, FlatCutter,
                             nullptr, G4ThreeVector(0, cutterCenterY, 0));
  G4LogicalVolume* BolometerLogical =
      new G4LogicalVolume(BolometerSolid,fBolometer,"BolometerLogical");
  G4VPhysicalVolume* BolometerPhysical =
      new G4PVPlacement(nullptr,G4ThreeVector(0.0,0.0,waferHalfThickness+bolometerHalfThickness),BolometerLogical,"BolometerPhysical",
                        worldLogical,false,0,checkOverlaps);


  // Surface between bolometer and Substrate determines phonon reflection/absorption
  // Here We set 1.0 for total phonon absorption
  if (!fConstructed) {
    topSurfProp = new G4CMPSurfaceProperty("TopAlSurf", 1.0, 0.0, 0.0, 0.0,
					  	        1.0, 1.0, 0.0, 0.0);
    wallSurfProp = new G4CMPSurfaceProperty("WallSurf", 0.0, 1.0, 0.0, 0.0,
					    	        1.0, 1.0, 0.0, 0.0);
  }

  // Separate surfaces for sensors vs. bare sidewall
  new G4CMPLogicalBorderSurface("detTop", SubstratePhys, BolometerPhysical,
				topSurfProp);

  new G4CMPLogicalBorderSurface("detWall", SubstratePhys, fWorldPhys,
				wallSurfProp);

  // Visualization attributes
  worldLogical->SetVisAttributes(G4VisAttributes::GetInvisible());
  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* simpleDetectorAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  SubstrateLogical1->SetVisAttributes(simpleBoxVisAtt);
  BolometerLogical->SetVisAttributes(
    simpleDetectorAtt
  //  G4VisAttributes::GetInvisible()
  );
}
