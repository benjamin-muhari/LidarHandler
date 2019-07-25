#include "Coordinates.h"

struct DataPoint
{
	double azimuth; //azimuth degree of the data
	double vertical; //vertical degree of the data
	float distance; //distance in centimeter
	unsigned char intensity; //Intensity
	unsigned char id; //Laser id (0-15)
	long long time; //Time stamp in microseconds.
	int color = 0;	// Coloring for visualisations, based on calculations
	struct Coordinates coordinates; //X, Y , Z coordinates

	const bool operator < (const struct DataPoint& laser) {
		if (azimuth == laser.azimuth) {
			return id < laser.id;
		}
		else {
			return azimuth < laser.azimuth;
		}
	}
};
