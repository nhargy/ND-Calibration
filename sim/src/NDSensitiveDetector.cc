#include "NDSensitiveDetector.hh"

NDSensitiveDetector::NDSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
}

NDSensitiveDetector::~NDSensitiveDetector()
{
}

void NDSensitiveDetector::Initialize(G4HCofThisEvent *)
{
}

G4bool NDSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4Track *track = aStep->GetTrack();

    G4int pdgID    = track->GetDefinition()->GetPDGEncoding();

    // Pre-step Coordinates
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4float fX1 = preStepPoint->GetPosition().x();
    G4float fY1 = preStepPoint->GetPosition().y();
    G4float fZ1 = preStepPoint->GetPosition().z();

    // Post-step Coordinates
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
    G4float fX2 = postStepPoint->GetPosition().x();
    G4float fY2 = postStepPoint->GetPosition().y();
    G4float fZ2 = postStepPoint->GetPosition().z();

    // Is it an entry particle?
    G4bool isEntry = (preStepPoint->GetStepStatus()==fGeomBoundary);

    // Kinetic Energy
    G4float fK1    = preStepPoint->GetKineticEnergy();
    G4float fK2    = postStepPoint->GetKineticEnergy();

    // Energy Deposition
    G4double fEdep = aStep->GetTotalEnergyDeposit();

    // Build Records Table
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0, 0,  eventID);
    analysisManager->FillNtupleIColumn(0, 1,  pdgID);
    analysisManager->FillNtupleIColumn(0, 2,  isEntry);
    analysisManager->FillNtupleDColumn(0, 3,  fX1);
    analysisManager->FillNtupleDColumn(0, 4, fY1);
    analysisManager->FillNtupleDColumn(0, 5, fZ1);
    analysisManager->FillNtupleDColumn(0, 6, fX2);
    analysisManager->FillNtupleDColumn(0, 7, fY2);
    analysisManager->FillNtupleDColumn(0, 8, fZ2);
    analysisManager->FillNtupleDColumn(0, 9, fK1);
    analysisManager->FillNtupleDColumn(0, 10, fK2);
    analysisManager->FillNtupleDColumn(0, 11, fEdep);
    analysisManager->AddNtupleRow(0);

    return true;
}

void NDSensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
}