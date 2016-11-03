#ifndef MONGOJOB
#define MONGOJOB
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "wqueue.cpp"
#include "thread.h"
typedef struct _jobinfo
{
    int intvalue;
    float floatvalue;
    string stringvalue;
    bool boolvalue;
    string level; // warning, sucesss, info, error, none 

    int jobtype; //1 update 2 write 3write console
    int valuetype;// 1 int  2float 3string 4bool
    string identifier;
} JobInfo;

class MongoJob
{
  public:
    JobInfo m_jobinfo;
    MongoJob(JobInfo thejobinfo) 
          :  m_jobinfo(thejobinfo) {}
    ~MongoJob() {}
 
    string getMessage() { return m_jobinfo.identifier; }
    int getNumber() { return m_jobinfo.jobtype; }
};
#endif /* !MONGOJOB */