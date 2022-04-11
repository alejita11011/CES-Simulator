# COMP3004 OASIS Final Project

## Team 2
 - Alejandra Tobar Garzon (101131004)
 - Sebastian Navas Chaparro (101116283)
 - David Liao (101103455)
 - Tharindu Hatharasinghage (101109324)

## Task Assignment

### Design Documentation
 - Use cases: ALL TEAM MEMBERS
 - Use case diagram: ALL TEAM MEMBERS
 - Class diagram: ALL TEAM MEMBERS
 - Traceability Matrix: ALL TEAM MEMBERS
 - Sequence diagrams: ALL TEAM MEMBERS

### Implementation
 - Initially creating the classes
   - EarClips + Battery: Tharindu
   - Session + Group: Sebastian
   - Controller: Alejandra
   - SessionType + Record : David
 - Additional setup
    - Adding resource icons: David
    - Power on/off idle timers: Alejandra
	- Adding the hardware background to the UI: Tharindu
	- Implement context-related methods and variables: Sebastian
 - Core functionality
    - Turn on/off: Alejandra
    - Select a session: Sebastian
    - Activate a session: David + Alejandra
    - Record a session: David
    - Connection test: Tharindu
 - Additional functionality
    - Add sliders to represent ear clips: Tharindu
    - Add progress bar to show battery percentage: Tharindu
    - Add detection of holding down the power button: Sebastian + David
    - Exhaustive quality assurance: Sebastian
    - Create a central text display widget: Alejandra + Tharindu
    - Add LED power light button: David
    - When device turned on, show battery level: Alejandra
    - Reset shutDownTimer: Alejandra + David
    - Add comments to document the code: ALL TEAM MEMBERS

## File organization:
```
├── README.md
├── design
│   ├── class-diagram.plantuml
│   ├── class-diagram.png
│   ├── use-case-diagram.plantuml
│   ├── use-case-diagram.png
│   └── Use Cases.pdf
└── src
    ├── battery.cpp
    ├── battery.h
    ├── controller.cpp
    ├── controller.h
    ├── earclips.cpp
    ├── earclips.h
    ├── group.cpp
    ├── group.h
    ├── main.cpp
    ├── mainwindow.cpp
    ├── mainwindow.h
    ├── mainwindow.ui
    ├── oasis.pro
    ├── record.cpp
    ├── record.h
    ├── resources.qrc
    ├── session.cpp
    ├── session.h
    ├── sessiontype.h
    ├── utils.h
    ├── images
    │   ├── CES.png
    │   ├── downButton.jpg
    │   ├── powerButton.jpg
    │   ├── selectButton.jpg
    │   ├── upButton.jpg
    │   ├── off
    │   │   ├── 20.png
    │   │   ├── 45.png
    │   │   ├── alpha.png
    │   │   ├── delta.png
    │   │   ├── longpulse.png
    │   │   ├── L.png
    │   │   ├── power.png
    │   │   ├── R.png
    │   │   ├── shortpulse.png
    │   │   ├── subdelta.png
    │   │   ├── theta.png
    │   │   └── ud.png
    │   ├── on
    │   │   ├── 20.png
    │   │   ├── 45.png
    │   │   ├── alpha.png
    │   │   ├── delta.png
    │   │   ├── longpulse.png
    │   │   ├── L.png
    │   │   ├── power.png
    │   │   ├── R.png
    │   │   ├── shortpulse.png
    │   │   ├── subdelta.png
    │   │   ├── theta.png
    │   │   └── ud.png
```

# Tested scenarios
All requirements were tested and are working as required.

**Note**: The user-designated group will not allow the user to run a session as the DAVID Session Editor is sold separately. 
