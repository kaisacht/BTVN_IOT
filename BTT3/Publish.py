import pika
import sys
import json

connection = pika.BlockingConnection(
    pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

data = {
    "id": 12,
    "packet_no": 126,
    "temperature": 40,
    "humidity": 60,
    "tds":1100,
    "pH": 5.0
}

json_data = json.dumps( data)
channel.exchange_declare(exchange='logs', exchange_type='fanout')
message = json_data
channel.basic_publish(exchange='logs', routing_key='', body=message)
data = json.loads( message)

print('')
print('id           :', data['id'])
print('packet_no    :', data['packet_no'])
print('temperature  :', data['temperature'])
print('humidity     :', data['humidity'])
print('tds          :', data['tds'])
print('pH           :', data['pH'])

connection.close()