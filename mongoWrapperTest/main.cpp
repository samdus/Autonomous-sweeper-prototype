#include "MongoWrapper.cpp"

int main(int argc, char** argv)
{
    MongoWrapper mongoWrapper;
	int test = 1;
    /*mongoWrapper.writeConsole("Test from raspberry", "warning");
    mongoWrapper.writeConsole("Test from raspberry2", "sucesss");
    mongoWrapper.writeConsole("Test from raspberry3", "info");
    mongoWrapper.writeConsole("Test from raspberry4", "error");
    mongoWrapper.addUpdate("working",  false);
mongoWrapper.addUpdate("leftWheel",  test);
mongoWrapper.addUpdate("rightWheel",  test);
mongoWrapper.addUpdate("kinectPolling",  test);*/
mongoWrapper.addUpdate("currentlyIn",  "raspberry");
mongoWrapper.addUpdate("currentlyIn",  "raspberry2");
    // Ctrl-C to end program
	
    printf("Enter Ctrl-C to end the program...\n");
    while (1);
    exit(0);
}