constexpr float points[4][2] = {{0, 57.5}, {43, 57.5}, {0, 0}, {43, 0}};


// ZMPを計算する関数
void calculateZMP(float forces[], int num_points, float &zmp_x, float &zmp_y) {

  // 合力と各点の重み付き座標の合計を初期化
  float total_force = 0;
  float weighted_sum_x = 0;
  float weighted_sum_y = 0;
  
  // 各点の座標と力を使って合力と重み付き座標の合計を計算
  for (int i = 0; i < num_points; i++) {
    total_force += forces[i];
    weighted_sum_x += points[i][0] * forces[i];
    weighted_sum_y += points[i][1] * forces[i];
  }
  
  // ZMPの座標を計算
  zmp_x = weighted_sum_x / total_force;
  zmp_y = weighted_sum_y / total_force;
}

void setup() {
  Serial.begin(9600);

  // 座標点とそれぞれの点にかかる力のリストを定義
  float forces[] = {50, 50, 50, 50};
  
  // ZMPを計算
  float zmp_x, zmp_y;
  calculateZMP(forces, sizeof(points) / sizeof(points[0]), zmp_x, zmp_y);
  
  // ZMPの座標をシリアルモニタに出力
  Serial.print("ZMPのx座標: ");
  Serial.println(zmp_x);
  Serial.print("ZMPのy座標: ");
  Serial.println(zmp_y);

}

void loop() {
  
}
