// The Pump object is a digital twin of a system consisting of:
// - irrigation pump, 
// - relay that activates the main contactor
// - current sensor (TA12-100) to measure the motor current
// - pressure transducer with a measuring range of 0 to 80PSI (p/n AEM 30-2131-80
// - analog temperature sensor TMP-36

#ifndef Pump_h
#define Pump_h

#include "application.h"
#define MANUAL 1
#define AUTO 0

class Pump
{
private:
    int relayPin;
    int pressureSensorPin;
    int currentSensorPin;
    int tempSensorPin;
    float p_factor, c_factor;
    

public:
    Pump( int r, int p, int c,int t, float pf, float cf);   //set pins (relay, pressure sensor, current sensor, temp sensor) and conv. factors
    void    turnOn();      // activate relay
    void    turnOff();     // de-energize relay
    int     getRelayStatus();    //reads relay pin
    double  getCurrent();  
    double  getPressure();
    double  getTemperature();
    int     relayStatus; // status variable exposed to Particle cloud
    double  temperature;
    double  current;
    double  pressure;
    int     mode;   // MANUAL or AUTO
    int     setMode(int m);   // manual or automatic
    int     getMode();
    int     setManualON();   // turn pump ON and set mode to MANUAL
    int     setManualOFF();  // turn pump OFF and set mode to MANUAL
    int     setModeAuto();   // set mode to AUTO
    int     testFunc(String c);
   
};

///////////////////////////////  Timer  ////////////////////////////////////
class CrappyTimer 
{
private:
    int duration; // in seconds
    int startTime;
    int elapsed;
    
public:
    CrappyTimer(int dur);
    bool expired();  // should run inside loop()
};

#endif