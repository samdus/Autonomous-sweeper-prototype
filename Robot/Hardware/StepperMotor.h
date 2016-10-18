// ---------------------------------------------------------------------------
// Stepper Robot Library - v1.03 - 06/04/2014
//
// AUTHOR/LICENSE:
// Created by Francesco A. Perrotti - faperrotti@hotmail.com
// Copyright 2014 License: GNU General Public License v3
// http://www.gnu.org/licenses/
//
// LINKS:
// Project home: https://code.google.com/p/stepper-robot-arduino-lib/
// Wiki reference: https://code.google.com/p/stepper-robot-arduino-lib/wiki/Reference
// Playground page: http://playground.arduino.cc/Main/StepperRobot
//
// Em portugues: https://fperrotti.wikispaces.com/Stepper+Lib+Arduino
//
// DISCLAIMER:
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
// ---------------------------------------------------------------------------
//
// Esta biblioteca � parte do projeto de desenvolvimento de
// rob�s m�veis desenvolvido por Francesco A. Perrotti na
// Fatec Americana.
// Pode ser usada para fins comerciais ou pessoais livremente,
// apenas deixe citado o autor.
//

#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <Arduino.h>
#include "Software\IStepper.h"

class StepperMotor : IStepper
{
  public:
	StepperMotor();
	virtual void init(byte mPin_1, byte mPin_2, byte mPin_3, byte mPin_4);

	virtual void nextStep(char direction);
	virtual void motorPinsOut(byte pins);

  private:
	byte stepNumber;
	byte mPin_1, mPin_2, mPin_3, mPin_4;
};

#endif // !STEPPER_MOTOR_H
