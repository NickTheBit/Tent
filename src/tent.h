#include <Arduino.h>
#include <pthread.h>

#define FILENAME "config.dat"
// Serialize deserialize

void *goUp(void *);
void *goDown(void *);

struct parser {
    int up_relay;
    int down_relay;
    float time;
};

class Tent {
  public:
    int id;
    int up_relay;
    int down_relay;
    // speed is 0-100 / total_time
    float speed;
    int cur_position;

    Tent();
    void setstat(int);
};

Tent::Tent() {
    id = 0;
    up_relay = 0;
    down_relay = 0;
    cur_position = 0;
    speed = 0;
}

void Tent::setstat(int position) {
    Serial.begin(115200);
    pthread_t thread;
    float moving_time(0);

    // Only one argument can be passed, struct essential
    // TODO this is very weird, need to fix
    struct parser *args;
    args = new parser[1];
    args[0].down_relay = down_relay;
    args[0].up_relay = up_relay;

    if (position > cur_position) {
        moving_time = (position - cur_position) * speed;
        // Serial.print("Moving up: ");
        // Serial.println(moving_time);
        cur_position = position;
        Serial.println(moving_time);
        args[0].time = moving_time;
        pthread_create(&thread, NULL, goUp, &args[0]);
    } else if (position < cur_position) {
        moving_time = (cur_position - position) * speed;
        // Serial.print("Moving down: ");
        // Serial.println(moving_time);
        cur_position = position;
        args[0].time = moving_time;
        pthread_create(&thread, NULL, goDown, &args[0]);
    } else {
        Serial.println("No movement");
    }
}

void *goUp(void *data) {
    struct parser *t = (struct parser *)data;
    Serial.println("Up active");
    digitalWrite(t->up_relay, LOW);
    Serial.println(t->time);
    delay(t->time);
    digitalWrite(t->down_relay, HIGH);
    Serial.println("Up finished");
}

void *goDown(void *data) {
    struct parser *t = (struct parser *)data;
    Serial.println("Down active");
    digitalWrite(t->down_relay, LOW);
    delay(t->time);
    digitalWrite(t->down_relay, HIGH);
    Serial.println("Down finished");
}
