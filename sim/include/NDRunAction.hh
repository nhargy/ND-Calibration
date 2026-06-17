#ifndef NDRUNACTION_HH
#define NDRUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class NDRunAction : public G4UserRunAction
{
public:
    NDRunAction();
    ~NDRunAction();

    virtual void BeginOfRunAction(const G4Run *);
    virtual void EndOfRunAction(const G4Run *);

    void SetOutputDirectory(const G4String& dir) { outputDirectory = dir; }
    G4String GetOutputDirectory() const { return outputDirectory; }

private:
    G4String outputDirectory;
};

#endif