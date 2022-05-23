# Diagrams
## Class Diagram
The MainWindow, Controller, and EarClips classes inherit the QObject class to make use of QT’s signals/slots mechanism. This is an implementation of the Observer pattern.
- The MainWindow observes changes in state from the Controller
- The EarClips observes the slider changes in the MainWindow
- The Controller observes button clicks, slider changes, etc from the MainWindow

The Controller is an implementation of the Mediator pattern. It acts as the central place for all backend logic. This keeps the other classes (MainWindow, Group, Session, Record, EarClips, Battery) from needing to have references to each other. UI elements do not need to keep references to each other. Rather, when a change in state happens in one of these other classes, the Controller is the first one to be notified, which then directs other classes to change state accordingly.

## Sequence Diagrams
### UC1
This diagram shows how the device is turned on. The user must press and hold the power button for 2 seconds until the device is turned on. If the current battery level is critically low, the device will turn off.

### UC2
The diagram starts with the user pressing and holding the OASIS’s power button. Once the button is clicked for at least two seconds, the device turns off.  

### UC3
To select a session, the user must make two choices; they must select a group and a session, where the group determines the planned time for the session. 

When choosing a group, the user may select the 20min or 45min groups -- the user designed one is not enabled since the DAVID Session Editor is sold separately. While scrolling through the groups, the device illuminates the icon of the group currently in focus.

Each of the two enabled groups contain 4 sessions that the user can select. The user utilizes the number graph to choose a session, where number 1 refers to the left-most session icon and number 4 refers to the right-most session icon. 	

### UC4
This sequence diagram describes the process of starting a session. 
When a session is selected and the check mark to start a session is clicked the controller checks if the ear clip connection level is good enough to proceed. If the ear clip is good enough the session will proceed, otherwise the session will not start until the connection level is acceptable. 

### UC5
This sequence diagram describes the process of adjusting the intensity of the device. 
If the intensity up button is clicked during an active session, the intensity is turned up by one increment for each time the button is clicked.
If the intensity down button is clicked during an active session, the intensity is turned down by one increment each time the button is clicked.

### UC6
Once a session ends, the device asks the user to record the session. If the user desires, they may click the checkmark button to store the session, or click the power button to skip the prompt. If the user wishes to record, a True value is passed to stopRecord. The record is then registered and then added to the QListWidgetItem listWidget to be displayed. If the user does not wish to record, a false value is passed to stopRecord. Then, the QListWidgetItem listWidget is raised to display old records.
