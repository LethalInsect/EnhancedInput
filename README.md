# EnhancedInput
This project is about remapping Enhanced Input Actions and integrating them into the UI.

![image](https://github.com/user-attachments/assets/1391e2dc-d435-469a-96cf-0a9aed83827e)
![image](https://github.com/user-attachments/assets/1d00137f-673b-44dd-9269-610b587778ed)




https://github.com/user-attachments/assets/0cab45cd-b019-444b-804f-a3cb8d4264b4

**The following is implemented:**
- Icons are used to represent keys.
- Separate remapping for keyboard and controller.
- Block specific keys from being accepted in the Key Selector.
- Save remapped keys in a custom SaveGame.
- Load remapped keys from SaveGame.
- You can assign an Input Action to the UI and it will automatically update the icon to the currently mapped key.
- A single key can directly provide its icon.
- Rich Text images can be set dynamically through code.
- Detect input device changes and broadcast an event to update all UI icons accordingly.
- Simple Rich Text formatting and integration.
- Disable controller navigation when selecting a key.
- Reset the Input Action to its default.

**Important Notes:**

- This is not fully optimized, but since it only runs at game start, it's fine for now.
- Tested with a standard keyboard and a DualSense controller using DS4Windows to simulate an Xbox controller.
- Movement keys ``(WASD)`` and controller stick movement are not accepted as valid inputs and can be adjusted in the ``WBP_Key`` widget.
- If an icon appears purple, it means the key is not included in the icon DataTable (I only added central keyboard keys).
- If something fails to load, delete the SaveGame file and try again.

# Set up

Enhanced Input Action:
To have your key picked up by this system, you must add Player Mappable Key Settings to the Input Action. Without this, it will be ignored.
![image](https://github.com/user-attachments/assets/9349873f-0b69-4883-b07b-6a6c89022b4b)

- The Name and Display Name should match — these are used to populate the remapping widget.
- The Category is used to group and sort keys in the UI.

[Here](https://dev.epicgames.com/community/learning/tutorials/oWne/unreal-engine-add-images-dynamically-to-rich-text-over-code) is a tutorial on how my Rich Text system works.

## Codex
![image](https://github.com/user-attachments/assets/613f4c5f-7314-4631-8782-3f86aec8cfff)

You can declare an image using ``{index}`` and place it anywhere in your text, as many times as needed. Each Input Action has keys for both input devices, but each FKey only stores one key — so the first key is for keyboard, the second for controller.

These keys are stored sequentially, where the pattern repeats for each action.
- The even index (0, 2, 4, ...) holds the keyboard key for an action.
- The odd index (1, 3, 5, ...) holds the controller key for the same action.

So for every pair of elements:
- The first is the keyboard key for Action N.
- The second is the controller key for Action N.

It first loops over the Input Action array, then over the Key array.The index used for the ``{}`` tags continues from the Input Action array into the Key array.For example: ``InputAction[0] = {0} → Key[0] & Key[1] = {1}``



Improvements:
- You could add mappable key settings to all Input Actions and group those that shouldn't be remapped into a "NotMappable" category. These can then be skipped when generating the key binding widgets. This allows for static input icons without requiring updates if keys are changed later.
- Icons could be stored in a Blueprint instead of a DataTable. Not sure if this would improve performance or workflow, but it’s another option.

