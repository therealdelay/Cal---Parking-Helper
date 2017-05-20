/*
 * ParkingSpot.cpp
 *
 *  Created on: 02/04/2017
 *      Author: MC-Guida
 */

#include "ParkingSpot.h"

ParkingSpot::ParkingSpot(long long id, float price) {
	this->id = id;
	this->price = price;
}

long long int ParkingSpot::getId() {
	return id;
}

float ParkingSpot::getPrice() {
	return price;
}

