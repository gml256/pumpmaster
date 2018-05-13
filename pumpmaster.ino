
// This #include statement was automatically added by the Particle IDE.
#include "DailyTimerSpark.h"

// This #include statement was automatically added by the Particle IDE.
#include "Pump.h"
#include <string.h>
#include "application.h"
#define PRESS_FACTOR 0.0195
#define CURR_FACTOR 1
#define CHECK_IN_INTERVAL 10000  //10 seconds

CrappyTimer checkInTimer(60);
CrappyTimer myTimer(10);
//CrappyTimer testTimer(5, helloTest);
DailyTimer  timer1(17,9,17,10, EVERY_DAY);  // zone 1 timer
DailyTimer  timer2(17,11,17,12, EVERY_DAY); // zone 2 timer
Pump        pump1(D3, A3, A1, A5, PRESS_FACTOR, CURR_FACTOR);
int         wifiSignal = 0;
int         setManualON(String command);
int         setManualOFF(String command);
int         setModeAuto(String command);

void setup() 
{
    Time.zone(-4);
    timer1.begin();
    timer2.begin();
    Serial.begin(9600); 
    delay(2000);
    Serial.println("Hello ");
    pump1.turnOff();
    Particle.variable("PumpRelay", pump1.relayStatus);
    Particle.variable("Temperature", pump1.temperature);
    Particle.variable("Current", pump1.current);
    Particle.variable("Pressure", pump1.pressure);
    Particle.variable("WiFi", wifiSignal);
    Particle.variable("Mode", pump1.mode);
    Particle.function("ManualON", setManualON);
    Particle.function("ManualOFF", setManualOFF);
    Particle.function("AutoMode", setModeAuto);
}

void loop()
{    
    pumpControl();
    consoleOutput();
    houseKeeping();
    delay(1000);
}

void pumpControl()
{   
    int   relay, relay_prev;
   
    if( pump1.getMode() == MANUAL)
    {
        return;
    }
    
    relay_prev = pump1.getRelayStatus();
    if ( timer1.isActive() || timer2.isActive())
    {
        pump1.turnOn();
    }    
    else
    {
        pump1.turnOff();
    }
    
    relay = pump1.getRelayStatus();
    if (relay != relay_prev) {
        Particle.publish("PumpStatusChange", String(relay));
    }
    
    pump1.getTemperature();

}

int setManualON(String command)
{
    pump1.setManualON();
}

int setManualOFF(String command)
{
    pump1.setManualOFF();
}

int setModeAuto(String command)
{
    pump1.setModeAuto();
}

void consoleOutput()
{
    Serial.println(Time.timeStr());
    Serial.printlnf("Zone_1: %i, Zone_2: %i, status: %i, I = %4.2f Amp., P = %4.2f PSI, T = %3.1f", 
                    timer1.isActive(), timer2.isActive(), pump1.getRelayStatus(), pump1.getCurrent(), pump1.getPressure(), pump1.getTemperature());
    Serial.printlnf("WiFi signal: %i", WiFi.RSSI() );
}

void houseKeeping()
{
    if( checkInTimer.expired()){
        Particle.publish("Check In", "alive");
    }
    if( myTimer.expired()){
        Particle.publish("MyTimer", "expired");
    }
    wifiSignal = WiFi.RSSI();
}


