/*
 *    Q0421
 *    AUTOR:   BrincandoComIdeias
 *    LINK:    https://www.youtube.com/brincandocomideias ; https://cursodearduino.net/
 *    COMPRE:  https://www.arducore.com.br/
 *    SKETCH:  Arduino sensor de obstáculo ultrassonico
 *    DATA:    21/01/2019
*/
#include <HCSR04.h>

// DEFINIÇÕES DE PINOS
#define pinTrig 4
#define pinEcho 5
#define motor1 9

// DEFINIÇÃO DE PARAMETROS
#define distObstaculo 30  // DISTANCIA EM CM PARA CONSIDERAR QUE HÁ OBSTÁCULO A FRENTE
#define tempoLeitura  50 // INTERVALO MINIMO ENTRE CADA LEITURA
#define medidaDist  30  // QUANTIDADE DE MEDIDAS QUE SERÃO ARMAZENADAS

#define DEBUG // DEIXAR DESCOMENTADO PARA RECEBER MAIS INFORMAÇÕES NO MONITOR SERIAL

// DECLARAÇÃO DE VARIÁVEIS
boolean       obstaculo = false;
boolean       parar = false;
boolean       novaLeitura;
boolean       mediaSensor[medidaDist]; // ARRAY PARA ARMAZENAR OS ULTIMOS VALORES E CONTROLAR DECISÃO
byte          posicao;
byte          contaObstaculo = 0;
unsigned long controleLeitura; // VARIÁVEL PARA CONTROLAR TEMPO ENTRE AS LEITURAS

// INSTANCIANDO OBJETOS
HCSR04 sensorHCSR04(pinTrig,pinEcho);


void setup() {
  #ifdef DEBUG 
    Serial.begin(9600);
    Serial.println(F("| ============================================= |"));
    Serial.println(F("| Arduino com sensor de obstáculos Ultrassonico |"));
    Serial.println(F("| ============================================= |"));
  #endif
  
  pinMode(motor1 , OUTPUT);
  
  for( byte i = 0 ; i<medidaDist ; i++ ){
    mediaSensor[i]=0;
    
    #ifdef DEBUG
      Serial.print("Setando posição ");Serial.print(i);Serial.println(" do array como FALSE");
    #endif
  }

  posicao = 0;
  
  #ifdef DEBUG
    Serial.println("Fim do Setup");
  #endif
  delay(2000);
}

void loop() {

  novaLeitura = false;
  
  if (millis() - controleLeitura > tempoLeitura){
    if (sensorHCSR04.dist() <= distObstaculo){
      obstaculo = true;
      novaLeitura = true;
    } else { 
      obstaculo = false;
      novaLeitura = true;
    }
    controleLeitura = millis();
  }

  if (novaLeitura == true){
    
    mediaSensor[posicao] = obstaculo;
    posicao = posicao + 1;
  
    for (byte i = 0 ; i < medidaDist ; i++){
      if (mediaSensor[i] == 1) contaObstaculo++;
    }
  
    if ( contaObstaculo >= ((medidaDist/2) +1)){
      if(parar == false){
        parar = true;
        Serial.println("PARA!!!");
        digitalWrite(motor1,HIGH);
      }
    } else {
      if(parar == true){
        parar = false;
        Serial.println("Caminho Livre");
        digitalWrite(motor1,LOW);
      }
    }
  
    contaObstaculo = 0;
    if ( posicao > medidaDist ){
      posicao = 0;
    }
  }
 
}