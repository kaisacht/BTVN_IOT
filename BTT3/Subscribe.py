import json
import pika, sys, os

def main():
    connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
    channel = connection.channel()

    channel.queue_declare(queue='hello')

    def callback(ch, method, properties, body):
        data = json.loads( body)
        print('')
        print('id           :', data['id'])
        print('packet_no    :', data['packet_no'])
        print('temperature  :', data['temperature'])
        print('humidity     :', data['humidity'])
        print('tds          :', data['tds'])
        print('pH           :', data['pH'])

    channel.basic_consume(queue='hello', on_message_callback=callback, auto_ack=True)

    print(' [*] Waiting for messages. To exit press CTRL+C')
    channel.start_consuming()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('Interrupted')
        try:
            sys.exit(0)
        except SystemExit:
            os._exit(0)