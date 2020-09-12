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
