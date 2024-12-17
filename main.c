#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAP_SIZE 5
#define MAX_ITEMS 3
#define MAX_NAME_LENGTH 20
#define SAVE_FILE_EXT ".sav"

typedef struct {
    char name[MAX_NAME_LENGTH];
    int damage;
} Item;

typedef struct {
    char description[50];
    Item items[3];
    int itemCount;
    int hasMonster;
    int monsterHealth;
} Room;

typedef struct {
    int health;
    int strength;
    Item inventory[MAX_ITEMS];
    int inventoryCount;
    int x, y; 
} Player;

Room dungeon[MAP_SIZE][MAP_SIZE];
Player player;

void initDungeon();
void printWelcomeMessage();
void printMenu();
void printMap(Player *p);
void look(Player *p);
void movePlayer(Player *p, char *direction);
void pickupItem(Player *p, char *itemName);
void attackCreature(Player *p);
void printInventory(Player *p);
void saveGame(const char *filepath);
void loadGame(const char *filepath);
void listSavedGames();

void initDungeon() {
    srand(time(NULL));
    int healthElixirCount = 0; 

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            dungeon[i][j].itemCount = rand() % 3; 
            dungeon[i][j].hasMonster = rand() % 2; 
            dungeon[i][j].monsterHealth = dungeon[i][j].hasMonster ? 50 : 0;
            strcpy(dungeon[i][j].description, "A mysterious room...");

            for (int k = 0; k < dungeon[i][j].itemCount; k++) {
                int randItem = rand() % 3;
                switch (randItem) {
                    case 0:
                        strcpy(dungeon[i][j].items[k].name, "Sword");
                        dungeon[i][j].items[k].damage = 15;
                        break;
                    case 1:
                        strcpy(dungeon[i][j].items[k].name, "Katana");
                        dungeon[i][j].items[k].damage = 20;
                        break;
                    case 2:
                        strcpy(dungeon[i][j].items[k].name, "Hammer");
                        dungeon[i][j].items[k].damage = 25;
                        break;
                }
            }

            if (healthElixirCount < 5 && rand() % 4 == 0) { 
                int slot = dungeon[i][j].itemCount;
                if (slot < 3) { 
                    strcpy(dungeon[i][j].items[slot].name, "Health Elixir");
                    dungeon[i][j].items[slot].damage = 0; 
                    dungeon[i][j].itemCount++;
                    healthElixirCount++;
                }
            }
        }
    }
}


void printWelcomeMessage() {
    printf("\nDungeon Adventure Game\n");
    printf("============================\n");
    printf("Welcome to the dungeon! Your goal is to defeat monsters and collect treasures.\n");
    printf("============================\n");
}

void printMenu() {
    printf("\nAvailable Commands:\n");
    printf("- move <direction> (up, down, left, right): Move to a different room.\n");
    printf("- look: See your surroundings, items, and monsters in the room.\n");
    printf("- pickup <item>: Pick up an item from the room.\n");
    printf("- attack: Fight the monster in the room.\n");
    printf("- inventory: View your inventory.\n");
    printf("- drink elixir: Drink a health elixir from your inventory.\n"); 
    printf("- save <filename>: Save the current game state.\n");
    printf("- load <filename>: Load a previously saved game.\n");
    printf("- list: List all saved games.\n");
    printf("- exit: Exit the game.\n");
}

void printMap(Player *p) {
    printf("\nDungeon Map:\n");
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (i == p->x && j == p->y)
                printf("P   "); 
            else
                printf("X   "); 
        }
        printf("\n\n");
    }
    printf("Player Health: %d\n", p->health);
}


void look(Player *p) {

    int roomNumber = p->x * MAP_SIZE + p->y + 1;  

    printf("You are in Room %d.\n", roomNumber);

    Room *currentRoom = &dungeon[p->x][p->y];
    printf("%s\n", currentRoom->description);

    if (currentRoom->itemCount > 0) {
        printf("This room contains the following items:\n");
        for (int i = 0; i < currentRoom->itemCount; i++) {
            printf("- %s (Damage: %d)\n", currentRoom->items[i].name, currentRoom->items[i].damage);
        }
    } else {
        printf("This room contains no items.\n");
    }

    if (currentRoom->hasMonster) {
        printf("There is a monster here! (Health: %d)\n", currentRoom->monsterHealth);
    } else {
        printf("There are no monsters here.\n");
    }
}

void movePlayer(Player *p, char *direction) {
    if (strcmp(direction, "up") == 0 && p->x > 0)
        p->x--;
    else if (strcmp(direction, "down") == 0 && p->x < MAP_SIZE - 1)
        p->x++;
    else if (strcmp(direction, "left") == 0 && p->y > 0)
        p->y--;
    else if (strcmp(direction, "right") == 0 && p->y < MAP_SIZE - 1)
        p->y++;
    else
        printf("You can't move in that direction.\n");
}

