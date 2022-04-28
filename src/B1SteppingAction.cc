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
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1Analysis.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
    pEnerg = 0;
    G4ThreeVector pos = G4ThreeVector(0, 0, 0);
    auto analysisManager = G4AnalysisManager::Instance();
    //Создаем указатель на трек для удобства
    G4Track* track = step->GetTrack();
    //Узнаем физический объем в котором находится частица
    G4VPhysicalVolume* vel = track->GetVolume();
    //Название физического объема в котором мы будем регистрировать
    //частицы, он у нас будет играть роль чуствительного объема или
    //детектора
    G4String name;
    G4LogicalVolume* detVolume = fScoringVolume;

    //Проверка условия, что частица находится в интересуещем нас объеме


    //ДЕТЕКТОР 1
    
    if (detVolume == vel->GetLogicalVolume()) {
        //Если находится то передаем все что связано с шаго
        //в EventAction
        pEnerg = step->GetTrack()->GetKineticEnergy();
        pos = step->GetTrack()->GetPosition();
        //G4ParticleDefinition* particle2;
        name = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
        //G4String particleName2 = particle2->GetParticleName();
        perpPos = pos.perp();
        if (pEnerg > 0) {
            if ((name == "e-") || (name == "positron")) {
                
                analysisManager->FillH1(0, pEnerg);
                analysisManager->FillH1(2, pEnerg);
            }
            //analysisManager->FillH2(0, pEnerg, perpPos);
            if (name == "gamma") {
                
                analysisManager->FillH1(1, pEnerg);
                analysisManager->FillH1(3, pEnerg);
            }
        }
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
    

    //ДЕТЕКТОР 2
/*
   if (name4 == vel->GetName() || name3 == vel->GetName() || name5 == vel->GetName()) {
        //Если находится то передаем все что связано с шаго
        //в EventAction
        pEnerg = step->GetTrack()->GetKineticEnergy();
        pos = step->GetTrack()->GetPosition();
        //G4ParticleDefinition* particle2;
        name = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
        //G4String particleName2 = particle2->GetParticleName();
        perpPos = pos.z();
        if (pEnerg > 0) {
            if ((name == "e-") || (name == "positron")) {
                analysisManager->FillH2(0, pEnerg, perpPos);
                analysisManager->FillH2(2, pEnerg, perpPos);
            }
            //analysisManager->FillH2(0, pEnerg, perpPos);
            if (name == "gamma") {
                analysisManager->FillH2(1, pEnerg, perpPos);
                analysisManager->FillH2(3, pEnerg, perpPos);
            }
        }
        //step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
*/
  /*
    if ((name2 == vel->GetName())||(name21 == vel->GetName())||(name22 == vel->GetName())) {
        //Если находится то передаем все что связано с шаго
        //в EventAction
        pEnerg = step->GetTrack()->GetKineticEnergy();
        pos = step->GetTrack()->GetPosition();
        //G4ParticleDefinition* particle2;
        name = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
        //G4String particleName2 = particle2->GetParticleName();
        perpPos = pos.z();
        if (pEnerg > 0) {
            if ((name == "e-") || (name == "positron")) {
                analysisManager->FillH2(0, pEnerg, perpPos);
                analysisManager->FillH1(2, perpPos, pEnerg);
            }
            //analysisManager->FillH2(0, pEnerg, perpPos);
            if (name == "gamma") {
                analysisManager->FillH2(1, pEnerg, perpPos);
                analysisManager->FillH1(3, perpPos, pEnerg);
            }
        }
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
    */
   
    
    //fEventAction->addParticle(step);
    //Нефизично, но нам нужно только подсчитать энергию частиц попавших
    //в детектор, поэтому эту частицу уничтожаем что бы она
    //не зарегистрировалась несколько раз



}


  


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

