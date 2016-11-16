#ifndef MONGOWORKER
#define MONGOWORKER
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <ctime>
#include "MongoJob.cpp"
#include "wqueue.cpp"
#include <sstream>
#include <vector>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

namespace utilities {

  struct DateTime {

    static int64_t millisSinceEpoch()
    {
      std::chrono::system_clock::duration duration{
        std::chrono::system_clock::now().time_since_epoch()
      };
      return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

  };
};


class MongoWorker : public Thread
{
        workqueu<MongoJob*>& m_queue;
        mongocxx::uri uri;
        mongocxx::client client;
    	mongocxx::database db;


    private:
        void updateStat(string identifier, string value, string containerName){
            mongocxx::collection coll = db[containerName];
            coll.update_one(bsoncxx::builder::stream::document{} << "statIdentifier" << identifier << finalize,
                            bsoncxx::builder::stream::document{} <<
                            "$set" << open_document <<
                                "statValue" << value <<
                            close_document <<
                            "$push" << open_document << "history" << open_document <<
                                "value" << value <<
                                "createdAt" << utilities::DateTime::millisSinceEpoch() <<
                            close_document << close_document <<
                            finalize);
        }

        void writeStat(string identifier, string value, string containerName){

            mongocxx::collection coll = db[containerName];
            auto builder = bsoncxx::builder::stream::document{};

            bsoncxx::document::value stat = builder
            << "statIdentifier" << identifier
            << "statValue" << value
            << "history" << bsoncxx::builder::stream::open_array
                << bsoncxx::builder::stream::open_document
                    << "value" << value
                    << "createdAt" << utilities::DateTime::millisSinceEpoch()
                << bsoncxx::builder::stream::close_document
            << close_array
            << bsoncxx::builder::stream::finalize;

            coll.insert_one(stat.view());
        }

        void identifyJob(MongoJob* job){
            //Stat work
            if(job->m_jobinfo.jobtype ==1 || job->m_jobinfo.jobtype == 2){
                switch(job->m_jobinfo.valuetype){
                    //int
                    case 1:
                        this->doJob(job->m_jobinfo.identifier, job->m_jobinfo.intvalue, job->m_jobinfo.jobtype);
                    break;
                    //float
                    case 2:
                        this->doJob(job->m_jobinfo.identifier, job->m_jobinfo.floatvalue, job->m_jobinfo.jobtype);
                    break;
                    //string

                    case 3:
                        this->doJobString(job->m_jobinfo.identifier, job->m_jobinfo.stringvalue, job->m_jobinfo.jobtype);
                    break;
                    //bool
                    case 4:
                        this->doJob(job->m_jobinfo.identifier, job->m_jobinfo.boolvalue, job->m_jobinfo.jobtype);
                    break;
                }
            //console work
            }else if(job->m_jobinfo.jobtype == 3){
                this->writeConsole(job->m_jobinfo.stringvalue, job->m_jobinfo.level);
            //map world
            }else if(job->m_jobinfo.jobtype == 4){
                this->mapInsert(job->m_jobinfo.themap);
            }
            
        }
        string bool_as_text(bool b){
            std::ostringstream buff;
            buff<<b;
            return buff.str();
        }
        string float_as_text(float myFloat){
            std::ostringstream buff;
            buff<<myFloat;
            return buff.str();   
        }
    public:
        MongoWorker(workqueu<MongoJob*>& queue) : m_queue(queue) {}
        void init(){
            this->uri = mongocxx::uri("mongodb://mainapp:ygMs9GQ@ds049456.mlab.com:49456/domotique_manager");
            this->client = mongocxx::client(this->uri);
            this->db = this->client["domotique_manager"];
        }
        void* run() {
            // Remove 1 item at a time and process it. Blocks if no items are 
            // available to process.
            for (int i = 0;; i++) {
                MongoJob* item = (MongoJob*)m_queue.remove();
                identifyJob(item);
                delete item;
            }
            return NULL;
        }

        void doJobString(string identifier, string value, int jobtype=1){
            string container = "statStringContainer";
            if(jobtype==1){
                updateStat( identifier,  value, container);
            }else{
                writeStat( identifier,  value, container);
            }
        }
        void doJob(string identifier, int value, int jobtype=1){
            string container = "statNumberContainer";
            if(jobtype==1){
                updateStat( identifier,   std::to_string(value), container);
            }else{
                writeStat( identifier,   std::to_string(value), container);
            }
        }
        void doJob(string identifier, float value, int jobtype=1){
            string container = "statNumberContainer";
            if(jobtype==1){
                updateStat( identifier,   float_as_text(value), container);
            }else{
                writeStat( identifier,   float_as_text(value), container);
            }
        }
        void doJob(string identifier, bool value, int jobtype=1){
            string container = "statBoolContainer";
            string stringvalue = bool_as_text(value);
            if(stringvalue == "1"){
            stringvalue = "true";
                }else{
            stringvalue = "false";
            }
            if(jobtype==1){
                updateStat( identifier,   stringvalue, container);
            }else{
                writeStat( identifier,   stringvalue, container);
            }
        }
        void writeConsole(string message, string stringLevel){

            mongocxx::collection coll = db["consoleContainer"];
            auto builder = bsoncxx::builder::stream::document{};

            bsoncxx::document::value consoleMessageDB = builder
            << "messageValue" << message
            << "messageLevel" << stringLevel
            << "createdAt" << utilities::DateTime::millisSinceEpoch()
            << bsoncxx::builder::stream::finalize;

            coll.insert_one(consoleMessageDB.view());
        }
        void mapInsert(std::vector<segment> map){
            mongocxx::collection coll = db["mapContainer"];
            /*auto builder = bsoncxx::builder::stream::document{};
            auto i;*/
            bsoncxx::builder::stream::document doc{};
            
            auto builder = doc
            << "createdAt" << utilities::DateTime::millisSinceEpoch()
            << "lines" << bsoncxx::builder::stream::open_array;
            
            for(auto i=map.begin(); i!=map.end(); ++i){
                builder << bsoncxx::builder::stream::open_document
                    << "x1" << i->debut.x
                    << "y1" << i->debut.y
                    << "x2" << i->fin.x
                    << "y2" << i->fin.y
                    << "nbpts" << i->nbPoint
                << bsoncxx::builder::stream::close_document;
            }
            auto mapInsert = builder << close_array
            << bsoncxx::builder::stream::finalize;
            coll.insert_one(mapInsert.view());
        }
};
#endif /* !MONGOWORKER */