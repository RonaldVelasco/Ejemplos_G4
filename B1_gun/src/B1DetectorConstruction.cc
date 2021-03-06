//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  
   //     
  // Cilindros
  //  
 G4Material* mat_int = nist -> FindOrBuildMaterial("G4_POLYSTYRENE");

   G4double innerRadius =17.9*mm;
   G4double outerRadius =19.4*mm;
   G4double hz =10.5*mm; 
   G4double startAngle =0.*deg;
   G4double spanningAngle =360.*deg;

   G4Tubs* solidpozo = new G4Tubs ("pozo", innerRadius, outerRadius, hz, startAngle, spanningAngle);


    G4LogicalVolume* logicalcilindro= new G4LogicalVolume (solidpozo,
							 mat_int,
							 "pozo");

   //Tapas

   G4double innerRadius1 =0.*mm;
   G4double outerRadius1 =19.4*mm;
   G4double hz1 =0.75*mm; 
   G4double startAngle1 =0.*deg;
   G4double spanningAngle1 =360.*deg;
 


 G4Tubs* tapa = new G4Tubs("tapa_inf", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);

 G4LogicalVolume* logicaltapa = new G4LogicalVolume (tapa,
						     mat_int,
						     "tapa_inf");


  //Cilindro 1

   G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
   rotationMatrix -> rotateX(90.*deg);


 G4VPhysicalVolume* pozo1 =  new G4PVPlacement(rotationMatrix,
					       G4ThreeVector(38.8*mm,0,38.8*mm),
					       logicalcilindro,
					       "pozo",
					       logicEnv,
					       false,
					       0,
					       checkOverlaps);

   

   // CIlindro 2

    G4VPhysicalVolume* pozo2 = new G4PVPlacement(rotationMatrix,
					      G4ThreeVector(0,0,38.8*mm),
					      logicalcilindro,
					      "pozo",
					      logicEnv,
					      false,
					      0,
					      checkOverlaps);

   // Cilindro 3

     G4VPhysicalVolume* pozo3 = new G4PVPlacement(rotationMatrix,
					      G4ThreeVector(-38.8*mm,0,38.8*mm),
					      logicalcilindro,
					      "pozo",
					      logicEnv,
					      false,
					      0,
					      checkOverlaps);

   // Cilindro 4

      G4VPhysicalVolume* pozo4 = new G4PVPlacement(rotationMatrix,
					      G4ThreeVector(38.8*mm,0,0),
					      logicalcilindro,
					      "pozo",
					      logicEnv,
					      false,
					      0,
					      checkOverlaps);

   // Cilindro 5

        G4VPhysicalVolume* pozo5 = new G4PVPlacement(rotationMatrix,
					      G4ThreeVector(),
					      logicalcilindro,
					      "pozo",
					      logicEnv,
					      false,
					      0,
					      checkOverlaps);

   // Cilindro 6

	 G4VPhysicalVolume* pozo6 = new G4PVPlacement(rotationMatrix,
						G4ThreeVector(-38.8*mm,0,0),
						logicalcilindro,
						"pozo",
						logicEnv,
						false,
						0,
						checkOverlaps);
   // Cilindro 7

	   G4VPhysicalVolume* pozo7 = new G4PVPlacement(rotationMatrix,
						 G4ThreeVector(38.8*mm,0,-38.8*mm),
						 logicalcilindro,
						 "pozo",
						 logicEnv,
						 false,
						 0,
						 checkOverlaps);;

   //Cilindro 8

	  G4VPhysicalVolume* pozo8 = new G4PVPlacement(rotationMatrix,
						  G4ThreeVector(0,0,-38.8*mm),
						  logicalcilindro,
						  "pozo",
						  logicEnv,
						  false,
						  0,
						  checkOverlaps);

   // Cilindro 9

	    G4VPhysicalVolume* pozo9 = new G4PVPlacement(rotationMatrix,
						   G4ThreeVector(-38.8*mm,0,-38.8*mm),
						   logicalcilindro,
						   "pozo",
						   logicEnv,
						   false,
						   0,
						   checkOverlaps);
   
  //     
  // Tapas
  //

 //tapa1
 G4VPhysicalVolume* tapa_infe = new G4PVPlacement(rotationMatrix,
						  G4ThreeVector(38.8*mm,-11.25*mm,38.8*mm),
						  logicaltapa,
						  "tapa_inf",
						  logicEnv,
						  false,
						  0,
						  checkOverlaps);
 
 //tapa2
 G4VPhysicalVolume* tapa_infe2 = new G4PVPlacement(rotationMatrix,
						   G4ThreeVector(0,-11.25*mm,38.8*mm),
						   logicaltapa,
						   "tapa_inf",
						   logicEnv,
						   false,
						   0,
						   checkOverlaps);
 //tapa3
 G4VPhysicalVolume* tapa_infe3 = new G4PVPlacement(rotationMatrix,
						   G4ThreeVector(-38.8*mm,-11.25*mm,38.8*mm),
						   logicaltapa,
						   "tapa_inf",
						   logicEnv,
						   false,
						   0,
						   checkOverlaps);

 //tapa4
 G4VPhysicalVolume* tapa_infe4 = new G4PVPlacement(rotationMatrix,
						   G4ThreeVector(38.8*mm,-11.25*mm,0),
						  logicaltapa,
						  "tapa_inf",
						  logicEnv,
						  false,
						  0,
						  checkOverlaps);
 //tapa5
  G4VPhysicalVolume* tapa_infe5 = new G4PVPlacement(rotationMatrix,
						    G4ThreeVector(0,-11.25*mm,0),
						    logicaltapa,
						    "tapa_inf",
						    logicEnv,
						    false,
						    0,
						    checkOverlaps);
  //tapa6
   G4VPhysicalVolume* tapa_infe6 = new G4PVPlacement(rotationMatrix,
						     G4ThreeVector(-38.8*mm,-11.25*mm,0),
						     logicaltapa,
						     "tapa_inf",
						     logicEnv,
						     false,
						     0,
						     checkOverlaps);
   //tapa7
    G4VPhysicalVolume* tapa_infe7 = new G4PVPlacement(rotationMatrix,
						      G4ThreeVector(38.8*mm,-11.25*mm,-38.8*mm),
						      logicaltapa,
						      "tapa_inf",
						      logicEnv,
						      false,
						      0,
						      checkOverlaps);
    //tapa8
     G4VPhysicalVolume* tapa_infe8 = new G4PVPlacement(rotationMatrix,
						       G4ThreeVector(0,-11.25*mm,-38.8*mm),
						       logicaltapa,
						       "tapa_inf",
						       logicEnv,
						       false,
						       0,
						       checkOverlaps);
     //tapa9
      G4VPhysicalVolume* tapa_infe9 = new G4PVPlacement(rotationMatrix,
							G4ThreeVector(-38.8*mm,-11.25*mm,-38.8*mm),
							logicaltapa,
							"tapa_inf",
							logicEnv,
							false,
							0,
							checkOverlaps);
                
  // Set Shape2 as scoring volume
  //
  
  fScoringVolume = logicalcilindro;

  //
  //always return the physical World
  //
  
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
