#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include "mythread.cpp"
#include "MongoCommand.cpp"
#include "MongoCommandListener.cpp"
#include "MongoJob.cpp"
#include "MongoWorker.cpp"
#include "wqueue.cpp"


#include <mongocxx/instance.hpp>
class MongoWrapper
{
    MongoWorker *mongoWorker[2];
    MongoCommandListener* commandListener;
    workqueu<MongoJob*>  queue;
    workqueu<MongoCommand*>  commandqueu;
 
  public:
    MongoWrapper(){
        mongoWorker[0] = new MongoWorker(queue);
        mongoWorker[1] = new MongoWorker(queue);

        try {
            mongoWorker[0]->init();
            mongoWorker[1]->init();
            mongoWorker[0]->start();
            mongoWorker[1]->start(); 

            commandListener = new MongoCommandListener(commandqueu);
            commandListener->init();
            commandListener->start();
        } catch (const std::exception& xcp) {
            std::cout << "connection failed: " << xcp.what() << "\n";
            //return EXIT_FAILURE;
         }

    }
    void addUpdate(string identifier, const char* value){
        JobInfo thejob;
        thejob.identifier = identifier;
        thejob.jobtype = 1;
        thejob.valuetype = 3;
        thejob.stringvalue = value;
        queue.add(new MongoJob(thejob));
    }
     void addUpdate(string identifier, string value){
        JobInfo thejob;
        thejob.identifier = identifier;
        thejob.jobtype = 1;
        thejob.valuetype = 3;
        thejob.stringvalue = value;
        queue.add(new MongoJob(thejob));
    }
    void addUpdate(string identifier, int value){
        JobInfo thejob;
        thejob.identifier = identifier;
        thejob.jobtype = 1;
        thejob.valuetype = 1;
        thejob.intvalue = value;
        queue.add(new MongoJob(thejob));
    }
    void addUpdate(string identifier, float value){
        JobInfo thejob;
        thejob.identifier = identifier;
        thejob.jobtype = 1;
        thejob.valuetype = 2;
        thejob.floatvalue = value;
        queue.add(new MongoJob(thejob));
    }
    void addUpdate(string identifier, bool value){
        JobInfo thejob;
        thejob.identifier = identifier;
        thejob.jobtype = 1;
        thejob.valuetype = 4;
        thejob.boolvalue = value;
        queue.add(new MongoJob(thejob));
    }
    void addWrite(string identifier, string value){
        JobInfo thejob;
        thejob.identifier = identifier;
        thejob.jobtype = 2;
        thejob.valuetype = 3;
        thejob.stringvalue = value;
        queue.add(new MongoJob(thejob));
    }
    void addWrite(string identifier, int value){
        JobInfo thejob;
        thejob.identifier = identifier;
        thejob.jobtype = 2;
        thejob.valuetype = 1;
        thejob.intvalue = value;
        queue.add(new MongoJob(thejob));
    }
    void addWrite(string identifier, float value){
        JobInfo thejob;
        thejob.identifier = identifier;
        thejob.jobtype = 2;
        thejob.valuetype = 2;
        thejob.floatvalue = value;
        queue.add(new MongoJob(thejob));
    }
    void addWrite(string identifier, bool value){
        JobInfo thejob;
        thejob.identifier = identifier;
        thejob.jobtype = 2;
        thejob.valuetype = 4;
        thejob.boolvalue = value;
        queue.add(new MongoJob(thejob));
    }
    //level : // warning, sucesss, info, error, none
    void writeConsole(string message, string level){
        JobInfo thejob;
        thejob.jobtype = 3;
	    thejob.level = level;
        thejob.stringvalue = message;
        queue.add(new MongoJob(thejob));
    }
    //level : // warning, sucesss, info, error, none
    void writeMap(std::vector<segment> map, int robotx=0, int roboty=0){
        JobInfo thejob;
        thejob.jobtype = 4;
        thejob.robotx = robotx;
        thejob.roboty = roboty;
        thejob.themap = map;
        queue.add(new MongoJob(thejob));
    }
      //level : // warning, sucesss, info, error, none
    void sendFile(string filename, string filecontent){
        JobInfo thejob;
        thejob.jobtype = 5;
        thejob.identifier = filename;
        thejob.stringvalue = filecontent;
        queue.add(new MongoJob(thejob));
    }
    MongoCommand* getCommand(){
        if(commandqueu.size() >0){
            MongoCommand* item = (MongoCommand*)commandqueu.remove();
            item->m_commandInfo.thecommand = getCommandEnum(item->m_commandInfo.command); 
            return item; 
        }
        return NULL;
    }

    MongoCommand* getBlockingCommand(){
         MongoCommand* item = (MongoCommand*)commandqueu.remove();
         item->m_commandInfo.thecommand = getCommandEnum(item->m_commandInfo.command); 
    	 return item;
    }

    commandEnum getCommandEnum(string stringcommand){
        if(stringcommand == "manual"){
            return MANUAL;
        }else if(stringcommand == "goto"){
            return GOTO;
        }else if(stringcommand == "close"){
            return CLOSE;
        }else if(stringcommand == "startdebug"){
            return STARTDEBUG;
        }else if(stringcommand == "stopdebug"){
            return STOPDEBUG;
        }else if(stringcommand == "scan"){
            return SCAN;
        }else if(stringcommand == "turn"){
             return TURN;
        }else if(stringcommand == "automatic"){
             return AUTOMATIC;
        }else if(stringcommand == "takephoto"){
            return TAKEPHOTO;
        }
        return INVALID;
    }
    ~MongoWrapper() {
 
    }
    
 
};