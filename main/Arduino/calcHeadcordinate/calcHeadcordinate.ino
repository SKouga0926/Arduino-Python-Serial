void setup() {

  Serial.begin(9600);

}

float x;
float y;

float height = 43.0;
float width = 57.5;

void calculateHeadCoordinate(float pressure_[])
{

  float ratio1 = pressure_[1] / (pressure_[1] + pressure_[0]);
  float x1 = width * ratio1;

  float ratio2 = pressure_[3] / (pressure_[3] + pressure_[2]);
  float x2 = width * ratio2;

  float r2 = width - x1;
  float r4 = width - x2;
  float hypotenuse = sqrt(pow((r2 - r4), 2) + pow(height, 2));

  float r5 = hypotenuse * (pressure_[0] + pressure_[1]) / (pressure_[0] + pressure_[1] + pressure_[2] + pressure_[3]);

  // deg Serial.println(atan2(1.73, 1) * 180 / 3.14);
  // 0.5 Serial.println(cos(3.14/3));

  if (x1 > x2)
  {
    float theta_rad = atan2(height, r4 - r2);
    x = x2 + r5 * cos(theta_rad);
    y = r5 * sin(theta_rad);

    Serial.print("x : ");
    Serial.println(x);
    delay(100);
    Serial.print("y : ");
    Serial.println(y);
  }

  else
  {
    float theta_rad = atan2(height, r2 - r4);
    x = x2 - r5 * cos(theta_rad);
    y = r5 * sin(theta_rad);

    Serial.print("x : ");
    Serial.println(x);
    delay(100);
    Serial.print("y : ");
    Serial.println(y);
  }
}

void loop() {

  float pressure[4] = {0.1, 0.3, 0.2, 100}; // m1g, m2g, m3g, m4g
  
  calculateHeadCoordinate(pressure);

}
