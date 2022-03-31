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
#include "G4GDMLParser.hh"

B1DetectorConstruction::B1DetectorConstruction(const G4GDMLParser& parser)
    : G4VUserDetectorConstruction(),
    fScoringVolume(0), fdeadendXY(25 * m), fdeadendZ(2 * mm), fParser(parser)
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
    const G4GDMLAuxMapType* auxmap = fParser.GetAuxMap();
    G4GDMLAuxMapType::const_iterator sv;
    G4cout << "Found " << auxmap->size()
        << " volume(s) with auxiliary information."
        << G4endl << G4endl;
    for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
        iter != auxmap->end(); iter++)
    {
        G4cout << "Volume " << ((*iter).first)->GetName()
            << " has the following list of auxiliary information: "
            << G4endl << G4endl;
        for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
            vit != (*iter).second.end(); vit++)
        {
            G4cout << "--> Type: " << (*vit).type
                << " Value: " << (*vit).value << G4endl;
        }

        //if (((*iter).first)->GetName() == "Tracker") {//"Tracker"
        sv = iter;
        G4cout << "Scoring Volume set to " << ((*sv).first)->GetName() << G4endl;
        //}


        //G4cout << G4endl;
    }
        fScoringVolume = (*sv).first;
    
  
  //
  //always return the physical World
  //
  return fParser.GetWorldVolume();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
