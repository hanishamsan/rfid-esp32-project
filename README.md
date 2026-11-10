\# ESP32-Based Multi-RFID Item Tracking and Payment Verification System



\## Project Overview



This project is an ESP32-based RFID system developed to manage item registration, sales tracking and payment verification using multiple PN532 RFID readers.



The system uses three RFID reader modules, each assigned a specific role in the item tracking and verification process. A buzzer provides audio feedback based on the detected payment status.



\## Project Objectives



\- Implement RFID-based item identification.

\- Register and identify RFID cards using multiple readers.

\- Track item sales through RFID scanning.

\- Verify payment status using RFID identification.

\- Provide audio alerts through a buzzer.



\## Hardware Components



| Component | Quantity |

|---|---:|

| ESP32 Development Board | 1 |

| PN532 RFID Reader | 3 |

| RFID Cards / Tags | 3 |

| Push Button | 1 |

| Buzzer | 1 |



\## Technologies Used



\- ESP32

\- Arduino IDE

\- Arduino C/C++

\- SPI Communication

\- PN532 RFID Module



\## System Architecture



The system consists of an ESP32 microcontroller connected to three PN532 RFID readers, a push button and a buzzer.



\### Block Diagram



!\[System Block Diagram](diagrams/block-diagram.jpg)



\### Circuit Diagram



\[View Circuit Diagram (PDF)](diagrams/circuit-diagram.PDF)



\## Main Functionalities



1\. \*\*RFID Card Registration\*\* – Registers RFID card UIDs for identification.

2\. \*\*Item Sales Tracking\*\* – Uses RFID scanning to mark registered items as sold.

3\. \*\*Payment Verification\*\* – Checks the recorded item status through another RFID reader.

4\. \*\*Audio Feedback\*\* – Activates the buzzer according to the payment verification result.



\## Project Structure



```text

rfid-esp32-project/

├── firmware/

│   └── esp32\_rfid\_system/

│       └── esp32\_rfid\_system.ino

├── diagrams/

│   ├── block-diagram.jpg

│   ├── block-diagram.pptx

│   └── circuit-diagram.PDF

├── screenshots/

└── README.md

```



\## Development Notes



The firmware was developed for ESP32 using Arduino C/C++ and the PN532 RFID library.



The project documentation includes the system block diagram, hardware circuit diagram and source code.



\*\*Note:\*\* The physical hardware is not currently available for additional testing. The repository documents the original implementation and its source code.



\## Developer



Developed as part of an industrial training project.



\*\*Technologies:\*\* ESP32 | RFID | PN532 | Arduino C/C++

