#include "T1MWDC.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"



#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"  //yuanzhu guanzi
#include "G4Cons.hh"
#include "G4Orb.hh"  //shixinqiu
#include "G4Torus.hh"  //quan
#include "G4Sphere.hh" //qiu  (duocanshu)
#include "G4Trd.hh"

#include "globals.hh"

#include "G4SystemOfUnits.hh"

//#include "T1MRPC.hh"
#include <iostream>
#include <string>
#include <vector>
#include "StringSplit.hh"
#include "ReadLine.hh"

using namespace std;
/*
void line_out_(ofstream& outFile, double var)
{
  outFile<<"1-1-1"<<" "<<"6"<<"0.55"<<" "<<"60"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
}
*/

T1MWDC::T1MWDC()
{
  for(int id=1;id<=6;id++){
    T1MWDC_exampledata(id);
    T1MWDC_0(id);
  }
}

void T1MWDC::T1MWDC_0(int MWDC_id)
{
  fstream theFile;  //   ifstream theFile;
  string filename="./data/MWDC"+to_string(MWDC_id)+".dat";
  string nameMWDC="CEE_MWDC"+to_string(MWDC_id);
  theFile.open(filename,ios::in);
  G4Box* solidMWDC;
  //string nameMWDC="CEE_MWDC";
  G4NistManager* nist = G4NistManager::Instance();



  G4bool checkOverlaps = true;

  ///hangshu=0;   // "将要读第hangshu+1行"

  vector<string> result;
  ///读第一行
  ReadLine(theFile,result);
  {
    G4ThreeVector T1MWDC_Position=G4ThreeVector(stod(result[0])*mm,stod(result[1])*mm,stod(result[2])*mm);
    G4RotationMatrix rot;

    rot.rotateZ(stod(result[3])*rad);
    rot.rotateY(stod(result[4])*rad);
    rot.rotateZ(stod(result[5])*rad);

    transMWDC[MWDC_id-1]=G4Transform3D(rot, T1MWDC_Position);
  }


  ///读第二行，构建MWDC
  ReadLine(theFile,result);
  {
    solidMWDC = new G4Box(nameMWDC+"_solid",stod(result[0])*mm,stod(result[1])*mm,0.5*stod(result[2])*mm);
    G4Material* matMWDC = nist->FindOrBuildMaterial("G4_AIR");                /////gai
    G4VisAttributes* visMWDC = new G4VisAttributes(G4Colour(0.1,0.4,1.5,0));  /////gai
    logicMWDC[MWDC_id-1] = new G4LogicalVolume(solidMWDC,matMWDC,nameMWDC+"_logic");
    logicMWDC[MWDC_id-1]-> SetVisAttributes(visMWDC);
  }

  ///读第三行
  G4int numofbox;
  ReadLine(theFile,result);
  {
    numofbox=stoi(result[0]);
  }

  ///循环读box
  for(int i=0;i<numofbox;i++){
    ///读boxid
    string nowboxid;
    ReadLine(theFile,result);
    {
      nowboxid=result[0];
    }


    ///读box数据，构建box
    G4LogicalVolume* logicMWDCbox;
    ReadLine(theFile,result);
    {
      G4double box_dx,box_dy,box_dz;
      G4double box_px,box_py,box_pz;
      G4double box_eZ1,box_eY,box_eZ2;

      box_dx=stod(result[0]);
      box_dy=stod(result[1]);
      box_dz=stod(result[2]);
      box_px=stod(result[3]);
      box_py=stod(result[4]);
      box_pz=stod(result[5]);
      box_eZ1=stod(result[6]);
      box_eY=stod(result[7]);
      box_eZ2=stod(result[8]);


      G4Box* solidMWDCbox = new G4Box(nameMWDC+"_box_"+nowboxid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
      G4Material* matMWDCbox = nist->FindOrBuildMaterial("G4_AIR");                /////gai
      G4VisAttributes* visMWDCbox = new G4VisAttributes(G4Colour(0.2,0.9,0.2,0));  /////gai
      logicMWDCbox = new G4LogicalVolume(solidMWDCbox,matMWDCbox,nameMWDC+"_box_"+nowboxid+"_logic");
      logicMWDCbox -> SetVisAttributes(visMWDCbox);


      G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
      G4RotationMatrix boxrot;
      boxrot.rotateZ(box_eZ1*rad);
      boxrot.rotateY(box_eY*rad);
      boxrot.rotateZ(box_eZ2*rad);
      G4Transform3D boxtrans(boxrot, boxp);

      new G4PVPlacement(boxtrans,
                        logicMWDCbox,            //its logical volume
                        nameMWDC+"_box_"+nowboxid+"_phys",               //its name
                        logicMWDC[MWDC_id-1],                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
    }

    {
      ///读MRPC数据，构建MRPC(MRPCbox)
      G4LogicalVolume* logicMWDCboxChamber;
      string MWDCChamberid;
      ReadLine(theFile,result);
      {
        G4double box_dx,box_dy,box_dz;
        G4double box_px,box_py,box_pz;
        G4double box_eZ1,box_eY,box_eZ2;

        MWDCChamberid = result[0];
        box_dx=stod(result[1]);
        box_dy=stod(result[2]);
        box_dz=stod(result[3]);
        box_px=stod(result[4]);
        box_py=stod(result[5]);
        box_pz=stod(result[6]);
        box_eZ1=stod(result[7]);
        box_eY=stod(result[8]);
        box_eZ2=stod(result[9]);


        G4Box* solidMWDCboxMRPC = new G4Box(nameMWDC+"_box_"+nowboxid+"_MRPC_"+MWDCChamberid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
        G4Material* matMWDCboxMRPC = nist->FindOrBuildMaterial("G4_AIR");                /////gai
        G4VisAttributes* visMWDCboxMRPC = new G4VisAttributes(G4Colour(0.2,0.9,0.2,0.5));  /////gai
        logicMWDCboxChamber = new G4LogicalVolume(solidMWDCboxMRPC,matMWDCboxMRPC,nameMWDC+"_box_"+nowboxid+"_MRPC_"+MWDCChamberid+"_logic");
        logicMWDCboxChamber -> SetVisAttributes(visMWDCboxMRPC);

        G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
        G4RotationMatrix boxrot;
        boxrot.rotateZ(box_eZ1*rad);
        boxrot.rotateY(box_eY*rad);
        boxrot.rotateZ(box_eZ2*rad);
        G4Transform3D boxtrans(boxrot, boxp);

        new G4PVPlacement(boxtrans,
                          logicMWDCboxChamber,            //its logical volume
                          nameMWDC+"_box_"+nowboxid+"_MRPC_"+MWDCChamberid+"_phys",               //its name
                          logicMWDCbox,                     //its mother  volume
                          false,                 //no boolean operation
                          0,                     //copy number
                          checkOverlaps);        //overlaps checking
      }

    }

  }

  //outFile.close();
  theFile.close();

}

void T1MWDC::T1MWDC_exampledata(int MWDC_id)
{
  //ofstream //outFile;
  ////outFile.open("error.txt",ios::app);
  ////outFile <<"T1MWDC 构造函数开始"<<endl;
  string filename="./data/MWDC"+to_string(MWDC_id)+".dat";
  G4double MWDC_px,MWDC_py,MWDC_pz;
  G4double MWDC_dx,MWDC_dy,MWDC_dz;
  G4double MWDC_eZ1,MWDC_eY,MWDC_eZ2;
  G4double phi_r,r0;
  if(MWDC_id<=3){
    phi_r = 0.35;
  }
  else{
    phi_r = -0.35;
  }
  switch(MWDC_id){
    case 1:
      phi_r = 0.35;
      r0 = 1400;
      break;
    case 2:
      phi_r = 0.35;
      r0 = 2000;
      break;
    case 3:
      phi_r = 0.35;
      r0 = 2600;
      break;
    case 4:
      phi_r = -0.35;
      r0 = 1400;
      break;
    case 5:
      phi_r = -0.35;
      r0 = 2000;
      break;
    case 6:
      phi_r = -0.35;
      r0 = 2600;
      break;

  }
  MWDC_px = r0*sin(phi_r);
  MWDC_py = 0;
  MWDC_pz = r0*cos(phi_r);
  MWDC_dx = fabs(2*r0*sin(phi_r-0.1));
  MWDC_dy = 1000;
  MWDC_dz = 100;
  MWDC_eZ1 = 0;
  MWDC_eY = phi_r;
  MWDC_eZ2 = 0;
  ofstream outFile;
  outFile.open(filename,ios::out);
  outFile<<"#1 相对于全局参考系的位置  殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) ##这是下一级坐标的参考系  单位：mm   注释行开头为#，不支持行内注释"<<endl;
  outFile<<MWDC_px<<" "<<MWDC_py<<" "<<MWDC_pz<<" "<<MWDC_eZ1<<" "<<MWDC_eY<<" "<<MWDC_eZ2<<endl;
  outFile<<"#2 dx dy dz（全长）"<<endl;
  outFile<<MWDC_dx<<" "<<MWDC_dy<<" "<<MWDC_dz<<endl<<endl<<endl;
  outFile<<"#3 box数目 "<<endl;
  outFile<<1<<endl<<endl;  ///to_string(nofBox0)
  outFile<<"##################################################################"<<endl;
  outFile<<"#- Boxid   ##"<<endl;
  outFile<<"#- Box:全长dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) 这个Box下MRPC数目  ##Box位置 作为下一级坐标的参考系   ##殴拉角单位：rad"<<endl;
  outFile<<"#- Chamber:Boxid-MRPCid dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z)"<<endl;
  outFile<<"##################################################################"<<endl<<endl;

  G4double dx = MWDC_dx-0.1;
  G4double dy = MWDC_dy-0.1;
  G4double dz = MWDC_dz-0.1;
  G4double px = 0;
  G4double py = 0;
  G4double pz = 0;

  outFile<<"1"<<endl;
  outFile<<dx<<" "<<dy<<" "<<dz<<" "<<px<<" "<<py<<" "<<pz<<" "<<0<<" "<<0<<" "<<0<<endl;//////
  outFile<<"1-1"<<" "<<dx-0.1<<" "<<dy-0.1<<" "<<dz-0.1<<" "<<px<<" "<<py<<" "<<pz<<" "<<0<<" "<<0<<" "<<0<<endl;

  outFile.close();
}
