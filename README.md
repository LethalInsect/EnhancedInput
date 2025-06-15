# EnhancedInput
This project is about remapping enhanced inputaction and integrating them into the ui.

![image](https://github.com/user-attachments/assets/1391e2dc-d435-469a-96cf-0a9aed83827e)
![image](https://github.com/user-attachments/assets/1d00137f-673b-44dd-9269-610b587778ed)




https://github.com/user-attachments/assets/0cab45cd-b019-444b-804f-a3cb8d4264b4

The Following is implemented:
- Using icons to represent keys
- separated remapping for keyboard and controller
- saving the remapped keys in a custom savegame
- load the remapped keys from savegame
- you can assign a Inputaction to the UI and it will automatically update the icon to the currently mapped key.
- With a single key you can get the icon directly
- Richtext images can be set dynamically over code
- Detect if the input device has changed and send out an event to update every ui icon to the new input device
- Easy Rich text Formatting and implementation
- Disable Navigation for Controller, when selecting a key
- reset the inputAction to its default

Important things to note:
- this is not optimized fully, but since it happens, when the game starts its alright to leave it as it is
- this was Tested with a normal keyboard and a DS5 controller with DS4Windows to simulate a Xbox controller
- The movement keys(wasd) and the stick movement of controller are not accepted by the inputs and can be adjusted in the WBP_Key widget.
- If an icon is purple, that means the key is not in the icon datatable (i didnt had Time for every key and only limited myself on the central part of the keyboard)
- if something doesnt load correctly, then delete the savegame file and try again.

# Set up

Enhanced Input Action:
If you want that your key is picked up by this system you will need to add the player mappable key settings, otherwise this input action will be ignored.
![image](https://github.com/user-attachments/assets/9349873f-0b69-4883-b07b-6a6c89022b4b)

The Name and Display name should be the same and are used to fill the name of the key inside the remapp widget.
the category will sort the keys and put every key from the same category together.

[Here](https://dev.epicgames.com/community/learning/tutorials/oWne/unreal-engine-add-images-dynamically-to-rich-text-over-code) I added a Tutorial on how my rich text works.

## Codex
![image](https://github.com/user-attachments/assets/613f4c5f-7314-4631-8782-3f86aec8cfff)

I made it so, that you can declare an image with {index} and put it anywhere in the text and also as many as you want.
The InputAction has already a key for both input devices, but the Key only have on key. Therefore i made it so, that the first key is keyboard and the second controller.
These keys are stored sequentially, where the pattern repeats for each action.
- The even index (0, 2, 4, ...) holds the keyboard key for an action.
- The odd index (1, 3, 5, ...) holds the controller key for the same action.

So for every pair of elements:
- The first is the keyboard key for Action N.
- The second is the controller key for Action N.

It first loop over the InputAction Array and then over the Key Array. The Index for the images continues from the input action array to the key array.
for example: InputAction[0] = {0} -> Key[0] & Key[1] = {1}



Improvements:
- you could add mappable key settings to every inputaction and put the ones, that should be remapped into a category named "notMappable" and sort this out before creating the keybind widgets. With this you could add the inputaction icon easier and savely (if you would change a key for these not mappbles action you wouldnt need to update them in every Widget, since with this approach you set them with hardcoded keys).
- The icon could also be stored in an blueprint instead of a datatable and read out from those, but i dont know if this would improve the performance or process.

