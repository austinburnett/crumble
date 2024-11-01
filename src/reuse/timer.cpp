#include "timer.hpp"

void Timer::start() {
    begin_ = std::chrono::steady_clock::now();
    has_started_ = true;
}

void Timer::stop() {
    if(!has_started_)
        return;
    else
        has_started_ = false;

    end_ = std::chrono::steady_clock::now();
    prev_elapsed_time_ = (end_ - begin_);
}

std::chrono::duration<double> Timer::get_elapsed_time() {
    return (end_ - begin_);
}
std::chrono::duration<double> Timer::get_prev_elapsed_time() {
    return prev_elapsed_time_;
}
