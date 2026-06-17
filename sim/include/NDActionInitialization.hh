#ifndef NDACTIONINITIALIZATION_HH
#define NDACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

#include "NDPrimaryGenerator.hh"
#include "NDRunAction.hh"

class NDActionInitialization : public G4VUserActionInitialization
{
public:
    NDActionInitialization(
        const G4String& outputFilePath = "output.root"
    );
    ~NDActionInitialization();

    void Build() const override;

private:
    G4String fOutputFilePath;
};

#endif