#ifndef MONGOCOMMANDLISTENER
#define MONGOCOMMANDLISTENER
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include "MongoCommand.cpp"
#include "wqueue.cpp"
#include <chrono>
#include <thread>

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


class MongoCommandListener : public Thread
{
        workqueu<MongoCommand*>& m_queue;
        mongocxx::uri uri;
        mongocxx::client client;
    	mongocxx::database db;
        int64_t fromTime;

    private:
        void pollCommand(){
           mongocxx::collection coll = db["commandContainer"]; 
           printf("Polling for a command \n");
           mongocxx::cursor cursor = coll.find((
            document{} << "createdAt" << bsoncxx::builder::stream::open_document <<
                "$gt" << this->fromTime <<
            << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize);
            for(auto doc : cursor) {
                std::cout << bsoncxx::to_json(doc) << "\n";
                MongoCommand thecommand;
                thecommand.command = "stop";
                queue.add(new MongoCommand(thecommand));
            }
            
        }
    public:
        MongoCommandListener(workqueu<MongoCommand*>& queue) : m_queue(queue) {}
        void init(){
            this->uri = mongocxx::uri("mongodb://mainapp:ygMs9GQ@ds049456.mlab.com:49456/domotique_manager");
            this->client = mongocxx::client(this->uri);
            this->db = this->client["domotique_manager"];
            this->fromTime = utilities::DateTime::millisSinceEpoch()
        }
        void* run() {
            // Remove 1 item at a time and process it. Blocks if no items are 
            // available to process.
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                pollCommand();
            }
            return NULL;
        }

};
#endif /* !MONGOCOMMANDLISTENER */