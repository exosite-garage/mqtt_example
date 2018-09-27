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

**NOTE:** Some errors may occur during installation.

Once the paho-mqtt library is installed you can go ahead and compile this example:

## Configuration

In order to use the `activate` and `pubsub` examples, you'll first need to set some `#defines` in the respective `.c` files.

### Murano Hostname

Open the Murano Product web UI and click the link that the image (below) is pointing to. This copies the Murano Product hostname into your clipboard.

![Finding the Murano hostname](find-product-hostname.png)

In both `activate` and `` examples, you will trim some characters from this URL to define the MQTT secure host to Murano.

Example using hostname `mqtt://f5330e4s8cho0000.m2.exosite.io/`:

```
#define ADDRESS     "ssl://f5330e4s8cho0000.m2.exosite.io:8883"
```

**NOTE:** This example uses port `8883`. If you changed this to `443` in your Murano Product Settings, then do so in the `ADDRESS` as well.

## Activate Example

The example code in `activate.c` illustrates how to get a device provisioned onto your Murano Product and retrieve its secure token. The token printed in the output should be stored in non-volatile memory on your device. Once this token is retrieved/activated, it is used in all further connections to Murano.

First, edit the `ADDRESS` (described above) and `ACTIVATE_TOPIC` in `activate.c`. The `<id>` is typically your device serial number.

```
#define ADDRESS             "ssl://<hostname>:<port>"
#define ACTIVATE_TOPIC      "$provision/<id>"
```

Next, compile the `activate` binary.

```
make activate
```

Finally, execute the `activate` binary to retrieve your device's token.

Example:

```
$ make activate
gcc activate.c -Wall -g -lpaho-mqtt3cs -o activate
$ ./activate
connecting...
publishing activation request...
sleeping until murano responds with token...
Message with token value 1 delivery confirmed
Provisioning successful.
Murano token: Ed9GHsPnc2gyZsOFoShPhxj80sRlzH9E1vZCTK9y
```

The last line of output, above, contains the Murano token for further connections.

## Pubsub Example

The example code in `pubsub.c` illustrates how to publish arbitrary data to your Murano Product's `data_in` resource while simultaneously monitoring all other Murano resources for new data.

First, specify the Murano token from the [activate step](#activate-example) and the same `ADDRESS`.

```
#define ADDRESS             "ssl://<hostname>:<port>"
#define TOKEN               "<YOUR_MURANO_TOKEN>"
```

Next, compile the `pubsub` binary.

```
make pubsub
```

Finally, execute the `pubsub` binary.

In the example, below, randomized JSON objects are published to `data_in` and a message is received from the Murano Product resource `data_out` after a few seconds.

```
$ make pubsub
gcc pubsub.c -Wall -g -lpaho-mqtt3cs -o pubsub
$ ./pubsub
Message with delivery token 1 delivered: {"001": 8}
Message with delivery token 2 delivered: {"001": 87}
Message with delivery token 3 delivered: {"001": 10}
Message with delivery token 4 delivered: {"001": 21}
Message with delivery token 5 delivered: {"001": 27}
Message with delivery token 6 delivered: {"001": 49}
Message with delivery token 7 delivered: {"001": 10}
Message with delivery token 8 delivered: {"001": 10}
Message with delivery token 9 delivered: {"001": 19}
Message arrived
     topic: $resource/data_out/1538080072997000
   message: reboot
Message with delivery token 10 delivered: {"001": 47}
Message with delivery token 11 delivered: {"001": 48}
Message with delivery token 12 delivered: {"001": 60}
Message with delivery token 13 delivered: {"001": 97}
^C
```
