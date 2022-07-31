package com.example.arduinocontrol;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.ToggleButton;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MainActivity extends AppCompatActivity {

    ToggleButton blueButton, redButton, relayButton;
    TextView connectText;
    Switch mqttConnectSwitch;
    SeekBar seekBarA0, seekBarA1;
    mqttClient myClient;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //connectText = (TextView) findViewById(R.id.connectTextView);
        blueButton = (ToggleButton) findViewById(R.id.BlueLEDButton);
        redButton = (ToggleButton) findViewById(R.id.RedLEDButton);
        relayButton = (ToggleButton) findViewById(R.id.RelayButton);
        mqttConnectSwitch = (Switch) findViewById(R.id.mqttSwitch);
        seekBarA0 = (SeekBar) findViewById(R.id.seekBarA0);
        seekBarA1 = (SeekBar) findViewById(R.id.seekBarA1);

        blueButton.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {

                if(myClient != null){

                if (b) {
                    try {
                        myClient.sendMessageToTopic(myClient.pubDigitalTopic, getResources().getString(R.string.BLOn));
                    } catch (MqttException e) {
                    }
                    ;
                } else if (!b) {
                    try {
                        myClient.sendMessageToTopic(myClient.pubDigitalTopic, getResources().getString(R.string.BLOff));
                    } catch (MqttException e) {}
                    ;
                }
            }}
        });

        redButton.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {

                if(myClient != null){

                if (b) {
                    try {
                        myClient.sendMessageToTopic(myClient.pubDigitalTopic, getResources().getString(R.string.RLOn));
                    } catch (MqttException e) {}
                } else if (!b) {
                    try {
                        myClient.sendMessageToTopic(myClient.pubDigitalTopic, getResources().getString(R.string.RLOff));
                    } catch (MqttException e) {}
                }
            }}
        });

        relayButton.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {

                if(myClient != null){

                if (b) {
                    try {
                        myClient.sendMessageToTopic(myClient.pubDigitalTopic, getResources().getString(R.string.RlyOn));
                    } catch (MqttException e) {
                    }
                    ;
                } else if (!b) {
                    try {
                        myClient.sendMessageToTopic(myClient.pubDigitalTopic, getResources().getString(R.string.RlyOff));
                    } catch (MqttException e) {
                    }
                    ;
                }
            }}
        });

        mqttConnectSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {

                if (b) {
                    startMQTT();
                } else {
                    try {
                        myClient.myPhonemqtt.disconnect();
                        MqttConnectOptions connectOptions = new MqttConnectOptions();
                        connectOptions.setCleanSession(true);
                    } catch (MqttException e) {
                    }

                }
            }
        });

        seekBarA0.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                try{
                myClient.sendMessageToTopic(myClient.pubAnalogTopic, "A0" + seekBarA0.getProgress());
            } catch (MqttException e){};
        }});

        seekBarA1.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                try{
                    myClient.sendMessageToTopic(myClient.pubAnalogTopic, "A1" + seekBarA1.getProgress());
                } catch (MqttException e){};
            }});
    }

    public void startMQTT() {

        myClient = new mqttClient(getApplicationContext());
        myClient.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {
               // mqttConnectSwitch.setTextColor(getResources().getColor(R.color.colorPrimary));
            }

            @Override
            public void connectionLost(Throwable cause) {
               // mqttConnectSwitch.setTextColor(getResources().getColor(R.color.colorAccent));
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {

            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {

            }
        });
    }
}