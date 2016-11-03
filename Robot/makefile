#====================================================================================
#Compile les differentes librairies utiles pour la communication avec le port serie
#Compile aussi un projet de test
#====================================================================================
#Dans le cadre du cours de projet A16 - Equipe GSD
#====================================================================================
#Dependances :
#	Software:
#	   	- Aucune dependance
#	Serial_wjwwood:
#		- setupapi.lib
#	CommunicatorArduino:
#	ExempleCommunicationRobot:
#====================================================================================
all: ExempleCommunicationRobot.exe

#====================================================================================
#Run
#====================================================================================
run: rebootArduino
	./RaspberryBin/ExempleCommunicationRobot.exe

#====================================================================================
#Build et run
#====================================================================================
pull_build_et_run: pull all run

#====================================================================================
#Met a jour le projet depuis le repo git
#====================================================================================
pull:
	git pull
	
#====================================================================================
#Redemmare le Arduino
#Le Arduino redemmare tout seul lors de la premiere 
#====================================================================================
rebootArduino:	
	stty -F /dev/ttyACM0 -hupcl
	
#====================================================================================
#Compilation du projet ExempleCommunicationRobot
#====================================================================================

ExempleCommunicationRobot.exe: CommunicatorArduino.a
	g++ -std=c++11 `pkg-config --cflags libmongocxx` -lpthread -pthread -I Serial_wjwwood/include/ ExempleCommunicationRobot/main.cpp RaspberryBin/CommunicatorArduino.a RaspberryBin/Software.a RaspberryBin/Serial.a -o RaspberryBin/ExempleCommunicationRobot.exe

#====================================================================================
#Preparation
#====================================================================================
Prepare:
	mkdir -p RaspberryBin
	cd RaspberryBin && mkdir -p objs
	
#====================================================================================
#Compilation du projet CommunicatorArduino
#====================================================================================
CommunicatorArduino.a: CommunicatorArduino.obj
	ar rvs RaspberryBin/CommunicatorArduino.a RaspberryBin/Software.a RaspberryBin/Serial.a RaspberryBin/objs/CommunicatorArduino.o

CommunicatorArduino.obj: Software.a Serial.a
	g++ -std=c++11 -lpthread -pthread -I Serial_wjwwood/include/ -c CommunicatorArduino/ArduinoCommunicator.cpp RaspberryBin/Serial.a RaspberryBin/Software.a -o RaspberryBin/objs/CommunicatorArduino.o
	
#====================================================================================
#Compilation du projet Serial_wjwwood
#====================================================================================
Serial.a: Prepare Serial_wjwwood/src/serial.cc Serial_wjwwood/src/impl/win.cc Serial_wjwwood/src/impl/list_ports/list_ports_linux.cc
	cd Serial_wjwwood       && mkdir -p build
	cd Serial_wjwwood/build && cmake ..
	cd Serial_wjwwood/build && make
	cp Serial_wjwwood/build/libserial.a RaspberryBin/Serial.a

#====================================================================================
#Compilation du projet Software
#====================================================================================
Software.a: ControlleurPrincipal.obj
	ar rvs RaspberryBin/Software.a RaspberryBin/objs/CompassDriver.o RaspberryBin/objs/SonarDriver.o RaspberryBin/objs/StepperDriver.o RaspberryBin/objs/ControlleurPrincipal.o

ControlleurPrincipal.obj: Drivers.objs Software/ControlleurPrincipal.cpp
	g++ -std=c++11 -c Software/ControlleurPrincipal.cpp -o RaspberryBin/objs/ControlleurPrincipal.o

Drivers.objs: Prepare Software/CompassDriver.cpp Software/SonarDriver.cpp Software/StepperDriver.cpp
	g++ -std=c++11 -c Software/CompassDriver.cpp -o RaspberryBin/objs/CompassDriver.o
	g++ -std=c++11 -c Software/SonarDriver.cpp -o RaspberryBin/objs/SonarDriver.o
	g++ -std=c++11 -c Software/StepperDriver.cpp -o RaspberryBin/objs/StepperDriver.o

