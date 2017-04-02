/*
 * ParkingSpot.h
 *
 *  Created on: 02/04/2017
 *      Author: MC-Guida
 */

#ifndef SRC_PARKINGSPOT_H_
#define SRC_PARKINGSPOT_H_

class ParkingSpot {
private:
	long long int id;
	double price;
public:
	ParkingSpot(long long id, float price);
	long long int getId();
	float getPrice();
};

#endif /* SRC_PARKINGSPOT_H_ */
