#ifndef MONGOCOMMAND
#define MONGOCOMMAND
#include <stdio.h>
#include <stdlib.h>
#include <string>
typedef struct _commandInfo
{
    string command; // Manuel (stopacction goto close startdebug stopdebug scan turn x) Automatique (resume)
    int x;
    int y;
} CommandInfo;

class MongoCommand
{
  public:
    CommandInfo m_commandInfo;
    MongoCommand(commandInfo thecommandInfo) 
          :  m_commandInfo(thecommandInfo) {}
    ~MongoCommand() {}
 
    string getMessage() { return m_commandInfo.command; }
};
#endif /* !MONGOCOMMAND */

