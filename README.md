# 🔐 CheckMate – Smart Employee Check-In System

A secure, real-time, and efficient employee attendance tracking system using RFID and dual-factor authentication (RFID + PIN). Developed as our final year engineering project at **Higher School of Computer Science – Sidi Bel Abbès (ESI-SBA)**, this system is designed to modernize and simplify employee check-ins in workplaces using IoT technologies.

---

## 📌 Project Overview

Traditional check-in systems are often manual, inefficient, and lack proper analytics. CheckMate solves these issues by combining hardware-based access control and a cloud-powered dashboard for real-time monitoring and management.

### 🎯 Core Features

- Dual-factor authentication: RFID card + 4-digit PIN
- Real-time attendance updates with cloud synchronization
- Employee statistics dashboard (daily/monthly with PDF export)
- Admin panel for employee and RFID management
- Secure access control via servo motor + visual feedback (LEDs)

---

## 🧠 Tech Stack

### 🔧 Hardware
- Arduino Uno
- NodeMCU ESP8266
- RFID Reader RC522
- 4x4 Keypad
- Servo Motor
- LEDs

### 🖥️ Software
- Backend: Node.js (Express)
- Database: MongoDB
- Deployment: Vercel
- Communication: Serial (Arduino ↔ ESP8266)

---

## 🛠️ My Role

I contributed primarily to the **hardware and embedded systems**, specifically:

- Wiring and integration of Arduino Uno + ESP8266
- Writing the complete **Arduino code** for RFID + keypad input + servo + LED handling
- Writing and debugging the **ESP8266 code** for Wi-Fi connection and API communication
- Real-time interaction between hardware and cloud

---

## 👥 Team & Contributions

This project was developed by a team of 5 students at ESI-SBA (4th year, 2nd Cycle – Information Systems Engineering):

- **[@Abderraouf Atsamnia]** – Hardware design, Arduino & ESP8266 code, system integration  
- **[@Senouci Ibrahim]** – Hardware design, Arduino & ESP8266 code, system integration  
- **[@Asmaa Baiche]** –  Frontend development, UI/UX 
- **[@Hallouche Abdessamed]** –  Frontend development, UI/UX 
- **[@Cherrak Ismail Anis]** –  Backend APIs, Database design, cloud deployment (MongoDB + Vercel) 

> Special thanks to our supervisor **Mr. Rahmoun Abdellatif** for his support and guidance throughout the project.

---

## 📸 Demo & Images

## 🎥 Demo Video
[Watch Demo](./assets/demo.mp4)


---

## 🚀 Future Improvements

- Mobile check-in via NFC or app authentication
- Predictive attendance analytics (AI)
- Payroll system integration

---

## 📄 License

This project is for academic and educational purposes.  
Feel free to explore or adapt the code with proper credit to the authors.

---

