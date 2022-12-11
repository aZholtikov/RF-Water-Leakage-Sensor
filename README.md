# RF water leakage sensor

Water leakage sensor on ATmega168/328 + RF24.

## Features

1. Power consumption in sleep mode approximately 10 μA. Up to 2 years of operation on one CR2450 battery (estimated).
2. Transmits battery level charge and sensor status (DRY) every hour (can be changed up to 65535 seconds).
3. When triggered transmits an alarm (ALARM) every minute. The sensor must be reset to stop transmitting.
4. Automatic restart in case of a hang-up.

## Note

A gateway is required. For details see "RF Gateway".

## Full config example for Home Assistant

```yml
mqtt:
  sensor:
    - name: "NAME"
      device_class: "voltage"
      unit_of_measurement: "V"
      state_topic: "homeassistant/rf_sensor/ID/water_leakage"
      value_template: "{{ value_json.battery }}"
      expire_after: 4500
      force_update: true
      qos: 2
  binary_sensor:
    - name: "NAME"
      device_class: "moisture"
      state_topic: "homeassistant/rf_sensor/ID/water_leakage"
      value_template: "{{ value_json.state }}"
      payload_on: "ALARM"
      payload_off: "DRY"
      expire_after: 4500
      force_update: true
      qos: 2
```
