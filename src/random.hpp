#include <iostream>
#include <random>
#include <unordered_map>

enum class Direction {
    INVALID    = -1,
    UP         = 0, 
    DOWN       = 1, 
    LEFT       = 2,
    RIGHT      = 3,
    UP_RIGHT   = 4,
    UP_LEFT    = 5,
    DOWN_RIGHT = 6,
    DOWN_LEFT  = 7
};

// Generates a random number within the range [from, to].
inline int gen_random_num(int from, int to) {
    std::random_device rand_device_seed;
    static std::mt19937 rand_generator(rand_device_seed());
    std::uniform_int_distribution<std::mt19937::result_type> distrib(from, to);
    return distrib(rand_generator);
}

inline Direction gen_random_direction() {
    const int random_num = gen_random_num(0, 7);
    return Direction(random_num);
}

inline Direction gen_random_horizontal_direction() {
    Direction random_direction = Direction::INVALID;

    // Discard directions that are not horizontal.
    while(random_direction != Direction::LEFT || random_direction != Direction::RIGHT) {
        random_direction = gen_random_direction();
    }
    return Direction(random_direction);
}

inline Direction gen_random_weighted_vertical_direction(int up_weight, int up_left_weight, int up_right_weight) {
    std::unordered_map<Direction, int> weight_of;
    weight_of[Direction::UP]       = up_weight;
    weight_of[Direction::UP_LEFT]  = up_left_weight;
    weight_of[Direction::UP_RIGHT] = up_right_weight;

    int weighted_total = up_weight + up_left_weight + up_right_weight;
    int random_num = gen_random_num(1, weighted_total);
    int running_total = 0;
    
    for(auto &it: weight_of) {
        running_total += it.second;

        if(running_total > random_num)
            return it.first;
    }
}
