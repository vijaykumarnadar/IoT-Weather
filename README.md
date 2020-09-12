[![Smart Distributed IoT Weather Monitoring System Using NodeMCU](https://content.instructables.com/ORIG/FMY/NORI/KBDNLXSV/FMYNORIKBDNLXSV.jpg?auto=webp&frame=1&width=933&height=1024&fit=bounds&md=3938d4a65d0b4317a2e019bbd55975ae)](https://content.instructables.com/ORIG/FMY/NORI/KBDNLXSV/FMYNORIKBDNLXSV.jpg?auto=webp&frame=1&width=1024&height=1024&fit=bounds&md=3938d4a65d0b4317a2e019bbd55975ae)

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

Solder All the Components and Upload the Program to the NodeMCU.
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

Configuring the SQL Server
----------------------------------

[![Configuring the SQL Server](https://content.instructables.com/ORIG/F3U/92NZ/KBF2Z16Z/F3U92NZKBF2Z16Z.png?auto=webp&frame=1&width=1024&fit=bounds&md=039bfa7d524895b655277f9c5c633d4d)](https://content.instructables.com/ORIG/F3U/92NZ/KBF2Z16Z/F3U92NZKBF2Z16Z.png?auto=webp&frame=1&width=1024&fit=bounds&md=039bfa7d524895b655277f9c5c633d4d)

The SQL Server setup is also really simple. Just create a database in SQL server and Import the setting by importing the file named "database_structure.txt". You can find the file in this step. As the instructable doesn't allow to upload ".sql" files, I've renamed the file to ".txt".

> Note: Rename the file from ".txt" to ".sql".

Configuring the File Server
-----------------------------------

Configuring the server is really easy if you own a website and it is hosted online. I won't go through the whole procedure of setting up a website and hosting it since it is beyond the scope of this tutorial. But you can host it in your own pc as localhost to try the working of the files.

Since the Instructable doesn't allow to upload PHP files, I've renamed the files to ".txt".

> Note: Please rename the extension of the files to ".php".

> Also don't forget to change the credentials of the "config.php" file.

Just upload the files to the server and you're good to go.

I'll give you brief information about the PHP files.

-   db_config.php:

In this file, all the credentials required to connect to the SQL server is stored.

-   db_connect:

In this file the class needed for database connection is present.

-   insert.php:

The NodeMCU calls this PHP file for uploading the data to the server using the GET method. This file is also responsible to store the same data to the SQL server.

-   retrieve.php:

The User/Client calls this PHP using the GET method. The server calculates the distance between the user and all the modules. Then the data of the nearest module is sent as a response to the client in JSON/XML format as preferred by the client.

-   update.php:

This PHP file is called by the module every day at a specific time to check whether the module is running the latest version of the firmware. Just place the latest ".bin" file in the file server and specify the directory of the file in the variable of the file.

If these many files seems daunting at first, I've included the user documentation in the next step.

User Documentation
--------------------------

[![User Documentation](https://content.instructables.com/ORIG/FWS/7AZJ/KBF2ZBYU/FWS7AZJKBF2ZBYU.png?auto=webp&frame=1&fit=bounds&md=8158583efc33feeed1fea77bd48e30f3)](https://content.instructables.com/ORIG/FWS/7AZJ/KBF2ZBYU/FWS7AZJKBF2ZBYU.png?auto=webp&frame=1&fit=bounds&md=8158583efc33feeed1fea77bd48e30f3)

[![User Documentation](https://content.instructables.com/ORIG/F4T/RWB2/KBF2ZBZU/F4TRWB2KBF2ZBZU.png?auto=webp&frame=1&fit=bounds&md=7d178e99b29b7156b243fba4fd16a196)](https://content.instructables.com/ORIG/F4T/RWB2/KBF2ZBZU/F4TRWB2KBF2ZBZU.png?auto=webp&frame=1&fit=bounds&md=7d178e99b29b7156b243fba4fd16a196)

Introduction:

The Weather API provides a simple interface to request the weather data for locations on the surface of the earth. You request the weather information for a specific latitude/longitude pair with the output format specified. The API returns the temperature, humidity, pressure, and Air quality index that was last recorded by the nearest module from the requested location.

Before you begin:

This document is intended for website and mobile developers who want to include weather information on an application that is being developed. It introduces the usage using the API and reference material on the available parameters.

Weather Data Requests:

Weather API requests are constructed as a URL string. The API returns weather data for a point on the earth, specified by a latitude/longitude pair. Note that the weather data accuracy is directly proportional to the density of the modules placed in an area.

A Weather API request takes the following form:

https://example.com/retrieve.php?lat=25.96446&lon=53.9443&format=json

Where output format(format) may be either of the following values:

-   JSON (recommended), indicates output in JavaScript Object Notation (JSON); or
-   XML, indicates output in XML, wrapped within the node.

Request Parameters:

As is standard in all URLs, parameters are separated using the ampersand (&) character. The list of parameters and their possible values are denoted below.

Required Parameters:

-   lat: Representing a latitude of a location to lookup. (e.g. lat=19.56875)
-   lon: Representing a longitude of a location to lookup. (e.g. lon=72.97568)

Optional Parameters:

-   format: Specifies the response output format of the weather data. It can be either JSON or XML. The default is JSON. (e.g. format=json or format=xml)

Weather Responses:

For each valid request, the time zone service will return a response in the format indicated within the request URL. Each response will contain the following elements:

-   success: a value indicating the status of the response.
    -   0: Negative; indicates that the request was malformed.
    -   1: Affirmative; indicates that the request was successful.
-   message: a string indicating the reason for the malformity of the request. Only available when the status is negative.
-   data: an array with multiple weather parameters.
    -   temp: the temperature data.
    -   hum: the humidity presence data.
    -   pres: the absolute pressure data.
    -   aqi: the present Air quality index.

> The examples response of both formats can be seen in the images.

Module Setup
--------------------

[![Module Setup](https://content.instructables.com/ORIG/FY1/ZRUW/KBF2ZL18/FY1ZRUWKBF2ZL18.png?auto=webp&frame=1&width=423&fit=bounds&md=9f46d1e149af9151970ee2354c37d88c)](https://content.instructables.com/ORIG/FY1/ZRUW/KBF2ZL18/FY1ZRUWKBF2ZL18.png?auto=webp&frame=1&fit=bounds&md=9f46d1e149af9151970ee2354c37d88c)

[![Module Setup](https://content.instructables.com/ORIG/F6N/8OZR/KBFVC82Y/F6N8OZRKBFVC82Y.png?auto=webp&frame=1&crop=3:2&width=777&fit=bounds&md=c9adc451c692bc7f6b84d7a599e5a0fa)](https://content.instructables.com/ORIG/F6N/8OZR/KBFVC82Y/F6N8OZRKBFVC82Y.png?auto=webp&frame=1&width=1024&fit=bounds&md=c9adc451c692bc7f6b84d7a599e5a0fa)

An Access-point is created and a webpage is hosted on an IP address(Default:192.168.4.1) to receive the credentials from the device manager/user on the very first boot or if the module does not find the already stored credentials in the EEPROM.

The user needs to enter the SSID and password to which the user wants the module to connect. The latitude and longitude gets automatically filled if you allow the browser the access the location.

Once all the details are entered, click on the "SEND" button, and then all the credentials are written in the EEPROM of the module.

This step is very crucial since while mass-producing the modules, it is not feasible to program all the modules with its exact location data and WiFi credentials. Also, it is not advisable to hard-code the credentials in the program since if at all we need to relocate the module to some other location or want to change the WiFi credentials, We will need to reprogram the module. To avoid this hassle, the initial setup function is implemented.


Now Its Time to Contribute Data to the Cloud.
-----------------------------------------------------

[![Now Its Time to Contribute Data to the Cloud.](https://content.instructables.com/ORIG/F46/6LFY/KBFVC82D/F466LFYKBFVC82D.png?auto=webp&frame=1&crop=3:2&width=600&fit=bounds&md=f0be0368543d19861206a878e2540efc)](https://content.instructables.com/ORIG/F46/6LFY/KBFVC82D/F466LFYKBFVC82D.png?auto=webp&frame=1&width=1024&fit=bounds&md=f0be0368543d19861206a878e2540efc)

[![Now Its Time to Contribute Data to the Cloud.](https://content.instructables.com/ORIG/F8O/X6Y3/KBFVC82E/F8OX6Y3KBFVC82E.png?auto=webp&frame=1&crop=3:2&width=600&fit=bounds&md=14b78ccf36109dc6b4d6786d53fca547)](https://content.instructables.com/ORIG/F8O/X6Y3/KBFVC82E/F8OX6Y3KBFVC82E.png?auto=webp&frame=1&width=1024&fit=bounds&md=14b78ccf36109dc6b4d6786d53fca547)

After all the previous steps are completed, now its time to let the module to upload the data to the server. It automatically starts uploading once you've saved the credentials.

It calls the "insert.php" as an API call with passing all the parameters to send in GET method.

The below code snippet shows how the parameters are processed.
```c
 if (isset($_GET['temp']) && isset($_GET['hum']) && isset($_GET['pres']) && isset<br>($_GET['aqi']) && isset($_GET['mac']) && isset($_GET['lat']) && isset($_GET['lon
 ']))
 {
 // main program
 }
```
Like so all the modules starts uploading the data.

> Note: Lower the upload frequency in the code if you feel the server is getting overloaded.


Over the Air(OTA) Update
--------------------------------

[![Over the Air(OTA) Update](https://content.instructables.com/ORIG/FFH/9NBX/KBFVC9K5/FFH9NBXKBFVC9K5.png?auto=webp&frame=1&fit=bounds&md=e1364b5ccaa4ec642b4316997ea540b4)](https://content.instructables.com/ORIG/FFH/9NBX/KBFVC9K5/FFH9NBXKBFVC9K5.png?auto=webp&frame=1&fit=bounds&md=e1364b5ccaa4ec642b4316997ea540b4)

After the module is all set up and starts to upload the data, it checks for firmware updates every day at a specific time mentioned in the program. If it finds any, it downloads and flashes the binary file in it. And if it doesn't, the normal operation of uploading the data is continued.

To check for a new update, the module calls the "update.php" by sending the MAC address in its request header. The server then checks if that specific MAC address has any new update, if yes, then it sends the binary file of the latest firmware in response.

It also checks for all the necessary headers required for basic authentication of the module.


How User/Client Can Access the Data...
----------------------------------------------

[![How User/Client Can Access the Data...](https://content.instructables.com/ORIG/F4E/0JTI/KBFVCBS4/F4E0JTIKBFVCBS4.png?auto=webp&frame=1&crop=3:2&width=853&fit=bounds&md=92084d6ab76ebfdc9bd6a42c29e72ff8)](https://content.instructables.com/ORIG/F4E/0JTI/KBFVCBS4/F4E0JTIKBFVCBS4.png?auto=webp&frame=1&width=1024&fit=bounds&md=92084d6ab76ebfdc9bd6a42c29e72ff8)

[![How User/Client Can Access the Data...](https://content.instructables.com/ORIG/FQE/35DU/KBFVCBSV/FQE35DUKBFVCBSV.png?auto=webp&frame=1&width=347&fit=bounds&md=d510b9c2dd5457d4b07e7190b67510fe)](https://content.instructables.com/ORIG/FQE/35DU/KBFVCBSV/FQE35DUKBFVCBSV.png?auto=webp&frame=1&fit=bounds&md=d510b9c2dd5457d4b07e7190b67510fe)

[![How User/Client Can Access the Data...](https://content.instructables.com/ORIG/FF1/CW0D/KBFVCBS3/FF1CW0DKBFVCBS3.png?auto=webp&frame=1&width=347&fit=bounds&md=f124d3579704baa0c512081b5d73acbc)](https://content.instructables.com/ORIG/FF1/CW0D/KBFVCBS3/FF1CW0DKBFVCBS3.png?auto=webp&frame=1&fit=bounds&md=f124d3579704baa0c512081b5d73acbc)

It is pretty straightforward to access the data from the server. Just by calling the "retrieve.php", we'll get the weather data in response in JSON format. After that, it is just a matter of parsing the JSON data to access the individual elements. Similar is with XML response. The user can always specify the preferred format of response in which the user is comfortable to work with. If the user doesn't specify the format, the default format is JSON.

A sample request is made using POSTMAN tool to check the working of the API.

An example of parsing JSON response in javascript is shown in the code snippet below.
``` js
var url = "https://example.com/retrieve.php?lat=19.044848&lon=72.8464373";
function httpGet(theUrl)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", theUrl, false); // false for synchronous request
	xmlHttp.send( null );
	return xmlHttp.responseText;
}
var myVar = httpGet(url);
var obj = JSON.parse(myVar); 
document.getElementById("aqi").innerHTML = obj.data[0].aqi;
document.getElementById("temperature").innerHTML = Math.round(obj.data[0].temp) + "°C";
document.getElementById("temp").innerHTML = Math.round(obj.data[0].temp) + "°C";
document.getElementById("humidity").innerHTML = Math.round(obj.data[0].hum) + "%";
document.getElementById("pressure").innerHTML = Math.round(obj.data[0].pres) + " mb";
```

The source code of the example HTML page that parses the JSON response is available at the end of this step.

> Note: Change the extension of the file to ".html".

Limitations of This Project
-----------------------------------

-   The project uses GET to send the data; even though it is not dealing with sensitive data, the data can be easily manipulated since it doesn't have any mechanism to check the authenticity of the source apart from checking the headers, which can be easily modified and even a normal device can be spoofed to seem like a weather module.
-   Since the module solely relies and dependent on other access-point (WIFI) to send the data which in most of the case would be of other organizations. If at all the access-point is down on service for some reason, the module would not be able to send data.
-   Even though the project is built to increase the accuracy of the existing system, the sensor available in the market is less accurate than expected which in result leads to fails its main purpose.
-   While planning the project, I planned to include a mode in which the server averages the data value based on location for error correction. But upon implementing this feature, I realized that it needed some third-party APIs to translate the coordinates to geographic regions.


Further Improvements That Can Be Made to This Project.
---------------------------------------------------------------

-   The accuracy of the module can be further improved by specially tailoring the sensors for the specific purpose instead of using the generic module that is available in the market.
-   The module can be modified to work even more independently by using a special chip that wirelessly communicates with Cell-towers to send the data thus improving fault tolerance.
-   Solar panel and battery system can be used in conjunction with the deep-sleep mode of ESP thus improving the power efficiency and making it more independent from an external power supply.
-   POST can be used to send data with some authentication mechanism like using cyclic codes for every transmission of data.
-   Instead of NodeMCU, which is a prototyping board, we can use a custom microcontroller in mass-production which not only reduces the cost but also makes the best use of the system resources.
-   In conjunction with the Google geolocation API and connecting to any available open WIFI, the module can work without even configuring it; ready to transmit data out the factory with no setup needed whatsoever.
