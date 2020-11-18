#ifndef MachineState_h
#define MachineState_h

class MotorState
{
public:

    enum ControlMode
    {
        kPctCtl=0,
        kVelCtl=1
    } controlMode;
    unsigned long controlTimestamp;
    int controlValue; // pct power [-100, 100]
    int currentValue; // for ramprate-based targeting
    int sensorValue;  // in control-mode-dependent units
    float rampRate;   // used to gradually move from currentValue to controlValue
                      // measured in dPct / ms

    MotorState()
    {
        controlMode = kPctCtl;
        controlTimestamp = 0;
        controlValue = 0;
        currentValue = 0;
        sensorValue = 0;
        rampRate = 1.0f; /* 1 pct per ms, -> 0->100% in 100ms */
    }

    void set(int val, unsigned long ts)
    {
        this->controlTimestamp = ts;
        this->controlValue = val;
        // don't update currentValue 'til update
    }

    void update(unsigned long ts) // read encoders
    {
        // apply ramp-rate (low pass filter on power-request change)
        if(this->controlValue != this->currentValue)
        {
            if(this->controlValue == 0)
                this->currentValue = 0; // force-stop (decelerate?)
            else
            {
                unsigned long dt = ts - this->controlTimestamp;
                int dval = this->controlValue - this->currentValue;
                int newval;
                if(dval > 0)
                {
                    newval = this->currentValue - dt * this->rampRate;
                    if(newval < this->controlValue)
                        newval = this->controlValue;
                }
                else
                {
                    newval = this->currentValue + dt * this->rampRate;
                    if(newval > this->controlValue)
                        newval = this->controlValue;
                }
                this->currentValue = newval;
            }
        }
    }

};

struct Rotation2d
{
    int angle; // measured in 10ths of degrees (0 - 3600).
    Rotation2d()
    {
        angle = 0;
    }
};

struct IMUState
{
    Rotation2d yaw;

    void update(unsigned long timestamp)
    {
        yaw.angle++;
        if(yaw.angle > 7200)
            yaw.angle = -7200;
    }
};

struct MachineState
{
    unsigned long timestamp;
    unsigned long lastchange;

    MotorState M1, M2;
    bool led;
    IMUState imu;
    char statebuf[100];

    void update(unsigned long now) // read sensors
    {
        this->timestamp = now;
        M1.update(now);
        M2.update(now);
        imu.update(now);
    }

    char const *serialize(char const *prefix)
    {
        snprintf(this->statebuf, 100, 
            "/%s?M1=%d&M2=%d&led=%d&yaw=%d&ts=%ld&",
            prefix, 
            this->M1.currentValue, this->M2.currentValue,
            this->led, this->imu.yaw.angle, this->timestamp);
        return this->statebuf;
    }
};

#endif