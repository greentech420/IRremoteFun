/**
   モータファンのリモートコントロールシステム
*/

#include <IRremote.h>

#define IRpin 8

//---------------------------
// IR Signal
//---------------------------
#define IR_POWER        0x00  //  電源
#define IR_VOLp         0x80  //  VOL+
#define IR_FUNC_STOP    0x40  //  FUNC/STOP
#define IR_QRETURN      0x20  //  早戻し
#define IR_PLAY         0xA0  //  再生
#define IR_FORWARD      0x60  //  早送り
#define IR_DOWN         0x10  //  ▽
#define IR_VOLm         0x90  //  VOL-
#define IR_UP           0x50  //  △
#define IR_EQ           0xB0  //  EQ
#define IR_ST_REPT      0x70  //  ST/REPT
#define IR_ZERO         0x30  //  0
#define IR_ONE          0x08  //  1
#define IR_TWO          0x88  //  2
#define IR_THREE        0x48  //  3
#define IR_FOUR         0x28  //  4
#define IR_FIVE         0xA8  //  5
#define IR_SIX          0x68  //  6
#define IR_SEVEN        0x18  //  7
#define IR_EIGHT        0x98  //  8
#define IR_NINE         0x58  //  9

IRrecv irrecv(IRpin);

//---------------------------
// LED pin
//---------------------------
#define D1    13
#define D2    12
#define D3    11

//---------------------------
// MOTOR pin
//---------------------------
int E1 = 5;
int M1 = 4;

void setup() {
  // put your setup code here, to run once:
  
  /* D1-D4 set... */
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);

  /* E1,M1 set... */
  pinMode(E1,OUTPUT);
  pinMode(M1,OUTPUT);

  /* D1-D4 All Off... */
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  

  /* IRremote Enable... */
  irrecv.enableIRIn();

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  decode_results results;
  int remote_data;
  
  if(irrecv.decode(&results)){          //もしデータ受信されたら...
    remote_data = readData(&results);   //  データを抽出する
    Serial.print("IRremote data : 0x");
    Serial.println(remote_data,HEX);
    if(remote_data==0x00){              //  もし「電源」だったら...
      digitalWrite(D1,LOW);             //    全LED消灯
      digitalWrite(D2,LOW);
      digitalWrite(D3,LOW);
      
      motorSTOP();                      //    ファン停止
      
    }

    else if(remote_data==0x08){         //  もし「１」だったら...
      digitalWrite(D1,HIGH);            //    D1点灯
      digitalWrite(D2,LOW);             //    D2消灯
      digitalWrite(D3,LOW);             //    D3消灯

      motorCW(120);                     //    弱風
    }

    else if(remote_data==0x88){         //  もし「２」だったら...
      digitalWrite(D1,LOW);             //    D1消灯
      digitalWrite(D2,HIGH);            //    D2点灯
      digitalWrite(D3,LOW);             //    D3消灯

      motorCW(180);                     //    中風
    }

    else if(remote_data==0x48){         //  もし「３」だったら...
      digitalWrite(D1,LOW);             //    D1消灯
      digitalWrite(D2,LOW);             //    D2消灯
      digitalWrite(D3,HIGH);            //    D3点灯
      
      motorCW(255);                     //    強風
    }


    else {                              //  もしその他だったら...
      // No operation...                //    なにもしない
    }

    irrecv.resume();                    //  データバッファクリア、受信再開
    delay(100);
  }

  
}

//--------------------
// リモコンデータ抽出
//--------------------
int readData(decode_results *results)
{
  return (int)((results->value >> 8)&0x000000ff);
  
}



//--------------------
// ＤＣモータ回転
// @ 回転スピード(0-255)
//--------------------
void motorCW(int sp)
{
  analogWrite(E1, sp);
  digitalWrite(M1, LOW);
}

//--------------------
// ＤＣモータ停止
//--------------------
void motorSTOP(void)
{
  analogWrite(E1,0);
}
