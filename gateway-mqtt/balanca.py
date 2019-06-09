import serial
import paho.mqtt.client as mqtt
import argparse

#open serial port
ser = serial.Serial("/dev/ttyACM0")
ser.baudrate = 115200

#MQTT callbacks
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    cli.subscribe("balanca/cmd",0)
    #cli.subscribe("balanca/massa",0)

def on_message(client, userdata, message):
    print("received message " + str(message.payload) + "topic: " + str(message.topic))
    if str(message.topic) == 'balanca/cmd':
        cmd = str(message.payload)[2:-1] + '\n';
        print("to send:" + cmd)
        ser.write( cmd.encode('utf-8'))

def on_log(client, userdata, level, buf):
    if (level == MQTT_LOG_ERR ) | (level == MQTT_LOG_WARNING):
        print("log:", buf)


cli = mqtt.Client(client_id="balancapi")
cli.on_connect = on_connect
cli.on_message = on_message
cli.on_log = on_log

cli.connect("localhost",8888, 120)

#starts background listener
cli.loop_start()
#cli.enable_logger()
cli.disable_logger()

while 1:
    mass  = ser.readline()
    #the array notation removes the bytearray and line termination
    msg = str(mass)[2:-5]
    print("mass: " + str(mass) + "sent: " + msg )
    cli.publish("balanca/massa",msg)
