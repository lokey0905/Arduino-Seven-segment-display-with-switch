/*
*ex5-4  用switch，分時多工，有3個按鈕，
*按PB1，加1。按PB2，減1，按PB3，歸零。
*/
#define interval2 50
unsigned long timer2=0;  //使用定義時間
const char segPin[]={6,7,8,9,10,11,12,13};  //定義pin6~13
const char segCom[]={4,5};  //從左到右
const char segData[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xd8,0x80,0x98,0xff};  //解碼資料
unsigned int D[]={0,0}; //定義資料(十,個)
unsigned int x,y,z,t1,t2=0;  //儲存資料
bool pressed=0,new_key1=0,old_key1=0,new_key2=0,old_key2=0,new_key3=0,old_key3=0;  //儲存按鈕的資料



void dispseg(){   //正確的方式為  全滅-->送資料-->點亮(循環)
 
    for (int j=0;j<2;++j) digitalWrite(segCom[j],LOW);     //全滅
    for (int k=0;k<8;++k){           //這三行是送資料
        x=bitRead(segData[D[z]],k);
        digitalWrite(segPin[k],x);
      } 
    digitalWrite(segCom[z],HIGH);        //點亮
  }

void timeint2(){    //計時中斷2，每50ms執行一次
  if (millis()>timer2+interval2){
    timer2=millis();
    readPB1();   //偵測按鍵副程式1
    readPB2();   //偵測按鍵副程式2
    readPB3();   //偵測按鍵副程式3
  }
}
 
void readPB1(){    //偵測按鍵副程式1
   new_key1=digitalRead(A0);   //取得A0按鈕狀態
   if (new_key1!=old_key1){    //比對按鈕狀態是否不同 
     if (new_key1==0){         //按鈕是否按下
          t2=t2+1;           //數值+1
          if (t2==100) t2=00;  //如果數值到9999就設定0
          Serial.print("t2="); //序列阜顯示數值
          Serial.println(t2);
      }
      old_key1=new_key1;   //儲存舊數值
    } 
    deData();   //呼叫副程式解碼
}

void readPB2(){   //偵測按鍵副程式2
   new_key2=digitalRead(A1);     //取得A1按鈕狀態
   if (new_key2!=old_key2){      //比對按鈕狀態是否不同 
     if (new_key2==0){           //按鈕是否按下
          t2=t2-1;               //數值-1
          if (t2==65535) t2=99; //如果數值到0就設定9999
          Serial.print("t2=");   //序列阜顯示數值
          Serial.println(t2);
      }
      old_key2=new_key2;   //儲存舊數值
    } 
    deData();   //呼叫副程式解碼
}

void readPB3(){   //偵測按鍵副程式3
   new_key3=digitalRead(A2);     //取得A2按鈕狀態
   if (new_key3!=old_key3){      //比對按鈕狀態是否不同 
     if (new_key3==0){           //按鈕是否按下
          t2=00;               //如果清除數值
          Serial.print("t2=");   //序列阜顯示數值
          Serial.println(t2);
      }
      old_key3=new_key3;   //儲存舊數值
    } 
    deData();   //呼叫副程式解碼
}

void deData(){  //把4位數拆解成千位、百位、十位、個位數
  D[0]=t2/10;  //10位數
  D[1]=t2%10;  //1位數
}

void setup(){
  Serial.begin(9600);
  for (int i=0;i<8;i++){  //設定7段顯示器全滅
      pinMode(segPin[i], OUTPUT);
      digitalWrite(segPin[i],HIGH);
  }
  for (int j=0;j<2;j++){  //設定7段顯示器全高態
      pinMode(segCom[j],OUTPUT);
      digitalWrite(segCom[j],HIGH);
  }
  pinMode(A0,INPUT_PULLUP);  //加1
  pinMode(A1,INPUT_PULLUP);  //減1
  pinMode(A2,INPUT_PULLUP);  //reset
}

void loop(){
  z=(z+1)%4;
  delayMicroseconds(900);
  //timeint1();  //計時中斷1，每1秒自動加1
  timeint2();    //計時中斷2，每50ms動作一次，讀取按鍵值
  switch(z){     //使用switch 若z是0,1,2,3則呼叫dispseg
    case 0:
      dispseg();  //顯示副程式
    break;
    case 1:
      dispseg();
    break;
    case 2:
      dispseg();
    break;
    case 3:
      dispseg();
    break;  
    }
}
