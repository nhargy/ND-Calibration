#include "NDActionInitialization.hh"

NDActionInitialization::NDActionInitialization(const G4String& outputFilePath):fOutputFilePath(outputFilePath){

}

NDActionInitialization::~NDActionInitialization(){

}

void NDActionInitialization::Build() const
{
    NDPrimaryGenerator *generator = new NDPrimaryGenerator();
    SetUserAction(generator);

    NDRunAction *runAction = new NDRunAction();
    runAction->SetOutputDirectory(fOutputFilePath);
    SetUserAction(runAction);
}