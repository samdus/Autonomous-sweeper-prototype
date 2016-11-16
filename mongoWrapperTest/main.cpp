#include "MongoWrapper.cpp"

int main(int argc, char** argv)
{
    MongoWrapper mongoWrapper;
    std::vector<segment> map;
	int test = 1;
    /*mongoWrapper.writeConsole("Test from raspberry", "warning");
    mongoWrapper.writeConsole("Test from raspberry2", "sucesss");
    mongoWrapper.writeConsole("Test from raspberry3", "info");
    mongoWrapper.writeConsole("Test from raspberry4", "error");
    mongoWrapper.addUpdate("working",  false);
    mongoWrapper.addUpdate("leftWheel",  test);
    mongoWrapper.addUpdate("rightWheel",  test);
    mongoWrapper.addUpdate("kinectPolling",  test);*/
    map.push_back(segment(Vector2(150,100), Vector2(200,25), 0));
    map.push_back(segment(Vector2(200,25), Vector2(200,150), 0));
    map.push_back(segment(Vector2(200,150), Vector2(150,100), 0));
    map.push_back(segment(Vector2(50,75), Vector2(150,175), 0));
    map.push_back(segment(Vector2(0,0), Vector2(150,150), 0));
    mongoWrapper.addUpdate("currentlyIn",  "raspberry8");
    mongoWrapper.writeMap(map);


    // Ctrl-C to end program
	
    printf("Enter Ctrl-C to end the program...\n");
    while (1);
    exit(0);
}