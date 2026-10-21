#include <string>
#include <cmath>

using namespace std;

//  Q1
class Sensor
{
    const int sensorId;
    double reading;
    static double calibrationOffset;

public:
    friend class Calibrator;

    // const sensorId is initialised in the initializer list
    Sensor(int id, double reading = 0.0) : sensorId(id), reading(reading) {}

    int getSensorId() const { return sensorId; }
    double getReading() const { return reading; }

    Sensor& setReading(double reading)
    {
        this->reading = reading;
        return *this;
    }

    static void setCalibrationOffset(double offset) { calibrationOffset = offset; }
    static double getCalibrationOffset() { return calibrationOffset; }
};

double Sensor::calibrationOffset = 0.0;

class Calibrator
{
public:
    double corrected(const Sensor& s) const
    {
        return s.reading - Sensor::calibrationOffset;
    }

    bool needsService(const Sensor& s) const
    {
        return corrected(s) > 100.0;
    }
};

// Q2
class Device
{
    const int deviceId;
    std::string hostname;
    int uptimeHours;
    static int devicesOnline;

public:
    friend bool needsReboot(const Device& d);

    Device(int id, const std::string& hostname = "", int uptime = 0)
        : deviceId(id), hostname(hostname), uptimeHours(uptime < 0 ? 0 : uptime)
    {
        ++devicesOnline;
    }

    // copies are devices too: keep the live count correct
    Device(const Device& other)
        : deviceId(other.deviceId), hostname(other.hostname), uptimeHours(other.uptimeHours)
    {
        ++devicesOnline;
    }

    ~Device() { --devicesOnline; }

    int getDeviceId() const { return deviceId; }
    const std::string& getHostname() const { return hostname; }
    int getUptime() const { return uptimeHours; }

    Device& setUptime(int hours)
    {
        if (hours >= 0)               // negative uptime rejected
            this->uptimeHours = hours;
        return *this;
    }

    static int getDevicesOnline() { return devicesOnline; }
};

int Device::devicesOnline = 0;

bool needsReboot(const Device& d)
{
    return d.uptimeHours > 720;
}

// Q3: Robot & Referee

class Robot
{
    const int robotId;
    double x;
    double y;
    static double stepLimit;

public:
    friend class Referee;

    Robot(int id, double x = 0.0, double y = 0.0) : robotId(id), x(x), y(y) {}

    int getRobotId() const { return robotId; }
    double getX() const { return x; }
    double getY() const { return y; }

    Robot& move(double dx, double dy)
    {
        if (dx > stepLimit)  dx = stepLimit;
        if (dx < -stepLimit) dx = -stepLimit;
        if (dy > stepLimit)  dy = stepLimit;
        if (dy < -stepLimit) dy = -stepLimit;
        this->x += dx;
        this->y += dy;
        return *this;
    }

    static void setStepLimit(double limit)
    {
        if (limit >= 0.0)             // negative limit rejected
            stepLimit = limit;
    }
    static double getStepLimit() { return stepLimit; }
};

double Robot::stepLimit = 5.0;

class Referee
{
public:
    bool inArena(const Robot& r) const
    {
        return std::fabs(r.x) <= 10.0 && std::fabs(r.y) <= 10.0;
    }

    std::string zone(const Robot& r) const
    {
        if (r.x == 0.0 || r.y == 0.0) return "AXIS";
        if (r.x > 0.0 && r.y > 0.0)   return "Q1";
        if (r.x < 0.0 && r.y > 0.0)   return "Q2";
        if (r.x < 0.0 && r.y < 0.0)   return "Q3";
        return "Q4";
    }
};

// Q4: Player & Coach
class Player
{
    const int playerId;
    int score;
    static int bonusPerLevel;

public:
    friend int levelOf(const Player& p);

    Player(int id, int score = 0) : playerId(id), score(score) {}

    int getPlayerId() const { return playerId; }
    int getScore() const { return score; }

    Player& addScore(int gain)
    {
        if (gain >= 0)                // negative gain rejected
            this->score += gain;
        return *this;
    }

    static void setBonusPerLevel(int bonus)
    {
        if (bonus > 0)                // zero / negative rejected
            bonusPerLevel = bonus;
    }
    static int getBonusPerLevel() { return bonusPerLevel; }
};

int Player::bonusPerLevel = 100;

int levelOf(const Player& p)
{
    return p.score / Player::bonusPerLevel;
}

// Q5: Thermostat & Climate Inspector
class Thermostat
{
    const int unitId;
    double target;
    static double deadband;

public:
    friend bool isComfortable(const Thermostat& t, double roomTemp);
    friend class ClimateInspector;

    Thermostat(int id, double target = 20.0) : unitId(id), target(target) {}

    int getUnitId() const { return unitId; }
    double getTarget() const { return target; }

    Thermostat& setTarget(double target)
    {
        this->target = target;
        return *this;
    }

    static void setDeadband(double band)
    {
        if (band >= 0.0)              // negative deadband rejected
            deadband = band;
    }
    static double getDeadband() { return deadband; }
};

double Thermostat::deadband = 1.0;

bool isComfortable(const Thermostat& t, double roomTemp)
{
    return std::fabs(roomTemp - t.target) <= Thermostat::deadband;
}

class ClimateInspector
{
public:
    double reportTarget(const Thermostat& t) const { return t.target; }
    bool isAggressive(const Thermostat& t) const { return t.target > 30.0; }
};

