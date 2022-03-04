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
/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "QGSP_BERT.hh"
#include "FTFP_BERT.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4GDMLParser.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void print_aux(const G4GDMLAuxListType* auxInfoList, G4String prepend = "|")
{
    for (std::vector<G4GDMLAuxStructType>::const_iterator iaux = auxInfoList->begin();
        iaux != auxInfoList->end(); iaux++)
    {
        G4String str = iaux->type;
        G4String val = iaux->value;
        G4String unit = iaux->unit;

        G4cout << prepend << str << " : " << val << " " << unit << G4endl;

        if (iaux->auxList) print_aux(iaux->auxList, prepend + "|");
    }
    return;
}

int main(int argc,char** argv)
{
    if (argc < 2)
    {
        G4cout << "Error! Mandatory input file is not specified!" << G4endl;
        G4cout << G4endl;
        return -1;
    }

    G4GDMLParser parser;

    // Uncomment the following if wish to avoid names stripping
    // parser.SetStripFlag(false);

    parser.SetOverlapCheck(true);
    parser.Read(argv[1]);

    if (argc > 4)
    {
        G4cout << "Error! Too many arguments!" << G4endl;
        G4cout << G4endl;
        return -1;
    }
  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif


  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new B1DetectorConstruction(
      parser));

  // Physics list
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new B1ActionInitialization());
  runManager->Initialize();
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  G4cout << std::endl;

  if (argc >= 3)
  {
      
      parser.SetRegionExport(true);
      //     parser.SetEnergyCutsExport(true);
      parser.Write(argv[2], G4TransportationManager::GetTransportationManager()
          ->GetNavigatorForTracking()->GetWorldVolume()->GetLogicalVolume());
  }






  // Process macro or start UI session
  //
  if (argc == 4)   // batch mode
  {
      G4String command = "/control/execute ";
      G4String fileName = argv[3];
      UImanager->ApplyCommand(command + fileName);
  }
  else           // interactive mode
  {
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
      UImanager->ApplyCommand("/control/execute vis.mac");
      ui->SessionStart();
      delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
