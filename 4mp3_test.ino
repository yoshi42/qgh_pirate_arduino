//підключаємо бібліотеки
  #include <SoftwareSerial.h> // Бібліотека серійного порта
  #include <DFPlayer_Mini_Mp3.h> // Бібліотека МП3хи

    int sens_podsvechnik1 = A10;
    int sens_podsvechnik2 = A11;
    int sens_podsvechnik3 = A12;
    int sens_podsvechnik4 = A13;

    void setup() // попередня настройка проги. Виконується 1 раз з моменту запуску
	  	{

		  // сетуп для МП3х
		    Serial.begin (9600); //MAIN+ Voice, SFX 1st, 2nd room
		    Serial1.begin (9600); //SFX sunduk
		    Serial2.begin (9600); //Music all
		    Serial3.begin (9600); // SFX 3rd room
		  
		  //пару команд для мп3хи
		  mp3_set_serial (Serial);
		  mp3_set_volume (30);
		  mp3_play (0001);

		  //пару команд для мп3хи
		  mp3_set_serial (Serial1);
		  mp3_set_volume (30);
		  mp3_play (0001);

		  //пару команд для мп3хи
		  mp3_set_serial (Serial2);
		  mp3_set_volume (30);
		  mp3_play (0001);

		  //пару команд для мп3хи
		  mp3_set_serial (Serial3);
		  mp3_set_volume (30);
		  mp3_play (0001);

		}

	void loop()
		{
			podsvechnik1();
		}

	void podsvechnik1()//
	  {
	    boolean q_podsv1 = 0;
	    boolean w_podsv1 = 0;

	    digitalRead(sens_podsvechnik1);
	    if ((sens_podsvechnik1) == HIGH)
	    {
	      delay(50); // защита от дребезга кнопки.    
	      q_podsv1 = 0; // меняем значение w на противоположное 0 на 1 или 1 на 0.
	    }

	    if ((sens_podsvechnik1) == LOW && w_podsv1 == 0 && q_podsv1 == 0) // если кнопка 5 нажата и перед этим была отжата
	    {
	      delay(50); // защита от дребезга кнопки.
	      w_podsv1 = 1; // меняем значение w на противоположное 0 на 1 или 1 на 0.
	      q_podsv1 = 1;

		  mp3_set_serial (Serial);
		  mp3_set_volume (30);
		  mp3_play (0001);
	      delay(1000);
	    }

	    if ((sens_podsvechnik1) == HIGH)
	    {
	      delay(50); // защита от дребезга кнопки.    
	      w_podsv1 = 0; // меняем значение w на противоположное 0 на 1 или 1 на 0.
	    }
	  }
