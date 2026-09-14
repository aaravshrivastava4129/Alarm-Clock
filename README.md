# Alarm Clock

A custom 4-key alarm clock built around the Seeed XIAO ESP32-C3 and a 2.25 inch TFT display.

The idea was to make a small, simple alarm clock with a custom PCB and a 3D printed enclosure instead of putting everything together with loose wires.

---

# The Idea

I wanted to build something that would let me learn the complete hardware design process, from making a schematic and PCB to designing a case around the actual hardware.

For this project, I decided to keep the design simple:

- 4 mechanical keys
- 2.25 inch TFT display
- Seeed XIAO ESP32-C3
- Piezo buzzer
- Custom PCB
- Custom 3D printed enclosure

**The PCB and enclosure are currently completed. Firmware and the physical build will come next.**

**PCB**

<img style="width:45%;" src="assets/PCB-Front.png"> <img style="width:45%;" src="assets/PCB-Back.png">

**Schematic**

<img style="width:45%;" src="assets/Schematic.png">

**Enclosure**

<img style="width:45%;" src="assets/Enclosure.png">

---

# Firmware

Currently, it only have a timer function. On clicking SW1 button it will start timer from '00:00:00'. When SW1 is re-pressed it will stop timer and the total time will remain displayed. When SW1 is re-pressed it will reset and restart the timer.

SW1 press -> Timer start - > SW1 re-pressed -> Timer Stops -> SW1 re-pressed -> Timer reset and restart

---

# AI Uses

AI is used for: 
- PCB design assistance
- Circuit troubleshooting
- CAD/enclosure design guidance
- Project organization

---

_**DEVEOPMENT IS UNDER PROGRESS. WAITING FOR COMPONENTS APPROVEL xD**_

