#ifndef MONGOCOMMAND
#define MONGOCOMMAND
#include <stdio.h>
#include <stdlib.h>
#include <string>
enum commandEnum{
    STOPACTION, GOTO, CLOSE, STARTDEBUG, STOPDEBUG, SCAN, TURN, RESUME, TAKEPHOTO
};
typedef struct _commandInfo
{
    std::string command; // Manuel (stopacction goto close startdebug stopdebug scan turn x) Automatique (resume)
    int x;
    int y;
    enum commandEnum thecommand;
} CommandInfo;

class MongoCommand
{
  public:
    CommandInfo m_commandInfo;
    MongoCommand(){

    }
    MongoCommand(CommandInfo thecommandInfo) 
          :  m_commandInfo(thecommandInfo) {}
    ~MongoCommand() {}
 
    std::string getMessage() { return m_commandInfo.command; }
};
#endif /* !MONGOCOMMAND */

