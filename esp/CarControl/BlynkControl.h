const int startMotorIndex = 25;

int slider = 0;
int x = 0;
int y = 0;
  
class BlynkControl{
  public:

  double GetPowerPercentage(){
    return (float)slider / 100.0;  
  }

  int GetAcceleration(){
    return x + startMotorIndex;  
  }

  int GetAngle(){
    return y + startMotorIndex;  
  }
};

BLYNK_WRITE(V0)
{
  slider = param.asInt();
}

BLYNK_WRITE(V3)
{
  x = param.asInt();
}

BLYNK_WRITE(V4)
{
  y = param.asInt();
}