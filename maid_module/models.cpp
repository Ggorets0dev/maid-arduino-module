#include "models.h"
#include <Arduino.h>

namespace Models
{
    Wheel::Wheel(byte spokes, unsigned short size_mm) { count_of_spokes = spokes; wheel_circumference_mm = size_mm; }
    byte Wheel::GetSpokesCount() { return count_of_spokes; }
    unsigned short Wheel::GetWheelCircumference() { return wheel_circumference_mm; }

    void BluetoothAdapter::TransferMessage(char prefix, String msg)
    {
        String command = " !" + msg + ";";
        command[1] = prefix;
        Serial.println(command);
    }
    void BluetoothAdapter::TransferRecords(float speed, float voltage, char prefix='#')
    {
        String command = " !" + String(speed, 2) + "_" + String(voltage, 2) + ";";
        command[1] = prefix;
        Serial.println(command);
    }

    void Speedometer::CountImpulse() { impulse_counter++; }
    void Speedometer::ResetCounter() { impulse_counter = 0; }
    float Speedometer::CalculateSpeed(byte time_spent_sec, Wheel wheel)
    {
        if (impulse_counter == 0) return 0.0;
        else 
        {
            float speed_kmh = ((float)impulse_counter / (float)wheel.GetSpokesCount() * (float)wheel.GetWheelCircumference() / 1000000) * (60 * 60 / (float)time_spent_sec);
            return speed_kmh;
        }
    }

    Voltmeter::Voltmeter(byte r1_kohm, byte r2_kohm) { R1_kohm = r1_kohm; R2_kohm = r2_kohm; }
    float Voltmeter::CalculateVoltage(int analog_read_result)
    {
        float vout = ((float)analog_read_result * 5.0) / 1024.0;
        float vin = vout / ((float)R2_kohm / ((float)R1_kohm + (float)R2_kohm));
        return vin;
    }

    void Signaler::TurnOnRightSignal() { digitalWrite(RIGHT_TURN_LAMP_PIN, HIGH); }
    void Signaler::TurnOffRightSignal() { digitalWrite(RIGHT_TURN_LAMP_PIN, LOW); }
    void Signaler::TurnOnLeftSignal() { digitalWrite(LEFT_TURN_LAMP_PIN, HIGH); }
    void Signaler::TurnOffLeftSignal() { digitalWrite(LEFT_TURN_LAMP_PIN, LOW); }
}