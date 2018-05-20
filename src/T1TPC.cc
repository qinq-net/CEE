#include "T1TPC.hh"
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

T1TPC::T1TPC()
{

  //ofstream //outFile;
  ////outFile.open("error.txt",ios::app);
  ////outFile <<"T1TPC 构造函数开始"<<endl;
  G4int nofBox0 = 1;
  ofstream outFile;
  outFile.open("./data/TPC.dat",ios::app);
  outFile<<"#1 相对于全局参考系的位置  殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) ##这是下一级坐标的参考系  单位：mm   注释行开头为#，不支持行内注释"<<endl;
  outFile<<"0 0 450 0 0 0"<<endl;
  outFile<<"#2 dx dy dz（全长）"<<endl;
  outFile<<"1050 850 850"<<endl<<endl;
  outFile<<"#3 box数目 "<<endl;
  outFile<<nofBox0<<endl<<endl;  ///to_string(nofBox0)
  outFile<<"##################################################################"<<endl;
  outFile<<"#- Boxid  ##"<<endl;
  outFile<<"#- Box:全长dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z)  ##Box位置 作为下一级坐标的参考系   ##殴拉角单位：rad"<<endl;
  outFile<<"##-- TPC:Boxid-TPCid dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z)"<<endl;
  outFile<<"##################################################################"<<endl<<endl;


  for(int i=1;i<=nofBox0;i++){

    outFile<<i<<endl;
    G4double px = 0;
    G4double py = 0;
    G4double pz = 0;

    outFile<<"1050"<<" "<<"850"<<" "<<"850"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;//////
    outFile<<"1-1"<<" "<<"1000"<<" "<<"800"<<" "<<"800"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;//////

  }


  outFile.close();



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





  fstream theFile;  //   ifstream theFile;
  theFile.open("./data/TPC.dat",ios::in);
  G4Box* solidTPC;
  string nameTPC="CEE_TPC";
  G4NistManager* nist = G4NistManager::Instance();



  G4bool checkOverlaps = true;
  ///hangshu=0;   // "将要读第hangshu+1行"

  vector<string> result;
  ///读第一行
  ReadLine(theFile,result);
  {
    G4ThreeVector T1TPC_Position=G4ThreeVector(stod(result[0])*mm,stod(result[1])*mm,stod(result[2])*mm);
    G4RotationMatrix rot;
    /*
    rot.rotateZ(stod(result[3])*rad);
    rot.rotateY(stod(result[4])*rad);
    rot.rotateZ(stod(result[5])*rad);
    */
    transTPC=G4Transform3D(rot, T1TPC_Position);
  }

  ///读第二行，构建TPC
  ReadLine(theFile,result);
  {
    solidTPC = new G4Box(nameTPC+"_solid",stod(result[0])*mm,stod(result[1])*mm,0.5*stod(result[2])*mm);
    G4Material* matTPC = nist->FindOrBuildMaterial("G4_AIR");                /////gai
    G4VisAttributes* visTPC = new G4VisAttributes(G4Colour(0.1,0.4,1.5,0));  /////gai
    logicTPC = new G4LogicalVolume(solidTPC,matTPC,nameTPC+"_logic");
    logicTPC -> SetVisAttributes(visTPC);
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


      G4Box* solidTPCbox = new G4Box(nameTPC+"_box_"+nowboxid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
      G4Material* matTPCbox = nist->FindOrBuildMaterial("G4_AIR");                /////gai
      G4VisAttributes* visTPCbox = new G4VisAttributes(G4Colour(0.1,0.8,1.1,0));  /////gai
      logicTPCbox = new G4LogicalVolume(solidTPCbox,matTPCbox,nameTPC+"_box_"+nowboxid+"_logic");
      logicTPCbox -> SetVisAttributes(visTPCbox);


      G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
      G4RotationMatrix boxrot;
      boxrot.rotateZ(box_eZ1*rad);
      boxrot.rotateY(box_eY*rad);
      boxrot.rotateZ(box_eZ2*rad);
      G4Transform3D boxtrans(boxrot, boxp);

      physTPCbox = new G4PVPlacement(boxtrans,
                        logicTPCbox,            //its logical volume
                        nameTPC+"_box_"+nowboxid+"_phys",               //its name
                        logicTPC,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps
    }

    ///读TPCchamber
    string nowchamberid;
    ReadLine(theFile,result);
    {
      G4double box_dx,box_dy,box_dz;
      G4double box_px,box_py,box_pz;
      G4double box_eZ1,box_eY,box_eZ2;
      nowchamberid = result[0];
      box_dx=stod(result[1]);
      box_dy=stod(result[2]);
      box_dz=stod(result[3]);
      box_px=stod(result[4]);
      box_py=stod(result[5]);
      box_pz=stod(result[6]);
      box_eZ1=stod(result[7]);
      box_eY=stod(result[8]);
      box_eZ2=stod(result[9]);


      G4Box* solidTPCboxchamber = new G4Box(nameTPC+"_box_"+nowboxid+"_chamber_"+nowchamberid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
      G4Material* matTPCboxchamber= nist->FindOrBuildMaterial("G4_AIR");                /////gai
      G4VisAttributes* visTPCboxchamber = new G4VisAttributes(G4Colour(0.1,0.8,0.2,0.5));  /////gai
      logicTPCboxchamber = new G4LogicalVolume(solidTPCboxchamber,matTPCboxchamber,nameTPC+"_box_"+nowboxid+"_chamber_"+nowchamberid+"_logic");
      logicTPCboxchamber -> SetVisAttributes(visTPCboxchamber);

      G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
      G4RotationMatrix boxrot;
      boxrot.rotateZ(box_eZ1*rad);
      boxrot.rotateY(box_eY*rad);
      boxrot.rotateZ(box_eZ2*rad);
      G4Transform3D boxtrans(boxrot, boxp);

      physTPCchamber = new G4PVPlacement(boxtrans,
                        logicTPCboxchamber,            //its logical volume
                        nameTPC+"_box_"+nowboxid+"_chamber_"+nowchamberid+"_phys",               //its name
                        logicTPCbox,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
    }

  }
  //outFile.close();
  theFile.close();
}
