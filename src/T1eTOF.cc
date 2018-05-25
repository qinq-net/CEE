#include "T1eTOF.hh"
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

T1eTOF::T1eTOF()
{

  T1eTOF_exampledata(1);
  T1eTOF_exampledata(2);

  T1eTOF_0(1);
  T1eTOF_0(2);
}

void T1eTOF::T1eTOF_0(int eTOF_id)
{
  fstream theFile;  //   ifstream theFile;
  string filename="./data/eTOF"+to_string(eTOF_id)+".dat";
  string nameeTOF="CEE_eTOF"+to_string(eTOF_id);
  theFile.open(filename,ios::in);
  G4Box* solideTOF;
  //string nameeTOF="CEE_eTOF";
  G4NistManager* nist = G4NistManager::Instance();



  G4bool checkOverlaps = true;

  ///hangshu=0;   // "将要读第hangshu+1行"

  vector<string> result;
  ///读第一行
  ReadLine(theFile,result);
  {
    G4ThreeVector T1eTOF_Position=G4ThreeVector(stod(result[0])*mm,stod(result[1])*mm,stod(result[2])*mm);
    G4RotationMatrix rot;

    rot.rotateZ(stod(result[3])*rad);
    rot.rotateY(stod(result[4])*rad);
    rot.rotateZ(stod(result[5])*rad);

    transeTOF[eTOF_id-1]=G4Transform3D(rot, T1eTOF_Position);
  }


  ///读第二行，构建eTOF
  ReadLine(theFile,result);
  {
    solideTOF = new G4Box(nameeTOF+"_solid",stod(result[0])*mm,stod(result[1])*mm,0.5*stod(result[2])*mm);
    G4Material* mateTOF = nist->FindOrBuildMaterial("G4_AIR");                /////gai
    G4VisAttributes* viseTOF = new G4VisAttributes(G4Colour(0.1,0.4,1.5,0));  /////gai
    logiceTOF[eTOF_id-1] = new G4LogicalVolume(solideTOF,mateTOF,nameeTOF+"_logic");
    logiceTOF[eTOF_id-1]-> SetVisAttributes(viseTOF);
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
    G4LogicalVolume* logiceTOFbox;
    G4int numofMRPC;
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
      numofMRPC=stoi(result[9]);


      G4Box* solideTOFbox = new G4Box(nameeTOF+"_box_"+nowboxid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
      G4Material* mateTOFbox = nist->FindOrBuildMaterial("G4_AIR");                /////gai
      G4VisAttributes* viseTOFbox = new G4VisAttributes(G4Colour(0.1,0.8,1.1,0));  /////gai
      logiceTOFbox = new G4LogicalVolume(solideTOFbox,mateTOFbox,nameeTOF+"_box_"+nowboxid+"_logic");
      logiceTOFbox -> SetVisAttributes(viseTOFbox);


      G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
      G4RotationMatrix boxrot;
      boxrot.rotateZ(box_eZ1*rad);
      boxrot.rotateY(box_eY*rad);
      boxrot.rotateZ(box_eZ2*rad);
      G4Transform3D boxtrans(boxrot, boxp);

      new G4PVPlacement(boxtrans,
                        logiceTOFbox,            //its logical volume
                        nameeTOF+"_box_"+nowboxid+"_phys",               //its name
                        logiceTOF[eTOF_id-1],                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
    }

    ///循环读MRPC
    for(int j=0;j<numofMRPC;j++){
      ///读MRPC数据，构建MRPC(MRPCbox)
      G4LogicalVolume* logiceTOFboxMRPC;
      string nowMRPCid;
      G4int numofceng;
      ReadLine(theFile,result);
      {
        G4double box_dx,box_dy,box_dz;
        G4double box_px,box_py,box_pz;
        G4double box_eZ1,box_eY,box_eZ2;

        nowMRPCid = result[0];
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


        G4Box* solideTOFboxMRPC = new G4Box(nameeTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
        G4Material* mateTOFboxMRPC = nist->FindOrBuildMaterial("G4_AIR");                /////gai
        G4VisAttributes* viseTOFboxMRPC = new G4VisAttributes(G4Colour(0.1,0.8,1.1,0));  /////gai
        logiceTOFboxMRPC = new G4LogicalVolume(solideTOFboxMRPC,mateTOFboxMRPC,nameeTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_logic");
        logiceTOFboxMRPC -> SetVisAttributes(viseTOFboxMRPC);
        SLVList[eTOF_id-1].push_back(logiceTOFboxMRPC);

        G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
        G4RotationMatrix boxrot;
        boxrot.rotateZ(box_eZ1*rad);
        boxrot.rotateY(box_eY*rad);
        boxrot.rotateZ(box_eZ2*rad);
        G4Transform3D boxtrans(boxrot, boxp);

        new G4PVPlacement(boxtrans,
                          logiceTOFboxMRPC,            //its logical volume
                          nameeTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_phys",               //its name
                          logiceTOFbox,                     //its mother  volume
                          false,                 //no boolean operation
                          0,                     //copy number
                          checkOverlaps);        //overlaps checking
      }
      ///循环读MRPCceng
      for(int k=0;k<numofceng;k++){
        ///读MRPCceng数据，构建MRPCceng
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

          G4Box* solideTOFboxMRPCceng = new G4Box(nameeTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_ceng_"+cengid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
          /////gai
          G4Material* mateTOFboxMRPCceng = nist->FindOrBuildMaterial("G4_AIR");                /////gai
          G4VisAttributes* viseTOFboxMRPCceng = new G4VisAttributes(G4Colour(0.2,0.5,0.8,0.9));  /////gai
          if(k%2!=0){
            mateTOFboxMRPCceng = nist->FindOrBuildMaterial("G4_AIR");                /////gai
            viseTOFboxMRPCceng = new G4VisAttributes(G4Colour(0.2,0.5,0.8,0.8));  /////gai
          }
          /////gai
          G4LogicalVolume* logiceTOFboxMRPCceng = new G4LogicalVolume(solideTOFboxMRPCceng,mateTOFboxMRPCceng,nameeTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_ceng_"+cengid+"_logic");
          logiceTOFboxMRPCceng -> SetVisAttributes(viseTOFboxMRPCceng);

          G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
          G4RotationMatrix boxrot;
          /*
          boxrot.rotateZ(box_eZ1*rad);
          boxrot.rotateY(box_eY*rad);
          boxrot.rotateZ(box_eZ2*rad);
          */
          G4Transform3D boxtrans(boxrot, boxp);

          new G4PVPlacement(boxtrans,
                            logiceTOFboxMRPCceng,            //its logical volume
                            nameeTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_ceng_"+cengid+"_phys",               //its name
                            logiceTOFboxMRPC,                     //its mother  volume
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

void T1eTOF::T1eTOF_exampledata(int eTOF_id)
{
  //ofstream //outFile;
  ////outFile.open("error.txt",ios::app);
  ////outFile <<"T1eTOF 构造函数开始"<<endl;
  G4int nofBox0 = 79;
  string filename="./data/eTOF"+to_string(eTOF_id)+".dat";
  G4double eTOF_px,eTOF_py,eTOF_pz;
  G4double eTOF_eZ1,eTOF_eY,eTOF_eZ2;
  G4double phi_r;
  if(eTOF_id==1){
    phi_r = 0.35;
  }
  else{
    phi_r = -0.35;
  }
  eTOF_px = 3000*sin(phi_r);
  eTOF_py = 0;
  eTOF_pz = 3000*cos(phi_r);
  eTOF_eZ1 = 0;
  eTOF_eY = phi_r;
  eTOF_eZ2 = 0;
  ofstream outFile;
  outFile.open(filename,ios::out);
  outFile<<"#1 相对于全局参考系的位置  殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) ##这是下一级坐标的参考系  单位：mm   注释行开头为#，不支持行内注释"<<endl;
  outFile<<to_string(eTOF_px)+" "+to_string(eTOF_py)+" "+to_string(eTOF_pz)+" "+to_string(eTOF_eZ1)+" "+to_string(eTOF_eY)+" "+to_string(eTOF_eZ2)<<endl;
  outFile<<"#2 dx dy dz（全长）"<<endl;
  outFile<<"2000 2000 30"<<endl<<endl;
  outFile<<"#3 box数目 "<<endl;
  outFile<<nofBox0*2<<endl<<endl;  ///to_string(nofBox0)
  outFile<<"##################################################################"<<endl;
  outFile<<"#- Boxid   ##"<<endl;
  outFile<<"#- Box:全长dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) 这个Box下MRPC数目  ##Box位置 作为下一级坐标的参考系   ##殴拉角单位：rad"<<endl;
  outFile<<"##-- MRPC:Boxid-MRPCid dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) 层数"<<endl;
  outFile<<"###--- MRPC每层:Boxid-MRPCid-id dx dy dz 位置x 位置y 位置z ##约定dz为气隙/玻璃厚度"<<endl;
  outFile<<"##################################################################"<<endl<<endl;


  for(int i=1;i<=nofBox0;i++){
    outFile<<i<<endl;  //Boxid
    //G4double phi_d=(i-1)*30;
    //G4double phi_r=(i-1)*30*pi/180;
    G4double px = 1005-i*25 ;
    G4double py = 0;
    G4double pz = 10;


    outFile<<"20"<<" "<<"2000"<<" "<<"10"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"3"<<endl;//////
    for(int j=1;j<=3;j++){
      //MRPCbox
      G4double px,pz;
      G4double py=0;
      switch (j) {
        case 1:
          pz = 2.5;
          px = 5;
          break;
        case 2:
          pz = 2.5;
          px = -5;
          break;
        case 3:
          pz = -2.5;
          px = 0;
          break;
      }
      outFile<<to_string(i)+"-"+to_string(j)<<" "<<"6"<<" "<<"2000"<<" "<<"4.5"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"6"<<endl;
      //MRPCceng
      outFile<<to_string(i)+"-"+to_string(j)+"-1"<<" "<<"6"<<" "<<"2000"<<" "<<"0.55"<<" "<<"0"<<" "<<"0"<<" "<<"1.825"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-2"<<" "<<"6"<<" "<<"2000"<<" "<<"0.25"<<" "<<"0"<<" "<<"0"<<" "<<"1.35"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-3"<<" "<<"6"<<" "<<"2000"<<" "<<"0.40"<<" "<<"0"<<" "<<"0"<<" "<<"1.025"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-4"<<" "<<"6"<<" "<<"2000"<<" "<<"0.25"<<" "<<"0"<<" "<<"0"<<" "<<"0.70"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-5"<<" "<<"6"<<" "<<"2000"<<" "<<"0.40"<<" "<<"0"<<" "<<"0"<<" "<<"0.375"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-6"<<" "<<"6"<<" "<<"2000"<<" "<<"0.25"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-7"<<" "<<"6"<<" "<<"2000"<<" "<<"0.40"<<" "<<"0"<<" "<<"0"<<" "<<"-0.375"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-8"<<" "<<"6"<<" "<<"2000"<<" "<<"0.25"<<" "<<"0"<<" "<<"0"<<" "<<"-0.70"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-9"<<" "<<"6"<<" "<<"2000"<<" "<<"0.40"<<" "<<"0"<<" "<<"0"<<" "<<"-1.025"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-10"<<" "<<"6"<<" "<<"2000"<<" "<<"0.25"<<" "<<"0"<<" "<<"0"<<" "<<"-1.35"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-11"<<" "<<"6"<<" "<<"2000"<<" "<<"0.55"<<" "<<"0"<<" "<<"0"<<" "<<"-1.825"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    }

  }
  for(int i=nofBox0+1;i<=2*nofBox0;i++){
    outFile<<i<<endl;  //Boxid
    //G4double phi_d=(i-1)*30;
    //G4double phi_r=(i-1)*30*pi/180;
    G4double px = -1005+(i-nofBox0)*25;
    G4double py = 0;
    G4double pz = -10;

    outFile<<"10"<<" "<<"20"<<" "<<"850"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"3"<<endl;//////
    for(int j=1;j<=3;j++){
      //MRPCbox
      G4double px,pz;
      G4double py=0;
      switch (j) {
        case 1:
          pz = 2.5;
          px = 5;
          break;
        case 2:
          pz = 2.5;
          px = -5;
          break;
        case 3:
          pz = -2.5;
          px = 0;
          break;
      }
      outFile<<to_string(i)+"-"+to_string(j)<<" "<<"6"<<" "<<"2000"<<" "<<"4.5"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"6"<<endl;
      //MRPCceng
      outFile<<to_string(i)+"-"+to_string(j)+"-1"<<" "<<"6"<<" "<<"2000"<<" "<<"0.55"<<" "<<"0"<<" "<<"0"<<" "<<"1.825"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-2"<<" "<<"6"<<" "<<"2000"<<" "<<"0.25"<<" "<<"0"<<" "<<"0"<<" "<<"1.35"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-3"<<" "<<"6"<<" "<<"2000"<<" "<<"0.40"<<" "<<"0"<<" "<<"0"<<" "<<"1.025"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-4"<<" "<<"6"<<" "<<"2000"<<" "<<"0.25"<<" "<<"0"<<" "<<"0"<<" "<<"0.70"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-5"<<" "<<"6"<<" "<<"2000"<<" "<<"0.40"<<" "<<"0"<<" "<<"0"<<" "<<"0.375"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-6"<<" "<<"6"<<" "<<"2000"<<" "<<"0.25"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-7"<<" "<<"6"<<" "<<"2000"<<" "<<"0.40"<<" "<<"0"<<" "<<"0"<<" "<<"-0.375"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-8"<<" "<<"6"<<" "<<"2000"<<" "<<"0.25"<<" "<<"0"<<" "<<"0"<<" "<<"-0.70"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-9"<<" "<<"6"<<" "<<"2000"<<" "<<"0.40"<<" "<<"0"<<" "<<"0"<<" "<<"-1.025"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-10"<<" "<<"6"<<" "<<"2000"<<" "<<"0.25"<<" "<<"0"<<" "<<"0"<<" "<<"-1.35"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-11"<<" "<<"6"<<" "<<"2000"<<" "<<"0.55"<<" "<<"0"<<" "<<"0"<<" "<<"-1.825"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    }
  }

  outFile.close();
}
