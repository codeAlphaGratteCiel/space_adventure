/**************************************************************************
 ***	author : nathanael MERAUD			october 2018	***
 ***									***
 ***	this class is created to help usage of modules composed of 4	***
 ***	seven-segment display.						***
 ***	the class comport 2 modes of fuctionnement :			***
 ***		-the first one print element contain in m_value		***
 ***		-the second one print an annimation defined on		***
 ***		 m_annim_tab						***
 ***		-those two mode can be combined to create a new one 	***
 ***		 for people who want annim display (for exemple a blink ***
 ***		 annimation)						***
 ***									***
 **************************************************************************/



#ifndef SEVEN_SEGMENT_DISPLAY
#define SEVEN_SEGMENT_DISPLAY

#include "mbed.h"
#include <vector>

class Seven_segment_display
{
	/* this transcoding_tab is use to convert an uint8_t containning in
	   [0, 15] to a table which represent it’s hexadecimal representation
	   on a seven-segment display*/ 
	uint8_t transcoding_tab[16][7] = {{1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}, {1,1,1,0,1,1,1}, {0,0,1,1,1,1,1}, {1,0,0,1,1,1,0}, {0,1,1,1,1,0,1}, {1,0,0,1,1,1,1}, {1,0,0,0,1,1,1}}; 

	public:
		/* you need to pass DigitalOut pointer to the class. Those pointer
		   represent board’s pins.*/
		Seven_segment_display(DigitalOut* select_pin[4], DigitalOut* value_pin[7]);
		~Seven_segment_display();
		void right_shift(uint8_t number);
		void left_shift(uint8_t number);
//		void print_one(uint8_t position, uint8_t value);//position need to be 0,1,2 or 3 anp value between 0 and 15


		// starter/stopper
		void start_print();
		//void start_print_time();
		void show_annim();
		void stop();

		//setters
		void set_value(const uint8_t value[]);
		void set_value(uint16_t value);
		void set_number(uint8_t position, uint8_t number);
		void set_time_between_2_print(float time);
		void set_time_between_2_annim_image(float times);
		void set_annim(std::vector<uint8_t*>);
		void set_time_ref(time_t t);
		//getter
		uint8_t* get_value()const;
	
	//	void print(uint8_t tab[]);

	private:
		void next_annim();
		void step_next_annim();

		void step_print();
		void print_time();
		void annim_step();

		void calc_time();
		
		bool m_annim;		
		bool m_print;
		
		time_t m_time_ref;
		std::vector<uint8_t* > m_annim_tab;//vector witch contain table of 4 integer, one integer is represent the state of the 7 segment (b6 = A, b5 = B,…, b0=G)
		uint8_t m_annim_step;
		uint8_t m_value_to_print[4];
		uint8_t m_time_to_print[4];
		DigitalOut** m_select_pin;//tab (size 4)
		DigitalOut** m_value_pin;//tab (size 7)
		Ticker m_timer;
		Ticker m_timer2;
		Ticker m_ticker_time;
		float m_time_between_2_print;
		float m_time_between_2_annim_image;
		uint8_t m_times_between_2_annim;
};


#endif//SEVEN_SEGMENT_DISPLAY
