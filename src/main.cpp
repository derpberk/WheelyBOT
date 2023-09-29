#include <Arduino.h>
#include "utils/PID.h"

static const uint8_t crc_table[] = {
 0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, 0x38, 0x3f, 0x36, 0x31,
 0x24, 0x23, 0x2a, 0x2d, 0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65,
 0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d, 0xe0, 0xe7, 0xee, 0xe9,
 0xfc, 0xfb, 0xf2, 0xf5, 0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
 0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85, 0xa8, 0xaf, 0xa6, 0xa1,
 0xb4, 0xb3, 0xba, 0xbd, 0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2,
 0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea, 0xb7, 0xb0, 0xb9, 0xbe,
 0xab, 0xac, 0xa5, 0xa2, 0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
 0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32, 0x1f, 0x18, 0x11, 0x16,
 0x03, 0x04, 0x0d, 0x0a, 0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42,
 0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a, 0x89, 0x8e, 0x87, 0x80,
 0x95, 0x92, 0x9b, 0x9c, 0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
 0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec, 0xc1, 0xc6, 0xcf, 0xc8,
 0xdd, 0xda, 0xd3, 0xd4, 0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c,
 0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44, 0x19, 0x1e, 0x17, 0x10,
 0x05, 0x02, 0x0b, 0x0c, 0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
 0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b, 0x76, 0x71, 0x78, 0x7f,
 0x6a, 0x6d, 0x64, 0x63, 0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b,
 0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13, 0xae, 0xa9, 0xa0, 0xa7,
 0xb2, 0xb5, 0xbc, 0xbb, 0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
 0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb, 0xe6, 0xe1, 0xe8, 0xef,
 0xfa, 0xfd, 0xf4, 0xf3
};

// Objetivo y la velocidad actual
float y_ref[4] = {0,0,0,0};
float y[4] = {0,0,0,0};

// Señal de control
float u[4] = {0,0,0,0};

// Tiempo de muestreo
unsigned long sample_time = 100; // en milisegundos

// Variables usadas para el calculo del Tm
unsigned long prev_time = 0;
unsigned long current_time = 0;
unsigned long time_change = 0;

// Variables para el calculo de la velocidad
volatile int enc_time[4],enc_end[4],enc_start[4];
volatile long enc_count[4] = {0,0,0,0};

// Variables para el seguimiento del sentido de giro
volatile char sent[4] = {0,0,0,0};

// Variables para la transmision de datos
const char STX = '\x24';
const char ETX = '\x3b';
char msg[15];
int vel,sen,ang,crc,check;
                                 //pwm, dir
PID PIDs[4]={PID(0.1, 0.01, 0.0005, 6, 7, 255, 0, sample_time), //mot 1 
             PID(0.1, 0.01, 0.0005, 8, 9, 255, 0, sample_time), //mot 2
             PID(0.1, 0.01, 0.0005, 4, 5, 255, 0, sample_time), //mot 3
             PID(0.1, 0.01, 0.0005, 2, 3, 255, 0, sample_time)  //mot 4
              };

void enc_isr1();
void enc_isr2();
void enc_isr3();
void enc_isr4();

uint8_t crc8(uint8_t *p, uint8_t len); //Es equivalente a byte

byte CRC8m(const byte *data, size_t dataLength); //En la jetson usamos crc8 basico con lookup table, este sin embargo este esta basado en las formulas distribuidas por la empresa Dallas/Maxim

