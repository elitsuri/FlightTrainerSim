// File: GlobalConfig.h
#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

class GlobalConfig {
public:
    static GlobalConfig& instance() {
        static GlobalConfig config;
        return config;
    }
    
    double updateRateHz() const { return m_updateRateHz; }
    double physicsTimeStep() const { return 1.0 / m_updateRateHz; }
    double maxAltitude() const { return m_maxAltitude; }
    double maxSpeed() const { return m_maxSpeed; }
    double gravity() const { return m_gravity; }
    bool useMetricUnits() const { return m_useMetric; }
    bool showDebugInfo() const { return m_showDebug; }
    int antiAliasingSamples() const { return m_aaSamples; }
    
    void setUpdateRate(double hz) { m_updateRateHz = hz; }
    void setUseMetric(bool metric) { m_useMetric = metric; }
    void setShowDebug(bool show) { m_showDebug = show; }

private:
    GlobalConfig() : m_updateRateHz(60.0), m_maxAltitude(50000.0), m_maxSpeed(1200.0)
        , m_gravity(9.81), m_useMetric(false), m_showDebug(false), m_aaSamples(4) {}
    GlobalConfig(const GlobalConfig&) = delete;
    GlobalConfig& operator=(const GlobalConfig&) = delete;
    
    double m_updateRateHz, m_maxAltitude, m_maxSpeed, m_gravity;
    bool m_useMetric, m_showDebug;
    int m_aaSamples;
};

#endif
