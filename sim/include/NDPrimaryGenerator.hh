#ifndef NDPRIMARYGENERATOR_HH
#define NDPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4GenericMessenger.hh"
#include "G4ThreeVector.hh"
#include <memory>

class NDPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    NDPrimaryGenerator();
    ~NDPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event *);
    virtual G4ThreeVector GetNeutronDirection();

private:
    G4ParticleGun *fParticleGun = nullptr;
};

#endif