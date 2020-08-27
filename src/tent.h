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
    int goUp(float);
    int goDown(float);
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
        moving_time = (position - cur_position) * speed;
        Serial.println(moving_time);
        goUp(moving_time);
        cur_position = position;
        Serial.print("Moving up: ");
        Serial.println(moving_time);
    } else if (position < cur_position) {
        moving_time = (cur_position - position) * speed;
        goDown(moving_time);
        cur_position = position;
        Serial.print("Moving down: ");
        Serial.println(moving_time);
    } else {
        Serial.println("No movement");
        return 0;
    }
    return 1;
}

int Tent::goUp(float time) {
    pinMode(up_relay, OUTPUT);
    digitalWrite(up_relay, HIGH);
    delay(time);
    digitalWrite(up_relay, LOW);
    return 0;
}

int Tent::goDown(float time) {
    pinMode(down_relay, OUTPUT);
    digitalWrite(down_relay, HIGH);
    delay(time);
    digitalWrite(down_relay, LOW);
    return 0;
}
