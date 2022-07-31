package com.example.arduinocontrol;


import android.content.Context;

import androidx.appcompat.app.AppCompatActivity;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.DisconnectedBufferOptions;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttTopic;

import static android.provider.ContactsContract.CommonDataKinds.Website.URL;

public class mqttClient extends AppCompatActivity {

    MqttAndroidClient myPhonemqtt;


    String clientID = "Jack's Phone";
    final String serverURL = "tcp://farmer.cloudmqtt.com:16616";
    final String pubDigitalTopic = "AndroidPub/Digital";
    final String pubAnalogTopic = "AndroidPub/Analog";
    final String subDigitalTopic = "ArduinoPub/Digital";
    final String subAnalogTopic = "ArduinoPub/Analog";
    final String userName = "ukwycffo";
    final String password = "UsIktYMq-F1Z";
    int qos = 1;

    public mqttClient(Context context) {

        myPhonemqtt = new MqttAndroidClient(context, serverURL, clientID);
        myPhonemqtt.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {

            }

            @Override
            public void connectionLost(Throwable cause) {

            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {

            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {

            }
        });

        connectToMQTTServer();

    }

    public void setCallback(MqttCallbackExtended callback) {
        myPhonemqtt.setCallback(callback);
    }

    private void connectToMQTTServer(){

        MqttConnectOptions mqttConnectOptions = new MqttConnectOptions();
        mqttConnectOptions.setAutomaticReconnect(true);
        mqttConnectOptions.setCleanSession(false);
        mqttConnectOptions.setUserName(userName);
        mqttConnectOptions.setPassword(password.toCharArray());

        try{

            myPhonemqtt.connect(mqttConnectOptions, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    DisconnectedBufferOptions disconnectedBufferOptions = new DisconnectedBufferOptions();
                    disconnectedBufferOptions.setBufferEnabled(true);
                    disconnectedBufferOptions.setBufferSize(100);
                    disconnectedBufferOptions.setPersistBuffer(false);
                    disconnectedBufferOptions.setDeleteOldestMessages(false);
                    myPhonemqtt.setBufferOpts(disconnectedBufferOptions);
                    subscribeToTopic();
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    System.out.println("Failed to connect to " + serverURL);
                }
            });
        } catch (MqttException e){
            e.printStackTrace();
        }
    }

    private void subscribeToTopic(){
        try {
            myPhonemqtt.subscribe(subDigitalTopic, 0, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    System.out.println("Subscribed to LED");
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    System.out.println("Failed to Subscribe to LED");
                }
            });

            myPhonemqtt.subscribe(subAnalogTopic, 0, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    System.out.println("Subscribed to RELAY");
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    System.out.println("Failed to Subscribe to RELAY");
                }
            });

        }catch (MqttException e){
            System.out.println("Error whilst subscribing");
            e.printStackTrace();
        }
    }

    public void sendMessageToTopic(String topic, String text) throws MqttException {

        MqttMessage msg = new MqttMessage(text.getBytes());
        msg.setQos(qos);
        msg.setRetained(true);

        myPhonemqtt.publish(topic, msg);

    }
}