void speed_data();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  // Set the pin mode

  pinMode(26, INPUT_PULLUP); //CLK_enc mot1 (a - verde) 
  pinMode(27, INPUT_PULLUP); //DT_enc (b - amarillo)

  pinMode(28, INPUT_PULLUP); //CLK_enc mot2
  pinMode(29, INPUT_PULLUP); //DT_enc

  pinMode(30, INPUT_PULLUP); //CLK_enc mot3
  pinMode(31, INPUT_PULLUP); //DT_enc

  pinMode(32, INPUT_PULLUP); //CLK_enc mot4
  pinMode(33, INPUT_PULLUP); //DT_enc

  attachInterrupt(digitalPinToInterrupt(26),enc_isr1,CHANGE); //Enc Mot1
  attachInterrupt(digitalPinToInterrupt(28),enc_isr2,CHANGE); //Enc Mot2
  attachInterrupt(digitalPinToInterrupt(30),enc_isr3,CHANGE); //Enc Mot3
  attachInterrupt(digitalPinToInterrupt(32),enc_isr4,CHANGE); //Enc Mot4
  
}

void loop() {

  if (Serial.available() > 0)
  {
    if (Serial.read() == STX)
    {
      char buffer[64]; // Búfer para almacenar los datos
      int bytesRead = Serial.readBytesUntil(';', buffer, sizeof(buffer)); // Leer hasta el punto y coma
      buffer[bytesRead] = '\0'; // Agregar un carácter nulo para que buffer sea una cadena válida

      if (bytesRead > 0) {
        char* token = strtok(buffer, " "); // Dividir la cadena en tokens usando el espacio como separador
        int valueIndex = 0;
        while (token != NULL) {
          if (valueIndex < 3) { // En la jetson se genera un crc8 de un mensaje del tipo: $XX XX X, luego se añade el ; antes de enviarlo. Extraemos ese mensaje para el calculo correcto del crc8
            strcat(msg, token); // Hay que tener en cuenta que del buffer solo extragimos XX XX X, ya que $ y ; son excluidos al ser las condiciones para guardar el mensaje
            strcat(msg, " ");   
          }
          if (valueIndex == 0) { // Cuando valueIndex sea 0, estamos en el valor de velocidad lineal
            vel = atoi(token); // Lo convertimos en entero
          }

          if (valueIndex == 1) { // Velocidad angular
            ang = atoi(token); 
          }

          if (valueIndex == 2) { // Valor sobrante, se deja como ejemplo para ver como se añaden mas parametros al mensaje
            sen = atoi(token); 
          }

          if (valueIndex == 3) { // Valor del CRC8 recibido
            crc = atoi(token);
            break; // Salir del bucle una vez que hayamos encontrado el valor de CRC
          }

          token = strtok(NULL, " "); // Obtener el siguiente token
          valueIndex++;
        }
    
      size_t msgLength = strlen(msg);

      // Como vimos antes tenemos que añadir $ para calcular el CRC8
      // Desplazamos los caracteres existentes hacia la derecha para hacer espacio al nuevo carácter
      for (int i = msgLength; i > 0; i--) {
        msg[i] = msg[i - 1];
      }

      msg[0] = '$';
      msg[msgLength] = '\0'; // Y añadimos el terminador de la cadena

      msgLength = strlen(msg);
      check = crc8((byte*)&msg,msgLength); // Calculamos el CRC8 (polinomial 0x07)
      /*
      Serial.print(vel);
      Serial.print(" ");
      Serial.print(ang);
      Serial.print(" ");
      Serial.print(sen);
      Serial.print(" ");
      Serial.print(crc);
      Serial.print(" ");
      Serial.print(msg);
      Serial.print(" ");
      Serial.print(check);
      Serial.println(" "); */

      memset(msg, '\0', sizeof(msg)); // Una vez hecho el calculo, borramos la cadena para estar listos para la siguiente

      if(check==crc){
        y_ref[0]=(2*vel-ang*24)/(2); // velangular=(2*vel-velA*L)/(2*R); vel lineal = velANG*R
        y_ref[1]=(2*vel+ang*24)/(2); 
        y_ref[2]=y_ref[0]; // Hacemos como si tuvieramos dos ruedas
        y_ref[3]=y_ref[1];
        }
      }
    }
  }

  // Calculamos la velocidad contando el tiempo que se tarda en dar una vuelta
  for(int i=0; i<4; i++){
    if(enc_count[i]>=200 || enc_count[i]<=-200){
      enc_count[i]=0;

      enc_end[i]=millis();
      enc_time[i]=enc_end[i]-enc_start[i];
      y[i]=(12*M_PI*1000/(30*enc_time[i])); //cm/s
      enc_start[i]=enc_end[i];
    
    }
  }
    
  // Obtenemos el tiempo actual
  current_time = millis();
  time_change = current_time - prev_time;

  // Bucle de control con un tiempo de muestreo
  if (time_change >= sample_time) {
    for(int i=0; i<4; i++){
      PIDs[i].update(y[i],y_ref[i],sent[i]);
      u[i] = PIDs[i].getU();
    }

    // Actualizamos los valores de tiempo
    prev_time = current_time;

    // Enviamos por puerto serie la telemetria
    speed_data();
  }

}

