#!/usr/bin/python

#usage:
#python3 websocket-python-client.py
 
import websocket
ws = websocket.WebSocket()
ws.connect("ws://localhost:8080")
ws.send("Hello, Server")
print(ws.recv())
ws.send("Hope you are doing good")
print(ws.recv())
ws.send("Why you are not talking??")
print(ws.recv())
ws.close()
