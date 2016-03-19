/*
 *  This program is written in Arduino IDE 1.6.7
 *  written by Shashank Singh ( Team Leader of Team CRITICAL)
 *  for TECHKRITI - 2016 ( ELECTROMANIA ) . 
 *  Team Members are Shashank Singh , Ausaf Jafri and Devesh Sethia .
 *  We are student of Motilal Nehru National Institute of Technology ( NIT ALLAHABAD ) , 2nd year pursuing Electronics and Communication Engineering .
 *  
 *  Program : - BREAK THE BRICKS GAME MADE FOR 24*16 LED Matrix
 *  
 *  
 *  
 *  
 *  ********************
 *  ****    *****   **** 
 *  **       ***      **   
 *  *         *        *                                  BReak The Bricks Game
 *  
 *  
 *  
 *           *  
 *   
 *       ****
 *  
 *  
 *  Game features :-
 *  1> Infinite Levels generation
 *  2> 1.5 minutes level
 *  3> All ( paddle , blocks and balls ) are randomized
 *  4> Supports extra spin
 *  5> Fixed maximum score for particular level
 *  6> Stores higest score and username in EEPROM
 *  7> Supports breaking bricks tone
 *  8> No extra LCD required all data is shown on big LED display such as Level name , score , highest score , username ,etc. 
 *  9> Interactive Game
 *  10> Name can be enter using 360 degree 1k potentiometer joystick control (All 26 alphabets can be used using POTENTIOMETER Joystick which we call as " Funstick " )
 *  11> Block size , ball and paddle speed changed according to LEVEL
 *  12> Ball speed even changes in a level
 *  13> Follow law of momentum
 *  
 */



#include <EEPROM.h>

#define max_col 16  //maximum x coordinate here 16
#define max_row 24   //maximum y coordinate here 24
#define rowDataPin 8//Pin no. for row data
#define coloumnDataPin 5//Pin no. for coloumn data 
#define rowLatchPin 9//latch Pin no. for row
#define coloumnLatchPin 6//latch Pin no. for coloumn
#define rowClockPin 10//clock pin no. for row
#define coloumnClockPin 7//coloumn clock pin

/*
 * Joystick controls
 */

#define leftPin A0 //Left control
#define rightPin A1 //right control
#define upPin A2  //Up control
#define downPin A3 //Down control
#define controlPin A4 //Control


/*
 * Configuration for tone
 */
 
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define melodyPin 3
//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
//Underworld melody
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};
//Underwolrd tempo
int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};
 


int song = 0;
 
void sing(int s) {
  // iterate over the notes of the melody:
  song = s;
  if (song == 2) {
    
    int size = sizeof(underworld_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
 
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / underworld_tempo[thisNote];
 
      buzz(melodyPin, underworld_melody[thisNote], noteDuration);
 
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
 
      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);
 
    }
 
  } else {
 
    
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
 
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];
 
      buzz(melodyPin, melody[thisNote], noteDuration);
 
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
 
      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);
 
    }
  }
}
 
void buzz(int targetPin, long frequency, long length) {
  
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  
 
}
//TONE


int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest

int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo1 = 300;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(melodyPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(melodyPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}


/*
 * 8 x 5 Characters Setup 
 */

byte po[127];
void po_init()
{
po[32] = 0;
po[33] = 1;
po[34] = 2;
po[35] = 3;
po[36] = 4;
po[37] = 5;
po[38] = 6;
po[39] = 7;
po[40] = 8;
po[41] = 9;
po[42] = 10;
po[43] = 11;
po[44] = 12;
po[45] = 13;
po[46] = 14;
po[47] = 15;
po[48] = 16;
po[49] = 17;
po[50] = 18;
po[51] = 19;
po[52] = 20;
po[53] = 21;
po[54] = 22;
po[55] = 23;
po[56] = 24;
po[57] = 25;
po[58] = 26;
po[59] = 27;
po[60] = 28;
po[61] = 29;
po[62] = 30;
po[63] = 31;
po[64] = 32;
po[65] = 33;
po[66] = 34;
po[67] = 35;
po[68] = 36;
po[69] = 37;
po[70] = 38;
po[71] = 39;
po[72] = 40;
po[73] = 41;
po[74] = 42;
po[75] = 43;
po[76] = 44;
po[77] = 45;
po[78] = 46;
po[79] = 47;
po[80] = 48;
po[81] = 49;
po[82] = 50;
po[83] = 51;
po[84] = 52;
po[85] = 53;
po[86] = 54;
po[87] = 55;
po[88] = 56;
po[89] = 57;
po[90] = 58;
po[91] = 59;
po[92] = 60;
po[93] = 61;
po[94] = 62;
po[95] = 63;
po[96] = 64;
po[97] = 65;
po[98] = 66;
po[99] = 67;
po[100] = 68;
po[101] = 69;
po[102] = 70;
po[103] = 71;
po[104] = 72;
po[105] = 73;
po[106] = 74;
po[107] = 75;
po[108] = 76;
po[109] = 77;
po[110] = 78;
po[111] = 79;
po[112] = 80;
po[113] = 81;
po[114] = 82;
po[115] = 83;
po[116] = 84;
po[117] = 85;
po[118] = 86;
po[119] = 87;
po[120] = 88;
po[121] = 89;
po[122] = 90;
po[123] = 91;
po[124] = 92;
po[125] = 93;
po[126] = 94;
}

byte CH[][5] = {
B00000000, B00000000, B00000000, B00000000, B00000000, // space
B01011111, B00000000, B00000000, B00000000, B00000000, // !
B00000011, B00000000, B00000011, B00000000, B00000000, // "
B00010100, B00111110, B00010100, B00111110, B00010100, // #
B00100100, B01101010, B00101011, B00010010, B00000000, // $
B01100011, B00010011, B00001000, B01100100, B01100011, // %
B00110110, B01001001, B01010110, B00100000, B01010000, // &
B00000011, B00000000, B00000000, B00000000, B00000000, // '
B00011100, B00100010, B01000001, B00000000, B00000000, // (
B01000001, B00100010, B00011100, B00000000, B00000000, // )
B00101000, B00011000, B00001110, B00011000, B00101000, // *
B00001000, B00001000, B00111110, B00001000, B00001000, // +
B10110000, B01110000, B00000000, B00000000, B00000000, // ,
B00001000, B00001000, B00001000, B00001000, B00000000, // -
B01100000, B01100000, B00000000, B00000000, B00000000, // .
B01100000, B00011000, B00000110, B00000001, B00000000, // /
B00111110, B01000001, B01000001, B00111110, B00000000, // 0
B01000010, B01111111, B01000000, B00000000, B00000000, // 1
B01100010, B01010001, B01001001, B01000110, B00000000, // 2
B00100010, B01000001, B01001001, B00110110, B00000000, // 3
B00011000, B00010100, B00010010, B01111111, B00000000, // 4
B00100111, B01000101, B01000101, B00111001, B00000000, // 5
B00111110, B01001001, B01001001, B00110000, B00000000, // 6
B01100001, B00010001, B00001001, B00000111, B00000000, // 7
B00110110, B01001001, B01001001, B00110110, B00000000, // 8
B00000110, B01001001, B01001001, B00111110, B00000000, // 9
B01010000, B00000000, B00000000, B00000000, B00000000, // :
B10000000, B01010000, B00000000, B00000000, B00000000, // ;
B00010000, B00101000, B01000100, B00000000, B00000000, // <
B00010100, B00010100, B00010100, B00000000, B00000000, // =
B01000100, B00101000, B00010000, B00000000, B00000000, // >
B00000010, B01011001, B00001001, B00000110, B00000000, // ?
B00111110, B01001001, B01010101, B01011101, B00001110, // @
B01111110, B00010001, B00010001, B01111110, B00000000, // A
B01111111, B01001001, B01001001, B00110110, B00000000, // B
B00111110, B01000001, B01000001, B00100010, B00000000, // C
B01111111, B01000001, B01000001, B00111110, B00000000, // D
B01111111, B01001001, B01001001, B01000001, B00000000, // E
B01111111, B00001001, B00001001, B00000001, B00000000, // F
B00111110, B01000001, B01001001, B01111010, B00000000, // G
B01111111, B00001000, B00001000, B01111111, B00000000, // H
B01000001, B01111111, B01000001, B00000000, B00000000, // I
B00110000, B01000000, B01000001, B00111111, B00000000, // J
B01111111, B00001000, B00010100, B01100011, B00000000, // K
B01111111, B01000000, B01000000, B01000000, B00000000, // L
B01111111, B00000010, B00001100, B00000010, B01111111, // M
B01111111, B00000100, B00001000, B00010000, B01111111, // N
B00111110, B01000001, B01000001, B00111110, B00000000, // O
B01111111, B00001001, B00001001, B00000110, B00000000, // P
B00111110, B01000001, B01000001, B10111110, B00000000, // Q
B01111111, B00001001, B00001001, B01110110, B00000000, // R
B01000110, B01001001, B01001001, B00110010, B00000000, // S
B00000001, B00000001, B01111111, B00000001, B00000001, // T
B00111111, B01000000, B01000000, B00111111, B00000000, // U
B00001111, B00110000, B01000000, B00110000, B00001111, // V
B00111111, B01000000, B00111000, B01000000, B00111111, // W
B01100011, B00010100, B00001000, B00010100, B01100011, // X
B00000111, B00001000, B01110000, B00001000, B00000111, // Y
B01100001, B01010001, B01001001, B01000111, B00000000, // Z
B01111111, B01000001, B00000000, B00000000, B00000000, // [
B00000001, B00000110, B00011000, B01100000, B00000000, // \ backslash
B01000001, B01111111, B00000000, B00000000, B00000000, // ]
B00000010, B00000001, B00000010, B00000000, B00000000, // hat
B01000000, B01000000, B01000000, B01000000, B00000000, // _
B00000001, B00000010, B00000000, B00000000, B00000000, // `
B00100000, B01010100, B01010100, B01111000, B00000000, // a
B01111111, B01000100, B01000100, B00111000, B00000000, // b
B00111000, B01000100, B01000100, B00101000, B00000000, // c
B00111000, B01000100, B01000100, B01111111, B00000000, // d
B00111000, B01010100, B01010100, B00011000, B00000000, // e
B00000100, B01111110, B00000101, B00000000, B00000000, // f
B10011000, B10100100, B10100100, B01111000, B00000000, // g
B01111111, B00000100, B00000100, B01111000, B00000000, // h
B01000100, B01111101, B01000000, B00000000, B00000000, // i
B01000000, B10000000, B10000100, B01111101, B00000000, // j
B01111111, B00010000, B00101000, B01000100, B00000000, // k
B01000001, B01111111, B01000000, B00000000, B00000000, // l
B01111100, B00000100, B01111100, B00000100, B01111000, // m
B01111100, B00000100, B00000100, B01111000, B00000000, // n
B00111000, B01000100, B01000100, B00111000, B00000000, // o
B11111100, B00100100, B00100100, B00011000, B00000000, // p
B00011000, B00100100, B00100100, B11111100, B00000000, // q
B01111100, B00001000, B00000100, B00000100, B00000000, // r
B01001000, B01010100, B01010100, B00100100, B00000000, // s
B00000100, B00111111, B01000100, B00000000, B00000000, // t
B00111100, B01000000, B01000000, B01111100, B00000000, // u
B00011100, B00100000, B01000000, B00100000, B00011100, // v
B00111100, B01000000, B00111100, B01000000, B00111100, // w
B01000100, B00101000, B00010000, B00101000, B01000100, // x
B10011100, B10100000, B10100000, B01111100, B00000000, // y
B01100100, B01010100, B01001100, B00000000, B00000000, // z
B00001000, B00110110, B01000001, B00000000, B00000000, // {
B01111111, B00000000, B00000000, B00000000, B00000000, // |
B01000001, B00110110, B00001000, B00000000, B00000000, // }
B00001000, B00000100, B00001000, B00000100, B00000000, // ~
};




unsigned long int highScore = 0;    // highScore stores Highest Score
char usr[3];  // usr stores 3 characters username who scores hghest
unsigned long int tm=0;
void setup() {
  
  Serial.begin(9600);
  //connecting to server
  pinMode(rowDataPin , OUTPUT);
  pinMode(coloumnDataPin , OUTPUT);
  pinMode(rowLatchPin , OUTPUT);
  pinMode(coloumnLatchPin , OUTPUT);
  pinMode(rowClockPin , OUTPUT);
  pinMode(coloumnClockPin , OUTPUT);
  pinMode(melodyPin, OUTPUT);
  pinMode(leftPin,INPUT);
  pinMode(rightPin,INPUT);
  pinMode(upPin,INPUT);
  pinMode(downPin,INPUT);
  pinMode(controlPin,INPUT);
  pinMode(11 , OUTPUT);
  digitalWrite(11,HIGH);
  randomSeed(analogRead(A5));
  po_init();
  for(int k =0 ; k<32 ;k++)
  highScore = highScore*10 + EEPROM.read(k);
  for(int k = 32; k<35 ;k++)
  usr[k-32] = EEPROM.read(k);
}


//keys
int left = 0;
int right = 0;
int menu = 0;
int select = 0;
int up = 0;
int down = 0;
int control = 0;
int funcontrol = 1;
int lastcontrol = 0;


//method to read key value

/*
 * Call key() to update values of
 * left , right , select , down , menu , up and control
 * It updates key value to '0'  if key is open 
 * else '1' if pressed 
 */

void key()
{
  left = !digitalRead(leftPin);
  right = !digitalRead(rightPin);
  select = down = !digitalRead(downPin);
  menu = up =   !digitalRead(upPin);
  control = analogRead(controlPin);
} 


/* Grid stores display
 * if grid[i][j] == 1
 *    then led of ith row and jth coloumn is on
 * else low if it is '0'
 */
unsigned int grid[max_row][max_col]={0};    // All leds are initially OFF

//Function to update display
/*
 * Call show(grid) 
 * to update display .
 * Function Complexity O(max_col * max_col * max_row) here O(16*16*24)
 */
void show(unsigned int grid[max_row][max_col])
{
   int i,j,k;
   for(j=0;j<max_col;j++)  //LOOP for coloumn multeplexing
   {
      digitalWrite(rowLatchPin,LOW);  // row latch pin set LOW
      digitalWrite(coloumnLatchPin,LOW);  // coloumn latch pin set LOW
      
      for(k=0;k<max_col;k++)  //Coloumn is LED anode so one coloumn is set high and all others are set LOW  
      {
          digitalWrite(coloumnClockPin,LOW);  // coloumn clock pin set to LOW
          
          if(k!=(max_col-j-1))
          digitalWrite(coloumnDataPin,HIGH); //one HIGH coloumn
           
          else
          digitalWrite(coloumnDataPin,LOW); // LOW coloumns
           
          digitalWrite(coloumnClockPin,HIGH); //clock pin is set HIGH for positive edge trigering
      } 

      for(i=0;i<max_row;i++)   //LOOP for updating LEDS in HIGH coloumn
      {
        digitalWrite(rowClockPin,LOW);
        digitalWrite(rowDataPin,grid[i][j]);
        digitalWrite(rowClockPin,HIGH); 
      }

       digitalWrite(rowLatchPin,HIGH);  // row latch pin is set high ( i.e. ROW data updated )
       digitalWrite(coloumnLatchPin,HIGH);  //coloumn latch pin is set high (i.e. COLOUMN data updated ) 
   }

}


int angle = 0;  // angle store ball angles
int radians;

int _angle; // temp variable to store _angle
int _px; // _px stores ball x position
int _py; // _py stores ball y position
int maxBlock = 0; // maxBlock stores maximum no. of blocks for a level
int _count = 0; // _count stores block left
int _speed = 0; // _speed stores current ball speed
unsigned long int _countPoints = 0; // stores point
int lv = 2; // lv stores next level value

/*
 * Whole game is divided into various scenes
 * for example :- In first scene it display 'BREAK'
 */ 
int scene = 0, i = 0; // scene variable to store scene to display and i is temperory variable 

// Call alloff() to set all LEDS to LOW
void alloff(){
  for(int i =0 ; i< max_row; i++)
  for(int j = 0 ; j<max_col; j++)
  grid[i][j]=0;
}

// Call on to set LED[X,Y] to HIGH
void on(byte x , byte y){
  grid[x][y]=1;
}

//Call off to set LED[X,Y] to LOW
void off(byte x,byte y)
{
  grid[x][y] = 0;
}


//Fuction to create random Block for particular level
void createBlock(int x_min , int x_max ,int y_min , int y_max)
{
  for(int i = x_min ;  i<= x_max ; i++)
  {
    for(int j = y_min ; j<=y_max ;j++)
    {
      if(random(0,2))
      on(i,j);
    }
  }
}

//Function to create 4 LEDS paddle at random position
void paddle()
{
  int i,j ;
  for(i = random(0,11) ,j=0 ; j<4 ; j++)
  on(22,i+j);
  _py = random(i,i+4);
}

int minspeed = 5;    // minspeed to store minimum speed of ball for particular level
void retorted(int angle)  // Function to retort ball angle
{
  _angle = angle;
 
  if (_speed > minspeed)
    _speed++;
}

//Method to update ball position
void calcAngleIncrement()
{
  
  int px=_px,py=_py;
  if (_angle == 0)
  {
    px += 1;
    py = _py;
  }
  else if (_angle == 45)
  {
    px += 1;
    py += 1;
  }
  else if (_angle == 135)
  {
    px -= 1;
    py += 1;
  }
  else if (_angle == 180)
  {
    px -= 1;
    py = _py;
  }
  else if (_angle == 225)
  {
    px -= 1;
    py -= 1;
  }
  else if (_angle == 315)
  {
    px += 1;
    py -= 1;
  }
  //if(grid[px][py]==1)
  //checkcollision();
  off(_px,_py);
  _px=px,_py=py;
  on(_px,_py);
}

void gameover(){      //GameOver scene
  if(tm+4000 > millis())
  { 
  printCh(0,0,'G');
  printCh(0,5,'A');
  printCh(0,10,'M');
  printCh(8,0,'E');
  printCh(8,5,' ');
  printCh(8,10,'O');
  printCh(16,0,'v');
  printCh(16,5,'e');
  printCh(16,10,'r');
  }
  else 
  update_highest();
}



void checkcollision()     //Function to check ball collisions
{
  if(_px == 21 && grid[22][_py]==0)
   {tm = millis();
   scene = 4;
   return;
   }
   
  //wall conditions
  if (_px == 0)
  {
    if (_angle == 135 || _angle == 180 || _angle == 225)
    {
      if (_py == 0 || _py == max_col - 1)
      {
        if (_angle == 135)
          retorted(315);
        else if (_angle == 180)
          _angle=0;
        else if (_angle == 225)
          retorted(45);
      }
      else 
      {
        if(_angle == 135)
        _angle = 45;
        else if(_angle == 180)
        _angle = 0;
        else if(_angle == 225)
        _angle = 315;
      }
    }
  }
  else if(_py==0)
  {
    if(_angle==225)
    _angle = 135;
    else if(_angle==315)
    _angle = 45;
  }
  else if(_py==max_col-1)
  {
    if(_angle==135)
    _angle = 225;
    else if(_angle==45)
    _angle = 315;
  }
  else if(_px == 21)
  {
    if( _angle == 45 && _py == 15 && grid[_px+1][_py] == 1)
    retorted(225);
    if( _angle == 315 && _py == 0 && grid[_px+1][_py] == 1)
    retorted(135);
  }
  
 //paddle control
 if(_px == 21)
 {
  if(_angle == 0)
  _angle = 180;
  else if(_angle = 45 )
  _angle = 135 ; 
  else if(_angle = 315 )
  _angle =225;
  if((left == 1||((lastcontrol - control))/10>0) && _angle != 225 )
  { _angle +=45;
  }
  else if((right == 1||((lastcontrol - control))/10<0 ) && _angle != 135)
  _angle -=45;
 }
 
 //brick remove
 int speedx , speedy ;
 if(_angle == 0)
 speedx = 1, speedy = 0;
 else if(_angle == 45)
 speedx = 1 , speedy = 1;
 else if(_angle == 135)
 speedx = -1 , speedy =1;
 else if(_angle == 180 )
 speedx = -1 , speedy = 0;
 else if(_angle == 225 )
 speedx = -1 , speedy = -1;
 else if(_angle == 315)
 speedx = 1 , speedy = -1;
 if(grid[_px+speedx][_py] == 1)
  {
    off(_px+speedx,_py);
    tone(3,786);
    delay(20);
    _countPoints+=1;
    Serial.println(_countPoints);
    _count--;
    //angle = _angle;
    //_angle = (540-angle)%360;
    if(_angle==225)
    _angle = 315;
    else if( _angle == 180)
    _angle = 0;
    else if(_angle == 135)
    _angle = 45;
    else if(_angle == 45)
    _angle = 135;
    else if(_angle == 0)
    _angle =180;
    else if(_angle == 315)
    _angle = 225;
  }
  
  else if(grid[_px][_py+speedy] == 1&&speedy!=0)
  {
    off(_px,_py+speedy);
    tone(3,786);
    delay(20);
    _countPoints+=1;
    Serial.println(_countPoints);
    _count--;
   if( _angle==225)
   _angle == 135;
   else if(_angle == 315)
   _angle = 45;
   else if(_angle== 45)
   _angle = 315;
   else if(_angle == 135)
   _angle = 225;
  }
  else if(grid[_px+speedx][_py+speedy] == 1&&speedy!=0&&speedx!=0)
  {
    off(_px+speedx,_py+speedy);
    tone(3,786);
    delay(20);
    _countPoints+=1;
    if(_angle == 225)
    _angle = 45;
    else if(_angle == 315)
    _angle = 135;
    else if(_angle == 45)
    _angle = 225;
    else if(_angle == 135 )
    _angle = 315;
  }
  calcAngleIncrement();
}

char nm[3]={' ',' ',' '}; //nm to store 3 characters username
unsigned long int paddle_speed=200 , paddle_last_update=0 , ball_last_update=0 ;

/*
 *Call  printCh( first_corner_x_position , first_corner_y_position , Character ) to print ASCII characters
 */
void printCh(int x, int y ,byte ch)
{
  for(int i=0;i<5;i++)
  for(int j=0;j<8;j++)
  grid[x+j][y+i] = CH[int(po[ch])][i]&(1<<j); 
}

/*
 * This game will take user to infinite level depth
 * i.e. Game will never end
 */
 //Call level(lv) to initialize level values 
void level(int l)
{
  //Display OFF
  alloff();
  //Set paddle
  paddle();
  
  //Set random ball
  _px = random(18,21);
  _angle = random(0,2)==0?0:180;
  _speed = 5*(pow(0.381,l)+1);
  on(_px,_py);

  //Create random block
  int minx = l<15?random(0,14-l):0;
  int miny = l<12?random(0,11-l):0;
  int maxx = l<13?(minx+3+l):17 ;
  int maxy = l<11?(miny+3+l):15;
  createBlock(minx,maxx,miny,maxy);
  int reqblock = pow(3*double(l)*(1-pow(0.381,l)),2);
  maxBlock = _count = reqblock = reqblock<pow(3+l,2)?reqblock:pow(3+l,2);
  int block = 0;
  for(int m =minx; m<=maxx ;m++)
  for(int n = miny ; n<=maxy ; n++)
  if(grid[m][n]==1)
  block++;
  reqblock -= block ;
  for(int m =minx; m<=maxx ;m++){
  for(int n = miny ; n<=maxy ; n++)
  {
    if( reqblock>0 && grid[m][n]==0)
    on(m,n),reqblock--;
    
    else if( reqblock<0 && grid[m][n]==1)
    off(m,n),reqblock++;
  }
    if(reqblock==0)
    break;
  }

  //Set paddle speed
  paddle_speed = 200*pow(0.381,l) + 170;
}

//Function to update highest score
void update_highest()
{
  alloff();
  unsigned long int tmp = _countPoints ;
  if(_countPoints > highScore)
  {
    highScore = _countPoints ;
    usr[0] = nm[0];
    usr[1] = nm [1];
    usr[2] = nm[2];
    for(int k = 31; k>=0 ;k--)
    {
      EEPROM.write(k ,tmp%10);
      tmp = tmp/10;
    }
    EEPROM.write(32,nm[0]);
    EEPROM.write(33,nm[1]);
    EEPROM.write(34,nm[2]);
    if(tm + 8000 > millis())
     {  printCh(0,0,'H');
    printCh(0,5,'i');
    printCh(0,10,'g');
    printCh(8,0,'h');
    printCh(8,5,'e');
    printCh(8,10,'s');
    printCh(16,0,'t');
    }
    else if(tm + 12000 >millis())
    {
      alloff();
      tmp = 0 ;
      for(int k = 0; k<6 ;k++)
      {
        tmp = _countPoints%10;
        _countPoints/=10;
      }
      printCh(0,0,tmp%10);
      tmp/=10;
      printCh(0,5,tmp%10);
      tmp/=10;
      printCh(0,10,tmp%10);
      tmp/=10;
      printCh(8,0,tmp%10);
      tmp/=10;
      printCh(8,5,tmp%10);
      tmp/=10;
      printCh(8,10,tmp%10);
      tmp/=10;
    }
  
  }
  else
  {
    if(tm + 8000 > millis())
     {  printCh(0,0,'S');
    printCh(0,5,'c');
    printCh(0,10,'o');
    printCh(8,0,'r');
    printCh(8,5,'e');
    printCh(8,10,':');
    }
    else if(tm + 12000 >millis())
    {
      int tmp2 = _countPoints;
      tmp = 0 ;
      for(int k = 0; k<6 ;k++)
      {
        tmp =tmp*10 + tmp2%10;
        tmp2/=10;
      }
      printCh(8,0,tmp%10+48);
      tmp/=10;
      printCh(8,5,tmp%10+48);
      tmp/=10;
      printCh(8,10,tmp%10+48);
      tmp/=10;
      printCh(16,0,tmp%10+48);
      tmp/=10;
      printCh(16,5,tmp%10+48);
      tmp/=10;
      printCh(16,10,tmp%10+48);
    }
  }
  
}

unsigned long int stage_time = 60000 , stage_counter;

int p_flag;

void loop()
{
  key();
  if(select == 1&&scene==3)
  {
    funcontrol=!funcontrol;
  }
  //scene 0
  if(millis()<2000)
  {
    alloff();
    printCh(0,3,'B');
    printCh(0,8,'R');
    printCh(8,5,'E');
    printCh(16,3,'A');
    printCh(16,8,'K');
  }
  //scene 1
  else if(millis()<4000)
  {
    alloff();
    printCh(16,1,'T');
    printCh(16,6,'H');
    printCh(16,11,'E');
  }
  //scene 3
 else if(millis()<6000)
 {
    alloff();
    printCh(8,1,'B');
    printCh(8,6,'R');
    printCh(8,11,'I');
    printCh(16,1,'C');
    printCh(16,6,'K');
    printCh(16,11,'$');
    scene = 1;
    i=1;
 }
 //scene 4
 else if(scene==1)
 {
    if(tm==0)
    alloff();
    printCh(1,2,'H');
    printCh(1,7,'i');
    printCh(1,12,'!');
    printCh(16,1,'_');
    printCh(16,6,'_');
    printCh(16,11,'_');
    printCh(14,1,nm[0]);
    printCh(14,6,nm[1]);
    printCh(14,11,nm[2]);
    if(tm+200<millis())
    {
    printCh(14,i,'A'+control/32);
    if(left == 1 &&i > 1 )
    i-=5;
    else if(right == 1&& i < 11)
    i+=5;
    nm[i/5]='A'+control/32;
    tm = millis();
    if(select == 1)
    scene = 2;
    tm=millis();
    lv = 2;
     _countPoints = 0;
    }
 }
 //scene 5
 else if(scene == 2)
  {
    if(tm+5000 > millis())
    {
      alloff();
      printCh(0,0,'L');
      printCh(0,5,'E');
      printCh(0,10,'V');
      printCh(8,0,'E');
      printCh(8,5,'L');
      printCh(16,0,(lv-1)/100+48);
      printCh(16,5,((lv-1)/10)%10+48);
      printCh(16,10,((lv-1)%10)+48);
    }
      else{
     alloff() ;
     level(lv);
     p_flag = 1;
     scene = 3;
     stage_counter = millis();
    }
  }
//scene 6
 else if(scene == 3)
  {
  if(paddle_last_update+paddle_speed < millis())
    {
     if(funcontrol == 1)
     {
        for(int i=max_row-2;i<max_row;i++)
        for(int j=0;j< max_col ; j++)
        {
          if(i==22 && j >= control/65 && j < (control/65 +4))
          {
             on(i,j);
             if(p_flag == 1)
             {
              off(_px,_py);
              _py = j ;
              on(_px,_py);
              p_flag=0;
             }
          }
          else 
          off(i,j);
        }
     }
     else if(left == 1)
     {
      if(grid[22][0]!=1&&grid[23][0]!=1)
      {
        for(int i=max_row-2;i<max_row;i++)
        for(int j=0;j< max_col-1 ; j++)
        grid[i][j] = grid[i][j+1];
        grid[max_row-2][max_col-1]=grid[max_row-1][max_col-1]=0;
      }
     }
     else if(right == 1)
     { 
      if(grid[22][15]!=1&&grid[23][15]!=1)
      {
        for(int i=max_row-2;i<max_row;i++)
        for(int j=15;j>0; j--)
        grid[i][j] = grid[i][j-1];
        grid[max_row-2][0]=grid[max_row-1][0]=0;
      } 
     }
     paddle_last_update = millis();
    }
    //check collision and ball update
    if(ball_last_update + _speed*50 < millis())
    {
      checkcollision();
     ball_last_update = millis();
    }
    if( _count == 0 || stage_counter + stage_time < millis())
    {
      lv++;
      tm = millis();
      scene = 2;
    }
   }  
   else if(scene == 4)
   {
    if(tm + 16000 <millis())
    { 
       alloff();
       printCh(0,0,'A');
       printCh(0,5,'U');
       printCh(0,10,'S');
       printCh(8,0,'0');
       printCh(8,5,'0');
       printCh(8,10,'0');
       printCh(16,0,'1');
       printCh(16,5,'9');
       printCh(16,10,'4');
    }
    else if(tm + 12000 < millis())
    {
      
      alloff();
      printCh(0,0,'H');
      printCh(0,5,'I');
      printCh(0,10,'G');
      printCh(8,0,'H');
      printCh(8,5,'S');
      printCh(8,10,'C');
      printCh(16,0,'O');
      printCh(16,5,'R');
      printCh(16,10,'E');
    }
    else
    {
      alloff();
      gameover();
    }
    
   }
noTone(3);
show(grid);  //Update display
}