void enc_isr1(){
// Si DT es distinto a CLK eso quiere decir que en encoder esta girando en sentido antihorario (Depende de la conexion del encoder y la polaridad del motor)

  if (digitalRead(26) != digitalRead(27)) {
    enc_count[0] ++;
    sent[0]= '0'; // Adelante
   } else {
    enc_count[0] --;
    sent[0]= '1'; //Atras
   }

}
void enc_isr2(){
  if (digitalRead(28) != digitalRead(29)) {
    enc_count[1] ++;
    sent[1]= '0';
   } else {
    enc_count[1] --;
    sent[1]= '1';
   }

}
void enc_isr3(){
  if (digitalRead(30) != digitalRead(31)) {
    enc_count[2] ++;
    sent[2]= '0';
   } else {
    enc_count[2] --;
    sent[2]= '1';
   }

}
void enc_isr4(){
  if (digitalRead(32) != digitalRead(33)) {
    enc_count[3] ++;
    sent[3]= '1';
   } else {
    enc_count[3] --;
    sent[3]= '0';
   }

}

uint8_t crc8(uint8_t *p, uint8_t len)
{
    uint16_t i;
    uint16_t crc = 0x0;

    while (len--) {
            i = (crc ^ *p++) & 0xFF;
            crc = (crc_table[i] ^ (crc << 8)) & 0xFF;
    }

    return crc & 0xFF;
}

//CRC-8 - based on the CRC8 formulas by Dallas/Maxim
//code released under the therms of the GNU GPL 3.0 license
byte CRC8m(const byte *data, size_t dataLength) // Se acaba optando por el uso del CRC8 estandar con lookuptable, ya que es el metodo implementado en la jetson y supone un menor costo en computacion
{
  byte crc = 0x00;
  while (dataLength--)
  {
    byte extract = *data++;
    for (byte tempI = 8; tempI; tempI--)
  {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum)
    {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}
 
void speed_data(){
  Serial.print(time_change);
  Serial.print(" ");
  Serial.print(y_ref[0]);
  Serial.print(" ");
  Serial.print(y_ref[1]);
  Serial.print(" ");
  Serial.print(y_ref[2]);
  Serial.print(" ");
  Serial.print(y_ref[3]);
  Serial.print(" ");
  Serial.print(y[0]);
  Serial.print(" ");
  Serial.print(y[1]);
  Serial.print(" ");
  Serial.print(y[2]);
  Serial.print(" ");
  Serial.print(y[3]);
  Serial.print(" ");
  Serial.print(sent[0]);
  Serial.print(" ");
  Serial.print(sent[1]);
  Serial.print(" ");
  Serial.print(sent[2]);
  Serial.print(" ");
  Serial.print(sent[3]);
  Serial.print(" ");
  Serial.print(u[0]);
  Serial.print(" ");
  Serial.print(u[1]);
  Serial.print(" ");
  Serial.print(u[2]);
  Serial.print(" ");
  Serial.print(u[3]);
  Serial.println(" ");
}