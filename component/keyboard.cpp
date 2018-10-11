#include "keyboard.h"

Keyboard::Keyboard(DigitalOut* line[4], DigitalIn* colum[4]): m_debug(USBTX, USBRX){
	for(uint8_t i = 0; i<4; i++){
		m_line[i]=line[i];
		m_colum[i]=colum[i];
		m_line[i]->write(0);
	}
	for(uint8_t i = 0; i<16; i++){
		m_button_state[i]= 0;
	}
	m_button_push = 0xFF;
	m_refresh_period = 0.01;
	m_repetition_period = 0.1;
	m_time_before_repetitions = 1;
	m_scan = false;
	m_repetition_available = false;
	m_function_button_push= &none;	
	m_function_button_pull= &none;	
	m_function_button_push_since_limit_time = &none;
	m_function_button_push_repeated = &none;
}

Keyboard::~Keyboard(){

}

void none(uint8_t i){}

void Keyboard::scan(){
	for(uint8_t i = 0; i<4; i++){
		m_line[i]->write(1);
		for(uint8_t j = 0; j<4; j++){
			uint8_t v = m_colum[j]->read();
			if(v != m_button_state[(i<<2)+j]){
				m_button_state[(i<<2)+j] = v;
				if(v == 1){
					if(m_repetition_available){
						reset_repetition();
						m_wait_for_repeat.attach(callback(this, &Keyboard::start_repetition_series), m_time_before_repetitions);
					}
					m_button_push = (i<<2)+j;
					(*m_function_button_push)((i<<2)+j);
				}
				else{
					if(m_repetition_available){
						reset_repetition();
					}
					m_button_push = 0xFF;
					(*m_function_button_pull)((i<<2)+j+1);
				}
			}
		}
		m_line[i]->write(0);
	}
}

void Keyboard::set_repetition_available(bool b){
	if(b){

	}
	else{
		reset_repetition();
	}
	m_repetition_available = b;
}

void Keyboard::reset_repetition(){
	m_wait_for_repeat.detach();
	m_repeat_ticker.detach();
}

void Keyboard::start_repetition_series(){
	m_repeat_ticker.attach(callback(this, (&Keyboard::member_function_button_push_repeated)),m_repetition_period);
}

void Keyboard::start_scanning(){
	m_scan = true;
	m_scan_ticker.attach(callback(this, &Keyboard::scan), m_refresh_period);
}

void Keyboard::stop_scanning(){
	m_scan = false;
	m_scan_ticker.detach();
}

void Keyboard::set_callback_push_button(void (*callback_fct)(uint8_t)){
	m_function_button_push = callback_fct;
}

void Keyboard::set_callback_pull_button(void (*callback_fct)(uint8_t)){
	m_function_button_pull = callback_fct;
}

void Keyboard::set_callback_push_button_since_limit_time(void (*callback_fct)(uint8_t)){
	m_function_button_push_since_limit_time = callback_fct;
}

void Keyboard::set_callback_push_button_repeated(void (*callback_fct)(uint8_t)){
	m_function_button_push_repeated = callback_fct;
}

void Keyboard::set_repetition_period(float p){
	m_repetition_period = p;
}

void Keyboard::set_time_before_repetition(float t){
	m_time_before_repetitions = t;
}

void Keyboard::member_function_button_push_since_limit_time(){
	(*m_function_button_push_since_limit_time)(m_button_push);
}

void Keyboard::member_function_button_push_repeated(){
	(*m_function_button_push_repeated)(m_button_push);
}

void Keyboard::set_refresh_period(float p){
	if(p>5){
		//TODO add an error
	}
	m_refresh_period = p;
}

uint8_t Keyboard::get_key(){
	return m_button_push;
}



#ifdef DEBUG
void Keyboard::print(Serial* pc){
	for(int i = 0; i<4; i++){
		
		for(int j = 0; j<4; j++){
			pc->printf("%hd ", m_button_state[(i*4)+j]);
		}
		pc->printf("\n");
	}
	pc->printf("\n");
	pc->printf("\n");
}
#endif//DEBUG
