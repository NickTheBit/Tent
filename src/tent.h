#include <iostream>
#include <Arduino.h>

#define FILENAME "config.dat"
// Serialize deserialize

class Tent {
    public:
        int id;
        int up_relay;
        int down_relay;
        // speed is 0-100 / total_time
        float speed;
        int cur_position;
        
        Tent();
        int setstat(int);

    
};

Tent::Tent() {
    id = 0;
    up_relay = 0;
    down_relay = 0;
    cur_position = 0;
    speed = 0;
}
    
int Tent::setstat(int position) {
    Serial.begin(115200);
    float moving_time(0);
    if (position > cur_position) {
        moving_time = (cur_position - position) * speed;
        // goup(id,moving_time);
        cur_position = position;
        Serial.println("Moving up: ");
        Serial.println(moving_time);
    } else if (position < cur_position) {
        moving_time = (position - cur_position) * speed;
        // godown(id,moving_time);
        cur_position = position;
        Serial.println("Moving down: ");
        Serial.println(moving_time);
    } else {
        Serial.println("No movement");
        return 0;
    }
    return 1;
}
