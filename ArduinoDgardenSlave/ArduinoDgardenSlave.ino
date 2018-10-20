#include <TimerOne.h>

#include <StaticThreadController.h>
#include <ThreadController.h>
#include <Thread.h>

#include <Wire.h>
#include "Plant.h"
#include "defines.h"


int dataCounter =0; //necessary counter to identify the correct piece of data


//global variables to manipulate incoming Plant Instances
unsigned int incomingPlantId;
unsigned int incomingPlantInterval;
unsigned int incomingPlantDuration;

//global variables to manipulate current plant data
unsigned int currentVioletInterval=0;
unsigned int currentVioletDuration=0;
unsigned int currentTulipInterval=0;
unsigned int currentTulipDuration=0;
unsigned int currentRoseInterval=0;
unsigned int currentRoseDuration=0;

//global variables as flags to plant states
bool isVioletActive = false;
bool isTulipActive = false;
bool isRoseActive = false;

//creating threads to run garden
Thread tChangeVioletState = Thread();
Thread tChangeTulipState = Thread();
Thread tChangeRoseState = Thread();

//Threads para função delete
Thread tDeleteWatchdog = Thread();
Thread tPerformDeletion = Thread();

//Controller to run all threads
ThreadController tGardenController = ThreadController();


void setup() {
  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(115200);
  delay(10);
  Serial.println("Setup Done");
  dataCounter=0;
  pinMode(VIOLET_GPIO,OUTPUT);
  pinMode(TULIP_GPIO,OUTPUT);
  pinMode(ROSE_GPIO,OUTPUT);

  //Triggering watchdog thread
  tDeleteWatchdog.setInterval(100000);
  tDeleteWatchdog.onRun(deleteWatchdog);
  tGardenController.add(&tDeleteWatchdog);
  
  
  
}



void loop() {  
      
  tGardenController.run();
      
}

void receiveEvent(int howMany) {

  char  plantData[howMany]; //global variable loaded with incoming data plant
  int byteCounter=0;
    
  
  while (0 < Wire.available()) { // loop through all but the last
  char incomingByte = Wire.read(); // receive byte as a character
  plantData[byteCounter]=incomingByte;
  byteCounter++;      
      }
      
      String plantDataConverted(plantData);//Convert the char array loaded befora to a String class type
     
      
      switch(dataCounter){ //analyses what type of data is coming: the name, the interval or the duration
        case IS_COMING_PLANT_ID:
          incomingPlantId=plantDataConverted.toInt();
          Serial.println(incomingPlantId); //This architecture has to be changed.
          dataCounter++;
          break;
        case IS_COMING_INTERVAL:
          incomingPlantInterval = plantDataConverted.toInt();
          Serial.println(incomingPlantInterval);
          dataCounter++;
          break;
        case IS_COMING_DURATION:          //At this point you can treat all - because the last needed data has come
          incomingPlantDuration = plantDataConverted.toInt();
          Serial.println(incomingPlantDuration);
          
          if (incomingPlantId == VIOLET_ID){ //In case the plant is violet-this if must work after changing plant name for plant Id

            isVioletActive = true;//reset violet state to avoid it to be deleted
            
            if(currentVioletInterval!=incomingPlantInterval || currentVioletDuration!=incomingPlantDuration){
              Serial.println("chegou novos dados para a violeta:disparando thread");
              
              tChangeVioletState.setInterval(incomingPlantInterval*60000); 
              tChangeVioletState.onRun(changeVioletWaterState);                                         
              tGardenController.add(&tChangeVioletState);                         
              
              currentVioletInterval= incomingPlantInterval;
              currentVioletDuration = incomingPlantDuration;

              digitalWrite(VIOLET_GPIO,LOW);            
              
              }
                 
            
            }
          else if (incomingPlantId==YELLOW_TULIP_ID){ //In case the plant is a Yellow Tulip

           isTulipActive = true; //reset tulip state to avoid it to be deleted
            
           if(currentTulipInterval!=incomingPlantInterval || currentTulipDuration!=incomingPlantDuration){
              Serial.println("chegou novos dados para a tulipa:disparando thread");
              
              tChangeTulipState.setInterval(incomingPlantInterval*60000);
              tChangeTulipState.onRun(changeTulipWaterState);                                         
              tGardenController.add(&tChangeTulipState);                         
              
              currentTulipInterval= incomingPlantInterval;
              currentTulipDuration = incomingPlantDuration;

              digitalWrite(TULIP_GPIO,LOW);            
              
              }
            }
          else if (incomingPlantId== RED_ROSE_ID){ //In case the plant is a Red Rose

            isRoseActive = true; //reset rose state to avoid it to be deleted           
            
            if(currentRoseInterval!=incomingPlantInterval || currentRoseDuration!=incomingPlantDuration){
              Serial.println("chegou novos dados para a rosa:disparando thread");
              
              tChangeRoseState.setInterval(incomingPlantInterval*60000);
              tChangeRoseState.onRun(changeRoseWaterState);                                         
              tGardenController.add(&tChangeRoseState);                         
              
              currentRoseInterval= incomingPlantInterval;
              currentRoseDuration = incomingPlantDuration;

              digitalWrite(ROSE_GPIO,LOW);            
              
              }
            
            }

                      
          dataCounter=0;
          //delete mPlant;
          break;
        default:
          Serial.println("Invalid data");         
          }
         
}

