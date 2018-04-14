#include "T1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"  //yuanzhu guanzi
#include "G4Cons.hh"
#include "G4Orb.hh"  //shixinqiu
#include "G4Torus.hh"  //quan
#include "G4Sphere.hh" //qiu  (duocanshu)
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4UnionSolid.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
//



#include "T1T0.hh"

using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

T1DetectorConstruction::T1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

T1DetectorConstruction::~T1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* T1DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* default_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4VisAttributes* default_Vis = new G4VisAttributes(G4Colour(0,0,0,0));

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = false;

  //////////////////////CEE-1

  G4double CEE_world_sx = 10*m;  //gao
  G4double CEE_world_sy = 5*m;  //kuan
  G4double CEE_world_sz = 10*m;  //chang  zhouxiang
  G4ThreeVector CEE_world_p = G4ThreeVector(0*cm, 0*cm, 0*cm);
  G4Material* CEE_world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4VisAttributes* CEE_world_Vis = new G4VisAttributes(G4Colour(0.0,0.0,0.0,0.0));


  G4double CEE_Pix_sx = 3*cm;
  G4double CEE_Pix_sy = 3*cm;
  G4double CEE_Pix_sz = 3*cm;
  G4double CEE_Pix_px = 0*cm;
  G4double CEE_Pix_py = 0*cm;
  G4double CEE_Pix_pz = -8*cm;
  G4ThreeVector CEE_Pix_p = G4ThreeVector(CEE_Pix_px, CEE_Pix_py, CEE_Pix_pz);
  G4Material* CEE_Pix_mat = nist->FindOrBuildMaterial("G4_Galactic");//Ar:CO2 7:3
  G4VisAttributes* CEE_Pix_Vis = new G4VisAttributes(G4Colour(0.0,0.0,1.0,1));

  G4double CEE_TPC_sx = 100*cm;
  G4double CEE_TPC_sy = 80*cm;
  G4double CEE_TPC_sz = 80*cm;
  G4double CEE_TPC_px = 0*cm;
  G4double CEE_TPC_py = 0*cm;
  G4double CEE_TPC_pz = 60*cm;
  G4ThreeVector CEE_TPC_p = G4ThreeVector(CEE_TPC_px, CEE_TPC_py, CEE_TPC_pz);
  G4Material* CEE_TPC_mat = nist->FindOrBuildMaterial("G4_Galactic");//Ar:CH4 9:1
  G4VisAttributes* CEE_TPC_Vis = new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.5));







  //////////////////////CEE-1






  //
  // CEE_World
  //

  G4Box* CEE_world_solid =
    new G4Box("CEE_world_solid",                       //its name
       0.5*CEE_world_sx, 0.5*CEE_world_sy, 0.5*CEE_world_sz);     //its size

  G4LogicalVolume* CEE_world_logic =
    new G4LogicalVolume(CEE_world_solid,          //its solid
                        CEE_world_mat,           //its material
                        "CEE_world_logic");            //its name

  G4VPhysicalVolume* CEE_world_phys =
    new G4PVPlacement(0,                     //no rotation
                      CEE_world_p,       //at (,,)
                      CEE_world_logic,            //its logical volume
                      "CEE_world_phys",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  CEE_world_logic -> SetVisAttributes(CEE_world_Vis);
//
// CEE_Pix
//

G4Box* CEE_Pix_solid =
  new G4Box("CEE_Pix_solid",                       //its name
     0.5*CEE_Pix_sx, 0.5*CEE_Pix_sy, 0.5*CEE_Pix_sz);     //its size

G4LogicalVolume* CEE_Pix_logic =
  new G4LogicalVolume(CEE_Pix_solid,          //its solid
                      CEE_Pix_mat,           //its material
                      "CEE_Pix_logic");            //its name


  new G4PVPlacement(0,                     //no rotation
                    CEE_Pix_p,       //at (,,)
                    CEE_Pix_logic,            //its logical volume
                    "CEE_Pix_phys",               //its name
                    CEE_world_logic,                     //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking
CEE_Pix_logic -> SetVisAttributes(CEE_Pix_Vis);
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// CEE_T0
//


T1T0 ceeT1T0;
  new G4PVPlacement(ceeT1T0.transT0,
                    ceeT1T0.logicT0,            //its logical volume
                    "CEE_T0_phys",               //its name
                    CEE_world_logic,                     //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// CEE_TPC
//

G4Box* CEE_TPC_solid =
  new G4Box("CEE_TPC_solid",                       //its name
     0.5*CEE_TPC_sx, 0.5*CEE_TPC_sy, 0.5*CEE_TPC_sz);     //its size

G4LogicalVolume* CEE_TPC_logic =
  new G4LogicalVolume(CEE_TPC_solid,          //its solid
                      CEE_TPC_mat,           //its material
                      "CEE_TPC_logic");            //its name


  new G4PVPlacement(0,                     //no rotation
                    CEE_TPC_p,       //at (,,)
                    CEE_TPC_logic,            //its logical volume
                    "CEE_TPC_phys",               //its name
                    CEE_world_logic,                     //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking
CEE_TPC_logic -> SetVisAttributes(CEE_TPC_Vis);
//////////////////////////////////////////////////////////////////////////////
/*

G4Box*  box =
  new G4Box("Box",1*m,2*m,3*m);
G4Tubs* cyl =
  new G4Tubs("Cylinder",0,20*cm,3*m,0,twopi);  // r:     0 mm -> 50 mm
                                                 // z:   -50 mm -> 50 mm
                                                 // phi:   0 ->  2 pi


G4RotationMatrix* yRot = new G4RotationMatrix;  // Rotates X and Z axes only
yRot->rotateY(pi/6.*rad);                     // Rotates 45 degrees
yRot->rotateZ(pi/4.*rad);
G4ThreeVector zTrans(0, 0, 2*m);
*/
  /*
G4UnionSolid* unionMoved =
  new G4UnionSolid("Box+CylinderMoved", box, cyl, yRot, zTrans);
  */
 //
 // The new coordinate system of the cylinder is translated so that
 // its centre is at +50 on the original Z axis, and it is rotated
 // with its X axis halfway between the original X and Z axes.

 // Now we build the same solid using the alternative method
 //
/*
 //G4RotationMatrix invRot = *yRot;//->invert();
G4Transform3D transform(*yRot, zTrans);
G4UnionSolid* unionMoved =
  new G4UnionSolid("Box+CylinderMoved", box, cyl, transform);


G4LogicalVolume* testlogic =
  new G4LogicalVolume(unionMoved,          //its solid
                      CEE_TPC_mat,           //its material
                      "CEE_testlogic");            //its name

new G4PVPlacement(0,                     //no rotation
                  G4ThreeVector(0,0,0),       //at (,,)
                  testlogic,            //its logical volume
                  "CEE_testphys",               //its name
                  CEE_world_logic,                     //its mother  volume
                  false,                 //no boolean operation
                  0,                     //copy number
                  checkOverlaps);        //overlaps checking
testlogic -> SetVisAttributes(CEE_TPC_Vis);
*/

  //
  //always return the physical World
  //
  return CEE_world_phys;
}
