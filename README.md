# Alarm Clock

A custom 4-key alarm clock built around the Seeed XIAO ESP32-C3 and a 2.25 inch TFT display.

The idea was to make a small, simple alarm clock with a custom PCB and a 3D printed enclosure instead of putting everything together with loose wires.

Demo -> <a href="https://youtu.be/RYXH7QkYXOU">Final Built</a>
---

# I Want To Say Something

Alarm Clock built is completed. I has soldered all components on PCB and placed PCB inside enclosure.

**Since it was my first time to make PCB and 3D print, I did some miscalculation in radius of screw holes so I can't screw PCB and top cover of enclosure in enclosure body. So, I glued PCB and Top cover in body and everything is fixed perfectly. Nothing is loose inside it.**

---

# Final Built

<img style="width:45%;" src="assets/front-view.jpeg"> <img style="width:45%;" src="assets/top-view.jpeg">
 
---

# Physical PCB

<img style="width:45%;" src="assets/PCB-Front(Physical).jpeg"> <img style="width:45%;" src="assets/PCB-Back(Physical).jpeg">


# The Idea

I wanted to build something that would let me learn the complete hardware design process, from making a schematic and PCB to designing a case around the actual hardware.

For this project, I decided to keep the design simple:

- 4 mechanical keys
- 2.25 inch TFT display
- Seeed Studio XIAO ESP32-C3
- Piezo buzzer
- Custom PCB
- Custom 3D printed enclosure


**PCB**

<img style="width:45%;" src="assets/PCB-Front.png"> <img style="width:45%;" src="assets/PCB-Back.png">

**Schematic**

<img style="width:45%;" src="assets/Schematic.png">

**Enclosure**

<img style="width:45%;" src="assets/Enclosure.png">

---

# Controls


SW1 - Enter time/alarm setting/switch between hour and minute

SW2 - Stop ringing alarm/increase selected value

SW3 - Decrease selected value

SW4 - Switch between Clock and Alarm/save and exit

---

# Hardware

Controller - Seeed Studio XIAO ESP32-C3

Display - 2.25-inch ST7789P3 TFT display

Input - Four mechanical keyboard switches are used as the main controls.

Sound - A passive buzzer connected to the XIAO ESP32-C3 is used for interface feedback and the alarm.

---

# AI Uses

AI is used for: 
- PCB design assistance
- Circuit troubleshooting
- CAD/enclosure design guidance
- Project organization

---

# Development Status

**Built is completed. Nothing is left.**

---