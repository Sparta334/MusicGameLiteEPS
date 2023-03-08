
#pragma once

#include "SSD1306Wire.h"

SSD1306Wire display(0x3C, SDA, SCL); 

int RightHitPin = 14;
int LeftHitPin = 16;
int buzzerPin = 13;


/* 音符定義區 */

float Aphlabat_radius = 6.25f;
int OffestSpeed = 2;
float OffestDelay = 1.0f;
int HitPos = 20 ;
float HitPosTolirance = 1.85f;
int HitPosStart_X = 125;
int HitPosStart_Y = 32;

float Score = 100;              
float Score_Prefect = 2.0f;
float Score_Good  = 1.5f;
float Score_Normal = 1.0f;
float Score_Bad = 0.7f;



bool Perfect = false;
bool Good = false;
bool Normal = false;
bool Bad = false;
bool Miss = false;

/* 音符宣告*/


class IAphlaBatPoint{




public:

  
  int OffestCurrentTime = 0;
  int OffestLastTime = 0  ;
  int _PointX;
  int _PointY;


  

  virtual void draw(){}
  virtual int Hit( ){
      
      float score = 0;
 
    if(_PointX > HitPos- (Aphlabat_radius*4)  &&_PointX < HitPos+ (Aphlabat_radius*6)   ){
   
        
        score = Score;
          if( _PointX <= HitPos + HitPosTolirance*2 &&  _PointX >= HitPos - HitPosTolirance*2  ){

                score *= Score_Prefect;
                Perfect  = true;
              /*Perfect 的範圍 */
            
          }
          else if( _PointX <= HitPos + (HitPosTolirance*4) &&  _PointX >= HitPos - (HitPosTolirance*4)  ){

            /*Good 的範圍 */
            score *= Score_Good;
            Good = true;
            

          }
          else if(_PointX <= HitPos + (HitPosTolirance*6 ) &&  _PointX >=HitPos - (HitPosTolirance*6) ){
            /*Normal 的範圍 */

            score *= Score_Normal;
            Normal = true;

            
          }
          else if(_PointX <=  HitPos + (HitPosTolirance*8 ) &&  _PointX >= HitPos - (HitPosTolirance*8 )){
            /*bad 的範圍 */

            score *= Score_Bad;
            Bad = true;
          }
          else{
            score= 1 ;

            Miss = true;
            
          }
   

    }
    else{
       
       score = 0;
    }
  


    return score;

  }
  virtual int offest(int cmd  ){ return 0;}
  virtual void offest( ){ 

    this->_PointX-=OffestSpeed;

  }
  IAphlaBatPoint(int PointX , int PointY){_PointX = PointX ; _PointY  = PointY;}
  IAphlaBatPoint(){_PointX = 128 ; _PointY  =32;}


};



/*實心大音符*/
class FillCircle : public IAphlaBatPoint{


public:
  FillCircle(int PointX , int PointY) : IAphlaBatPoint(PointX , PointY){
    
  }

   
  int offest(int cmd) override {
    int Temp = 0;
    this->_PointX-=OffestSpeed;


     
    if(cmd == 1 ){

        
        Temp = this->Hit();
    
    }

    return Temp;

  }


   virtual void draw() override {

    display.fillCircle( _PointX , _PointY , Aphlabat_radius );

  }

};


class HollowCircle: public IAphlaBatPoint{

public:

  HollowCircle(int PointX , int PointY) : IAphlaBatPoint(PointX , PointY){

  }


  int offest(int cmd ) override {

      int Temp = 0;
      this->_PointX-=OffestSpeed;

    if( cmd == 2 ){
       Temp = this->Hit();



    
    }

    return Temp;

  }

  virtual void draw() override {

    display.drawCircle( _PointX , _PointY , Aphlabat_radius  );

  }

};




class FillComboHit : public IAphlaBatPoint{


public:
  FillComboHit(int PointX , int PointY) : IAphlaBatPoint(PointX , PointY){
    
  }

  int Hit(  ){
      float score = 0;
      if(_PointX <= HitPos + (HitPosTolirance*22) &&  _PointX >= HitPos - (HitPosTolirance*4)  ){

        score += (Score*Score_Good)+2;
        Perfect = true ;

      }
      else{
        
        score = 0;
      }

      return score;

  }
  int offest(int cmd ) override {

    int Score = 0; 
    this->_PointX-=OffestSpeed;
    OffestLastTime = OffestCurrentTime;

    if(  cmd == 1 || cmd == 2 ){

         Score+=this->Hit();
    }

    return Score;

  }


  void draw() override {

    display.fillCircle( _PointX , _PointY , Aphlabat_radius*2.3f );
    display.fillCircle( _PointX+4 , _PointY , Aphlabat_radius*2.3f);
    display.fillCircle( _PointX+8 , _PointY , Aphlabat_radius*2.3f);
    display.fillCircle( _PointX+12 , _PointY , Aphlabat_radius*2.3f);
    display.fillCircle( _PointX+16 , _PointY , Aphlabat_radius*2.3f);
    display.fillCircle( _PointX+20 , _PointY , Aphlabat_radius*2.3f);
  }

};                                                                                                                 
