# Roland Smart Tally client for M5StickC
Roland Smart Tally client for M5Stick-C is created to read Tally data from Roland Smart Tally compatible video switchers.
The bus and devicenames are hard coded in the .ino file, so each M5StickC should be loaded with it's own bus number and name corresponding with the bus ut will be used for.

## Installing Sketch and Libraries
1. Follow the [tutorial on the M5Stack website](https://docs.m5stack.com/#/en/arduino/arduino_development) to download, install, and configure the Arduino IDE program. This is necessary to compile the code for your device.
1. Once you have the Arduino IDE installed and configured for your OS, install the following libraries (if not already installed):
	* `M5StickC` / `M5StickCPlus` (depending on the device you are using)
	* `HTTPClient` by MAdrian McEwen
	* `MultiButton` by Martin Poelstra

These will have to be included with the sketch file in order for it to compile properly.
## Compile and Upload the Sketch to the Device
1. Once all libraries are downloaded, open the `SmartTallyClient-m5stickc.ino` file in the Arduino IDE.
1. Modify these lines at the top of the file to reflect your wireless network, the Roland Smart Tally ip address and bus and the name of the camera:
	```c++
	//Wifi SSID and password
	const char * networkSSID = "YourNetwork";
	const char * networkPass = "YourPassword";

	//RST variables
	String TallyUrl = "http://192.168.100.240/tally/1/status"; //DeviceID's for XS62-S 1 tot 6
	String DeviceName = "CAM 1";
	int waittime = 100; //waittime between http get's
	```
1. Save the file.
1. Connect your M5Stick-C device to the computer via the provided USB-C cable.
1. If not already on, power the device on by holding down the power button (located on the bottom left-hand side) for a couple seconds.
1. Go to Tools > Board > ESP32 Arduino > and choose `M5Stick-C`. If it's not listed, you may need to install it through the Boards Manager.
1. Go to Tools > Upload Speed > and choose `750000` (one less from the maximum speed).
1. Go to Tools > Port > and choose the serial port that represents your device.
1. Go to Sketch > and choose `Upload`. The code will compile and upload to the device.

Once the code is successfully compiled and uploaded to the device. the M5Stick-C will boot up and automatically try to connect to your Roland Smart Tally server. It will be assigned to the hard coded bus/device in the .ino file.

## Using the Device
When you turn on the M5Stick-C device after it has been programmed, it will automatically connect to the wireless network using the settings provided, and then initiate a connection to the programmed Roland Smart Tally compatible device. If the Roland device is offline or the M5StickC has been programmed with the wrong url, the M5StickC will flash the screen and red led intermittently for a few times and show the text "Error" in a pink screen..

If you press the `M5` button on the device, you can toggle between the tally window display and the Settings window, which shows the IP address and the url from which it wil try to get the tally data.

If you press the `action` button on the right side of the device, you can adjust the brightness.


# Improvements and Suggestions
It's my first project on github. If you have any suggestions or improvements please let me know.

## Thanks
Thank you to [Guido Visser](https://github.com/guido-visser) and [Joseph D. Adams](https://github.com/josephdadams), inspiration for this client came from their project, respectively [vMix M5Stick-C Tally Light](https://github.com/guido-visser/vMix-M5Stick-Tally-Light) and [TallyArbiter-M5StickCListener](https://github.com/josephdadams/tallyarbiter-m5stickclistener).