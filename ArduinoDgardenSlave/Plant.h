#include <WString.h>  

class Plant {
public:
	Plant::Plant(String plantName,int plantInterval,int plantDuration);
	Plant::Plant();
	Plant::~Plant();
	String getPlantName();
	void setPlantName(String plantId);
	int getPlantInterval();
	void setPlantInterval(int plantInterval);
	int getPlantDuration();
	void setPlantDuration(int plantDuration);
private:
	String plantName;
	int plantInterval;
	int plantDuration;

};