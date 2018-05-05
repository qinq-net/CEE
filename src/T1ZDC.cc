#include "T1ZDC.hh"
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

//#include "T1Tower.hh"
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

T1ZDC::T1ZDC()
{

  T1ZDC_exampledata();

  fstream theFile;  //   ifstream theFile;
  string filename="./data/ZDC.dat";
  string nameZDC="CEE_ZDC";
  theFile.open(filename,ios::in);
  G4Box* solidZDC;
  //string nameZDC="CEE_ZDC";
  G4NistManager* nist = G4NistManager::Instance();



  G4bool checkOverlaps = true;

  ///hangshu=0;   // "将要读第hangshu+1行"

  vector<string> result;
  ///读第一行
  ReadLine(theFile,result);
  {
    G4ThreeVector T1ZDC_Position=G4ThreeVector(stod(result[0])*mm,stod(result[1])*mm,stod(result[2])*mm);
    G4RotationMatrix rot;

    rot.rotateZ(stod(result[3])*rad);
    rot.rotateY(stod(result[4])*rad);
    rot.rotateZ(stod(result[5])*rad);

    transZDC=G4Transform3D(rot, T1ZDC_Position);
  }


  ///读第二行，构建TPC
  ReadLine(theFile,result);
  {
    solidZDC = new G4Box(nameZDC+"_solid",stod(result[0])*mm,stod(result[1])*mm,0.5*stod(result[2])*mm);
    G4Material* matZDC = nist->FindOrBuildMaterial("G4_AIR");                /////gai
    G4VisAttributes* visZDC = new G4VisAttributes(G4Colour(0.1,0.4,1.5,0));  /////gai
    logicZDC = new G4LogicalVolume(solidZDC,matZDC,nameZDC+"_logic");
    logicZDC-> SetVisAttributes(visZDC);
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
    G4LogicalVolume* logicZDCbox;
    G4int numofTower;
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
      numofTower=stoi(result[9]);


      G4Box* solidZDCbox = new G4Box(nameZDC+"_box_"+nowboxid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
      G4Material* matZDCbox = nist->FindOrBuildMaterial("G4_AIR");                /////gai
      G4VisAttributes* visZDCbox = new G4VisAttributes(G4Colour(0.1,0.8,1.1,0));  /////gai
      logicZDCbox = new G4LogicalVolume(solidZDCbox,matZDCbox,nameZDC+"_box_"+nowboxid+"_logic");
      logicZDCbox -> SetVisAttributes(visZDCbox);


      G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
      G4RotationMatrix boxrot;
      boxrot.rotateZ(box_eZ1*rad);
      boxrot.rotateY(box_eY*rad);
      boxrot.rotateZ(box_eZ2*rad);
      G4Transform3D boxtrans(boxrot, boxp);

      new G4PVPlacement(boxtrans,
                        logicZDCbox,            //its logical volume
                        nameZDC+"_box_"+nowboxid+"_phys",               //its name
                        logicZDC,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
    }

    ///循环读Tower
    for(int j=0;j<numofTower;j++){
      ///读Tower数据，构建Tower(Towerbox)
      G4LogicalVolume* logicZDCboxTower;
      string nowTowerid;
      G4int numofceng;
      ReadLine(theFile,result);
      {
        G4double box_dx,box_dy,box_dz;
        G4double box_px,box_py,box_pz;
        G4double box_eZ1,box_eY,box_eZ2;

        nowTowerid = result[0];
        box_dx=stod(result[1]);
        box_dy=stod(result[2]);
        box_dz=stod(result[3]);
        box_px=stod(result[4]);
        box_py=stod(result[5]);
        box_pz=stod(result[6]);
        box_eZ1=stod(result[7]);
        box_eY=stod(result[8]);
        box_eZ2=stod(result[9]);
        numofceng=stoi(result[10]);


        G4Box* solidZDCboxTower = new G4Box(nameZDC+"_box_"+nowboxid+"_Tower_"+nowTowerid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
        G4Material* matZDCboxTower = nist->FindOrBuildMaterial("G4_AIR");                /////gai
        G4VisAttributes* visZDCboxTower = new G4VisAttributes(G4Colour(0.1,0.8,1.1,0));  /////gai
        logicZDCboxTower = new G4LogicalVolume(solidZDCboxTower,matZDCboxTower,nameZDC+"_box_"+nowboxid+"_Tower_"+nowTowerid+"_logic");
        logicZDCboxTower -> SetVisAttributes(visZDCboxTower);

        G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
        G4RotationMatrix boxrot;
        boxrot.rotateZ(box_eZ1*rad);
        boxrot.rotateY(box_eY*rad);
        boxrot.rotateZ(box_eZ2*rad);
        G4Transform3D boxtrans(boxrot, boxp);

        new G4PVPlacement(boxtrans,
                          logicZDCboxTower,            //its logical volume
                          nameZDC+"_box_"+nowboxid+"_Tower_"+nowTowerid+"_phys",               //its name
                          logicZDCbox,                     //its mother  volume
                          false,                 //no boolean operation
                          0,                     //copy number
                          checkOverlaps);        //overlaps checking
      }
      ///循环读Towerceng
      for(int k=0;k<numofceng;k++){
        ///读Towerceng数据，构建Towerceng
        string cengid;
        ReadLine(theFile,result);
        {
          G4double box_dx,box_dy,box_dz;
          G4double box_px,box_py,box_pz;
          //G4double box_eZ1,box_eY,box_eZ2;
          cengid = result[0];
          box_dx=stod(result[1]);
          box_dy=stod(result[2]);
          box_dz=stod(result[3]);
          box_px=stod(result[4]);
          box_py=stod(result[5]);
          box_pz=stod(result[6]);
          //box_eZ1=stod(result[7]);
          //box_eY=stod(result[8]);
          //box_eZ2=stod(result[9]);

          G4Box* solidZDCboxTowerceng = new G4Box(nameZDC+"_box_"+nowboxid+"_Tower_"+nowTowerid+"_ceng_"+cengid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
          /////gai
          G4Material* matZDCboxTowerceng = nist->FindOrBuildMaterial("G4_AIR");                /////gai
          G4VisAttributes* visZDCboxTowerceng = new G4VisAttributes(G4Colour(0.9,0.1,0.1,0.5));  /////gai
          /*
          if(k%2!=0){
            matZDCboxTowerceng = nist->FindOrBuildMaterial("G4_AIR");                /////gai
            visZDCboxTowerceng = new G4VisAttributes(G4Colour(1.1,0.1,0.1,0.9));  /////gai
          }
          */
          /////gai
          G4LogicalVolume* logicZDCboxTowerceng = new G4LogicalVolume(solidZDCboxTowerceng,matZDCboxTowerceng,nameZDC+"_box_"+nowboxid+"_Tower_"+nowTowerid+"_ceng_"+cengid+"_logic");
          logicZDCboxTowerceng -> SetVisAttributes(visZDCboxTowerceng);

          G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
          G4RotationMatrix boxrot;
          /*
          boxrot.rotateZ(box_eZ1*rad);
          boxrot.rotateY(box_eY*rad);
          boxrot.rotateZ(box_eZ2*rad);
          */
          G4Transform3D boxtrans(boxrot, boxp);

          new G4PVPlacement(boxtrans,
                            logicZDCboxTowerceng,            //its logical volume
                            nameZDC+"_box_"+nowboxid+"_Tower_"+nowTowerid+"_ceng_"+cengid+"_phys",               //its name
                            logicZDCboxTower,                     //its mother  volume
                            false,                 //no boolean operation
                            0,                     //copy number
                            checkOverlaps);        //overlaps checking
        }

      }
    }

  }

  //outFile.close();
  theFile.close();
}


void T1ZDC::T1ZDC_exampledata()
{
  //ofstream //outFile;
  ////outFile.open("error.txt",ios::app);
  ////outFile <<"T1ZDC 构造函数开始"<<endl;
  G4int nofBox0 = 100;
  string filename="./data/ZDC.dat";
  G4double ZDC_px,ZDC_py,ZDC_pz;
  G4double ZDC_eZ1,ZDC_eY,ZDC_eZ2;
  ZDC_px = 0;
  ZDC_py = 0;
  ZDC_pz = 4000;
  ZDC_eZ1 = 0;
  ZDC_eY = 0;
  ZDC_eZ2 = 0;
  ofstream outFile;
  outFile.open(filename,ios::out);
  outFile<<"#1 相对于全局参考系的位置  殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) ##这是下一级坐标的参考系  单位：mm   注释行开头为#，不支持行内注释"<<endl;
  outFile<<to_string(ZDC_px)+" "+to_string(ZDC_py)+" "+to_string(ZDC_pz)+" "+to_string(ZDC_eZ1)+" "+to_string(ZDC_eY)+" "+to_string(ZDC_eZ2)<<endl;
  outFile<<"#2 dx dy dz（全长）"<<endl;
  outFile<<"2000 2000 850"<<endl<<endl;
  outFile<<"#3 box数目 "<<endl;
  outFile<<nofBox0<<endl<<endl;  ///to_string(nofBox0)
  outFile<<"##################################################################"<<endl;
  outFile<<"#- Boxid   ##"<<endl;
  outFile<<"#- Box:全长dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) 这个Box下Tower数目  ##Box位置 作为下一级坐标的参考系   ##殴拉角单位：rad"<<endl;
  outFile<<"##-- Tower:Boxid-Towerid dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) 层数"<<endl;
  outFile<<"###--- Tower每层:Boxid-Towerid-id dx dy dz 位置x 位置y 位置z ##约定dz为气隙/玻璃厚度"<<endl;
  outFile<<"##################################################################"<<endl<<endl;


  for(int i=1;i<=nofBox0;i++){
    outFile<<i<<endl;  //Boxid
    //G4double phi_d=(i-1)*30;
    //G4double phi_r=(i-1)*30*pi/180;
    G4double px;
    G4double py;
    G4double pz = 0;
    G4int xx = (i-1)/10;
    G4int yy = (i-1)%10;
    px = -500+50+100*xx;
    py = -500+100*yy;


    outFile<<"99.95"<<" "<<"99.95"<<" "<<"829.5"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"1"<<endl;//////
    //Tower
    outFile<<to_string(i)+"-1"<<" "<<"99.9"<<" "<<"99.9"<<" "<<"829"<<" "<<0<<" "<<0<<" "<<0<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"130"<<endl;
    //Towerceng
    for(int k=1;k<=65*2;k++){
      if(k%2!=0){
        outFile<<to_string(i)+"-1-"+to_string(k)<<" "<<"80"<<" "<<"80"<<" "<<"10"<<" "<<"0"<<" "<<"0"<<" "<<-412.75+12.7*(k-1)/2+5<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      }
      else{
        outFile<<to_string(i)+"-1-"+to_string(k)<<" "<<"80"<<" "<<"80"<<" "<<"2.5"<<" "<<"0"<<" "<<"0"<<" "<<-412.75+12.7*(k-2)/2+11.30<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      }
    }

  }

  outFile.close();
}
