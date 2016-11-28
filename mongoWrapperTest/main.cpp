#include "MongoWrapper.cpp"

int main(int argc, char** argv)
{
    MongoWrapper mongoWrapper;
    std::vector<segment> map;
	/*int test = 1;
    mongoWrapper.writeConsole("Test from raspberry", "warning");
    mongoWrapper.writeConsole("Test from raspberry2", "sucesss");
    mongoWrapper.writeConsole("Test from raspberry3", "info");
    mongoWrapper.writeConsole("Test from raspberry4", "error");
    mongoWrapper.addUpdate("working",  false);
    mongoWrapper.addUpdate("leftWheel",  test);
    mongoWrapper.addUpdate("rightWheel",  test);
    mongoWrapper.addUpdate("kinectPolling",  test);*/
    segment seg;
    seg.fromString("150 100 200 25 0");
    map.push_back(seg);
    seg.fromString("200 25 200 150 0");
    map.push_back(seg);
    seg.fromString("200 150 150 100 0");
    map.push_back(seg);
    seg.fromString("50 75 150 150 175");
    map.push_back(seg);
    seg.fromString("0 0 150 150 0");
    map.push_back(seg);
    mongoWrapper.addUpdate("currentlyIn",  "raspberry8");
    mongoWrapper.writeMap(map, 20 , 20);


    // Ctrl-C to end program
    MongoCommand* itema  =mongoWrapper.getCommand();
    MongoCommand* item = mongoWrapper.getBlockingCommand();
	
    switch(item->m_commandInfo.thecommand){
        case MANUAL:
            printf("test MANUAL\n");
        break;
        case GOTO:
            printf("test GOTO\n");
        break;
        case CLOSE:
            printf("test close\n");
        break;
        case STARTDEBUG:
            printf("test STARTDEBUG\n");
        break;
        case STOPDEBUG:
            printf("test STOPDEBUG\n");
        break;
        case SCAN:
            printf("test SCAN\n");
        break;
        case TURN:
            printf("test TURN\n");
        break;
        case AUTOMATIC:
            printf("test AUTOMATIC\n");
        break;
        case TAKEPHOTO:
            printf("test TAKEPHOTO\n");
        break;
         case INVALID:
            printf("test INVALID\n");
        break;
    }

    printf("Enter Ctrl-C to end the program...\n");
    while (1);
    exit(0);
}