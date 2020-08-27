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
        Serial.print("Moving up: ");
        Serial.println(moving_time);
        goUp(moving_time);
        cur_position = position;
    } else if (position < cur_position) {
        moving_time = (cur_position - position) * speed;
        Serial.print("Moving down: ");
        Serial.println(moving_time);
        goDown(moving_time);
        cur_position = position;
    } else {
        Serial.println("No movement");
        return 0;
    }
    return 1;
}

int Tent::goUp(float time) {
    Serial.println("Up active");
    digitalWrite(up_relay, LOW);
    delay(time);
    digitalWrite(up_relay, HIGH);
    Serial.println("Up finished");
    return 0;
}

int Tent::goDown(float time) {
    Serial.println("Down active");
    digitalWrite(down_relay, LOW);
    delay(time);
    digitalWrite(down_relay, HIGH);
    Serial.println("Down finished");
    return 0;
}
