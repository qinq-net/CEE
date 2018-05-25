#include "T1iTOF.hh"
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

T1iTOF::T1iTOF()
{

  T1iTOF_exampledata(1);
  T1iTOF_exampledata(2);

  T1iTOF_0(1);
  T1iTOF_0(2);
}

void T1iTOF::T1iTOF_0(int iTOF_id)
{
  fstream theFile;  //   ifstream theFile;
  string filename="./data/iTOF"+to_string(iTOF_id)+".dat";
  string nameiTOF="CEE_iTOF"+to_string(iTOF_id);
  theFile.open(filename,ios::in);
  G4Box* solidiTOF;
  //string nameiTOF="CEE_iTOF";
  G4NistManager* nist = G4NistManager::Instance();



  G4bool checkOverlaps = true;

  ///hangshu=0;   // "将要读第hangshu+1行"

  vector<string> result;
  ///读第一行
  ReadLine(theFile,result);
  {
    G4ThreeVector T1iTOF_Position=G4ThreeVector(stod(result[0])*mm,stod(result[1])*mm,stod(result[2])*mm);
    G4RotationMatrix rot;
    /*
    rot.rotateZ(box_eZ1*rad);
    rot.rotateY(box_eY*rad);
    rot.rotateZ(box_eZ2*rad);
    */
    transiTOF[iTOF_id-1]=G4Transform3D(rot, T1iTOF_Position);
  }


  ///读第二行，构建iTOF
  ReadLine(theFile,result);
  {
    solidiTOF = new G4Box(nameiTOF+"_solid",stod(result[0])*mm,stod(result[1])*mm,0.5*stod(result[2])*mm);
    G4Material* matiTOF = nist->FindOrBuildMaterial("G4_AIR");                /////gai
    G4VisAttributes* visiTOF = new G4VisAttributes(G4Colour(0.1,0.4,1.5,0));  /////gai
    logiciTOF[iTOF_id-1] = new G4LogicalVolume(solidiTOF,matiTOF,nameiTOF+"_logic");
    logiciTOF[iTOF_id-1]-> SetVisAttributes(visiTOF);
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
    G4LogicalVolume* logiciTOFbox;
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


      G4Box* solidiTOFbox = new G4Box(nameiTOF+"_box_"+nowboxid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
      G4Material* matiTOFbox = nist->FindOrBuildMaterial("G4_AIR");                /////gai
      G4VisAttributes* visiTOFbox = new G4VisAttributes(G4Colour(0.1,0.8,1.1,0));  /////gai
      logiciTOFbox = new G4LogicalVolume(solidiTOFbox,matiTOFbox,nameiTOF+"_box_"+nowboxid+"_logic");
      logiciTOFbox -> SetVisAttributes(visiTOFbox);


      G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
      G4RotationMatrix boxrot;
      boxrot.rotateZ(box_eZ1*rad);
      boxrot.rotateY(box_eY*rad);
      boxrot.rotateZ(box_eZ2*rad);
      G4Transform3D boxtrans(boxrot, boxp);

      new G4PVPlacement(boxtrans,
                        logiciTOFbox,            //its logical volume
                        nameiTOF+"_box_"+nowboxid+"_phys",               //its name
                        logiciTOF[iTOF_id-1],                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
    }

    ///循环读MRPC
    for(int j=0;j<numofMRPC;j++){
      ///读MRPC数据，构建MRPC(MRPCbox)
      G4LogicalVolume* logiciTOFboxMRPC;
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


        G4Box* solidiTOFboxMRPC = new G4Box(nameiTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
        G4Material* matiTOFboxMRPC = nist->FindOrBuildMaterial("G4_AIR");                /////gai
        G4VisAttributes* visiTOFboxMRPC = new G4VisAttributes(G4Colour(0.1,0.8,1.1,0));  /////gai
        logiciTOFboxMRPC = new G4LogicalVolume(solidiTOFboxMRPC,matiTOFboxMRPC,nameiTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_logic");
        logiciTOFboxMRPC -> SetVisAttributes(visiTOFboxMRPC);
        SLVList.push_back(logiciTOFboxMRPC);

        G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
        G4RotationMatrix boxrot;
        boxrot.rotateZ(box_eZ1*rad);
        boxrot.rotateY(box_eY*rad);
        boxrot.rotateZ(box_eZ2*rad);
        G4Transform3D boxtrans(boxrot, boxp);

        new G4PVPlacement(boxtrans,
                          logiciTOFboxMRPC,            //its logical volume
                          nameiTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_phys",               //its name
                          logiciTOFbox,                     //its mother  volume
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

          G4Box* solidiTOFboxMRPCceng = new G4Box(nameiTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_ceng_"+cengid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
          /////gai
          G4Material* matiTOFboxMRPCceng = nist->FindOrBuildMaterial("G4_AIR");                /////gai
          G4VisAttributes* visiTOFboxMRPCceng = new G4VisAttributes(G4Colour(0.1,0.8,1.1,1));  /////gai
          if(i>numofbox){
            matiTOFboxMRPCceng = nist->FindOrBuildMaterial("G4_AIR");                /////gai
            visiTOFboxMRPCceng = new G4VisAttributes(G4Colour(0.1,1.1,0.8,1));  /////gai
          }
          /////gai
          G4LogicalVolume* logiciTOFboxMRPCceng = new G4LogicalVolume(solidiTOFboxMRPCceng,matiTOFboxMRPCceng,nameiTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_ceng_"+cengid+"_logic");
          logiciTOFboxMRPCceng -> SetVisAttributes(visiTOFboxMRPCceng);

          G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
          G4RotationMatrix boxrot;
          /*
          boxrot.rotateZ(box_eZ1*rad);
          boxrot.rotateY(box_eY*rad);
          boxrot.rotateZ(box_eZ2*rad);
          */
          G4Transform3D boxtrans(boxrot, boxp);

          new G4PVPlacement(boxtrans,
                            logiciTOFboxMRPCceng,            //its logical volume
                            nameiTOF+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_ceng_"+cengid+"_phys",               //its name
                            logiciTOFboxMRPC,                     //its mother  volume
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

void T1iTOF::T1iTOF_exampledata(int iTOF_id)
{
  //ofstream //outFile;
  ////outFile.open("error.txt",ios::app);
  ////outFile <<"T1iTOF 构造函数开始"<<endl;
  G4int nofBox0 = 28;
  string filename="./data/iTOF"+to_string(iTOF_id)+".dat";
  G4double iTOF_px;
  if(iTOF_id==1){
    iTOF_px = 550;
  }
  else{
    iTOF_px = -550;
  }
  ofstream outFile;
  outFile.open(filename,ios::out);
  outFile<<"#1 相对于全局参考系的位置  殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) ##这是下一级坐标的参考系  单位：mm   注释行开头为#，不支持行内注释"<<endl;
  outFile<<to_string(iTOF_px)+" 0 450 0 0 0"<<endl;
  outFile<<"#2 dx dy dz（全长）"<<endl;
  outFile<<"30 880 860"<<endl<<endl;
  outFile<<"#3 box数目 "<<endl;
  outFile<<nofBox0*2<<endl<<endl;  ///to_string(nofBox0)
  outFile<<"##################################################################"<<endl;
  outFile<<"#- Boxid   ##"<<endl;
  outFile<<"#- Box:全长dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) 这个Box下MRPC数目  ##Box位置 作为下一级坐标的参考系   ##殴拉角单位：rad"<<endl;
  outFile<<"##-- MRPC:Boxid-MRPCid dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) 层数"<<endl;
  outFile<<"###--- MRPC每层:Boxid-MRPCid-id dx dy dz 位置x 位置y 位置z ##约定dz为玻璃厚度"<<endl;
  outFile<<"##################################################################"<<endl<<endl;


  for(int i=1;i<=nofBox0;i++){
    outFile<<i<<endl;  //Boxid
    //G4double phi_d=(i-1)*30;
    //G4double phi_r=(i-1)*30*pi/180;
    G4double px = 10 ;
    G4double py = 440-i*30;
    G4double pz = 0;


    outFile<<"10"<<" "<<"20"<<" "<<"850"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"3"<<endl;//////
    for(int j=1;j<=3;j++){
      //MRPCbox
      G4double px,py;
      G4double pz=0;
      switch (j) {
        case 1:
          px = 2.5;
          py = 5;
          break;
        case 2:
          px = 2.5;
          py = -5;
          break;
        case 3:
          px = -2.5;
          py = 0;
          break;
      }
      outFile<<to_string(i)+"-"+to_string(j)<<" "<<"4.5"<<" "<<"6"<<" "<<"850"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"6"<<endl;
      //MRPCceng
      outFile<<to_string(i)+"-"+to_string(j)+"-1"<<" "<<"0.55"<<" "<<"6"<<" "<<"850"<<" "<<"1.825"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-2"<<" "<<"0.25"<<" "<<"6"<<" "<<"850"<<" "<<"1.35"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-3"<<" "<<"0.40"<<" "<<"6"<<" "<<"850"<<" "<<"1.025"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-4"<<" "<<"0.25"<<" "<<"6"<<" "<<"850"<<" "<<"0.70"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-5"<<" "<<"0.40"<<" "<<"6"<<" "<<"850"<<" "<<"0.375"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-6"<<" "<<"0.25"<<" "<<"6"<<" "<<"850"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-7"<<" "<<"0.40"<<" "<<"6"<<" "<<"850"<<" "<<"-0.375"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-8"<<" "<<"0.25"<<" "<<"6"<<" "<<"850"<<" "<<"-0.70"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-9"<<" "<<"0.40"<<" "<<"6"<<" "<<"850"<<" "<<"-1.025"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-10"<<" "<<"0.25"<<" "<<"6"<<" "<<"850"<<" "<<"-1.35"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-11"<<" "<<"0.55"<<" "<<"6"<<" "<<"850"<<" "<<"-1.825"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    }

  }
  for(int i=nofBox0+1;i<=2*nofBox0;i++){
    outFile<<i<<endl;  //Boxid
    //G4double phi_d=(i-1)*30;
    //G4double phi_r=(i-1)*30*pi/180;
    G4double px = -10;
    G4double py = -440+(i-nofBox0)*30;
    G4double pz = 0;

    outFile<<"10"<<" "<<"20"<<" "<<"850"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"3"<<endl;//////
    for(int j=1;j<=3;j++){
      //MRPCbox
      G4double px,py;
      G4double pz=0;
      switch (j) {
        case 1:
          px = 2.5;
          py = 5;
          break;
        case 2:
          px = 2.5;
          py = -5;
          break;
        case 3:
          px = -2.5;
          py = 0;
          break;
      }
      outFile<<to_string(i)+"-"+to_string(j)<<" "<<"4.5"<<" "<<"6"<<" "<<"850"<<" "<<px<<" "<<py<<" "<<pz<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"6"<<endl;
      //MRPCceng
      outFile<<to_string(i)+"-"+to_string(j)+"-1"<<" "<<"0.55"<<" "<<"6"<<" "<<"850"<<" "<<"1.825"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-2"<<" "<<"0.25"<<" "<<"6"<<" "<<"850"<<" "<<"1.35"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-3"<<" "<<"0.40"<<" "<<"6"<<" "<<"850"<<" "<<"1.025"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-4"<<" "<<"0.25"<<" "<<"6"<<" "<<"850"<<" "<<"0.70"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-5"<<" "<<"0.40"<<" "<<"6"<<" "<<"850"<<" "<<"0.375"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-6"<<" "<<"0.25"<<" "<<"6"<<" "<<"850"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-7"<<" "<<"0.40"<<" "<<"6"<<" "<<"850"<<" "<<"-0.375"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-8"<<" "<<"0.25"<<" "<<"6"<<" "<<"850"<<" "<<"-0.70"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-9"<<" "<<"0.40"<<" "<<"6"<<" "<<"850"<<" "<<"-1.025"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      //outFile<<to_string(i)+"-"+to_string(j)+"-10"<<" "<<"0.25"<<" "<<"6"<<" "<<"850"<<" "<<"-1.35"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
      outFile<<to_string(i)+"-"+to_string(j)+"-11"<<" "<<"0.55"<<" "<<"6"<<" "<<"850"<<" "<<"-1.825"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    }
  }

  outFile.close();
}
