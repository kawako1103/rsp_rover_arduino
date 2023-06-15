#include <Wire.h>
#include "BMX_READ.h"
#include "GPS.h"
#include <SD.h>
#include <SPI.h>
#include <String.h>
#include "motor.h"


#define landing_threshold 9.0　 //加速度の絶対値
#define heght_threshold   2000  //高さの絶対値
#define land_heght 20 //高さの絶対値//

//SPI for SD　(SDに関しては、Arduino標準ライブラリがあった)
#define SS      10
#define MOSI    11
#define MISO    12
#define SCK     13

void setup(){
    // Wire(Arduino-I2C)の初期化
  Wire.begin();
  // デバッグ用シリアル通信は9600bps
  Serial.begin(9600);
  //BMX055 初期化
  BMX055_Init();
  delay(300);
//  Serial.begin(9600);
//  Serial.println("Start");

    //SDカード関係
    if(!SD.begin(SS)){
    Serial.println("Card Mount Failed");
    return;
  }

  //落下検知フェーズ//3段階
   while(true){

    //SDカード書き込み
    String data_1 = String(get_acclx()) + "," + String(get_accly()) + "," + String(get_acclz()) + "\n";//文字列を
    file = SD.open("Accl.csv", FILE_WRITE);
    if(file){
      file.println(data_1);
    }
    file.close();


    String data_2 = String(get_long()) + "," + String(get_lati()) + "," + String(get_alt()) + "\n";//文字列を
    file = SD.open("GPS.csv", FILE_WRITE);
    if(file){
      file.println(data_2);
    }
    file.close();

    //SDカード書き込み

    
   float A = get_accl_abs();
   float B = get_alt();
   if ((A > landing_threshold) && (get_alt() < heght_threshold)){
    delay(10);
    continue;
   }else { 
    break;
   }
   }//landing_thresholdが関与する条件は振動
   //heght_thresholdが関与する条件は高度　振動をしながら高度2000を超えたらbreakする

   //これで上空に行ったことを検知できる。
   
   //次は着地 条件はものすごい加速度を検知し、かつ標高が10 m以下になったこと
   while(true){
   float A = get_accl_abs();
   float B = get_alt();
   if ((A > landing_threshold) && (B< land_heght)){
    delay(10);
    continue;
   }else { 
    break;
   }
   }

   //上のフェーズを超えて　次に加速度が安定したことを検知する　これで着地判定

   while(true){

       //SDカード書き込み
    String data_1 = String(get_acclx()) + "," + String(get_accly()) + "," + String(get_acclz()) + "\n";//文字列を
    file = SD.open("Accl.csv", FILE_WRITE);
    if(file){
      file.println(data_1);
    }
    file.close();


    String data_2 = String(get_long()) + "," + String(get_lati()) + "," + String(get_alt()) + "\n";//文字列を
    file = SD.open("GPS.csv", FILE_WRITE);
    if(file){
      file.println(data_2);
    }
    file.close();

    //SDカード書き込み
   
   float A = get_accl_abs();
   float B = get_alt();
   if (((A > 8) && (A < 10)) && (B< land_heght)){
    delay(10);
    continue;
   }else { 
    break;
   }
   }

  //サーボ
  servo_setup();//servo_return()が逆
  servo_move();
  
  //キャリブ　値渡しの実装要
    // put your setup code here, to run once:
//  motor_setup()；
//  DC_Manipulator("LEFT", 1000, 45, 150);//
  calibration();//値渡し実装必要
  
    
  
}

