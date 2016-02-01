#pragma once

class Utils
{
public:
	static float clamp(float data, float from, float to)
	{
		if (data > to) data = to;
		else if (data < from) data = from;
		return data;
	}

	static float normalizeData(float data, float min, float max)
	{
		return (data - min) / (max - min);
	}

	static float denormalizeData(float data, float min, float max)
	{
		return data * (max - min) + min;
	}
};