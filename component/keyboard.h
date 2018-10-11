/**************************************************************************
 ***	author : nathanael MERAUD 		october 2018		***
 ***									***
 ***	this class is creat to help usage of a matrix keyboard.		***
 ***	to use the class you need first first call the start_scruting	***
 ***	methode, then the class will check the button state every 	***
 ***	m_refresh_period seconds (0.001 in default). When the object	***
 ***	detect a state changement, it gona call a callbackfunction	***
 ***	which you can choose.						***
 ***									***	
 ***	/!\ YOU HAVE TO BE CAREFULL because of material limitation, the	***
 ***	    object can’t garant you a correct working of the class if 	***
 ***	    the user descide to push more than one button at the same	***
 ***	    time ! If you want to detect multiple push, you need to	***
 ***	    take another technologie.					***
 ***									***
 **************************************************************************/



#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "mbed.h"

void none(uint8_t i);
class Keyboard{
	public:
		/* you need to pass DigitalOut and DigitalIn pointer to the
		   class which represent pins on the board*/
		Keyboard(DigitalOut* line[4], DigitalIn* colum[4]);
		virtual ~Keyboard();
		/*this fuction start the scrutation process if you forget to
		  call no button state will be read, the fuction will start
		  a callback system (mbed::Ticker) to call the fuction scrut()
		  every m_refresh_period second*/
		void start_scanning();
		/* stop the scan (keyboard gonna be useless) */
		void stop_scanning();
		/* return the value of the pressed button result not garranted
		   if more than one button is pressed, returned 0xFF if no key
		   are pressed */
		uint8_t get_key();

		//setters
		void set_callback_push_button(void (*callback_fct)(uint8_t));
		void set_callback_pull_button(void (*callback_fct)(uint8_t));
		void set_callback_push_button_since_limit_time(void (*callback_fct)(uint8_t));
		void set_callback_push_button_repeated(void (*callback_fct)(uint8_t));
		void set_refresh_period(float p);
		void set_time_before_repetition(float t);
		void set_repetition_period(float p);
	
		void set_repetition_available(bool b);

#ifdef DEBUG
		/* a debug method */
		void print(Serial* pc);
#endif

	private:
		void scan();
		void start_repetition_series();
		void reset_repetition();

		uint8_t m_button_state[16];//S1 : top_left, S4 : top_right, S13 : bottom_left…
		void member_function_button_push_since_limit_time();
		void member_function_button_push_repeated();
		
		void (*m_function_button_push)(uint8_t);
		void (*m_function_button_pull)(uint8_t);
		void (*m_function_button_push_since_limit_time)(uint8_t);
		void (*m_function_button_push_repeated)(uint8_t);
		DigitalOut* m_line[4];
		DigitalIn* m_colum[4];
		float m_refresh_period;
		float m_time_before_repetitions;
		float m_repetition_period;
		bool m_repetition_available;
		bool m_scan;
		uint8_t m_button_push;
		Ticker m_scan_ticker;
		Timeout m_wait_for_repeat;
		Ticker m_repeat_ticker;
		Serial m_debug;
};

#endif //KEYBOARD_H
