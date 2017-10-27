#include <DFPlayer_Mini_Mp3.h> // Бібліотека МП3хи

byte micPin = A1;
int rel_lampa = 24;
int rel_sunduk = 25;

int dotDuration = 200; //dot duration time
int listenTime = 20;//listen time in seconds
int threshold = 150; //чьототам порог. По ідеї - похибка того, наскільки точно стукаєш

boolean listenFlag = false;
boolean q_sunduk_done = false;

static const struct {const char letter, *code;} MorseMap[] =
{
	{ 'A', ".-" },
	{ 'B', "-..." },
	{ 'C', "-.-." },
	{ 'D', "-.." },
	{ 'E', "." },
	{ 'F', "..-." },
	{ 'G', "--." },
	{ 'H', "...." },
	{ 'I', ".." },
	{ 'J', ".---" },
	{ 'K', ".-.-" },
	{ 'L', ".-.." },
	{ 'M', "--" },
	{ 'N', "-." },
	{ 'O', "---" },
	{ 'P', ".--." },
	{ 'Q', "--.-" },
	{ 'R', ".-." },
	{ 'S', "..." },
	{ 'T', "-" },
	{ 'U', "..-" },
	{ 'V', "...-" },
	{ 'W', ".--" },
	{ 'X', "-..-" },
	{ 'Y', "-.--" },
	{ 'Z', "--.." },
	{ ' ', "     " }, //Gap between word, seven units 
		
	{ '1', ".----" },
	{ '2', "..---" },
	{ '3', "...--" },
	{ '4', "....-" },
	{ '5', "....." },
	{ '6', "-...." },
	{ '7', "--..." },
	{ '8', "---.." },
	{ '9', "----." },
	{ '0', "-----" },
		
	{ '.', ".-.-.-" },
	{ ',', "--..--" },
	{ '?', "..--.." },
	{ '!', "-.-.--" },
	{ ':', "---..." },
	{ ';', "-.-.-." },
	{ '(', "-.--." },
	{ ')', "-.--.-" },
	{ '"', ".-..-." },
	{ '@', ".--.-." },
	{ '&', ".-..." },
};


void setup() {

	    Serial.begin (9600);  //SFX sunduk
        Serial1.begin (9600); //sfx 1st room
        Serial2.begin (9600); //Music all
        Serial3.begin (9600); //MAIN+ Voice, SFX 1st, 2nd room

	pinMode(rel_lampa,OUTPUT);
	pinMode(rel_sunduk,OUTPUT);
	pinMode(micPin,INPUT);
	
	digitalWrite(rel_lampa, HIGH);
	digitalWrite(rel_sunduk, HIGH);

  mp3_set_serial (Serial);
  mp3_set_volume (22);
  delay (50);
	
	{
		Serial.print("Simone_said: ");
	}

}

void loop() {

	while(q_sunduk_done == false)
	{
	say();
	delay(200);
	listen();
	}
	
	if(q_sunduk_done == true)
	{
		open_sunduk();
	}
}

void say()
{
	//set led state
	String sayString = encode("O"); // codeword: "tuk tuk tuk"
	Serial.println(sayString);
	for(int i=0;i<=sayString.length();i++)
	{
		switch(sayString[i])
		{
		case '.': //dot
			mp3_play (0006); //Файл тук короткий
			delay(dotDuration);
			delay(dotDuration);
		break;
		case '-': //dash
			mp3_play (0005); //Файл тук довгий
			delay(dotDuration*3);
			delay(dotDuration);  
		break;
		//case ' ': //gap
		//	delay(dotDuration);
    }
  }	
}

String encode(const char *string)
{  
	size_t i, j;
	String morseWord = "";
	for( i = 0; string[i]; ++i )
	{
		for( j = 0; j < sizeof MorseMap / sizeof *MorseMap; ++j )
		{
			if( toupper(string[i]) == MorseMap[j].letter )
			{
				morseWord += MorseMap[j].code;
				break;
			}
		}
		//morseWord += " "; //Add tailing space to seperate the chars
	}
	return morseWord;
}

String decode(String morse)
{
	String msg = ""; 
	int lastPos = 0;
	int pos = morse.indexOf(' ');
	while( lastPos <= morse.lastIndexOf(' ') )
	{    
		for( int i = 0; i < sizeof MorseMap / sizeof *MorseMap; ++i )
		{
			if( morse.substring(lastPos, pos) == MorseMap[i].code )
			{
				msg += MorseMap[i].letter;
			}
		}
		lastPos = pos+1;
		pos = morse.indexOf(' ', lastPos);
    
		// Handle white-spaces between words (7 spaces)
		while( morse[lastPos] == ' ' && morse[pos+1] == ' ' )
		{
			pos ++;
		}
	}
	return msg;
} 
 void listen()
{
	unsigned long millisNow = 0;
	unsigned long millisPrew = 0;
	boolean knockFlag = false;
	boolean first = true;
	String listenString = "";
	long diff = 0;
	for (int i = 0;i<=2500;i++)
	{
		if (analogRead(micPin)>200) //налаштування чутливості мікрофону
		{
			if (first)
			{
				first = false;
				millisNow = millis();
				millisPrew = millisNow;
			}
			millisPrew = millisNow;
			millisNow = millis();		
			delay(100);
			knockFlag = true;
			diff = millisNow-millisPrew;			
			Serial.print("diff = ");
			Serial.println(diff);
		}

		if ((diff>=dotDuration-threshold)&&(diff<=dotDuration+threshold)&&knockFlag)
		{
			listenString=listenString+'.';
			knockFlag = false;
			millisPrew = millisNow;
		}
		if ((diff>=dotDuration*3-threshold)&&(diff<=dotDuration*3+threshold)&&knockFlag)
		{

			listenString=listenString+'-';
			knockFlag = false;
			millisPrew = millisNow;
		}
	delay(2);
	}
	Serial.println("Listen string is:");
	Serial.println(listenString);
	String cmpString = "--"; // 
	boolean  result=(listenString == cmpString);
	Serial.print("cmp_q_sunduk_done ");
	Serial.println(result);
	if (result)
	{
	q_sunduk_done = 1;
	}
}
void open_sunduk()
{
	digitalWrite(rel_lampa, LOW);
	digitalWrite(rel_sunduk, LOW);
	q_sunduk_done = 1;
}
