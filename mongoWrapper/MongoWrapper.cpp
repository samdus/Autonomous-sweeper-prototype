#include <iostream>
#include <string>
#include "MongoJob.cpp"
#include "MongoWorker.cpp"
#include "wqueue.cpp"


#include <mongocxx/instance.hpp>
class MongoWrapper
{
    MongoWorker *mongoWorker[2];
    workqueu<MongoJob*>  queue;
 
  public:
    MongoWrapper(){
        mongoWorker[0] = new MongoWorker(queue);
        mongoWorker[1] = new MongoWorker(queue);

        mongoWorker[0]->init();
        mongoWorker[1]->init();
        
        mongoWorker[0]->start();
        mongoWorker[1]->start(); 
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
    ~MongoWrapper() {
 
    }
    
 
};