void loop(){//キャリブ、走行決定、走行をここに入れるべきか　終了判定も忘れずに(距離が1 m以内とか)

  //とりあえずシリアルモニタで確認できるコード
  Serial.println(get_accl_abs(),6);
//  Serial.println(get_magx(),6);
//  Serial.println(get_magy(),6);
  Serial.print("LAT="); Serial.println(get_long(), 6);
  Serial.print("LONG="); Serial.println(get_lati(), 6);
  Serial.print("Alti="); Serial.println(get_alt(), 6);
//  delay(100);
  File file;

  //   /*
   //* 加速度、GPSの読み取り．SDにも格納する    */
//    float pos_x = get_long();
//    float pos_y = get_lati();
//    float pos_z = get_alt();


    String data_1 = String(get_acclx()) + "," + String(get_accly()) + "," + String(get_acclz()) + "\n";//文字列を
    file = SD.open("Accl.csv", FILE_WRITE);
    if(file){
      file.println(data_1);
    }
    file.close();


    String data_2 = String(get_long()) + "," + String(get_lati()) + "," + String(get_alt()) + "\n";//文字列を
    file = SD.open("GPS.csv", FILE_WRITE);
    if(file){
      file.println(data_2);
    }
    file.close();
    
}


//
//#include <Wire.h>
//#include <SoftwareSerial.h>
//#include <SPI.h>
////#include <Serial.h>//なんか元々入ってそうよね
//#include <Servo.h>
//#include <SD.h>
//#include <String.h>
//
//#include "BMX_READ.h"
//
//
//#define landing_threshold 9.0
//#define heght_threshold   2000
//

//
///******************************************************************************
// * PIN アサインは確認してください
// ******************************************************************************/
//// Serial for GPS
//#define RXD_PIN 2
//#define TXD_PIN 3
//
////SPI for SD
//#define SS      10
//#define MOSI    11
//#define MISO    12
//#define SCK     13
//
////I2C for DC
//#define EN_1    5
//#define EN_2    6
//#define PH_1    7
//#define PH_2    8
//
//#define SRV_PIN 9
//
//// 目標位置　適宜変更すること
#define TARGET_LATITUDE     100
#define TARGET_LONGITUDE    200
//

//

