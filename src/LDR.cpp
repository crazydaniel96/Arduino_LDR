#include "Arduino.h"
#include "LDR.h"

LDR::LDR(float percentage, int LDR_Pin,String name,float Loop_delay){
	
  	this->th_percentage = percentage;
	this->LDR_Pin = LDR_Pin;
	this->reference = 0;
	this->name=name;
	this->acquisition_mode = 1;
	this->Loop_delay = Loop_delay;
	this->Delta_Perc = 0;
}


void LDR::calibrate(){
	
	int i, max, min, sum=0;
	
	for (i = 0; i < 10; i++){
		this->lectures[i] = analogRead(this->LDR_Pin);	
		delay(100);   //setup correct delay here
	}
	
	max=this->lectures[0];
	min=this->lectures[0];
	for (i = 0; i < 10; i++){
    	if (this->lectures[i] > max)
        	max = this->lectures[i];
       	if (this->lectures[i] < min)
        	min = this->lectures[i];
	}	
    //mean excluding outliers
    for (i = 0; i < 10; i++)
   		sum += this->lectures[i];
	sum-=min+max;
   	this->reference=sum/8;
   	//Serial.print(this->name);
	//Serial.print(" LDR calibrated with reference of ");
	//Serial.println(this->reference);
}


bool LDR::check_activation(){
	
	currentMillis=millis();
	int i=0,j=0;
	if (currentMillis - startMillis >= period && !acquisition_mode){
	
		//Serial.println("STARTING PATTERN EVALUATION");
		acquisition_mode=1;
		
		//find peak

		for (i = 0; i < 49; i++){
			if (this->pattern[i] < this->pattern[j])
				j = i;
		}
		
		//evaluate curve
		
		for (i=j+1; i<50; i++){
			
			if ((this->pattern[i]-this->pattern[i-1])/this->Loop_delay < 0) // derivate (time of each loop is 50ms
				break;
		}
		
		if ((i-j)*this->Loop_delay<100| (i-j)*this->Loop_delay>1000) //check if constantness of derivative persist for a minimum amount of time but not too much
			return false;
		
		
		for (i=j-1; i>0; i--){
			
			if ((this->pattern[i]-this->pattern[i-1])/this->Loop_delay > 0) // derivate (time of each loop is 50ms
				break;
		}
		if ((j-i)*this->Loop_delay<100| (i-j)*this->Loop_delay>1000) //check if constantness of derivative persist for a minimum amount of time but not too much
			return false;
		
		return true;  // if we got a good peak, we have our gesture 
	}
	
	//shift pattern values (glass LDR excluded)
	if (this->th_percentage > 0)
		for (i=0; i<49; i++)
			this->pattern[i]=this->pattern[i+1];
	 
	this->pattern[49] = float(analogRead(this->LDR_Pin));
	
	if (acquisition_mode){
		
		this->Delta_Perc = ( this->reference - this->pattern[49] )/1023; // 1023 = FS
		
		if (this->th_percentage < 0){
			if (this->Delta_Perc < this->th_percentage){
				//Serial.print("(NO CURVE EVALUATION) Triggererd with delta percentage = ");Serial.print(this->Delta_Perc);
				//Serial.print(" and value = ");Serial.println(this->pattern[49]);
				return true;
			}
		}
		else{		
			if (this->Delta_Perc > this->th_percentage){
				//Serial.print("Triggererd with delta percentage = ");Serial.print(this->Delta_Perc);
				//Serial.print(" and value = ");Serial.println(this->pattern[49]);
				startMillis = millis(); //start count time to finish signal acquisition
				acquisition_mode=0;
			}
		}
	}
	return false;

}


float LDR::read_volts(){

	int lecture = analogRead(this->LDR_Pin);
	return map(lecture,0,1023,0,5);
}

int LDR::read_bits(){
	return analogRead(this->LDR_Pin);
}

/*
float Calc_resist(float V_ldr){
	float Resistance = 10000*(V_ldr)/(5-V_ldr);
	return Resistance;
}*/


