#ifndef LDR_h
#define LDR_h

// library interface description

class LDR {
	private:
		byte LDR_Pin;
	  	float th_percentage;
	  	bool acquisition_mode;
	  	unsigned long startMillis; 
		unsigned long currentMillis;
		const unsigned long period = 1000; //1 second peak wide
		float Loop_delay;
	  	//methods
	  	//float Calc_resist(float V_ldr);
	  
	public:
		LDR( float percentage, int LDR_Pin, String name,float Loop_delay);
	  	void calibrate();
	  	bool check_activation();
	  	float read_volts();
	  	int read_bits();
	  	
	  	//variables
	  	int reference;
	  	String name;
	  	float pattern[50];
	  	float Delta_Perc;
	  	int lectures[10];
};


#endif

