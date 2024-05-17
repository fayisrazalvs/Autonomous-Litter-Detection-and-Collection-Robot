# Autonomous-Litter-Detection-and-Collection-Robot

## INTRODUCTION 

This project aims to develop an autonomous litter detection and collection robot using the TensorFlow Lite libraries to run object detection on a Raspberry Pi. The idea of the robot is to be mobile and detect litter objects along its path and use a collection mechanism to collect the litter object and drop it into a container mounted on the body of the robot.

It is a very ambitious project which requires complex algorithms, circuits and mechanical design to operate effectively. But for the time being, I have developed a simple design and system to execute the fundamental concept of this project for my final semester project. This project is my second project in robotics and at a significantly higher level than my previous one. This project will always be a work in progress until an outcome that satisfies me is achieved.

## CIRCUIT AND COMPONENTS

![Circuit Diagram Labeled](https://github.com/fayisrazalvs/Autonomous-Litter-Detection-and-Collection-Robot/assets/108331765/620f12d7-3632-4933-8be3-6aeb39d25642)

|No. | Component Name |
|---|---|
|1 | 12 V battery |
|2 | Servo-Suction cup switch |
|3 | L298N H-Bridge driver module switch |
|4 | 12 V DC gear motor (1) |
|5 | 12 V DC gear motor (2) |
|6 | LM2596 DC-DC buck converter module (11.1 V – 6V) |
|7 | LM2596 DC-DC buck converter module (11.1 V – 5.2V) |
|8 | Raspberry Pi switch |
|9 | LM2596 DC-DC buck converter module (12 V) |
|10 | L298N H-Bridge motor driver |
|11 | Raspberry Pi |
|12 | Raspberry Pi camera |
|13 | Node MCU |
|14 | PCA9685 PWM driver |
|15 | MG995 servo motor (1) |
|16 |MG995 servo motor (2) |
|17 |Ultrasonic sensor |
|18 |Three-way electronic valve |
|19 |Vacuum pump |
|20 |Suction cup |

## CONCEPT DESIGN

![Proposed Design](https://github.com/fayisrazalvs/Autonomous-Litter-Detection-and-Collection-Robot/assets/108331765/163df202-2ce6-4b45-bab6-d5fec22e62c2)



## WORKING

* The NodeMCU serves as the central controller for the robot, coordinating the actions of the wheel motors, servo mechanism, suction pump, and electronic valve.

* Upon detection of litter by the Raspberry Pi's custom object detection model built using TensorFlow Lite, which was trained using 200 litter images for object recognition running on the Raspberry Pi using the Pi camera module, a signal is sent to the NodeMCU, prompting it to halt the movement of the robot and activate the litter collection system.

![Full working](https://github.com/fayisrazalvs/Autonomous-Litter-Detection-and-Collection-Robot/assets/108331765/fdbd2e0f-a4e1-468a-8a13-4c2156e02ee6)

* The NodeMCU and Raspberry Pi communicate with each other through UART. This system, comprising the servo mechanism, suction pump, and electronic valve, engages to collect the detected litter.

* In the absence of detected litter, the NodeMCU autonomously resumes control of the wheel motors, allowing the robot to continue its forward movement. This integration of Raspberry Pi detection and NodeMCU control ensures efficient and autonomous litter collection while optimizing the robot's operational capabilities.

* When litter is detected, the Raspberry Pi sends signals to the NodeMCU to initiate the appropriate actions, including stopping the wheel motors, activating the servo motors for litter collection, and controlling the suction pump and electronic valve.

* The Node MCU also controls the drive motors, which are operated through an LM2596 H-Bridge Driver module. Acting as the central controller for the robot.

## FINAL MODEL

![WhatsApp Image 2024-04-22 at 05 57 57_e6b84344](https://github.com/fayisrazalvs/Autonomous-Litter-Detection-and-Collection-Robot/assets/108331765/1a4c3aa3-f4a8-407c-9e5e-b4907106d214)

![WhatsApp Image 2024-04-22 at 05 57 50_7efcc689](https://github.com/fayisrazalvs/Autonomous-Litter-Detection-and-Collection-Robot/assets/108331765/f716e2a7-445d-4a73-bf73-695ca8dfe059)

![WhatsApp Image 2024-04-22 at 05 57 51_01c48c3c](https://github.com/fayisrazalvs/Autonomous-Litter-Detection-and-Collection-Robot/assets/108331765/ff82492e-8c96-46d0-9f1d-8b18d9610b55)

![WhatsApp Image 2024-04-22 at 05 57 52_fccfde7e](https://github.com/fayisrazalvs/Autonomous-Litter-Detection-and-Collection-Robot/assets/108331765/ea2658ae-3d8c-4880-8a4e-4101e6bbbd96)

## CONCLUSION

The current model successfully completes the sole purpose of the project. Although it is an autonomous robot it lacks in many areas and has a lot of conflicts and issues both in design as well as programming aspects. But like I said the project is an ambitious one and I aim to bring updates and changes to it as needed. I have made notes for upgrades that will make a significant impact in the operational capacity and will be working to introduce them in the future.

### UPGRADE NOTES

1. **ROS Introduction**

2. **Additional Navigation Logic**: Implement logic for the robot to determine the direction of the detected litter relative to its current position and orientation. This involves calculating the angle or direction to steer the robot towards the litter.

3. **Integration with Litter Detection**: The navigation logic needs to be integrated with the litter detection module. When the litter is detected and determined to be far away, the robot should switch from litter pickup mode to navigation mode and start moving towards the litter.

4. **Obstacle Avoidance Adaptation**: The obstacle avoidance module would need to be modified to operate concurrently with the navigation towards litter. The robot should avoid obstacles while navigating towards the litter to ensure safe movement.

5. **Sensor Fusion and Decision Making**: Implement sensor fusion techniques to combine information from multiple sensors (e.g., camera for litter detection and ultrasonic sensor for obstacle avoidance) to make informed navigation decisions.

6. **Wireless Communication**: Instead of using UART, you could explore wireless communication protocols like Wi-Fi or Bluetooth. This would eliminate the need for physical connections and allow for more flexibility in positioning the components of your system.

7. **Integration with Existing Systems**: If you have existing systems or platforms for object detection and control, you could integrate your litter collection mechanism with those systems. For example, if you're already using a home automation platform like Home Assistant, you could trigger the litter collection mechanism based on events detected by sensors connected to that platform.

8. **Cloud-Based Control**: You could implement a cloud-based control system where the Raspberry Pi sends data to a cloud server, which then triggers actions on the NodeMCU based on predefined rules or conditions. This would allow you to remotely control your litter collection mechanism from anywhere with an internet connection.

9. **Testing and Tuning**: Implement thorough testing protocols and methods, collect as much data as possible. Introduce proper system for record keeping.

---

