#pragma once
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

class Window;

class EngineTime
{
public:
    static void initialize();
    static double getDeltaTime();
    static double getUnscaledDeltaTime();


    static void LogFrameStart();
    static void LogFrameEnd();

    static void setTimeScale(double scale);

private:
    EngineTime();
    ~EngineTime();

    EngineTime(EngineTime const&) {};
    EngineTime& operator=(EngineTime const&) {};


    static EngineTime* sharedInstance;

    chrono::system_clock::time_point start;
    chrono::system_clock::time_point end;

    double deltaTime = 0.0f;
    double timeScale = 1.f;


    friend class Window;


};