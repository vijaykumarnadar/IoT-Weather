# iotweather
You all might be aware of the traditional weather station; but have you ever wondered how it actually works? Since the traditional weather station is costly and bulky, the density of these stations per unit area is very less which contributes to the inaccuracy of the data. I'll explain you how: Suppose a station is located in the middle of a city and it is the only station that is located in 'x' meter radius, it can be easily biased if any pollution-causing agent is present in the vicinity of the station showing the entire 'x' meter radius area as polluted since that single station is responsible to determine the weather data of the whole area.

To overcome this problem, the density of the modules must be increased which is possible only if the modules are cheaper and takes a smaller footprint than the existing one.

This is the reason my proposed solution is the perfect solution for this problem, It costs less than $10 and also rests easily on my palm.

How it works...

There are 3 major parts of this project.

-   Device side:

The Device is an IoT module shown in the picture that sends the weather data to the server every 'x' interval of time. The data includes the actual weather data, the geographic location of the module; i.e. Its coordinates, its MAC address; to uniquely identify the device, the firmware version it is currently running on. The device-side comprises N-modules distributed across the area actively contributing data to the server.

-   Server-side:

As the name suggests, it is the centralized server that handles several operations like receiving the data from the modules and storing it in the database, updating the module with the latest firmware if it is running on an older version, sending the weather data to the client on request.

-   Client/User side:

It is the end-user who request the weather data from the server. The client sends the current location and based on the location, the server calculates the distance between the client and all the modules and sends the weather data of the nearest module to the client which is considered as accurate.

### Supplies:

-   NodeMCU (ESP8266-12E)

-   DHT11 (Humidity and temperature sensor)

-   BMP180 (Pressure and Temperature sensor)

-   MQ-135 (Air quality index sensor)

-   USB cable (to upload the program)

-   5 volt power supply

-   Capacitors (Optional: to be placed parallel to the powerline)

-   Arduino IDE (To debug and upload the program)

-   POSTMAN application (optional: to debug the API)

-   A Website (to host the PHP and MySQL server)

Step 1: Solder All the Components and Upload the Program to the NodeMCU.
------------------------------------------------------------------------

[![Solder All the Components and Upload the Program to the NodeMCU.](https://content.instructables.com/ORIG/FSC/CAFP/KBDNLVBQ/FSCCAFPKBDNLVBQ.png?auto=webp&frame=1&width=690&fit=bounds&md=ca74d868117bb3ad1df98ad9399b421c)](https://content.instructables.com/ORIG/FSC/CAFP/KBDNLVBQ/FSCCAFPKBDNLVBQ.png?auto=webp&frame=1&fit=bounds&md=ca74d868117bb3ad1df98ad9399b421c)

[![Solder All the Components and Upload the Program to the NodeMCU.](https://content.instructables.com/ORIG/FG0/8V8U/KBDNLVCD/FG08V8UKBDNLVCD.jpg?auto=webp&frame=1&width=510&height=1024&fit=bounds&md=5795bd24558b25fa0e5e9d7a8fb360b1)](https://content.instructables.com/ORIG/FG0/8V8U/KBDNLVCD/FG08V8UKBDNLVCD.jpg?auto=webp&frame=1&width=1024&height=1024&fit=bounds&md=5795bd24558b25fa0e5e9d7a8fb360b1)

Solder all the components to the NodeMCU as shown in the circuit diagram on a perf board. Also, solder a capacitor in parallel to the powerlines since the power surges during actively transmitting and receiving data.

Once the soldering work is done, upload the code provided in the file "code.c".

> Note: Don't forget to replace the credentials with your own credentials. Also place the file named "html_file.h" inside the arduino sketch folder.

> All the header files used in this project can be found [here](https://drive.google.com/drive/folders/1sbKtOpjxqjh4vP7X5cjhXm7t3kx_DNZg?usp=sharing)

### Features of the code:

Access Point: Since it is difficult to program every module with the credentials in mass production, the module hosts a webpage on its first boot to accept the credentials of the WiFi to which the modules have to connect and stores in the EEPROM for later use.

Once the credentials are configured, the NodeMCU checks the EEPROM for credentials and connects to the WiFi credentials present in the EEPROM.

After successfully connecting to the WiFi, the NodeMCU starts uploading the data to the server every 'x' interval of time, the data includes weather data, MAC address of the module, version of the firmware, geographic location of the device.

OTA update: The module also checks for new firmware update every day at a specific time specified in the code. This feature is useful since it is not possible for any manufacturer to go on and change the program of an individual module in case there are any changes to be made.

Watchdog Timer: Atlast there must be a way to recover itself without any human intervention if it gets stuck or crashes. This can be achieved by using the Watchdog timer. The way this works is: There is an Interrupt sub-routine that runs every second. The ISR increments the counter every time it executes and checks whether the counter has reached the maximum count. Once the counter reaches the maximum value, the module resets itself assuming it has crashed. On normal operation, the counter always gets reset before it reaches the maximum count.