void changeVioletWaterState(){
  
  if(digitalRead(VIOLET_GPIO)==LOW){
    Serial.println("liga Violeta");
    
    digitalWrite(VIOLET_GPIO,HIGH);

    tChangeVioletState.setInterval(currentVioletDuration*1000);
    
    }

    else if(digitalRead(VIOLET_GPIO)==HIGH){
      
      Serial.println("desliga Violeta");
      digitalWrite(VIOLET_GPIO,LOW);

      tChangeVioletState.setInterval(currentVioletInterval*60000);
      
      } 
            
  }

  void changeTulipWaterState(){
  
  if(digitalRead(TULIP_GPIO)==LOW){
    Serial.println("liga Tulipa");
    
    digitalWrite(TULIP_GPIO,HIGH);

    tChangeTulipState.setInterval(currentTulipDuration*1000);
    
    }

    else if(digitalRead(TULIP_GPIO)==HIGH){
      
      Serial.println("desliga Tulipa");
      digitalWrite(TULIP_GPIO,LOW);

      tChangeTulipState.setInterval(currentTulipInterval*60000);
      
      }  
      
  }

  void changeRoseWaterState(){
  
  if(digitalRead(ROSE_GPIO)==LOW){
    Serial.println("liga Rosa");
    
    digitalWrite(ROSE_GPIO,HIGH);

    tChangeRoseState.setInterval(currentRoseDuration*1000);
        
    }

    else if(digitalRead(ROSE_GPIO)==HIGH){
      
      Serial.println("desliga Rosa");
      digitalWrite(ROSE_GPIO,LOW);

      tChangeRoseState.setInterval(currentRoseInterval*60000);
     
      
      }  
      
  }


  void deleteWatchdog(){

    isVioletActive = false;
    isTulipActive = false;
    isRoseActive = false;

    Serial.println("Wathcdog disparando thread para verificar plantas ociosas");

    tPerformDeletion.setInterval(15000);//enough time to all flags to be reset to active again
    tPerformDeletion.onRun(deleteIdlePlants);
    tGardenController.add(&tPerformDeletion);

    
    
    }

  void deleteIdlePlants(){

    if (!isVioletActive){
      tGardenController.remove(&tChangeVioletState);
      Serial.println("Violeta foi deletada. Removendo thread");
      }
    else{Serial.println("Violeta ativa");
      }

    if (!isTulipActive){
      tGardenController.remove(&tChangeTulipState);
      Serial.println("Tulipa foi deletada. Removendo thread");
      }
      else{Serial.println("Tulipa ativa");
      }

      if (!isRoseActive){
      tGardenController.remove(&tChangeTulipState);
      Serial.println("Rosa foi deletada. Removendo thread");
      }
      else{Serial.println("Rosa ativa");
      }

            
    tGardenController.remove(&tPerformDeletion);
    
    
    }
