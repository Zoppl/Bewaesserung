#include <Arduino.h>
#include "WateringPlan.h"
#include "settings.h"

WateringPlan::WateringPlan(Soil *_soil, Valve *_valve, double _moistureTarget, int _wateringDurationMS, int _wateringDeadtimeMS)
{
  soil = _soil;
  valve = _valve;
  moistureTarget = _moistureTarget;
  wateringDurationMS = _wateringDurationMS;
  wateringDeadtimeMS = _wateringDeadtimeMS;
}

void WateringPlan::loop()
{
  if (ContvsFixed == WATERING_CONT)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - millisLastWatering >= wateringDeadtimeMS)
    {
      if (soil->getMoisture() < moistureTarget)
      {
        if (valve->getState() == VALVE_CLOSED)
        {
          valve->setState(VALVE_OPEN);
          wateringStarttimeMS = millis();

          Serial.print("Start Watering ");
          Serial.print(valve->getName());
          Serial.print("... \n");
          wateringCount++;
        }

        // delay(wateringDurationMS);
        // valve->setState(VALVE_CLOSED);
        // Serial.println("Done.");

        // millisLastWatering = currentMillis;
      }
      if (valve->getState() == VALVE_OPEN)
      {
        if (millis() > wateringStarttimeMS + wateringDurationMS)
        {
          valve->setState(VALVE_CLOSED);
          Serial.print("Done Watering ");
          Serial.print(valve->getName());
          Serial.print("... \n");
          millisLastWatering = currentMillis;
        }
      } 
   }
  }
  else  // WATERING_FIXED
  {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
      Serial.println("Failed to obtain time");
      return;
    }
    if (valve->getState() == VALVE_CLOSED && timeinfo.tm_hour == WATERING_DAY_TIME_HOUR && timeinfo.tm_min == WATERING_DAY_TIME_MINUTE && timeinfo.tm_sec == 00)
    {
      for (int x = 0; x < HISTORY_SLOTS; x++)
      {
        if (soil->getHistoricState(x) <= CRITICAL_VALUE)
        {
          valve->setState(VALVE_OPEN);
          wateringStarttimeMS = millis();

          Serial.print("Start Watering ");
          Serial.print(valve->getName());
          Serial.print("... \n");
          wateringCount++;
          break;
        }
      }
    }
    if (valve->getState() == VALVE_OPEN)
    {
      if (millis() > wateringStarttimeMS + wateringDurationMS)
      {
        valve->setState(VALVE_CLOSED);
        Serial.print("Done Watering ");
        Serial.print(valve->getName());
        Serial.print("... \n");
        // millisLastWatering = currentMillis;
      }
    }
  }
  

}

Soil WateringPlan::getSoil()
{
  return *soil;
}

Valve WateringPlan::getValve()
{
  return *valve;
}

unsigned long WateringPlan::getMSsinceLastWatering()
{
  return millis() - millisLastWatering;
}

bool WateringPlan::getContvsFixed()
{
  return ContvsFixed;
}

void WateringPlan::setContvsFixed(bool newState)
{
  if(newState == WATERING_CONT)  // cont has 1, fixed has 0
  {
    ContvsFixed = WATERING_CONT;
    Serial.println("New watering mode set: Continuous");
  }
  else
  {
    ContvsFixed = WATERING_FIXED;
    Serial.println("New watering mode set: Fixed Time");
  }
}

double WateringPlan::getMoistureTarget()
{
  return moistureTarget;
}

int WateringPlan::getWateringCount()
{
  return wateringCount;
}

