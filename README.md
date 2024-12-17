Overview
Dungeon Adventure is a text-based RPG where the player navigates through a dungeon, fights monsters, and collects items such as weapons and health potions. The player’s goal is to survive the dungeon, defeat monsters, and enhance their strength by collecting powerful items. The game supports features like saving and loading the game state, managing an inventory, and drinking health elixirs.

Features
Exploration: Move around a dungeon represented by a grid.
Combat: Fight monsters that randomly appear in rooms.
Items: Collect weapons and health elixirs that improve your strength and health.
Save/Load: Save the game to a file and load it later.
Inventory: View and manage collected items.
Health: Use health elixirs to restore health

Game Commands
The game supports the following commands:

move <direction>: Move the player to a neighboring room.

Directions: up, down, left, right
look: View the current room’s description, items, and monsters.

pickup <item>: Pick up an item from the current room. The player can pick up weapons or health elixirs.

attack: Attack the monster in the current room (if there is one).

inventory: View the player's inventory, which holds weapons and health elixirs.

drink elixir: Drink a health elixir from your inventory to restore health.

save <filename>: Save the current game state to a file.

load <filename>: Load a previously saved game from a file.

list: List all saved games in the current directory.

exit: Exit the game.

Game Mechanics
The dungeon consists of a 5x5 grid, with each room containing:

A description of the room.
A random number of items (e.g., Sword, Katana, Hammer, or Health Elixir).
A monster (with health) that can be defeated by attacking.
The player has health and strength. Strength is increased by collecting weapons.

Monsters deal damage to the player when attacked. The player’s health decreases after every monster attack.

Health elixirs restore health when consumed.

Saving and Loading Games
Saving: Use the save <filename> command to save the game to a file. This file will store both the player’s state and the dungeon map.

Loading: Use the load <filename> command to load a previously saved game from a file.

List Saved Games: Use the list command to display all saved game files in the current directory.

Example Game Flow
You start in the center of the dungeon.
Move through rooms, looking for items and monsters.
Pick up weapons and health elixirs to improve your health and strength.
Fight monsters when they appear.
Save your progress as you explore, or load a previous saved game to continue your journey

