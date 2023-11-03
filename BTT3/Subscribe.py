import pika
import json
connection = pika.BlockingConnection(
    pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.exchange_declare(exchange='logs', exchange_type='fanout')

result = channel.queue_declare(queue='', exclusive=True)
queue_name = result.method.queue

channel.queue_bind(exchange='logs', queue=queue_name)

print(' [*] Waiting for logs. To exit press CTRL+C')

def callback(ch, method, properties, body):
    data = json.loads( body)
    print('')
    print('id           :', data['id'])
    print('packet_no    :', data['packet_no'])
    print('temperature  :', data['temperature'])
    print('humidity     :', data['humidity'])
    print('tds          :', data['tds'])
    print('pH           :', data['pH'])
channel.basic_consume(
    queue=queue_name, on_message_callback=callback, auto_ack=True)

channel.start_consuming()