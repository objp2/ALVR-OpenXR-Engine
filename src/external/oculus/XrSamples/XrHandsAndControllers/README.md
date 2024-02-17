# XrHandsAndControllers
## Introduction
This sample shows the ability to track both hands and controllers at the same time. The native sample will enable the top level hands paths to be filled with either hands data or controller data depending on whether a controller is held or not. Also, controllers that are not held are tracked and their pose data used to populate new detached_controller paths (doesn't work on PC). The native sample demonstrates this by printing out the contents of the paths.

## Supported Devices
* Mobile
    * Quest Pro + Quest Touch Pro Controller
* PC
    * Quest Pro + Quest Touch Pro Controller


## Expected Behavior
### Behavior 1:
Check that you can interact with the UI using both controllers / both hands. Hit the "click me!" button and confirm that the counter increments. Try it with both hands and controllers
* Expected Result: The counter increments

### Behavior 2:
Confirm the system gesture works. With your right hand, make the system gesture and confirm that the system gesture menu appears
* Expected Result: The system gesture menu appears and can be used to select options.

### Behavior 3:
Check that the system menu button works when using controllers. On the right controller, press and hold the Oculus/Meta button until the system menu appears
* Expected Result: The system menu appears and can be used to select options

### Behavior 4:
Check that you can pause and unpause the app normally. Pause the app and confirm that the pause menu is not in simultaneous hands + controllers mode (rather, only one or the other only). Then, unpause and confirm that the app resumes in simultaneous hands + controllers mode.
* Expected Result: App resumes in simultaneous hands + controllers mode. Paused menu is not in simultaneous hands and controllers mode

### Behavior 5:
Exit the app and confirm that Shell returns to a non-simultaneous input mode
* Expected Result: Shell is not in simultaneous hands and controllers mode.

### Behavior 6:
Behavior that the hand stops being rendered when a controller is picked up (left hand holding left controller or right hand holding right controller)
* Expected Result: The controller should be rendered in place of the hand. The controller will resume being rendered when it is subsequently placed on the desk again.

### Behavior 7:
Behavior fast movements. Pick up the controller in one hand and move the arm around at a fairly fast pace. Twist and contort the arm in different ways. Repeat with the other hand
* Expected Result: The controller should remain rendered and the hand should not be rendered. It may occasionally “pop in”, but we hope this effect is very minimal.

### Behavior 8:
Behavior that opposite hand held-controllers do render together with hands (ie, when the left hand holds the right controller, or the right hand holds the left controller, these do not count as “held”; both the hand and controller are rendered for this case)
* Expected Result: The controllers render as expected

### Behavior 9:
Behavior that a controller placed on the table continues to render. Place a controller on the table.
* Expected Result: The controller renders

### Behavior 10:
Press the “Toggle Concurrent Input” button and ensure that the simultaneous hands and controllers tracking stops (the system reverts to a hands only or system only input model)
* Expected Result: The system suddenly changes to only one modality; it will be clear. Only controllers or only hands will render, and you will be able to switch between them (in both hands), but will not get one-of-each

### Behavior 11:
Press it again and ensure that simultaneous tracking resumes
* Expected Result: Simultaneous tracking of hands and controllers resumes

### Behavior 12:
Face to the side and then using either the controller or the hand, re-center the pose. After re-centering, confirm things work as they should (hands do not render when controllers are held, and the hands and controllers can still interact with the UI in the same way)

## Edge Cases:
Please try stressing the feature. Leave controllers on the table for a long time. Hit the enable/disable button repeatedly.
