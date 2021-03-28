Google Cloud IoT controlled water pump
===

## Commands

### Root folder commands

* `ping` publishes a pong under the subdirectory `test`
* `manual` puts the device into manual mode. Automatic pumping and sensor reporting is disabled.
* `auto` puts the device into automatic mode.
* `wifi` reports WiFi signal strength.
* `moisture` reports moisture sensor value.

### Subfolder commands

* `/pump(s)` turns on the pump for the specified duration in seconds.
* `/pump_seconds(s)` changes the pump duration in automatic mode to 
  the specified duration in seconds.
* `/moisture_seconds(s)` changes the interval which the moisture sensor 
  value is reported in seconds.
* `/moisture_threshold(n)` threshold for moisture sensor to start pumping.
* `/pump_check_seconds(s)` changes the interval in seconds for condition 
  checking in automatic mode. This determines the automatic pumping frequency.

