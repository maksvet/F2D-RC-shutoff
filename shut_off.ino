   // PIM  July 2013
// 2.4 shutoff rev 0.00
// drive the input to an LP4 module to use in F2D shutoff on demand application.
//  Hold down button during binding.
//  press button to activate shut off
// fail safe will shut off
// If the servo motion is backwards then jumper from 8 to 9 and rebind
//  RF module connects to tx0
//
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int reverse1 = 9;
const int reverse2 = 8;

int buttonState = 0;         // variable for reading the pushbutton status
int bindmode = 0;  //enter bind mode at start fo 
int rev = 0;    //reverse servo if 8 and 9 jumpered
void setup()
{
Serial.begin(125000);
pinMode(buttonPin, INPUT_PULLUP); 
pinMode(reverse1,INPUT_PULLUP);
pinMode(reverse2,OUTPUT);
pinMode(ledPin, OUTPUT); 
}
unsigned long time,bind_time;
char data[14]={0x00,0x00,0x00,0xaa,0x05,0xff,0x09,0xff,0x0d,0xff,0x13,0x54,0x14,0xaa};


void loop()

{
  digitalWrite(reverse2,LOW);
  if (digitalRead(reverse1)==LOW) rev=1 ;else rev = 0;
  
time = millis()+20;
//bind_time = time+5000;
if (digitalRead(buttonPin)==LOW) bindmode=1;
else bindmode=0;
while(1)  // do forever
  {
   if (digitalRead(reverse1)==LOW) rev=1 ;else rev = 0;
   if (bindmode) 
   {
     data[0] = 0x80;
     digitalWrite(ledPin, HIGH);  
   }
   else 
   {
     data[0] = 0x00;
         digitalWrite(ledPin, LOW); 
   } 
//   if (bind_time<time) bindmode = 0;
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    //low throttle    
    if (rev==0) {data[2] = 0x01;data[3]=0x00;}
   else {data[2] = 0x03;data[3]=0xff;}
  } 
  else {
    // high throttle
    if (rev==0) {data[2] = 0x03;data[3]=0xff;}
   else {data[2] = 0x01;data[3]=0x00;} 
  }
    
   
  if (time < millis()) //ready to output the data?
    {
    time = millis()+20;
    for (int i=0;i<14;i++)Serial.write(data[i]);
    }
  }
}
