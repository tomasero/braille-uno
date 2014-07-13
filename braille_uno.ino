#define CHAR_ARRAY_SIZE 72
#define PIN_COUNT 6
 
byte byteRead;
 
int braillePins[] = { 2, 3, 4, 5, 6, 7 };
int refreshSpeed = 2000;
 
String inData;
String caps = "000001";
String number = "001111";
String space = "111111";
 
// Array of braille binary representations mapped to their equivalent letter
String chars[] = {"a", "100000",
                  "b", "110000",
                  "c", "100100",
                  "d", "100110",
                  "e", "100010",
                  "f", "110100",
                  "g", "110110",
                  "h", "110010", 
                  "i", "010100",
                  "j", "010110",
                  "k", "101000",
                  "l", "111000",
                  "m", "101100",
                  "n", "101110",
                  "o", "101010",
                  "p", "111100",
                  "q", "111110",
                  "r", "111010",
                  "s", "011100",
                  "t", "011110",
                  "u", "101001",
                  "v", "111001",
                  "w", "010111",
                  "x", "101101",
                  "y", "101111",
                  "z", "101001",
                  ".", "010011",
                  ",", "010000",
                  "!", "011010",
                  ";", "011000",
                  ":", "100011",
                  "@", "000100",
                  "&amp;", "111101",
                  "+", "001101",
                  "-", "001001",
                  "=", "111111"
                };          
 
void setup() {
  Serial.begin(4800);
  for (int i=0; i < PIN_COUNT; i++){     
    pinMode(braillePins[i], OUTPUT); // Set all pins as an output     
    digitalWrite(braillePins[i], LOW);   
  }   
  delay(refreshSpeed); 
} 

void loop() {   // Implement variable speed using pentotiometer      // Here we listen for serial input to translate the words   // to braille representation   
  inData = "";   
  Serial.flush();
  int h = Serial.available()-1;
  if( h > 0 ){
    char temp;
    for (int i = 0; i < h; i++) {
      temp = (char)Serial.read();
      inData += temp;
    }
    if(inData.length() > 0) {
      convertStringToBraille(inData);
      sendBinaryToArduino(space);
      delay(refreshSpeed);
      offAll();
    }
  }
}

void offAll() {
  for (int i=0; i < PIN_COUNT; i++){     
    digitalWrite(braillePins[i], LOW);   
  }   
}
 
void convertStringToBraille( String text ) {
 
 int stringLength = text.length();

 for( int i=0; i < stringLength; i++ ) {        
   char currentChar = text.charAt(i);        // Handle special case for capital letters,    // numbers, spaces and then standard lower    // case letters    
   if(currentChar>='A' && currentChar<='Z'){     
     Serial.println("CAPS");
     sendBinaryToArduino(caps); 
     delay(refreshSpeed);
     String braille = getBrailleEquivalent(currentChar+32);   
     sendBinaryToArduino(braille);
   } else if(currentChar>='0' && currentChar<='9'){
     Serial.print("NUMB: ");
     Serial.println(currentChar);
     sendBinaryToArduino(number);
     delay(refreshSpeed);
     String braille = getBrailleEquivalent(isANumber(currentChar));
     sendBinaryToArduino(braille);
   } else if( currentChar == ' ' ){ 
     Serial.println("SPACE");
     sendBinaryToArduino(space);
   } else {
     Serial.println("CHAR");
     String braille = getBrailleEquivalent(currentChar);
     sendBinaryToArduino(braille);
   }
   delay(refreshSpeed);
   Serial.flush();
 }
 
}
 
char isANumber( char c ){
  // In braille numbers 0 - 1 are shown as letters a - j with a
  // # in front of the letter, so here we just return the 
  // equivalent letter
  if(c == '0'){
    return 'j';
  } else if(c == '1'){
    return 'a';
  } else if(c == '2'){
    return 'b';
  } else if(c == '3'){
    return 'c';
  } else if(c == '4'){
    return 'd';
  } else if(c == '5'){
    return 'e';
  } else if(c == '6'){
    return 'f';
  } else if(c == '7'){
    return 'g';
  } else if(c == '8'){
    return 'h';
  } else if(c == '9'){
    return 'i';
  }
}
 
void sendBinaryToArduino(String binary){
  // Loop through the binary braille representation
  // and set the pin status for each bit
  for( int i = 0; i < binary.length(); i++ ) {
   char currentChar = binary.charAt(i);
 
   if( currentChar == '1') {
     digitalWrite(braillePins[i], HIGH); 
   } else if ( currentChar == '0') {
     digitalWrite(braillePins[i], LOW); 
   }
 
  }
}
 
String getBrailleEquivalent( char c ) {
  Serial.println(String(c));
  // Loop through the array of characters and match the character
  // parameter to return the binary equivelant
  for (int i=0; i < CHAR_ARRAY_SIZE; i+=2) {
     if (String(c) == chars[i]) {
       return chars[i+1];
       break;
     }
  }
 
}
