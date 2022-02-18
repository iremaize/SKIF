//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include <G4TransportationManager.hh>
#include "B1DetectorMessenger.hh"
#include "G4IntegrationDriver.hh"
#include "G4FieldManager.hh" 
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4ThreeVector.hh"

B1DetectorConstruction::B1DetectorConstruction()
    : G4VUserDetectorConstruction(),
    fScoringVolume(0), fdeadendXY(25 * m), fdeadendZ(2 * mm)
{
    fMessenger = new B1DetectorMessenger(this);
}


B1DetectorConstruction::~B1DetectorConstruction()
{ }


void B1DetectorConstruction::SetDeadendThickness(double val) {
    this->fdeadendZ = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}


G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  

    //Define diamond
    G4double A = 12.01 * g / mole;
    G4double Z = 6;
    G4Material* diamond = new G4Material("diamond", Z, A, 3.515 * g / cm3);

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 120*cm, env_sizeZ = 30*m;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_Galactic");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2 * env_sizeXY;
  G4double world_sizeZ = 1.2 * env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Box* solidWorld =
      new G4Box("World",                       //its name
          world_sizeXY, world_sizeXY, world_sizeZ);     //its size

  G4LogicalVolume* logicWorld =
      new G4LogicalVolume(solidWorld,          //its solid
          world_mat,           //its material
          "World");            //its name

  G4VPhysicalVolume* physWorld =
      new G4PVPlacement(0,                     //no rotation
          G4ThreeVector(),       //at (0,0,0)
          logicWorld,            //its logical volume
          "World",               //its name
          0,                     //its mother  volume
          false,                 //no boolean operation
          0,                     //copy number
          checkOverlaps);        //overlaps checking

  //     
  /* Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        env_sizeXY, env_sizeXY, env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
    */
  
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Material* shapepb_mat = nist->FindOrBuildMaterial("G4_Pb");
  G4ThreeVector pos1 = G4ThreeVector(0, 0, 1*m);
  G4ThreeVector pos12 = G4ThreeVector(0, 0, 2.5*m);
  
  //diamond plate 
  
  G4Tubs* solidDiamondPlate =
      new G4Tubs("DiamondPlate", 0, 14 * mm, fdeadendZ, 0, 2 * 3.14159265358979323846);

  G4LogicalVolume* logicDiamondPlate =
      new G4LogicalVolume(solidDiamondPlate,         //its solid
          diamond,          //its material
          "DiamondPlate");           //its name

  new G4PVPlacement(0,                       //no rotation
      G4ThreeVector(0, 0, 1195 * cm),                    //at position
      logicDiamondPlate,             //its logical volume
      "DiamondPlate",                //its name
      logicWorld,                //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      checkOverlaps);          //overlaps checking

   
  //magnet

  G4Box* magnet_box =
      new G4Box("Magnet_box",                    //its name
          50 * mm, 32 * mm, 50 * cm); //its size

  G4LogicalVolume* logicMag =
      new G4LogicalVolume(magnet_box,         //its solid
          world_mat,          //its material
          "Magnet_box");         //its name

  new G4PVPlacement(0,                       //no rotation
      pos12,                    //at position
      logicMag,             //its logical volume
      "Magnet_box",                //its name
      logicWorld,                //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      checkOverlaps);           //overlaps checking

  G4MagneticField* magField = new G4UniformMagField(G4ThreeVector(0., 2 * kilogauss, 0.));
  G4FieldManager* localFieldMgr =
      new G4FieldManager(magField);
  logicMag->SetFieldManager(localFieldMgr,
     true);

  /*
  G4Tubs* solidShape1 =
      new G4Tubs("Shape1", 14*mm, 15 * mm, fdeadendZ/2, 0, 2* 3.14159265358979323846);
        //fdeadendXY / 2, fdeadendXY / 2, fdeadendZ / 2);
                      
  G4LogicalVolume* logicShape1 =                         
    new G4LogicalVolume(solidShape1,         //its solid
                        shape1_mat,          //its material
                        "Shape1");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos1,                    //at position
                    logicShape1,             //its logical volume
                    "Shape1",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
   */
              //first tube part
              G4Tubs* solidShape1 =
                  new G4Tubs("Shape1", 14 * mm, 15 * mm, 1*m, 0, 2 * 3.14159265358979323846);
              //fdeadendXY / 2, fdeadendXY / 2, fdeadendZ / 2);

              G4LogicalVolume* logicShape1 =
                  new G4LogicalVolume(solidShape1,         //its solid
                      shape1_mat,          //its material
                      "Shape1");           //its name

              new G4PVPlacement(0,                       //no rotation
                  pos1,                    //at position
                  logicShape1,             //its logical volume
                  "Shape1",                //its name
                  logicWorld,                //its mother  volume
                  false,                   //no boolean operation
                  0,                       //copy number
                  checkOverlaps);          //overlaps checking
     

              G4Tubs* solidShape12 =
                  new G4Tubs("Shape12", 14 * mm, 15 * mm, 0.5*m, 0, 2 * 3.14159265358979323846);
              //fdeadendXY / 2, fdeadendXY / 2, fdeadendZ / 2);

              G4LogicalVolume* logicShape12 =
                  new G4LogicalVolume(solidShape12,         //its solid
                      shape1_mat,          //its material
                      "Shape12");           //its name

              new G4PVPlacement(0,                       //no rotation
                  G4ThreeVector(0, 0, 0),                    //at position
                  logicShape12,             //its logical volume
                  "Shape12",                //its name
                  logicMag,                //its mother  volume
                  false,                   //no boolean operation
                  0,                       //copy number
                  checkOverlaps);          //overlaps checking


              //third tube part

              G4Tubs* solidShape13 =
                  new G4Tubs("Shape13", 14 * mm, 15 * mm, 11*m, 0, 2 * 3.14159265358979323846);
              //fdeadendXY / 2, fdeadendXY / 2, fdeadendZ / 2);

              G4LogicalVolume* logicShape13 =
                  new G4LogicalVolume(solidShape13,         //its solid
                      shape1_mat,          //its material
                      "Shape13");           //its name

              new G4PVPlacement(0,                       //no rotation
                  G4ThreeVector(0, 0, 14 * m),                    //at position
                  logicShape13,             //its logical volume
                  "Shape13",                //its name
                  logicWorld,                //its mother  volume
                  false,                   //no boolean operation
                  0,                       //copy number
                  checkOverlaps);          //overlaps checking
              
  

  


  
  //ÏÎÇÈÖÈß ÄÅÒÅÊÒÎÐÀ
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4ThreeVector pos2 = G4ThreeVector(0, 0, 1201*cm);

  // Conical section shape       
  G4double shape2_rmina = 100. * cm, shape2_rmaxa = 100. * cm;
  G4double shape2_rminb = 1. * cm;
  
  G4Tubs* solidShapeDet =
      new G4Tubs("ShapeDet", 0, 15 * mm, 1*cm, 0, 2 * 3.14159265358979323846);
          //shape2_rmina, shape2_rmaxa, shape2_rminb);

  G4LogicalVolume* logicShapeDet =
      new G4LogicalVolume(solidShapeDet,         //its solid
          shape2_mat,          //its material
          "ShapeDet");           //its name

  new G4PVPlacement(0,                       //no rotation
      pos2,                    //at position
      logicShapeDet,             //its logical volume
      "ShapeDet",                //its name
      logicWorld,                //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      checkOverlaps);          //overlaps checking









  G4Tubs* solidShape3 =
      new G4Tubs("Shape3", 15 * mm, 16 * mm, 0.5*m, 0, 2 * 3.14159265358979323846);
  //shape2_rmina, shape2_rmaxa, shape2_rminb);

  G4LogicalVolume* logicShape3 =
      new G4LogicalVolume(solidShape3,         //its solid
          shape2_mat,          //its material
          "Shape3");           //its name

  new G4PVPlacement(0,                       //no rotation
      G4ThreeVector(0, 0, 0),                    //at position
      logicShape3,             //its logical volume
      "Shape3",                //its name
      logicMag,                //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      checkOverlaps);          //overlaps checking



  G4Tubs* solidShape4 =
      new G4Tubs("Shape4", 15 * mm, 16 * mm, 1*m, 0, 2 * 3.14159265358979323846);
  //shape2_rmina, shape2_rmaxa, shape2_rminb);

  G4LogicalVolume* logicShape4 =
      new G4LogicalVolume(solidShape4,         //its solid
          shape2_mat,          //its material
          "Shape4");           //its name

  new G4PVPlacement(0,                       //no rotation
      G4ThreeVector(0, 0, 1*m),                    //at position
      logicShape4,             //its logical volume
      "Shape4",                //its name
      logicWorld,                //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      checkOverlaps);          //overlaps checking


  G4Tubs* solidShape5 =
      new G4Tubs("Shape5", 15 * mm, 16 * mm, 11 * m, 0, 2 * 3.14159265358979323846);
  //shape2_rmina, shape2_rmaxa, shape2_rminb);

  G4LogicalVolume* logicShape5 =
      new G4LogicalVolume(solidShape5,         //its solid
          shape2_mat,          //its material
          "Shape5");           //its name

  new G4PVPlacement(0,                       //no rotation
      G4ThreeVector(0, 0, 14 * m),                    //at position
      logicShape5,             //its logical volume
      "Shape5",                //its name
      logicWorld,                //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      checkOverlaps);          //overlaps checking
 /*
    // ÂÀÊÓÓÌÍÎÅ ÇÀÏÎËÍÅÍÈÅ 
  G4Tubs* solidShapeVac1 =
      new G4Tubs("ShapeVac1", 0 * mm, 14 * mm, 0.5 * m, 0, 2 * 3.14159265358979323846);
  //shape2_rmina, shape2_rmaxa, shape2_rminb);

  G4LogicalVolume* logicShapeVac1 =
      new G4LogicalVolume(solidShapeVac1,         //its solid
          env_mat,          //its material
          "ShapeVac1");           //its name

  new G4PVPlacement(0,                       //no rotation
      G4ThreeVector(0, 0, 0),                    //at position
      logicShapeVac1,             //its logical volume
      "ShapeVac1",                //its name
      logicMag,                //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      checkOverlaps);          //overlaps checking


  G4Tubs* solidShapeVac2 =
      new G4Tubs("ShapeVac2", 0 * mm, 14 * mm, 11 * m, 0, 2 * 3.14159265358979323846);
  //shape2_rmina, shape2_rmaxa, shape2_rminb);

  G4LogicalVolume* logicShapeVac2 =
      new G4LogicalVolume(solidShapeVac2,         //its solid
          env_mat,          //its material
          "ShapeVac2");           //its name

  new G4PVPlacement(0,                       //no rotation
      G4ThreeVector(0, 0, 14 * m),                    //at position
      logicShapeVac2,             //its logical volume
      "ShapeVac2",                //its name
      logicWorld,                //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      checkOverlaps);          //overlaps checking

  G4Tubs* solidShapeVac3 =
      new G4Tubs("ShapeVac3", 0 * mm, 14 * mm, 1.2 * m, 0, 2 * 3.14159265358979323846);
  //shape2_rmina, shape2_rmaxa, shape2_rminb);

  G4LogicalVolume* logicShapeVac3 =
      new G4LogicalVolume(solidShapeVac3,         //its solid
          env_mat,          //its material
          "ShapeVac3");           //its name

  new G4PVPlacement(0,                       //no rotation
      G4ThreeVector(0, 0, 0.8 * m),                    //at position
      logicShapeVac3,             //its logical volume
      "ShapeVac3",                //its name
      logicWorld,                //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      checkOverlaps);          //overlaps checking

  
  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()
      ->GetFieldManager();
  fieldMgr->SetDetectorField(magField);

  fieldMgr->CreateChordFinder(magField);
  */
  





  
  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
