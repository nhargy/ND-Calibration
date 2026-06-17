#ifndef NDDETECTORCONSTRUCTION_HH   
#define NDDETECTORCONSTRUCTION_HH 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include <map>

// Forward declarations
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class NDDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    NDDetectorConstruction();
    virtual ~NDDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    G4double GetGeometryParameter(const char *szParameter);

private:
    void DefineMaterials();
    void DefineGeometryParameters();

    void ConstructLab();
    void ConstructArikNDs();
    void ConstructRoom();

    virtual void ConstructSDandField();

    static std::map<G4String, G4double> m_hGeometryParameters;

    /* Lab */
    G4LogicalVolume*   logic_Lab = nullptr;
    G4VPhysicalVolume* phys_Lab  = nullptr;

    /* Room */
    G4LogicalVolume*   logic_Floor = nullptr;
    G4VPhysicalVolume* phys_Floor  = nullptr;

    G4LogicalVolume*   logic_Wall = nullptr;
    G4VPhysicalVolume* phys_Wall  = nullptr;

    /* NDs */
    G4LogicalVolume*   logic_ArikND = nullptr;
    G4VPhysicalVolume* phys_ArikND1 = nullptr;
    G4VPhysicalVolume* phys_ArikND2 = nullptr;

    // Material definitions
    G4Material* Air      = nullptr;
    G4Material* NE213    = nullptr;
    G4Material* EJ309    = nullptr;
    G4Material* Concrete = nullptr;
    G4Material* Steel    = nullptr;
};

#endif
