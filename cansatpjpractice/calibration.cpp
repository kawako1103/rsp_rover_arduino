/*
* 機体をを360度回転させて，BMXを一周させる．このときのゼロ点のずれを確認する
*/

#include "motor.h"
#include "BMX_READ.h"

float magx[16];
float magy[16];

void calibration(){
  
  int i = 0;
  
  while(0<=i && i<16){
    magx[i] = get_magx();
    magy[i] = get_magy();

    motor_setup()；
    DC_Manipulator("LEFT", 1000, 45, 150);//45°回転させて
    i = i + 1;//インクルメンタル
  }

  float sum_magx；
  float sum_magy；
  while(0<=i && i<16){
    sum_magx = magx[i];
    sum_magy = magy[i];
  }
  
  
  cal_x = sum_magx/16;
  cal_y = sum_magy/16;


   calib_dir[0] = mx- cal_x;
   calib_dir[1] = my- cal_y;
// calib_dir[2] = mz- cal_z;

   calib_deg = atan2(calib_dir[1]/calib_dir[0]); // 現在姿勢に対する，基準方向
//   
}
