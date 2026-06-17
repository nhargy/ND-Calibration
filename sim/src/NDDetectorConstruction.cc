#include "NDDetectorConstruction.hh"
#include "NDSensitiveDetector.hh"

#include <cmath>
#include <algorithm>
#include "globals.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4GenericMessenger.hh"

std::map<G4String, G4double> NDDetectorConstruction::m_hGeometryParameters;

NDDetectorConstruction::NDDetectorConstruction()
{
}

NDDetectorConstruction::~NDDetectorConstruction()
{
}

void NDDetectorConstruction::DefineGeometryParameters()
{
    //============================ Laboratory =================================
    m_hGeometryParameters["Lab_Width"]  = 4000. *mm;
    m_hGeometryParameters["Lab_Length"] = 4000. *mm;
    m_hGeometryParameters["Lab_Height"] = 4000. *mm;

    //============================= Arik ND ===================================
    m_hGeometryParameters["Arik_ND_oD"] = 25.4 *mm;
    m_hGeometryParameters["Arik_ND_iD"] = 20.4 *mm;
    m_hGeometryParameters["Arik_ND_oL"] = 50.8 *mm;
    m_hGeometryParameters["Arik_ND_iL"] = 45.8 *mm;

    //============================= Roi ND ====================================
    m_hGeometryParameters["Roi_ND_oD"] = 76.2 *mm;
    m_hGeometryParameters["Roi_ND_iD"] = 71.2 *mm;
    m_hGeometryParameters["Roi_ND_oL"] = 76.2 *mm;
    m_hGeometryParameters["Roi_ND_iL"] = 71.2 *mm;

    //============================ Floor ======================================
    m_hGeometryParameters["Floor_Width"]  = 3999. *mm;
    m_hGeometryParameters["Floor_Length"] = 3999. *mm;
    m_hGeometryParameters["Floor_Height"] = 100. *mm;

    //============================ Wall =======================================
    m_hGeometryParameters["Wall_Width"]  = 100. *mm;
    m_hGeometryParameters["Wall_Length"] = 3999. *mm;
    m_hGeometryParameters["Wall_Height"] = 2000. *mm;
}

void NDDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();

    // --- Air ----------------------------------------------------------------
    Air = nist->FindOrBuildMaterial("G4_AIR");

    // --- Required Elements --------------------------------------------------
    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* Fe = nist->FindOrBuildElement("Fe");
    G4Element* Co = nist->FindOrBuildElement("Co");

    // --- NE213 --------------------------------------------------------------
    G4double NE213_density = 0.87 * g/cm3;

    NE213 = new G4Material(
        "NE213",
        NE213_density,
        2
    );

    // NE213 ratios, from:
    // https://eljentechnology.com/products/liquid-scintillators/ej-301-ej-309
    NE213->AddElement(C, 398);
    NE213->AddElement(H, 482);

    // --- EJ309 --------------------------------------------------------------
    G4double EJ309_density = 0.96 * g/cm3;

    EJ309 = new G4Material(
        "EJ309",
        EJ309_density,
        2
    );

    // EJ309 ratios
    EJ309->AddElement(C, 435);
    EJ309->AddElement(H, 543);

    // --- Concrete -----------------------------------------------------------
    Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

    // --- Steel --------------------------------------------------------------
    Steel = new G4Material(
        "Steel", 
        7.7 * g / cm3, 
        3
    );

    Steel->AddElement(C, 0.04);
    Steel->AddElement(Fe, 0.88);
    Steel->AddElement(Co, 0.08);
}

G4double NDDetectorConstruction::GetGeometryParameter(const char *szParameter)
{
  if (m_hGeometryParameters.find(szParameter) != m_hGeometryParameters.end()){
    return m_hGeometryParameters[szParameter];
  }
  else {
    G4cout<< "Parameter: " << szParameter << " is not defined!!!!!" << G4endl;
    return 0;
  }
}

// **************************************************************************//
// *** Start Construction ***************************************************//
// **************************************************************************//

G4VPhysicalVolume* NDDetectorConstruction::Construct()
{
    DefineMaterials();
    DefineGeometryParameters();
    ConstructLab();
    ConstructArikNDs();
    ConstructRoom();

    return phys_Lab;
}

void NDDetectorConstruction::ConstructLab()
{
    G4double Lab_Width  = GetGeometryParameter("Lab_Width");
    G4double Lab_Length = GetGeometryParameter("Lab_Length");
    G4double Lab_Height = GetGeometryParameter("Lab_Height");

    G4Box* solid_Lab = new G4Box(
        "solid_Lab", 
        Lab_Width/2, 
        Lab_Length/2, 
        Lab_Height/2
    );

    logic_Lab = new G4LogicalVolume(
        solid_Lab, 
        Air, 
        "logic_Lab"
    );

    auto vector_Lab = G4ThreeVector(0., 0., 0.);
    phys_Lab = new G4PVPlacement(
        0, 
        vector_Lab, 
        logic_Lab, 
        "phys_Lab", 
        nullptr, 
        false, 
        0, 
        true
    );
}

