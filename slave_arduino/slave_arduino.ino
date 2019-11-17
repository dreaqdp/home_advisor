

// ultrasound
#define trig 11
#define echo 12
#define out 10
#define dist_thresh 10

// ultrasound
long duration;
float dist, err, ef;

void setup () {
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(out, OUTPUT);

    Serial.begin(115200);
}


bool doorOpen(){
    // Clear trig pin
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    // trigger signal
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);

    // Reading the echo pin
    duration = pulseIn(echo, HIGH);
    dist = (float)duration/58;
    Serial.print(dist);

    if ( dist <= dist_thresh ){
        Serial.println("Door is openned");
    }else{
        Serial.println("Door is closed");
    }
    return dist <= dist_thresh; 
}

void loop () {
    // Clear trig pin
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    // trigger signal
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);

    // Reading the echo pin
    duration = pulseIn(echo, HIGH);
    dist = (float)duration/58;
    Serial.print(dist);

    if (dist <= dist_thresh){
        Serial.println("Door is openned");
    }else{
        Serial.println("Door is closed");
    }
    digitalWrite(out, dist <= dist_thresh);
    


}
