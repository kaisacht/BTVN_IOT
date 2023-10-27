import json
from paho.mqtt import client as mqtt_client
from options import args_parser

args = args_parser()

broker = 'broker.hivemq.com'
port = 1883
topic = "/IOT/24"
client_id = '3ed958a5-7866-419d-818d-f4a6536682d8'

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

data = {
    "id": client_id,
    "packet_no": 126,
    "temperature": 40,
    "humidity": 60,
    "tds":1100,
    "pH": 5.0
}

json_data = json.dumps( data)

def publish(client):
    result = client.publish(topic, json_data)
    status = result[0]
    if status == 0:
        print(f"Send '{data}' to topic `{topic}`")
    else:
        print(f"Failed to send message to topic {topic}")

def run_publish():
    client = connect_mqtt()
    client.loop_start()
    publish(client)
    client.loop_stop()

def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        data = json.loads(msg.payload.decode())
        print('')
        print('id           :', data['id'])
        print('packet_no    :', data['packet_no'])
        print('temperature  :', data['temperature'])
        print('humidity     :', data['humidity'])
        print('tds          :', data['tds'])
        print('pH           :', data['pH'])
    client.subscribe(topic)
    client.on_message = on_message

def run_subcribe():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()

if __name__ == '__main__':
    if args.option == 'publish':
        run_publish()
    elif args.option == 'subscribe':
        run_subcribe()