void NDDetectorConstruction::ConstructArikNDs()
{
    G4double Arik_ND_oD  = GetGeometryParameter("Arik_ND_oD");
    G4double Arik_ND_iD  = GetGeometryParameter("Arik_ND_iD");
    G4double Arik_ND_oL  = GetGeometryParameter("Arik_ND_oL");
    G4double Arik_ND_iL  = GetGeometryParameter("Arik_ND_iL");

    G4Tubs* solid_ArikND = new G4Tubs(
        "solid_ArikND",
        0.,
        Arik_ND_oD/2,
        Arik_ND_oL/2,
        0.  *deg,
        360.*deg
    );

    logic_ArikND = new G4LogicalVolume(
        solid_ArikND,
        Steel,
        "logic_ArikND"
    );
    
    G4double r1_ArikND     = 990. *mm;
    G4double theta1_ArikND = 57. *deg ;
    G4double z1_ArikND     = 0. *mm;

    G4double x1_ArikND = r1_ArikND * std::cos(theta1_ArikND);
    G4double y1_ArikND = r1_ArikND * std::sin(theta1_ArikND);
    auto pos1_ArikND = G4ThreeVector(x1_ArikND, -y1_ArikND ,z1_ArikND);

    auto rotTheta1 = new G4RotationMatrix();
    rotTheta1->rotateY(90.0 *deg);
    rotTheta1->rotateX(theta1_ArikND);

    phys_ArikND1 = new G4PVPlacement(
        rotTheta1,
        pos1_ArikND,
        logic_ArikND,
        "phys_ArikND1",
        logic_Lab,
        false,
        0,
        true
    );

    G4double r2_ArikND     = 1070. *mm;
    G4double theta2_ArikND = 13. * deg;
    G4double z2_ArikND     = 0. *mm;

    G4double x2_ArikND = r2_ArikND * std::cos(theta2_ArikND);
    G4double y2_ArikND = r2_ArikND * std::sin(theta2_ArikND);
    auto pos2_ArikND = G4ThreeVector(x2_ArikND, y2_ArikND ,z2_ArikND);

    auto rotTheta2 = new G4RotationMatrix();
    rotTheta2->rotateY(90.0 *deg);
    rotTheta2->rotateX(-theta2_ArikND);

    phys_ArikND2 = new G4PVPlacement(
        rotTheta2,
        pos2_ArikND,
        logic_ArikND,
        "phys_ArikND2",
        logic_Lab,
        false,
        0,
        true
    );

    /* VisAttributes */
    auto colour_ArikND = G4Colour(0.9, 1., 1., 1.);
    G4VisAttributes *vis_ArikND = new G4VisAttributes(colour_ArikND);
    vis_ArikND   ->SetVisibility(true);
    vis_ArikND   ->SetForceSolid(true);
    vis_ArikND   ->SetForceAuxEdgeVisible(true);
    logic_ArikND ->SetVisAttributes(vis_ArikND);
}

void NDDetectorConstruction::ConstructRoom()
{
    G4double Floor_Width  = GetGeometryParameter("Floor_Width");
    G4double Floor_Length = GetGeometryParameter("Floor_Length");
    G4double Floor_Height = GetGeometryParameter("Floor_Height");

    G4Box* solid_Floor = new G4Box(
        "solid_Floor", 
        Floor_Width/2, 
        Floor_Length/2, 
        Floor_Height/2
    );

    logic_Floor = new G4LogicalVolume(
        solid_Floor, 
        Concrete, 
        "logic_Floor"
    );

    auto vector_Floor = G4ThreeVector(0.,0., -1400 - Floor_Height/2);
    phys_Floor = new G4PVPlacement(
        0, 
        vector_Floor, 
        logic_Floor, 
        "phys_Floor", 
        logic_Lab, 
        false, 
        0, 
        true
    );

    G4double Wall_Width  = GetGeometryParameter("Wall_Width");
    G4double Wall_Length = GetGeometryParameter("Wall_Length");
    G4double Wall_Height = GetGeometryParameter("Wall_Height");

    G4Box* solid_Wall = new G4Box(
        "solid_Wall", 
        Wall_Width/2, 
        Wall_Length/2, 
        Wall_Height/2
    );

    logic_Wall = new G4LogicalVolume(
        solid_Wall, 
        Concrete, 
        "logic_Wall"
    );

    auto vector_Wall = G4ThreeVector(1855.,0., -1400 + - Floor_Height/2 + (Wall_Height/2));
    phys_Wall = new G4PVPlacement(
        0, 
        vector_Wall, 
        logic_Wall, 
        "phys_Wall", 
        logic_Lab, 
        false, 
        0, 
        true
    );

    /* VisAttributes */
    auto colour_Concrete = G4Colour(0.5, 0.5, 0.5, 1.);
    G4VisAttributes *vis_Concrete = new G4VisAttributes(colour_Concrete);
    vis_Concrete   ->SetVisibility(true);
    vis_Concrete   ->SetForceSolid(true);
    vis_Concrete   ->SetForceAuxEdgeVisible(true);
    logic_Floor ->SetVisAttributes(vis_Concrete);
    logic_Wall ->SetVisAttributes(vis_Concrete);
}

void NDDetectorConstruction::ConstructSDandField()
{
    NDSensitiveDetector *sensDet = new NDSensitiveDetector("SensitveDetector");
    logic_ArikND->SetSensitiveDetector(sensDet);
}