///*
// * 参照渡しを使って値を渡す．知らない人はこれを見てください
// * https://qiita.com/agate-pris/items/05948b7d33f3e88b8967
// */
//
//
//void DC_Flont(int millimeters);
//void DC_Richt(int degree);
//void DC_Left(int degree);
//void DC_Back(int millimeters);
//
//void SD_Write(float gps_ata);
//void CompassCalibration(float &cal_x, float &cal_y, float &cal_z);
//
//
//
//
///*
// * 命名規則
// * 関数名：キャメル
// * 変数名：スネークケース
// * メンバ変数　A_
// * プライベート変数 B_
// * 
// */
//int calib_dir[3];
//int calib_deg;
//int cal_x, cal_y, cal_z;
//
//
//BMX_Read BMX();
// 
//void setup() {
//  // put your setup code here, to run once:
//  
//  Serial.begin(9600);//シリアル速度は状況に応じて変えてください
//  //GPSSerial.begin(9600);//シリアル速度は状況に応じて変えてください
//  
//  if(!SD.begin(SS)){
//    Serial.println("Card Mount Failed");
//    return;
//  }
//  
//  
//  
//  /*
//   * 着陸判定
//   * GPSや加速度を読み取って判定
//   * GPS参考サイト
//   * https://www.petitmonte.com/robot/howto_gysfdmaxb.html
//   * ここからスタート
//   */
//   while(true){
//   float A = get_accl_abs();
//   //float A = ReadAccelABS();
//   ReadGPS(float pos_x, float pos_y, float pos_z);
//   if (A > landing_threshold && GPS < heght_threshold){
//    delay(10);
//    continue;
//   }else { 
//    break;
//   }
//   }
//   /*
//    * ちゃくりくはんていおわり　
//    */
//
//   Servo.write(180);
//   DC_flont(5000);
//
//   delay(1000);
//   
//   /*
//    * 機体をを360度回転させて，BMXを一周させる．このときのゼロ点のずれを確認する
//    */
//   CompassCalibration(cal_x,cal_y, cal_z);
//   ReadMagnet(int mx, int my, int mz);
//
//   calib_dir[0] = mx - cal_x;
//   calib_dir[1] = my- cal_y;
//   calib_dir[2] = mz- cal_z;
//
//   calib_deg = atan2(calib_dir[1]/calib_dir[0]); // 現在姿勢に対する，基準方向
//   
//
//}
//int count_calib = 0;
//
//void loop() {
//  // put your main code here, to run repeatedly:
//
//  /*
//   * 現在位置の測定
//   */
//   
//   ReadMagnet(float mx, float my, float mz);
//   float direc_now = atan2((my - cal_y)/(mx - cal_x));
//   float correc_angle = calib_deg - direc_now;
//
//   ReadGPS(float pos_x, float pos_y, float pos_z);
//   float dir_x = TARGET_LATITUDE - pos_x;
//   float dir_y = TARGET_LONGITUDE - pos_y;
//   float dir_angle = atan2(dir_y / dir_x);
//
//   float rotate_angle = correc_angle - (90 - dir_angle)// dir_angle, correc_angleを使って，回転角度を特定する
//
//   bool correc_flag = (abs(correc_angle>= 10)? true : false);     
//   //誤差が前後10度以内なら，補正しない
//   //三項演算子っていうifの書式です
//  /*
//   * DCの制御
//   * 旋回および直進を実行する
//   */
//   
//    if (correc_flag){
//     if(correction_angle >= 0){
//      DC_Left(int(rotate_angle));
//     }else{
//      DC_Right(int(-rotate_angle));
//     }
//    }
//
//   DC_Flont(10000); // 10 m　走る
//
//   /*
//    * GPSの読み取り．SDにも格納する
//    */
//    
//    
//    String data = String(pos_x) + "," + String(pos_y) + "," + String(pos_z) + "\n";
//    file = SD.open("GPS.csv", FILE_WRITE);
//    if(file){
//      file.write(data)
//    }
//    file.close();
//
//
//    /*
//     *終了処理は後で書いてください
//     *
//     */
//
//
//}
//
//
//
//
//float ReadAccelABS(){
//  BMX.BMX055_Accl();
//  return sqrt(BMX.xAccl * BMX.xAccl + BMX.yAccl * BMX.yAccl + BMX.zAccl * BMX.zAccl);
//}
//
//
//void ReadMagnet(float& mx, float& my, float& mz){
//  BMX.BMX055_Mag();
//  mx = BMX.xMag;
//  my = BMX.yMag;
//  mz = BMX.zMag;
//
//}
//void ReadGPS(float& pos_x, float& pos_y, float& pos_z){
//  String line = mySerial.readStringUntil('\n');
//  //下の関数を使って，位置と海抜読み込んでください
//  pos_x = //緯度;
//  pos_y = //経度;
//  pos_z = //海抜;
//  
//
//}
//
//// NMEAの緯度経度を「度分秒」(DMS)の文字列に変換する
//String _NMEA2DMS(float val) {
//  int d = val / 100;
//  int m = ((val / 100.0) - d) * 100.0;
//  float s = ((((val / 100.0) - d) * 100.0) - m) * 60;
//  return String(d) + "度" + String(m) + "分" + String(s, 1) + "秒";
//}
// 
//// (未使用)NMEAの緯度経度を「度分」(DM)の文字列に変換する
//String _NMEA2DM(float val) {
//  int d = val / 100;
//  float m = ((val / 100.0) - d) * 100.0;
//  return String(d) + "度" + String(m, 4) + "分";
//}
// 
//// NMEAの緯度経度を「度」(DD)の文字列に変換する
//String _NMEA2DD(float val) {
//  int d = val / 100;
//  int m = (((val / 100.0) - d) * 100.0) / 60;
//  float s = (((((val / 100.0) - d) * 100.0) - m) * 60) / (60 * 60);
//  return String(d + m + s, 6);
//}
//
//
//void DC_Flont(int millimeters){
//  
//}
//void DC_Richt(int degree){
//
//}
//void DC_Left(int degree){
//
//}
//void DC_Back(int millimeters){
//
//}
//
//void SD_Write(float gps_ata){
//
//}
//void CompassCalibration(float &cal_x, float &cal_y, float &cal_z){
//
//}
