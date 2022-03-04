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
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1Analysis.hh"
// #include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction()
: G4UserRunAction(),
  Energy(0),
  fEdep(0.),
  fEdep2(0.)
{ 
   


  //analysis&hist
  auto analysisManager = G4AnalysisManager::Instance();
  //analysisManager->CreateH2("E1", "Outside tube Energy e- and p+", 100, 0., 500 * MeV, 100, 0, 5 * m);
  //analysisManager->CreateH2("E2", "Outside tube Energy gamma", 100, 0., 500 * MeV, 100, 0, 5 * m);
  //analysisManager->CreateH2("E3", "Outside tube Energy e- and p+", 100, 0., 500 * MeV, 100, 5*m, 25 * m);
  //analysisManager->CreateH2("E4", "Outside tube Energy gamma", 100, 0., 500 * MeV, 100, 5*m, 25 * m);

  analysisManager->CreateH1("E1", "Weight Energy e- and p+", 100, 0., 1 * MeV);
  analysisManager->CreateH1("E2", "Weight Energy gamma", 100, 0., 1 * MeV);
  analysisManager->CreateH1("E5", "Weight Energy e- and p+", 100, 0., 500*MeV);
  analysisManager->CreateH1("E6", "Weight Energy gamma", 100, 0., 500 * MeV);
  //analysisManager->CreateH2("E&R", "Energy&Radius", 100, 0., 5 * MeV, 100, 0., 50 * cm);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{
    delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
    //Ѕерем полное число частиц, которое будет запущено
    eventsNumber = aRun->GetNumberOfEventToBeProcessed();
    //ѕеремена€ используема€ дл€ вывода прогресса
    printModulo = eventsNumber / 100;
    if (printModulo == 0)
        printModulo = 1;
    //ќбнул€ем переменную, в нее будем записывать энергию
    //частиц приход€щих в детектор
    particlesEnergy = 0;
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
 // G4String fileName = "HISTOGRAM";
  analysisManager->OpenFile();

}

void B1RunAction::FillEnergy(G4double energy)
{
    particlesEnergy += energy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{  
    auto analysisManager = G4AnalysisManager::Instance();

    //analysisManager->SetH1(0, 100, 0.1*keV, 100 * keV);

  //if (IsMaster()) {
  //    Energy = particlesEnergy;
  //};

  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  //G4double edep  = fEdep.GetValue();
  //G4double edep2 = fEdep2.GetValue();
  //particlesEnergy = Energy.GetValue();

  //G4double rms = edep2 - edep*edep/nofEvents;
 // if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;  

  ////const B1DetectorConstruction* detectorConstruction
  // = static_cast<const B1DetectorConstruction*>
  //   (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  //G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
  //G4double dose = edep/mass;
  //G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  //const B1PrimaryGeneratorAction* generatorAction
   //= static_cast<const B1PrimaryGeneratorAction*>
  //   (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  //G4String runCondition;
 // if (generatorAction)
 // {
 //   const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
  //  runCondition += particleGun->GetParticleDefinition()->GetParticleName();
   // runCondition += " of ";
   // G4double particleEnergy = particleGun->GetParticleEnergy();
  //  runCondition += G4BestUnit(particleEnergy,"Energy");
 // }//
        
  // Print
  //  
  if (IsMaster()) {

    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  
  //G4cout
   //   << G4endl
   //   << " The run consists of " << nofEvents << " " << runCondition
    //  << G4endl
   //   << " Cumulated dose per run, in scoring volume : "
   //   << G4BestUnit(edep, "Energy") << " rms = " << G4BestUnit(rmsDose, "Dose") << " Ёнерги€ = " << G4BestUnit(particlesEnergy, "Energy")
   //   << G4endl
    //  << "------------------------------------------------------------"
   //   << G4endl
    //  << G4endl;




  // save histograms & ntuple
//
  analysisManager->Write();
  analysisManager->CloseFile();

}

//G4cout << G4BestUnit(edep, "Energy");
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void B1RunAction::AddEdep(G4double edep)
//{
//.  fEdep  += edep;
//  fEdep2 += edep*edep;
//}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

