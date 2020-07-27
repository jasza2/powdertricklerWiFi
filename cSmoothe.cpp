#include "cSmoothe.h"



cSmoothe::cSmoothe()
{
}


cSmoothe::~cSmoothe()
{
}

float cSmoothe::reset(float newreading)
{
	iMedianPosition = 0;
	for (int i = 0; i < SAMPLE_SIZE; ++i)
	{
		RunningAverageBuffer[i] = newreading;
	}
	return newreading;
}

float cSmoothe::CalcAve(float RawReading)
{
	RunningAverageBuffer[NextRunningAverage++] = RawReading;
	if (NextRunningAverage >= SAMPLE_SIZE)
	{
		NextRunningAverage = 0;
	}
	float RunningAverageTemperature = 0;
	for (int i = 0; i < SAMPLE_SIZE; ++i)
	{
		RunningAverageTemperature += RunningAverageBuffer[i];
	}
	if (SAMPLE_SIZE == 0) return  RawReading;
	RunningAverageTemperature /= SAMPLE_SIZE;

	return RunningAverageTemperature;
}


float cSmoothe::CalcMedian(float RawReading)
{
	float iresult;
	//put the value in the array
	MedianBuffer[iMedianPosition] = RawReading;
	iMedianPosition += 1;
	if (iMedianPosition >= SAMPLE_SIZE) iMedianPosition = 0; //dont want buffer overruns
	
       // sort the array using a bubble sort:	
		float analogValues[SAMPLE_SIZE];
		int out, in, swapper;
		for (out = 0; out < iMedianPosition; out++) {  // outer loop
			for (in = out; in < (iMedianPosition - 1); in++) {  // inner loop
				if (analogValues[in] > analogValues[in + 1]) {   // out of order?
				  // swap them:
					swapper = analogValues[in];
					analogValues[in] = analogValues[in + 1];
					analogValues[in + 1] = swapper;
				}
			}
		}

		// get the middle element:
		iresult = analogValues[iMedianPosition / 2];
	
		// print the results:
/*		Serial.print("Array: [");
		for (int j = 0; j < iMedianPosition; j++) {
			Serial.print(analogValues[j], 3);
			Serial.print(", ");
		}
		Serial.print("]\r\n");		
		Serial.print("\tMedian = ");
		Serial.print(iresult, 3);
		Serial.print("\r\n");
*/
		return iresult;

}

