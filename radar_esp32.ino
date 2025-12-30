// ====== RADAR COM ESP32 + ULTRASSÔNICO ======
// Autor: Ronaldo
// Sensor: HC-SR04
// Servo: SG90

#include <Arduino.h>

// ==== PINOS ====
// Ajuste se necessário
#define TRIG 5
#define ECHO 18
#define SERVO 4

// ==== CONFIGURAÇÃO DO SERVO ESP32 (PWM) ====
int canalPWM = 2;
int freq = 50;
int resolucao = 10; 

long duracao;
int distancia;

// Converte angulo em pulso PWM (500us - 2500us)
int anguloParaPWM(int angulo) {
  return map(angulo, 0, 180, 26, 128);
}

// Medir distância
int lerDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duracao = pulseIn(ECHO, HIGH, 30000);
  
  int d = duracao * 0.034 / 2;
  if (d == 0 || d > 400) return -1;

  return d;
}

void moverServo(int angulo) {
  int valorPWM = anguloParaPWM(angulo);
  ledcWrite(canalPWM, valorPWM);
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  ledcSetup(canalPWM, freq, resolucao);
  ledcAttachPin(SERVO, canalPWM);

  Serial.println("Radar iniciado...");
}

void loop() {

  // Varredura para a direita
  for (int angulo = 0; angulo <= 180; angulo++) {
    moverServo(angulo);
    distancia = lerDistancia();
    
    Serial.print("Angulo: ");
    Serial.print(angulo);
    Serial.print("  | Distancia: ");
    
    if (distancia == -1)
      Serial.println("Sem leitura");
    else {
      Serial.print(distancia);
      Serial.println(" cm");
    }
    
    delay(40);
  }

  // Varredura para a esquerda
  for (int angulo = 180; angulo >= 0; angulo--) {
    moverServo(angulo);
    distancia = lerDistancia();
    
    Serial.print("Angulo: ");
    Serial.print(angulo);
    Serial.print("  | Distancia: ");
    
    if (distancia == -1)
      Serial.println("Sem leitura");
    else {
      Serial.print(distancia);
      Serial.println(" cm");
    }

    delay(40);
  }
}
