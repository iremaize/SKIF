#pragma once
#include <G4UImessenger.hh>

#include <G4UIdirectory.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

class B1DetectorConstruction;

class B1DetectorMessenger : public G4UImessenger
{
public:
	B1DetectorMessenger(B1DetectorConstruction*);
	~B1DetectorMessenger();
	virtual void SetNewValue(G4UIcommand*, G4String);

private:
	G4UIdirectory*		    ftargetDir;
	B1DetectorConstruction* fdet;
	//G4UIcmdWithADouble*     fcoeff;
	G4UIcmdWithADoubleAndUnit* fdendZ;
};
