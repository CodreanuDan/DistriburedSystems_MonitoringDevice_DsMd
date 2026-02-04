# Distributed Monitoring and Control System  

## 1. Overview  
IoT-based Smart Home solution using two independent ESP32 nodes communicating in real-time via MQTT. The system monitors environmental parameters (temperature, air quality, light) and automatically controls power loads (DC motor, LED) with hysteresis logic or manual override through a Node-RED dashboard.

![Block Diagram](HW/drawio/HW_Overall/HW_Diagram.svg)  
*System block diagram showing sensor-actuator distribution and MQTT communication*

## 2. Hardware  
Node 1 handles power control and consumption monitoring. Node 2 performs environmental data acquisition and visual feedback.

![Breadboard](HW/Fritzing/DistriburedSystems_MonitoringDevice_DsMd_bb.svg)  
*Full breadboard assembly of both nodes*

![Motor Control Circuit; LED Control Circuit](HW/ICircuit/Motor_and_LED_Control.png)  
*TIP142 Darlington transistor driver for DC motor with flyback diode protection; S8050 NPN transistor driver for LED control*


## 3.1 Software – ESP32 Nodes  

![SW Diagram Node 1](SW/Design/SW_ESP_1/SW_ESP_1.svg)  
*Software flow for Node 1 (motor control, MAX471, LDR, WiFi/MQTT, LCD, button FSM)*

![SW Diagram Node 2](SW/Design/SW_ESP_2/SW_ESP_2.svg)  
*Software flow for Node 2 (BME680 task, LED dimmer, WiFi/MQTT, LCD, button FSM)*

## 3.2 MQTT & Node-RED Dashboard  

![Dashboard Node 1 – Motor & LDR](IoT/Design/ESP_Node1_Panel.png)  
*Real-time motor power, current, voltage and LDR status monitoring*

![Dashboard Node 2 – Climate & LED](IoT/Design/ESP_Node2_Panel.png)  
*Temperature, humidity, IAQ and LED control visualization*

![Dashboard Control – Motor & LED](IoT/Design/Control_Panel_Motor_LED.png)  
*Manual/Auto mode selection and direct control buttons for motor and LED*

![Node-RED Flow ESP1](IoT/Design/ESP32_Node1_Flow.png)  
*Data processing and dashboard flow for Node 1*

![Node-RED Flow ESP2](IoT/Design/ESP32_Node2_Flow.png)  
*Sensor data processing and LED command flow for Node 2*

![MQTT Broker Workflow](Docs/img/MqttServer.png)  
*MQTT communication flow between ESP32 nodes and Node-RED dashboard*
