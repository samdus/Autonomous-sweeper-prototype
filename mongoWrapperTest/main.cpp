#include "MongoWrapper.cpp"

int main(int argc, char** argv)
{
    MongoWrapper mongoWrapper;
    mongoWrapper.writeConsole("Test from raspberry", "warning");
    mongoWrapper.writeConsole("Test from raspberry2", "sucesss");
    mongoWrapper.writeConsole("Test from raspberry3", "info");
    mongoWrapper.writeConsole("Test from raspberry4", "error");
    mongoWrapper.addUpdate("leftWheel",  2);
    mongoWrapper.addUpdate("rightWheel",  2);
mongoWrapper.addUpdate("kinectPolling",  2);
    mongoWrapper.addUpdate("working",  false);
    mongoWrapper.addUpdate("currentlyIn",  "RAPSBERRY");
    // Ctrl-C to end program

    printf("Enter Ctrl-C to end the program...\n");
    while (1);
    exit(0);
}