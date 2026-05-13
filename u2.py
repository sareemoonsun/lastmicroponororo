from machine import Pin
import network
import socket
import time

# Define GPIO pins
GPIO1 = Pin(22, Pin.OUT)
GPIO2 = Pin(23, Pin.OUT)

# Initialize GPIO states
GPIO1.value(0)  # OFF
GPIO2.value(0)  # OFF

GPIO1_state = "GPIO 1 is OFF"
GPIO2_state = "GPIO 2 is OFF"

#WiFi credentials
ssid = 'YOUR_SSID'
password = 'YOUR_PASSWORD'

wlan = network.WLAN(network.STA_IF)

#function to connect to Wi-Fi network
def cnctWifi():
    wlan.active(True)
    print('Attempting to connect to the network...')
    wlan.connect(ssid, password)        
    max_wait = 10
    while max_wait > 0 and not wlan.isconnected():
        max_wait -= 1
        print('waiting for connection...')
        time.sleep(1)
    
    # Manage connection errors
    if not wlan.isconnected():
        print('Network Connection has failed')
    else:
        print('Connected to the network successfully.')
        status = wlan.ifconfig()
        print( 'Enter this address in browser = ' + status[0] )

#HTML + CSS for webpage
html = """<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>MicroPython Web Server</title>
  <style>
    html {
      font-family: Arial;
      display: inline-block;
      margin: 0px auto;
      text-align: center;
    }
    
    h1 {
      font-family: Arial;
      color: #2551cc;
    }
    
    .button1,
    .button2 {
      -webkit-border-radius: 10;
      -moz-border-radius: 10;
      border-radius: 10px;
      font-family: Arial;
      color: #ffffff;
      font-size: 30px;
      padding: 10px 20px 10px 20px;
      text-decoration: none;
      display: inline-block;
      margin: 5px;
    }
    
    .button1 {
      background: #339966;
    }
    
    .button2 {
      background: #993300;
    }
  </style>
</head>

<body>
  <h1>MicroPython Web Server</h1>
  <p>%s</p>
  <p>
    <a href="/GPIO1/on"><button class="button1">GPIO 1 ON</button></a>
    <a href="/GPIO1/off"><button class="button2">GPIO 1 OFF</button></a>
  </p>
  <p>%s</p>
  <p>
    <a href="/GPIO2/on"><button class="button1">GPIO 2 ON</button></a>
    <a href="/GPIO2/off"><button class="button2">GPIO 2 OFF</button></a>
  </p>
</body>
</html>
"""
# Connect to Wi-Fi
cnctWifi()
    
# Set up socket for web server
addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
s = socket.socket()
s.setblocking(0)
s.bind(addr)
s.listen(1)

print('listening on', addr)

# Main loop for handling client requests
while True:
    if not wlan.isconnected():
        print("Connection failed. Trying to reconnect")
        wlan.disconnect()
        cnctWifi()
    if wlan.isconnected():
        try:
            cl, addr = s.accept()
            print('client connected from', addr)
            request = cl.recv(1024)
            print(request)

            request = str(request)
            GPIO1_on = request.find('/GPIO1/on')
            GPIO1_off = request.find('/GPIO1/off')
            GPIO2_on = request.find('/GPIO2/on')
            GPIO2_off = request.find('/GPIO2/off')

            if GPIO1_on == 6:
                print("GPIO 1 is on")
                GPIO1.value(1)
                GPIO1_state = "GPIO 1 is ON"

            if GPIO1_off == 6:
                print("GPIO 1 is off")
                GPIO1.value(0)
                GPIO1_state = "GPIO 1 is OFF"

            if GPIO2_on == 6:
                print("GPIO 2 is on")
                GPIO2.value(1)
                GPIO2_state = "GPIO 2 is ON"

            if GPIO2_off == 6:
                print("GPIO 2 is off")
                GPIO2.value(0)
                GPIO2_state = "GPIO 2 is OFF"

            response = html % (GPIO1_state, GPIO2_state)
            cl.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
            cl.send(response)
            cl.close()

        except:
            pass
    time.sleep(0.1)

ภาษาที่ใช้ในการเขียนโค้ด:  Python  ( python )
