#include "Defination.h"
#include "song.h"
#include "Vector.h"


bool IsHitL = false;
bool IsHitR = false;
int cmd = 0;

bool GameStart = false;

 float CurrentTime = 0;
 float LastTime = 0;
 float CurrentTimeHit = 0;
 float LastTimeHit = 0;
 float TextTimeCurrent = 0;
 float TextTimeLast = 0;

int BuzzerIndex = 0;
int HitIndex = 0;

int Final_Score = 0;






Vector<IAphlaBatPoint *> V0;





void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LeftHitPin, INPUT_PULLUP);
  pinMode(RightHitPin, INPUT_PULLUP);

  pinMode(buzzerPin ,OUTPUT);
  display.init();           
  display.flipScreenVertically() ;  
  GameStart = false;                                                    
  V0.Clear();

}

void loop()
{
  // put your main code here, to run repeatedly:
  display.clear();
  
  CurrentTime = millis();
  cmd = playerHit();
  Serial.println(cmd);
  
    if ( GameStart == false)
    {
      display.setFont( ArialMT_Plain_16);
      display.drawString(0 , 0 , "**Music Game**");

      display.setFont( ArialMT_Plain_10);
      display.drawString( 0 ,22,"Hit anything to start!" );


      if (cmd > 0 )
      {
        GameStart = true;
        LastTime = CurrentTime;
      }

      // 　reset
      BuzzerIndex = 0;
      HitIndex = 0;
      Final_Score = 0;
    }
    else
    {

      if (StartingCount())
      {

        if (BuzzerIndex < SongLength-1)
        {
          DrawBack();
          PlayMusic();
          Draw();
        }
        else
        {

          noTone(buzzerPin);
          while (!playerHit())
          { 
            display.clear();
            display.setFont(ArialMT_Plain_16);
            display.drawString(0 , 0 , String(Final_Score));
            display.setFont(ArialMT_Plain_10);
            display.drawString( 0 , 45 , "Hit anything to Restart!");
            display.display();
            
          }
          V0.Clear();
          
          GameStart = false;
        }
      }
    }

    
    display.display();
    delay(14);
}

int playerHit()
{

  int playerHitResult = 0;

  if ((digitalRead(RightHitPin) == LOW) && (!IsHitR))
  {
    playerHitResult = 1;
    IsHitR = true;
  }
  if ((digitalRead(RightHitPin) == HIGH) && IsHitR)
  {
    IsHitR = false;
  }
  if ((digitalRead(LeftHitPin) == LOW) && (!IsHitL))
  {

    playerHitResult = 2;
    IsHitL = true;
  }
  if ((digitalRead(LeftHitPin) == HIGH) && IsHitL)
  {
    IsHitL = false;
  }

  return playerHitResult;
}

bool StartingCount()
{

  if (CurrentTime - LastTime < 5000)
  {

    char displayTime = (6-((CurrentTime - LastTime) / 1000)) + '0';

    display.setFont(ArialMT_Plain_24);
    display.drawString( 22 , 0 , String(displayTime));

    return false;
  }
  else{
    return true;
  }

  return true;
}

// 歌曲與生成音符

void DrawBack()
{
  display.drawVerticalLine(HitPos, 0, 62);
  // display.drawVerticalLine(HitPos , 0 , 62);
}

void PlayMusic()
{

     CurrentTime = millis();

    if( (CurrentTime - LastTime) >= ( pgm_read_float(& (Song_duration[BuzzerIndex])) )+5000+SongStartTime   ){
      
      if(pgm_read_float( (&Song[BuzzerIndex])) > 0){

        tone( buzzerPin , pgm_read_float(&Song[BuzzerIndex]) );
      }
      else{
        noTone(buzzerPin);
      }
      BuzzerIndex += 1;
    }
}

IAphlaBatPoint *String2Aphlabat()
{

  switch ( pgm_read_byte( &(Hit[HitIndex])))
  {
  case '1':
    return new FillCircle(HitPosStart_X, HitPosStart_Y);
    break;
  case '2':
    return new HollowCircle(HitPosStart_X, HitPosStart_Y);
    break;
  case '8':
    return new FillComboHit(HitPosStart_X, HitPosStart_Y);
    break;
  case '0':
    return nullptr;
    break;
  }

  return nullptr;
}

void SpawnHitPoint()
{

  CurrentTimeHit = millis();

  if (CurrentTimeHit - LastTimeHit > ( PushSpeed / SongBPM))
  {

    IAphlaBatPoint *Temp = String2Aphlabat();

    if (Temp != nullptr)
    {
      V0.PushBack(Temp);
      
    }

    HitIndex += 1;

    LastTimeHit = CurrentTimeHit;
  }
}

void Draw()
{

  int IsTemp = 0;

  SpawnHitPoint();

  if (V0.Size() > 0)
  {
    IsTemp = V0[0]->offest(cmd);
  }

  for (int i = 1; i < V0.Size(); i++)
  {

    V0[i]->offest();
  }

  for (int i = 0; i < V0.Size(); i++)
  {
    V0[i]->draw();
  }

  if (IsTemp > 0  && IsTemp != ((Score*Score_Good)+2) )
  {

    if (IsTemp != 1)
    {
      Final_Score += IsTemp;
    }

    

    Destroy(0);
  }

  if (V0.Size() > 0)
  {
    if ((V0[0]->_PointX <= -26 ))
    {

      Destroy(0);
    }
  }

  TextTimeCurrent = millis();


  if (Perfect)
  {
    display.setFont(ArialMT_Plain_24);
    display.drawString(40 ,40 , "Perfect");

  }
  else if (Good)
  {

    display.setFont(ArialMT_Plain_24);
    display.drawString(40 ,40 , "Good");
  }
  else if (Normal)
  {
    display.setFont(ArialMT_Plain_24);
    display.drawString(40 ,40 , "Normal");

  }
  else if (Bad)
  {
    display.setFont(ArialMT_Plain_24);
    display.drawString(40 ,40 , "Bad");

  }
  else if (Miss)
  {
    display.setFont(ArialMT_Plain_24);
    display.drawString(40 ,40 , "Miss");


  }
  else
  {
    TextTimeLast = TextTimeCurrent;
  }

 

  if (TextTimeCurrent - TextTimeLast > 2000)
  {
    Bad = false;
    Miss = false;
    Normal = false;
    Good = false;
    Perfect = false;
    TextTimeLast = TextTimeCurrent;
  }
}

void Destroy(int i)
{

  IAphlaBatPoint *temp = V0[i];

  V0.Erase(i);
  delete temp;

  Serial.println("Destroy");

}