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
/// \file B1PrimaryGeneratorAction.cc
/// \brief Implementation of the B1PrimaryGeneratorAction class

#include "B1PrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <random>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#define G4UniformRand() CLHEP::HepRandom::getTheEngine()->flat()
B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fEnvelopeBox(0)
{ 
  /*
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="e-");
  fParticleGun->SetParticleDefinition(particle);




  
  float distrib_array[724][3];
  ifstream fp("distrib.txt");
  if (!fp) {
      cout << "Error, distrib file couldn't be opened" << endl;
  }
  for (int row = 0; row < 724; row++) {  // stop loops if nothing to read
      for (int column = 0; column < 3; column++) {
          fp >> distrib_array[row][column];
          if (!fp) {
              cout << "Error reading file for element " << row << "," << column << endl;
          }
      }
  }


  
  CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
  //CLHEP::HepRandom::setTheSeed((unsigned)clock());
  CLHEP::HepRandom::setTheSeed(time(NULL));
  int distrib_number = round(G4UniformRand()*724);
  int x_pos;
  int y_pos;
  float rand_pos = distrib_array[distrib_number][0];
  float rand_angle_x = distrib_array[distrib_number][1];
  float rand_angle_y = distrib_array[distrib_number][2];
  if (rand_angle_x > 0) { x_pos = 10; }
  else { x_pos = -10; }
  if (rand_angle_y > 0) { y_pos = 10; }
  else { y_pos = -10; }

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(rand_angle_x*100, rand_angle_y*100, 1)); //0.0017
  fParticleGun->SetParticlePosition(G4ThreeVector(G4UniformRand() * 724, y_pos, rand_pos));
  fParticleGun->SetParticleEnergy(3.*GeV);
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{    
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // default particle kinematic
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle
        = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);

    float distrib_array[724][3];
    ifstream fp("distrib.txt");
    if (!fp) {
        cout << "Error, distrib file couldn't be opened" << endl;
    }
    for (int row = 0; row < 724; row++) {  // stop loops if nothing to read
        for (int column = 0; column < 3; column++) {
            fp >> distrib_array[row][column];
            if (!fp) {
                cout << "Error reading file for element " << row << "," << column << endl;
            }
        }
    }



    CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
    //CLHEP::HepRandom::setTheSeed((unsigned)clock());
    CLHEP::HepRandom::setTheSeed(time(NULL));
    int distrib_number = round(G4UniformRand() * 724);
    int x_pos;
    int y_pos;
    float rand_pos = distrib_array[distrib_number][0];
    float rand_angle_x = distrib_array[distrib_number][1];
    float rand_angle_y = distrib_array[distrib_number][2];
    if (rand_angle_x > 0) { x_pos = 10; }
    else { x_pos = -10; }
    if (rand_angle_y > 0) { y_pos = 10; }
    else { y_pos = -10; }
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(rand_angle_x, rand_angle_y, 1)); //0.0017
  fParticleGun->SetParticlePosition(G4ThreeVector(x_pos, y_pos, rand_pos));
  fParticleGun->SetParticleEnergy(3. * GeV);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

