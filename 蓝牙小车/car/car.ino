#include "SR04.h"
#define TRIG_PIN 10
#define ECHO_PIN 11
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);

unsigned char val, val1;
int s = 128, a = 0, b = 0;
long d;

void setup() {
  Serial.begin(57600);
  tone(A0, 1500, 400); //开机响一声

}

void loop() {


  if (Serial.available() > 0)
  {
    val = Serial.read();
  }
  switch (val)
  {
    case 'c':
      Serial.print("Distance = ");
      d = sr04.Distance();
      Serial.print(d);
      Serial.println("cm"); val = "";
      break;                //pc端发送字符c，若返回testOk，说明连接正常
    case 'w':
      up(s);
      val1 = val;
      break;                //按下w键，小车直行
    case 's':
      down(s);
      val1 = val;
      break;                //按下s键，小车后退
    case 'a':
      left(s);
      break;                //按下a键，向后左转
    case 'd':
      right(s);
      break;                 //按下d键，向后右转
    case 'q':
      s = s + 10;
      if (s > 255)
      {
        s = 255;
      }
      Serial.println("Speed = ");
      Serial.println(s);
      val = "";
      break;                  //按下q键，速度加
    case 'e':
      s = s - 10;
      if (s < 0)
      {
        s = 0;
      }
      Serial.println("Speed = ");
      Serial.println(s);
      val = "";
      break;                    //按下e键，速度减
    case 'z':
      rollleft(s);
      break;                    //按下z键，向后左转
    case 'x':
      rollright(s);
      break;                       //按下x键，向后右转
    case 'r':
      tone(A0, 2000);
      break;                        //喇叭
    case 't':
      noTone(A0);
      break;
    case 'f':
      motorstop();
      val1 = val;
      break;                           //未接收到数据时，小车停止
    case 'g':
      val = val1;
      val1 = "";
      break;
    case 'j':
      b = 1;
      val = "";
      break;
    case 'b':
      a = 0;
      val = "";
      break;
    case 'n':
      a = 1;
      val = "";
      break;
  }

  if (a == 1)           //避障模式
  {
    d = sr04.Distance();

    if (d < 15 )
    {
      rollright(130);
      tone(A0, 2000, 50);
      delay(300);
      if (d > 15)
        motorstop();
    }
  }

  while (b == 1)     //全自动模式，尚未完善
  {
    up(s);
    d = sr04.Distance();
    if (d < 20 && a == 1)
    {
      rollright(s);
      tone(A0, 2500, 20);
      delay(20);
    }
    val = Serial.read();
    if (val == 'h')
    {
      b = 0;
    }
  }

}

void up(int v)
{
  analogWrite(3, v);
  analogWrite(5, 0); //右轮
  analogWrite(6, 0);
  analogWrite(9, v); //左轮
}

void down(int v)
{
  analogWrite(3, 0);
  analogWrite(5, v);
  analogWrite(6, v);
  analogWrite(9, 0);
}

void left(int v)
{
  analogWrite(3, v);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, v / 2.5);
}

void right(int v)
{
  analogWrite(3, v / 2.5);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, v);
}

void motorstop(void)
{
  analogWrite(3, 0);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, 0);
}

void rollleft(int v)
{
  analogWrite(3, v);
  analogWrite(5, 0);
  analogWrite(6, v);
  analogWrite(9, 0);
}

void rollright(int v)
{
  analogWrite(3, 0);
  analogWrite(5, v);
  analogWrite(6, 0);
  analogWrite(9, v);
}