void pickupItem(Player *p, char *itemName) {
    Room *currentRoom = &dungeon[p->x][p->y];
    for (int i = 0; i < currentRoom->itemCount; i++) {
        if (strcasecmp(currentRoom->items[i].name, itemName) == 0) {
            if (p->inventoryCount < MAX_ITEMS) {
                p->inventory[p->inventoryCount++] = currentRoom->items[i];
                printf("You picked up: %s\n", currentRoom->items[i].name);

                p->strength += currentRoom->items[i].damage;

                for (int j = i; j < currentRoom->itemCount - 1; j++) {
                    currentRoom->items[j] = currentRoom->items[j + 1];
                }
                currentRoom->itemCount--;
                return;
            } else {
                printf("Your inventory is full!\n");
                return;
            }
        }
    }
    printf("Item not found in this room.\n");
}

void drinkElixir(Player *p) {
    Room *currentRoom = &dungeon[p->x][p->y]; 

    for (int i = 0; i < currentRoom->itemCount; i++) {
        if (strcasecmp(currentRoom->items[i].name, "health elixir") == 0) {

            p->health += 10;
            printf("You drank a Health Elixir from the room! Your health is now %d.\n", p->health);

            for (int j = i; j < currentRoom->itemCount - 1; j++) {
                currentRoom->items[j] = currentRoom->items[j + 1];
            }
            currentRoom->itemCount--; 
            return;
        }
    }
    printf("There is no Health Elixir in this room.\n");
}


void dropItem(Player *p, char *itemName) {
    int itemIndex = -1;
    
    for (int i = 0; i < p->inventoryCount; i++) {
        if (strcasecmp(p->inventory[i].name, itemName) == 0) {
            itemIndex = i;
            break;
        }
    }

    if (itemIndex != -1) {
        Room *currentRoom = &dungeon[p->x][p->y];
        currentRoom->items[currentRoom->itemCount++] = p->inventory[itemIndex];
        printf("You dropped: %s\n", p->inventory[itemIndex].name);

        for (int i = itemIndex; i < p->inventoryCount - 1; i++) {
            p->inventory[i] = p->inventory[i + 1];
        }
        p->inventoryCount--; 
    } else {
        printf("Item not found in your inventory.\n");
    }
}


void attackCreature(Player *p) {
    Room *currentRoom = &dungeon[p->x][p->y];
    if (currentRoom->hasMonster) {
        while (currentRoom->monsterHealth > 0 && p->health > 0) {
            printf("You attack the monster with %d damage!\n", p->strength);  
            currentRoom->monsterHealth -= p->strength; 
            if (currentRoom->monsterHealth > 0) {
                printf("The monster attacks you!\n");
                p->health -= 10;
            }
        }
        if (p->health <= 0) {
            printf("You have been defeated!\n");
            exit(0);
        } else {
            printf("You defeated the monster!\n");
            currentRoom->hasMonster = 0;
        }
    } else {
        printf("There is no monster here to attack.\n");
    }
}


void printInventory(Player *p) {
    if (p->inventoryCount == 0) {
        printf("Your inventory is empty.\n");
    } else {
        printf("Your inventory contains:\n");
        for (int i = 0; i < p->inventoryCount; i++) {
            printf("- %s (Damage: %d)\n", p->inventory[i].name, p->inventory[i].damage);
        }
    }
}

void saveGame(const char *filepath) {
    FILE *file = fopen(filepath, "wb");
    if (file == NULL) {
        printf("Failed to save game.\n");
        return;
    }
    fwrite(&player, sizeof(Player), 1, file);
    fwrite(dungeon, sizeof(dungeon), 1, file);
    fclose(file);
    printf("Game saved to %s.\n", filepath);
}

void loadGame(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        printf("Failed to load game.\n");
        return;
    }
    fread(&player, sizeof(Player), 1, file);
    fread(dungeon, sizeof(dungeon), 1, file);
    fclose(file);
    printf("Game loaded from %s.\n", filepath);
}

void listSavedGames() {
    printf("Listing all files in the current directory:\n");
    int result = system("dir /b");
    if (result == -1) {
        printf("Failed to list files.\n");
    }
}


int main() {
    char command[50], argument[50];
    initDungeon();
    player.health = 100;
    player.strength = 20;
    player.x = MAP_SIZE / 2;
    player.y = MAP_SIZE / 2;

    printWelcomeMessage();
    printMenu();

    while (1) {
        printMap(&player);
        printf("> ");
        scanf("%s", command);

        if (strcmp(command, "look") == 0) {
            look(&player);
        } else if (strcmp(command, "move") == 0) {
            scanf("%s", argument);
            movePlayer(&player, argument);
        } else if (strcmp(command, "pickup") == 0) {
            scanf("%s", argument);
            pickupItem(&player, argument);
        } else if (strcmp(command, "attack") == 0) {
            attackCreature(&player);
        } else if (strcmp(command, "inventory") == 0) {
            printInventory(&player);
        } else if (strcmp(command, "save") == 0) {
            scanf("%s", argument);
            saveGame(argument);
        } else if (strcmp(command, "load") == 0) {
            scanf("%s", argument);
            loadGame(argument);
        } else if (strcmp(command, "list") == 0) {
            listSavedGames();
        } else if (strcmp(command, "drink") == 0) {
            drinkElixir(&player); 
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting the game...\n");
            break;
        } else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}
