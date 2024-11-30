#ifndef TIMER_H
#define TIMER_H

#include <chrono>

// A class used to determine the amount of elapsed time that something takes.
// This class should be used like a stopwatch. Why not call it stopwatch then?
class Timer {
public:
    Timer() = default;
    
    // Start the timer.
    void start();

    // Stop the timer.
    void stop();

    // Returns the elapsed time between calls to stop and start.
    std::chrono::duration<double> get_elapsed_time();

    // Returns the previously recorded elapsed time.
    std::chrono::duration<double> get_prev_elapsed_time();

private:
    bool has_started_ = 0;
    std::chrono::time_point<std::chrono::steady_clock> begin_, end_;
    std::chrono::duration<double> prev_elapsed_time_;
};

#endif
