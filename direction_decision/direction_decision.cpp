#include <GPS.h>
#include <BMX055.h>

//ターゲットの座標(経度がx方向、緯度がy方向ということに注意)絶対確認
//targetのx座標(経度)をa, y座標(緯度)をbとする
#define target_long　139.863630;//一旦葛飾キャンパスの体育館と実験棟の間のところ
#define target_lati 35.772412 ;

float a = target_long;
float b = target_lati;
float x2 = get_long();
float y2 = get_lat;()

float alpha = 0.0;
float beta = 0.0;

int T;//ローバーの傾きを分類する
int R;//進むべき方向の傾きを分類する

//地磁気センサからローバーの傾きを算出し、ローバの向いている方向を8通りに振り分ける
void rover_direction(mag_x, mag_y){
  alpha = atan2(mag_x/mag_y);//地磁気センサから座標においてどれほど傾いているか算出
   //#2ローバの向いている方向の角度について8通りに振り分ける
    if (0.0<=alpha<45.0){
      T=1;
      return T;
    }   
    else if (45.0<=alpha<90.0){
      T=2;
      return T;
    }
    else if (90.0<=alpha<135.0){
      T=3;
      return T;
    }
    else if (135.0<=alpha<=180.0){
      T=4;
      return T;
    }
    else if (-180.0<=alpha<-135.0){
      T=5;
      return T;
    }
    else if (-135.0<=alpha<-90.0){
      T=6;
      return T;
    }
    else if (-90.0<=alpha<-45.0){
      T=7;
      return T;
    }
    else if (-45.0<=alpha<0.0){
      T=8;
      return T;
    }
}

//ローバーの現在のGPS値とターゲットのGPS値との差から進むべき方向のベクトルの傾きを算出si,
//向かうべき方向を8通りにわける
void target_direction(x2, y2){//a, bは基本変わらないからdefine扱いでも良いかも
//  x2 = get_long;
//  y2 = get_lat;
  zx = a - x2;//向かうべきベクトルの傾き()
  zy = b - y2;
  beta = atan2(zy/zx);
  //#3ターゲットへのベクトルの角度を8通りに振り分ける
    if (0.0<=beta<45.0){
      R=1;
      return R;
    }   
    else if (45.0<=beta<90.0){
      R=2;
      return R;
    }
    else if (90.0<=beta<135.0){
      R=3;
      return R;
    }
    else if (135.0<=beta<=180.0){
      R=4;
      return R;
    }
    else if (-180.0<=beta<-135.0){
      R=5;
      return R;
    }
    else if (-135.0<=beta<-90.0){
      R=6;
      return R;
    }
    else if (-90.0<=beta<-45.0){
      R=7;
      return R;
    }
    else if (-45.0<=beta<0.0){
      R=8;
      return R;
    }
}

//直進か回転(回転なら+-3通りずつと+180°回転の7通り) duty比で進行しながら向きを変えられれば良いかな
void direction_dicision(T, R){//duty比を返すのどうだろう、Arduinoだからやるとしたらアドレス渡しとかやらないと
   //#4上で出てきた返り値をもとに行動決定
    if (R-T == 0){//傾きの差が45°以内なので直進
        return 255, 255;//直進
    }  
    else if (R-T == 1 or R-T == -7){//区間1　左回りしつつ前進したい
        return 200, 255;//
    }
    else if (R-T == 2 or R-T == -6){//傾きの差が90°以上135°以下
        return 100, 255;
    }
    else if (R-T == 3 or R-T == -5){//傾きの差が135°以上180°以下
        return 0, 255;
    }
    else if (R-T == 4 or R-T == -4){//傾きの差が180°以上225°以下
        return -255, 255;
    }
    else if (R-T == -1 or R-T == 7){//傾きの差が225°以上270°以下
        return 255, 200;
    }
    else if (R-T == -2 or R-T == 6){//傾きの差が315°以上360°以下
        return 255, 100;
    }
    else if (R-T == -3 or R-T == 5){
        return 255, 0;
    }
    else{
        return 255, 255;
    } 
}
