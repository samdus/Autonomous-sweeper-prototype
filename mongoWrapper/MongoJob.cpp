#ifndef MONGOJOB
#define MONGOJOB
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../lib/Segment.h"
#include "../lib/Vectors.h"
#include <vector>
typedef struct _jobinfo
{
    int intvalue;
    float floatvalue;
    std::string stringvalue;
    bool boolvalue;
    std::string level; // warning, sucesss, info, error, none 

    int jobtype; //1 update 2 write 3write console 4setmap 5savefile
    int valuetype;// 1 int  2float 3string 4bool
    std::string identifier;
    
    std::vector<segment> themap;
    int roboty;
    int robotx;
} JobInfo;

class MongoJob
{
  public:
    JobInfo m_jobinfo;
    MongoJob(JobInfo thejobinfo) 
          :  m_jobinfo(thejobinfo) {}
    ~MongoJob() {}
 
    std::string getMessage() { return m_jobinfo.identifier; }
    int getNumber() { return m_jobinfo.jobtype; }
};
#endif /* !MONGOJOB */

