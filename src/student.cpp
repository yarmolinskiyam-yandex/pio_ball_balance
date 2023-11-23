#include "student.h"

/**
 * @brief Инициализировать систему управления
 * @details Вызывется один раз перед вызовом
 * функции \p controllerTick
 */
void controllerInit (Overlord &over)
{
    over.setSlider (SliderEnum::prog1, -12000, 12000);
    over.setSlider (SliderEnum::prog2, -10000, 10000);
}

/**
 * @brief Выполнить одну итерацию системы управления
 * @details Вызывается раз в 5мс
 */
void controllerTick (Overlord &over)
{
    float setPoint = -over.getSetpoint ();
    // setPoint = over.getSlider (SliderEnum::prog2)*0.0001;
    // bool button = !digitalRead (13);
    // float v = voltageSlider.getValue () * button;

    static float constexpr K = 0.55;
    static float constexpr T = 0.04;
    static float constexpr Tu = 0.015;

    static float constexpr Tk = T;
    static float constexpr Kk = T / (2*K*Tu);

    static float constexpr ia = 9/2;
    static float constexpr g = 9.81;

    float Tum = Tu;

    static PID piVel (Kk, Kk/Tk, 0, 1, 0.006, -12, 12);
    Tum *= 2;
    static PID pPos (1 / (2*Tum), 0, 0, 1, 0.006, -0.1, 0.1);
    Tum = 2*Tum + 0.2;
    static PID pCarVel (ia / (2*g*Tum), 1, 0, 1, 0.006, -0.3, 0.3);
    Tum = 2*Tum;
    static PID pCarPos (1 / (2*Tum), 0, 0, 1, 0.006, -0.2, 0.2);
    
    float carX = -over.getCarX ();
    float carVel = -over.getCarVel ();
    
    float carVelSetpoint = pCarPos.tick (setPoint - carX);
    // carVelSetpoint = setPoint;
    float motorSetPoint = pCarVel.tick (carVelSetpoint - carVel);

    // motorSetPoint = setPoint;

    float motorAngle = over.getMotorTheta ();
    float motorVel = over.getMotorVel ();
    float u = piVel.tick (pPos.tick (motorSetPoint - motorAngle) - motorVel);

    over.setMotorU (u);

    // Serial.print (setPoint);
    // Serial.print ('\t');
    // // Serial.print (carX);
    // // Serial.print ('\t');
    // // Serial.print (carVel);
    // // Serial.print ('\t');
    // Serial.print (motorAngle);
    // Serial.print ('\t');
    // Serial.print (motorVel);
    // Serial.print ('\t');
    // Serial.print (u);
    // Serial.print ('\t');
    // Serial.println ();
}
