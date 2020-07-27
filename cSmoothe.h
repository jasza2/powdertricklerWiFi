#pragma once
class cSmoothe
{
public:
	cSmoothe();
	~cSmoothe();
	float CalcAve(float RawReading);
	float reset(float newreading);
	float CalcMedian(float RawReading);
	
private:
	#define SAMPLE_SIZE 15
	float RunningAverageBuffer[SAMPLE_SIZE];
	float MedianBuffer[SAMPLE_SIZE];
	int  iMedianPosition;
	int NextRunningAverage;
};


