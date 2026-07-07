#include "NDPrimaryGenerator.hh"
#include "Randomize.hh"
#include "G4RandomTools.hh"

#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4PhysicalConstants.hh"

#include <cmath>
#include <algorithm>

#include <fstream>
#include <sstream>
#include <cctype>

NDPrimaryGenerator::NDPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
    G4ThreeVector pos(0., 0., 0.);
    fParticleGun->SetParticlePosition(pos);
}

NDPrimaryGenerator::~NDPrimaryGenerator()
{
    delete fParticleGun;
}

G4ThreeVector NDPrimaryGenerator::GetNeutronDirection()
{
    auto P0 = [](G4double) -> G4double
    {
        return 1.0;
    };

    auto P2 = [](G4double x) -> G4double
    {
        return 0.5 * (3.0*x*x - 1.0);
    };

    auto P4 = [](G4double x) -> G4double
    {
        const G4double x2 = x*x;
        const G4double x4 = x2*x2;

        return (1.0/8.0) * (35.0*x4 - 30.0*x2 + 3.0);
    };

    auto LegendrePolynomial = [&](G4double mu) -> G4double
    {
        // mu = cos(theta)
        const G4double A0 = 0.758;
        const G4double A2 = 0.241;
        const G4double A4 = 0.001;

        return A0*P0(mu) + A2*P2(mu) + A4*P4(mu);
    };

    G4double mu_sample = 0.0;

    while (true)
    {
        const G4double mu = 2.0*G4UniformRand() - 1.0; 
        const G4double fx = LegendrePolynomial(mu);
        const G4double y  = G4UniformRand();            

        if (y <= fx)
        {
            mu_sample = mu;
            break;
        }
    }

    const G4double phi_sample = CLHEP::twopi * G4UniformRand();

    const G4double sin_theta =
        std::sqrt(std::max(0.0, 1.0 - mu_sample*mu_sample));

    // Beam axis is +x.
    const G4double x_dir = mu_sample;
    const G4double y_dir = sin_theta * std::cos(phi_sample);
    const G4double z_dir = sin_theta * std::sin(phi_sample);

    G4ThreeVector neutronDirection(x_dir, y_dir, z_dir);

    return neutronDirection.unit();
}


void NDPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ThreeVector pos(0., 0., 0.);
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleEnergy(2.445 * MeV);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(2112);
    fParticleGun->SetParticleDefinition(particle);

    // Particle Direction
    G4ThreeVector dir = GetNeutronDirection();
    fParticleGun->SetParticleMomentumDirection(dir);

    // Create vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
