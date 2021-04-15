int slider = 1;
int x = 0;
int y = 0;
  
class BlynkControl{
  public:

  double GetPowerPercentage(){
    return (float)slider / 100.0;  
  }

  int GetAcceleration(){
    return x;
  }

  int GetAngle(){
    return y;  
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
