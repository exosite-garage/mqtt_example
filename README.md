# About Murano MQTT C Sample

This example shows how to create a basic connection to the Exosite Murano Platform using MQTT and the open source paho-mqtt library.

Note: A free account and product at Murano is required: http://info.exosite.com/platform-sign-up 

License is BSD, Copyright 2018, Exosite LLC (see LICENSE file)

Tested on Ubuntu 17.10

## Help
Troubleshooting information and help can be in the Exosite Community Forum.

[Community Forum](https://community.exosite.com/)



## Installing

First install the paho-mqtt library and compile on your system. Ensure you've got git and a c compiler installed.

```
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make
sudo make install
```

Once the paho-mqtt library is installed you can go ahead and compile this example:

```
make
./publish
```

## Updating the Example to your Product and Device

In this configuration the paho-mqtt example is sending data to a pre-created product instance on Exosite Murano. To change it to your own instance open publish.c and change the constants to point to your product id and a valid device token:

```
#define ADDRESS     "ssl://<YOUR_PRODUCT_ID_HERE>.m2.exosite.io:443"
#define TOKEN       "<YOUR_DEVICE_TOKEN_HERE>"
```

