#!/usr/bin/env python
from pyModbusTCP.client import ModbusClient
 
client = ModbusClient(host="192.168.43.115", port=502, auto_open=True, auto_close=True, timeout=10)
data2 = client.write_single_register(14, 2) # FUNCTIE 06 - Schrijven (enkele register) (register=14, waarde=1)
data = client.read_holding_registers(14, 5) # FUNCTIE 03 - Lees register (enkele register) (register=14, lengte=1)
print ("Waarde register: ", data[0])
client.close()