# RF water leakage sensor

Water leakage sensor on ATmega168/328 + nRF24.

## Features

1. Power consumption in sleep mode approximately 10 μA. Up to 2 years of operation on one CR2450 battery (estimated).
2. Transmits battery level charge and sensor status (DRY) every hour (can be changed up to 65535 seconds).
3. When triggered transmits an alarm (ALARM) every minute. The sensor must be reset to stop transmitting.
4. Automatic restart in case of a hang-up.

## Note

A gateway is required. For details see [RF Gateway](https://github.com/aZholtikov/RF-Gateway).

Any feedback via [e-mail](mailto:github@zh.com.ru) would be appreciated. Or... [Buy me a coffee](https://paypal.me/aZholtikov).
