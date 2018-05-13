#include "Pump.h"

//constr.   assign relay/pressure/current pins and conversion factors
Pump::Pump( int r, int p, int c, int t, float pf, float cf)     
{
    relayPin = r;
    pressureSensorPin = p;
    currentSensorPin = c;
    tempSensorPin = t;
    pinMode(relayPin, OUTPUT);
    pinMode(pressureSensorPin, INPUT);
    pinMode(currentSensorPin, INPUT);
    pinMode(tempSensorPin, INPUT);
    p_factor = pf;
    c_factor = cf;
    turnOff();
    relayStatus = 0;
    mode = AUTO;  // default mode
    Particle.function("TestFunc_"+String(relayPin), &Pump::testFunc, this);
    
}
    
void Pump::turnOn()
{
    digitalWrite(relayPin, HIGH);
    getRelayStatus();  //make relayStatus = 1
}

void Pump::turnOff()
{
    digitalWrite(relayPin, LOW);
    getRelayStatus();  //make relayStatus = 0
}

int Pump::getRelayStatus()
{
    return relayStatus=digitalRead(relayPin);
}

double Pump::getTemperature(){
   
    temperature = (double)analogRead(tempSensorPin)*330/4095 - 50.0; //celsius
    temperature = temperature * 9/5 + 32.0;
}   

int Pump::setMode(int m)     // MANUAL (1) or AUTO (0)
{
    mode = m;
    if( (mode != 1) && (mode!=0) ){
        mode = 0;
    }
    return mode;
}

int Pump::setManualON()
{
    turnOn();
    return (mode = MANUAL);
}
int Pump::setManualOFF()
{
    turnOff();
    return (mode = MANUAL);
    
}

int Pump::setModeAuto()
{
    return (mode = AUTO);
}

int Pump::getMode()
{
    return mode;
}

int Pump::testFunc(String c)
{
    Particle.publish("TestFunc_"+String(relayPin), c);
    return 0;
}


double Pump::getCurrent(){
    int v;

    v = analogRead(currentSensorPin); 
    if (v <= 645){
        return current = ( 0.0 + v/172.0);
    }
    if (v <= 670){
        return current = (3.75 + (v-645)/25.3);
    }
    if (v <= 1387){
        return current = (4.75 + (v-670)/410.0);
    }
    if (v <= 1989) {
        return current = (6.75 + (v-1387)/286.0);
    }
    if (v <= 3077) {
        return current = ( 8.75 + (v-1989)/271.0);
    }
    return current = (12.5 + (v-3077)/250.0);  // extrapolated
}


double Pump::getPressure(){
    return pressure=(float)analogRead(pressureSensorPin)*p_factor;  // 80/4095
    
}

////////////////////////////////////////////////////////////////////////////////////
//
//CrappyTimer -->  simple countdown timer. 
// returns TRUE when expired and starts over.
// Works inside the main loop()
////////////////////////////////////////////////////////////////////////////////////

CrappyTimer::CrappyTimer(int dur)
{
    duration = (dur-1) * 1000;
    startTime = 0;
}
    
bool CrappyTimer::expired()   // must run inside loop()
{
    if (startTime == 0 )   // first pass
    {
        startTime = millis();
    }
    elapsed = millis() - startTime;
    if( elapsed >= duration){  // timer expired
        startTime = 0;
    }
    return ( elapsed >= duration );
}


