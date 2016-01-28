#pragma once

class Utils
{
public:
	static float normalizeData(float data, float min, float max)
	{
		return (data - min) / (max - min);
	}

	static float denormalizeData(float data, float min, float max)
	{
		return data * (max - min) + min;
	}
};