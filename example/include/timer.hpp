#pragma once

#include <chrono>
#include <thread>
#include <atomic>
#include <functional>

class Timer
{
    std::thread worker;
    std::atomic<bool> active{false};
    std::function<void()> tCallback;
    int intervalMS;

public:
    Timer(int intervalMS = 1000) : intervalMS{intervalMS}, active{false} {}
    ~Timer() { stop(); }

    template <typename TOwner>
    void setCallback(TOwner *obj, void (TOwner::*callback)())
    {
        tCallback = [obj, callback]()
        { (obj->*callback)(); };
    }

    void stop()
    {
        active = false;
        if (worker.joinable())
            worker.join();
    }
    void start()
    {
        active = true;
        worker = std::thread([this]()
                             {
            while (active)
            {
                if (tCallback) tCallback();
                std::this_thread::sleep_for(std::chrono::milliseconds(intervalMS));
            } });
    }
};