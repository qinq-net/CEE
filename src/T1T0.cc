#include "T1T0.hh"
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

using namespace std;
/*
void line_out_(ofstream& outFile, double var)
{
  outFile<<"1-1-1"<<" "<<"6"<<"0.55"<<" "<<"60"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
}
*/

T1T0::T1T0()
{

  //ofstream //outFile;
  ////outFile.open("error.txt",ios::app);
  ////outFile <<"T1T0 构造函数开始"<<endl;
  G4int nofBox0 = 12;
  ofstream outFile;
  outFile.open("./data/T0.dat",ios::app);
  outFile<<"#1 相对于全局参考系的位置  殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) ##这是下一级坐标的参考系  单位：mm   注释行开头为#，不支持行内注释"<<endl;
  outFile<<"0 0 0 0 0 0"<<endl;
  outFile<<"#2 rmin;rmax;T0的dz（全长）"<<endl;
  outFile<<"0 30 80"<<endl<<endl;
  outFile<<"#3 box数目 #每个box数据行数：4  ##之后可改为3"<<endl;
  outFile<<nofBox0*2<<endl<<endl;  ///to_string(nofBox0)
  outFile<<"##################################################################"<<endl;
  outFile<<"#- Boxid 这个Box下MRPC数目  ##"<<endl;
  outFile<<"#- Box:全长dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z)  ##Box位置 作为下一级坐标的参考系   ##殴拉角单位：rad"<<endl;
  outFile<<"##-- MRPC:Boxid-MRPCid dx dy dz 位置x 位置y 位置z 殴拉角1(Z) 殴拉角2(Y) 殴拉角3(Z) 层数（奇数）"<<endl;
  outFile<<"###--- MRPC每层:Boxid-MRPCid-id dx dy dz 位置x 位置y 位置z ##约定dz为气隙/玻璃厚度"<<endl;
  outFile<<"##################################################################"<<endl<<endl;


  for(int i=1;i<=nofBox0;i++){
    outFile<<i<<" "<<"1"<<endl;
    G4double phi_d=(i-1)*30;
    G4double phi_r=(i-1)*30*pi/180;
    G4double px = 18*sin(phi_r);
    G4double py = 18*cos(phi_r);
    G4double pz = 0;

    outFile<<"4"<<" "<<"6"<<" "<<"60"<<" "<<px<<" "<<py<<" "<<pz<<" "<<-phi_r<<" "<<"0"<<" "<<"0"<<" "<<"1"<<endl;//////
    outFile<<"1-1"<<" "<<"6"<<" "<<"4"<<" "<<"60"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"11"<<endl;
    outFile<<"1-1-1"<<" "<<"6"<<" "<<"0.55"<<" "<<"60"<<" "<<"0"<<" "<<"1.775"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-2"<<" "<<"6"<<" "<<"0.25"<<" "<<"60"<<" "<<"0"<<" "<<"1.3"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-3"<<" "<<"6"<<" "<<"0.40"<<" "<<"60"<<" "<<"0"<<" "<<"0.975"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-4"<<" "<<"6"<<" "<<"0.25"<<" "<<"60"<<" "<<"0"<<" "<<"0.65"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-5"<<" "<<"6"<<" "<<"0.40"<<" "<<"60"<<" "<<"0"<<" "<<"0.325"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-6"<<" "<<"6"<<" "<<"0.25"<<" "<<"60"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-7"<<" "<<"6"<<" "<<"0.40"<<" "<<"60"<<" "<<"0"<<" "<<"-0.325"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-8"<<" "<<"6"<<" "<<"0.25"<<" "<<"60"<<" "<<"0"<<" "<<"-0.65"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-9"<<" "<<"6"<<" "<<"0.40"<<" "<<"60"<<" "<<"0"<<" "<<"-0.975"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-10"<<" "<<"6"<<" "<<"0.25"<<" "<<"60"<<" "<<"0"<<" "<<"-1.3"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-11"<<" "<<"6"<<" "<<"0.55"<<" "<<"60"<<" "<<"0"<<" "<<"-1.775"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
  }
  for(int i=1;i<=nofBox0;i++){
    outFile<<i+nofBox0<<" "<<"1"<<endl;
    G4double phi_d=15+(i-1)*30;
    G4double phi_r=(15+(i-1)*30)*pi/180;
    G4double px = 22.5*sin(phi_r);
    G4double py = 22.5*cos(phi_r);
    G4double pz = 0;

    outFile<<"4"<<" "<<"6"<<" "<<"60"<<" "<<px<<" "<<py<<" "<<pz<<" "<<-phi_r<<" "<<"0"<<" "<<"0"<<" "<<"1"<<endl;//////
    outFile<<"1-1"<<" "<<"6"<<" "<<"4"<<" "<<"60"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"11"<<endl;
    outFile<<"1-1-1"<<" "<<"6"<<" "<<"0.55"<<" "<<"60"<<" "<<"0"<<" "<<"1.775"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-2"<<" "<<"6"<<" "<<"0.25"<<" "<<"60"<<" "<<"0"<<" "<<"1.3"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-3"<<" "<<"6"<<" "<<"0.40"<<" "<<"60"<<" "<<"0"<<" "<<"0.975"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-4"<<" "<<"6"<<" "<<"0.25"<<" "<<"60"<<" "<<"0"<<" "<<"0.65"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-5"<<" "<<"6"<<" "<<"0.40"<<" "<<"60"<<" "<<"0"<<" "<<"0.325"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-6"<<" "<<"6"<<" "<<"0.25"<<" "<<"60"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-7"<<" "<<"6"<<" "<<"0.40"<<" "<<"60"<<" "<<"0"<<" "<<"-0.325"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-8"<<" "<<"6"<<" "<<"0.25"<<" "<<"60"<<" "<<"0"<<" "<<"-0.65"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-9"<<" "<<"6"<<" "<<"0.40"<<" "<<"60"<<" "<<"0"<<" "<<"-0.975"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-10"<<" "<<"6"<<" "<<"0.25"<<" "<<"60"<<" "<<"0"<<" "<<"-1.3"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
    outFile<<"1-1-11"<<" "<<"6"<<" "<<"0.55"<<" "<<"60"<<" "<<"0"<<" "<<"-1.775"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
  }
  outFile.close();









  fstream theFile;  //   ifstream theFile;
  theFile.open("./data/T0.dat",ios::in);
  G4int hangshu=0;
  string str;
  G4Tubs* solidT0;
  string nameT0="CEE_T0";
  G4NistManager* nist = G4NistManager::Instance();
  G4int numofbox=0;
  string nowboxid;
  string nowMRPCid;

  G4bool checkOverlaps = false;
  hangshu=0;   // "将要读第hangshu+1行"
  while(hangshu<=2){
    if((getline(theFile,str))){
      ////outFile <<"T1T0 构造函数 56  读入数据前三行"<<"  str:"<<str<<endl;
      if(!str.empty()){
        ////outFile <<"T1T0 构造函数 58  读入数据前三行"<<"  str:"<<str<<endl;
        if(str[0]!=' '&&str[0]!='#'){
          ////outFile <<"T1T0 构造函数 60  读入数据前三行"<<"  str:"<<str<<"  hangshu:"<<hangshu+1<<endl;
          hangshu=hangshu+1;
          if(hangshu==1){
            vector<string> result=StringSplit(str," ");
            G4ThreeVector T1T0_Position=G4ThreeVector(stod(result[0])*mm,stod(result[1])*mm,stod(result[2])*mm);
            G4RotationMatrix boxrot;
            /*
            boxrot.rotateZ(box_eZ1*rad);
            boxrot.rotateY(box_eY*rad);
            boxrot.rotateZ(box_eZ2*rad);
            */
            transT0=G4Transform3D(boxrot, T1T0_Position);
          }
          else if(hangshu==2){
            vector<string> result=StringSplit(str," ");
            solidT0 = new G4Tubs(nameT0+"_solid",stod(result[0])*mm,stod(result[1])*mm,0.5*stod(result[2])*mm,0,2*pi);
            G4Material* matT0 = nist->FindOrBuildMaterial("G4_AIR");                /////gai
            G4VisAttributes* visT0 = new G4VisAttributes(G4Colour(0.1,0.4,1.5,0));  /////gai
            logicT0 = new G4LogicalVolume(solidT0,matT0,nameT0+"_logic");
            logicT0 -> SetVisAttributes(visT0);
          }
          else if(hangshu==3){
            //outFile <<"T1T0 构造函数 79  读入数据前三行"<<"  str:"<<str<<endl;
            vector<string> result=StringSplit(str," ");
            //outFile <<"T1T0 构造函数 81  读入数据前三行"<<"  result:"<<result[0]<<endl;
            numofbox=stoi(result[0]);
          }


        }
      }
    }
  }
  //outFile <<"T1T0 构造函数 87  读入数据读完前三行"<<"  numofbox:"<<numofbox<<endl;
  for(int i=0;i<numofbox;i++){
    G4int hangshu0=hangshu;
    G4int numofMRPC=0;
    G4LogicalVolume* logicT0box;
    while(hangshu==hangshu0){
      if(getline(theFile,str)){
        if(!str.empty()){
          if(str[0]!=' '&&str[0]!='#'){
            hangshu=hangshu+1;
            //outFile <<"T1T0 构造函数 96  读入数据 4  str:"<<str<<"  i:"<<i<<endl;
            vector<string> result=StringSplit(str," ");
            nowboxid=result[0];
            //numofMRPC=stoi(result[1]);
          }
        }
      }
    }
    //outFile <<"T1T0 构造函数 102  读入数据 4 numofMRPC:"<<numofMRPC<<endl;
    hangshu0=hangshu;
    while(hangshu==hangshu0){
      if(getline(theFile,str)){
        //outFile <<"T1T0 构造函数 111  读入数据 5"<<"  str:"<<str<<endl;
        if(!str.empty()){
          if(str[0]!=' '&&str[0]!='#'){
            //outFile <<"T1T0 构造函数 114  读入数据 5"<<"  str:"<<str<<endl;
            hangshu=hangshu+1;

            G4double box_dx,box_dy,box_dz;
            G4double box_px,box_py,box_pz;
            G4double box_eZ1,box_eY,box_eZ2;
            vector<string> result=StringSplit(str," ");
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
            //outFile <<"T1T0 构造函数 130  读入数据"<<"  box_eZ2:"<<box_eZ2<<endl;

            G4Box* solidT0box = new G4Box(nameT0+"_box_"+nowboxid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
            G4Material* matT0box = nist->FindOrBuildMaterial("G4_AIR");                /////gai
            G4VisAttributes* visT0box = new G4VisAttributes(G4Colour(0.1,0.8,1.1,0));  /////gai
            logicT0box = new G4LogicalVolume(solidT0box,matT0box,nameT0+"_box_"+nowboxid+"_logic");
            logicT0box -> SetVisAttributes(visT0box);
            //outFile <<"T1T0 构造函数 137  读入数据 hangshu:"<<hangshu<<endl;

            G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
            G4RotationMatrix boxrot;
            boxrot.rotateZ(box_eZ1*rad);
            boxrot.rotateY(box_eY*rad);
            boxrot.rotateZ(box_eZ2*rad);
            G4Transform3D boxtrans(boxrot, boxp);

            new G4PVPlacement(boxtrans,
                              logicT0box,            //its logical volume
                              nameT0+"_box_"+nowboxid+"_phys",               //its name
                              logicT0,                     //its mother  volume
                              false,                 //no boolean operation
                              0,                     //copy number
                              checkOverlaps);        //overlaps checking
          }
        }
      }
    }
    //outFile <<"T1T0 构造函数 156  开始MRPC"<<"  numofMRPC:"<<numofMRPC<<endl;
    for(int j=0;j<numofMRPC;j++){
      //outFile <<"T1T0 构造函数 158  开始MRPC"<<"  numofMRPC:"<<numofMRPC<<" j:"<<j<<endl;
      G4int numofceng=0;
      G4LogicalVolume* logicT0boxMRPC;
      hangshu0=hangshu;
      while(hangshu==hangshu0){
        if(getline(theFile,str)){
          if(!str.empty()){
            if(str[0]!=' '&&str[0]!='#'){
              hangshu=hangshu+1;

              G4double box_dx,box_dy,box_dz;
              G4double box_px,box_py,box_pz;
              G4double box_eZ1,box_eY,box_eZ2;
              vector<string> result=StringSplit(str," ");
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


              G4Box* solidT0boxMRPC = new G4Box(nameT0+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
              G4Material* matT0boxMRPC = nist->FindOrBuildMaterial("G4_AIR");                /////gai
              G4VisAttributes* visT0boxMRPC = new G4VisAttributes(G4Colour(0.1,0.8,1.1,0));  /////gai
              logicT0boxMRPC = new G4LogicalVolume(solidT0boxMRPC,matT0boxMRPC,nameT0+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_logic");
              logicT0boxMRPC -> SetVisAttributes(visT0boxMRPC);

              G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
              G4RotationMatrix boxrot;
              boxrot.rotateZ(box_eZ1*rad);
              boxrot.rotateY(box_eY*rad);
              boxrot.rotateZ(box_eZ2*rad);
              G4Transform3D boxtrans(boxrot, boxp);

              new G4PVPlacement(boxtrans,
                                logicT0boxMRPC,            //its logical volume
                                nameT0+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_phys",               //its name
                                logicT0box,                     //its mother  volume
                                false,                 //no boolean operation
                                0,                     //copy number
                                checkOverlaps);        //overlaps checking

            }
          }
        }
      }
      for(int k=0;k<numofceng;k++){
        hangshu0=hangshu;
        while(hangshu==hangshu0){
          if(getline(theFile,str)){
            if(!str.empty()){
              if(str[0]!=' '&&str[0]!='#'){
                hangshu=hangshu+1;

                string cengid;
                G4double box_dx,box_dy,box_dz;
                G4double box_px,box_py,box_pz;
                //G4double box_eZ1,box_eY,box_eZ2;
                vector<string> result=StringSplit(str," ");
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

                G4Box* solidT0boxMRPCceng = new G4Box(nameT0+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_ceng_"+cengid+"_solid",0.5*box_dx*mm,0.5*box_dy*mm,0.5*box_dz*mm);
                G4Material* matT0boxMRPCceng = nist->FindOrBuildMaterial("G4_AIR");                /////gai
                G4VisAttributes* visT0boxMRPCceng = new G4VisAttributes(G4Colour(0.1,0.8,1.1,1));  /////gai
                G4LogicalVolume* logicT0boxMRPCceng = new G4LogicalVolume(solidT0boxMRPCceng,matT0boxMRPCceng,nameT0+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_ceng_"+cengid+"_logic");
                logicT0boxMRPCceng -> SetVisAttributes(visT0boxMRPCceng);

                G4ThreeVector boxp(box_px*mm,box_py*mm,box_pz*mm);
                G4RotationMatrix boxrot;
                /*
                boxrot.rotateZ(box_eZ1*rad);
                boxrot.rotateY(box_eY*rad);
                boxrot.rotateZ(box_eZ2*rad);
                */
                G4Transform3D boxtrans(boxrot, boxp);

                new G4PVPlacement(boxtrans,
                                  logicT0boxMRPCceng,            //its logical volume
                                  nameT0+"_box_"+nowboxid+"_MRPC_"+nowMRPCid+"_ceng_"+cengid+"_phys",               //its name
                                  logicT0boxMRPC,                     //its mother  volume
                                  false,                 //no boolean operation
                                  0,                     //copy number
                                  checkOverlaps);        //overlaps checking

              }
            }
          }
        }
      }
    }

  }

  //outFile.close();
  theFile.close();


}
