/*
 * ParkingSpot.h
 *
 *  Created on: 02/04/2017
 *      Author: MC-Guida
 */

#ifndef SRC_PARKINGSPOT_H_
#define SRC_PARKINGSPOT_H_

/**
 * ParkingSpot class keeps information about the ID of the vertex it's in and the price of the parking.
 */
class ParkingSpot {
private:
	long long int id;///< ID of the vertex
	double price; ///< Price to park
public:
	/**
	 * Class constructor. It gives the class attribute the values given.
	 */
	ParkingSpot(long long id, float price);
	/**
	 * This function returns the ID of the ParkingSpot.
	 */
	long long int getId();
	/**
	 * This function returns the price of the ParkingSpot.
	 */
	float getPrice();
};

#endif /* SRC_PARKINGSPOT_H_ */
