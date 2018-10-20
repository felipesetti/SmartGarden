#include "Plant.h"

Plant::Plant(String plantName,int plantInterval,int plantDuration){
	this->plantName = plantName;
	this->plantInterval = plantInterval;
	this->plantDuration = plantDuration;
}

Plant::Plant(){}

Plant::~Plant(){}

String Plant::getPlantName(){
	return this->plantName;
}

void Plant::setPlantName(String plantName){
	this->plantName = plantName;
}

int Plant::getPlantInterval(){
	return this->plantInterval;
}

void Plant::setPlantInterval(int plantInterval){
	this->plantInterval = plantInterval;	
}

int Plant::getPlantDuration(){
	return this->plantDuration;
}

void Plant::setPlantDuration(int plantDuration){
	this->plantDuration = plantDuration;
}





