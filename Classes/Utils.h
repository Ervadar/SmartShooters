#pragma once

/*
Class with useful functions
*/
class Utils
{
public:
	static float clamp(float data, float from, float to)
	{
		if (data > to) data = to;
		else if (data < from) data = from;
		return data;
	}

	// From min..max to [0, 1]
	static float normalizeData(float data, float min, float max)
	{
		return (data - min) / (max - min);
	}

	// From [0, 1] to min..max
	static float denormalizeData(float data, float min, float max)
	{
		return data * (max - min) + min;
	}
};