#include "B1DetectorMessenger.hh"

#include "B1DetectorConstruction.hh"

B1DetectorMessenger::B1DetectorMessenger(B1DetectorConstruction* d) : fdet(d)
{
	ftargetDir = new G4UIdirectory("/customiseDet/");
	ftargetDir->SetGuidance("Set custom parameters of detector");
	

	fdendZ = new G4UIcmdWithADoubleAndUnit("/customiseDet/deadendThickness", this);

	fdendZ->SetGuidance("Set thickness of beam stopper");
	fdendZ->SetParameterName("dendThickness", false);
	fdendZ->SetUnitCategory("Length");
	fdendZ->AvailableForStates(G4State_PreInit, G4State_Idle);
	fdendZ->SetToBeBroadcasted(false);
}
B1DetectorMessenger::~B1DetectorMessenger() {}

void B1DetectorMessenger::SetNewValue(G4UIcommand *icommand, G4String string) {
	
	if (icommand == fdendZ) {
		fdet->SetDeadendThickness(fdendZ->GetNewDoubleValue(string));
	}

}
