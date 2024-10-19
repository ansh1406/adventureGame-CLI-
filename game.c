#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>

#define ATTR_BASE_VALUE 2
#define BASE_SKILL_POINTS 15
#define BASE_HP 100
#define EXP_DROP_PER_LEVEL 50
#define MAX_EXP_INC_PER_LEVEL 50
#define MAX_HP_INC_PER_LEVEL 10
#define SKILL_POINTS_PER_LEVEL 5
#define INVENTORY_SIZE 32

///Structures for better data handling and clean code (primitive classes)
typedef struct
{
    char *name;
    int lvl;
    long int hp;
    int atk;
    int def;
    int agi;
    long int exp;
    long int max_hp;
    long int max_exp;
    int skill_points;
    int death_count;
    int *inventory;
} Player;

typedef struct
{
    char *name;
    int lvl;
    long int hp;
    int atk;
    int def;
    int agi;
    long int exp;
} Monster;


typedef struct
{
    char* name;
    int power;
    int accuracy;
} attack;

///Global variables um.... I don't know what to explain here
Player player;
Monster monsters[26][3];
attack attacks[5];
int isInFreeZone;
char c;

///Enumerators for better code readability
enum Attacks {head_bash,punch,kick};
enum Items {empty,healing_herb,healing_fruit,healing_sand,healing_gem,low_grade_health_potion,middle_grade_health_potion,high_grade_health_potion,supreme_grade_health_potion};

///Start Screen
void newGame();
void oldGame();

///Main Menu
void main_menu();
void map_screen();
void brewing_screen();
void inventory_screen();
void status_screen();
void help(int code);
void save();

///Actions
void hunt(int low,int high);
void gather_item(int area);

///Item Management
int search_for_item(int item);
void store_item(int item);
char* name_of_item(int item);

///Battle Functions
Monster spawn_monster(int level);
void fight(Monster m);
int player_attack(Monster m);
attack choose_attack();
int monster_attack(Monster m);
void player_lost();
void monster_lost(Monster m);

///Misc
void calibrate();
void goToMainMenu();
void add_delay(float sec);

///Monster Initialization
void initialize_monsters_area1();
void initialize_monsters_area2();
void initialize_monsters_area3();
void initialize_monsters_area4();
void initialize_monsters_area5();
void initialize_attacks();

int main()
{
    system("cls");

    srand(time(NULL)); ///Set seed for random number generator function rand()

    ///Stored Monsters into array
    initialize_monsters_area1();
    initialize_monsters_area2();
    initialize_monsters_area3();
    initialize_monsters_area4();
    initialize_monsters_area5();

    initialize_attacks(); ///Stored Attacks into Array

    ///Main Screen
    printf("___TEXT__ADVENTURE____\n");
    printf("\nEnter option number to select.\n");
    printf("\n1 : New Game\n");
    printf("2 : Continue Old Game\n");
    printf("3 : Help\n");

    int option=0;

    /** While loop is used to keep repeating the input command until a valid response is given
    I'll be using it pretty often and I won't explain Again **/
    while(1)
    {
        scanf("%d",&option);
        while((c=getchar())!='\n' && c!=EOF);

        switch(option)
        {
        case 1:
            newGame();
            break;

        case 2:
            oldGame();
            break;

        case 3:
            help(0);
            break;

        default :
            printf("\nWrong Choice !!!\nTry Again\n");
            break;
        }
    }
}

/// Main Menu screen
void main_menu()
{
    system("cls");
    printf("\n____MAIN__MENU____\n");
    int option=0;
    char flag;
    printf("\nEnter option number to select.\n");
    printf("\n1 : Map\n");
    printf("2 : Potion Brewery\n");
    printf("3 : Inventory\n");
    printf("4 : Status\n");
    printf("5 : Save\n");
    printf("6 : Help\n");
    printf("7 : Exit\n");


    scanf("%d",&option);
    while((c=getchar())!='\n' && c != EOF);
    switch(option)
    {
    case 1:
        map_screen();
        break;

    case 2:
        brewing_screen();
        break;

    case 3:
        inventory_screen();
        break;

    case 4:
        status_screen();
        break;

    case 5:
        save();
        goToMainMenu();
        break;

    case 6:
        help(1);
        break;

    case 7:
        printf("\nDo you want to save before exiting (Y/else)\n");
        scanf("%c",&flag);
        while((c=getchar())!='\n' && c != EOF);
        if(flag=='Y'||flag=='y')
        {
            save();
            exit(1);
        }
        else
            exit(1);
        break;

    default :
        printf("\nNo corresponding option found.\n");
        goToMainMenu();
        break;
    }
}

void map_screen()
{
    system("cls");
    printf("____MAP____\n");
    printf("\n1 : Beginner Forest (Level 1-5)\n");
    printf("\n2 : Poison Swamp (Level 6-10)\n");
    printf("\n3 : Misty River (Level 11-15)\n");
    printf("\n4 : Mutant Mountain (Level 16-20)\n");
    printf("\n5 : Treasure Peak (Level 21-25)\n");
    printf("\n6 : Free Zone (Level 26 and Above)\n");
    printf("\n7 : Main Menu\n");

    int option =0,probability = 0;
    isInFreeZone = 0;
    while(1)
    {
        scanf("%d",&option);
        while((c=getchar())!='\n' && c != EOF);
        /** To insure players are unable to enter an area before defeating previous boss monster.
            Different cases represent corresponding areas.
            Probability variable is used to randomly encounter either a monster or an item **/
        if(player.lvl > (option-1)*5 || option == 7)
        {
            switch(option)
            {
            case 1:
                probability = rand()%2;
                if(probability==0)
                    hunt(1,5);
                if(probability==1)
                    gather_item(1);
                break;

            case 2:
                probability = rand()%2+rand()%3;
                if(probability==0 || probability == 1)
                    hunt(6,10);
                if(probability==2 || probability == 3)
                    gather_item(2);
                break;

            case 3:
                probability = rand()%2+rand()%3;
                if(probability==0 || probability == 1)
                    hunt(11,15);
                if(probability==2 || probability == 3)
                    gather_item(3);
                break;

            case 4:
                probability = rand()%2+rand()%3;
                if(probability==0 || probability == 1)
                    hunt(16,20);
                if(probability==2 || probability == 3)
                    gather_item(4);
                break;

            case 5:
                probability = rand()%2+rand()%3;
                if(probability==1 || probability == 2)
                    hunt(21,25);
                if(probability==0 || probability == 3)
                    gather_item(5);
                break;

            case 6:
                isInFreeZone = 1;
                probability = rand()%2+rand()%3;
                if(probability==1 || probability == 2)
                    hunt(0,0);
                if(probability==0 || probability == 3)
                    gather_item(6);
                break;

            case 7:
                main_menu();
                break;

            default :
                printf("\nWrong Choice !!! \n");
                break;
            }
        }
        else
        {
            printf("\nPlease clear last area first !!!! \n\nChoose Again .\n");
        }
    }
}

///idk what to explain here everything is evident in the name and that flag is used for confirmation
void gather_item(int area)
{
    system("cls");
    char flag;
    int found_item=0;
    switch(area)
    {
    case 1:
        found_item = healing_herb;
        printf("\n 1 %s found press (Y) to collect (else) to leave.\n",name_of_item(found_item));
        scanf("%c",&flag);
        while((c=getchar())!='\n' && c != EOF);
        if(flag=='y' || flag=='Y')
            store_item(found_item);
        else
        {
            printf("\nYou ignored %s.\n",name_of_item(found_item));
            add_delay(1);
            map_screen();
        }
        break;

    case 2:
        found_item = healing_fruit;
        printf("\n 1 %s found press (Y) to collect (else) to leave.\n",name_of_item(found_item));
        scanf("%c",&flag);
        while((c=getchar())!='\n' && c != EOF);
        if(flag=='y' || flag=='Y')
            store_item(found_item);
        else
        {
            printf("\nYou ignored %s.\n",name_of_item(found_item));
            add_delay(1);
            map_screen();
        }
        break;

    case 3:
        found_item = healing_sand;
        printf("\n 1 %s found press (Y) to collect (else) to leave.\n",name_of_item(found_item));
        scanf("%c",&flag);
        while((c=getchar())!='\n' && c != EOF);
        if(flag=='y' || flag=='Y')
            store_item(found_item);
        else
        {
            printf("\nYou ignored %s.\n",name_of_item(found_item));
            add_delay(1);
            map_screen();
        }
        break;

    case 4:
        found_item = healing_gem;
        printf("\n 1 %s found press (Y) to collect (else) to leave.\n",name_of_item(found_item));
        scanf("%c",&flag);
        while((c=getchar())!='\n' && c != EOF);
        if(flag=='y' || flag=='Y')
            store_item(found_item);
        else
        {
            printf("\nYou ignored %s.\n",name_of_item(found_item));
            add_delay(1);
            map_screen();
        }
        break;

    case 5:
        found_item = healing_gem;
        printf("\n 1 %s found press (Y) to collect (else) to leave.\n",name_of_item(found_item));
        scanf("%c",&flag);
        while((c=getchar())!='\n' && c != EOF);
        if(flag=='y' || flag=='Y')
            store_item(found_item);
        else
        {
            printf("\nYou ignored %s.\n",name_of_item(found_item));
            add_delay(1);
            map_screen();
        }
        break;

    case 6:
        found_item = 1+rand()%4;
        printf("\n 1 %s found press (Y) to collect (else) to leave.\n",name_of_item(found_item));
        scanf("%c",&flag);
        while((c=getchar())!='\n' && c != EOF);
        if(flag=='y' || flag=='Y')
            store_item(found_item);
        else
        {
            printf("\nYou ignored %s.\n",name_of_item(found_item));
            add_delay(1);
            map_screen();
        }
        break;
    }
    add_delay(1);
    map_screen();
}

void hunt(int low,int high)
{
    system("cls");
    Monster m;

    if(low == 0) ///for free hunting
    {
        int draw_random_level=(rand()%5)*5+1+rand()%4;
        m = spawn_monster(draw_random_level);
    }
    else
    {
        if(player.lvl>high) ///If player returned to a previous area
            m = spawn_monster(low+rand()%4);
        else  /// For current area
            m=spawn_monster(player.lvl);
    }

    ///To display stats of encountered monster
    printf(" You encountered a/an %s \n\n",m.name);
    printf("Name  = %s\n",m.name);
    printf("Level = %d\n",m.lvl);
    printf("HP    = %ld\n",m.hp);
    printf("ATK   = %d\n",m.atk);
    printf("DEF   = %d\n",m.def);
    printf("AGI   = %d\n",m.agi);

    printf("\nPress any key to start the battle.\n");
    getch();
    fight(m);
}

Monster spawn_monster(int level)
{
    Monster monster;
    if(level%5 ==0) ///To spawn boss monster at levels which multiples of 5 (before free play)
    {
        monster = monsters[level][0];
    }
    else /// in other cases
    {
        monster = monsters[level][rand()%3];
    }
    return monster;
}

void newGame()
{
    system("cls");
    printf("\n______NEW___GAME______\n");
    printf("\nEnter Player Name (A-Z,a-z,0-9,max-length=12) : ");

    char name[13];
    while (1)
    {
        scanf("%s",name);
        while((c=getchar())!='\n' && c != EOF);
        ///To check if name is empty
        if(strcmp(name,"")==0)
        {
            printf("\nName cannot be empty !!!\n\nTry Again.\n");
            strcpy(name,"");
        }
        else
        {
            break;
        }
    }

    Player temp= {name,1,BASE_HP,ATTR_BASE_VALUE,ATTR_BASE_VALUE,ATTR_BASE_VALUE,0,100,100,BASE_SKILL_POINTS,0,(int*) calloc(INVENTORY_SIZE,sizeof(int))};
    player=temp;
    status_screen();
}

void oldGame()
{
    system("cls");
    char a[8];
    char name[13];
    FILE* f;

    ///To check if such file exists or not
    if(fopen("player.dat","r"))
        f = fopen("player.dat","r");
    else
    {
        printf("\nNo saved games found !!!\n");
        printf("\nPress any key to start a New Game.\n");
        getch();
        newGame();
    }

    ///Import data from save file
    fscanf(f," %s",name);
    Player temp= {name,0,0,0,0,0,0,0,0,0,0,(int*) calloc(INVENTORY_SIZE,sizeof(int))};

    fscanf(f," %s",a);
    temp.lvl=atoi(a);
    strcpy(a,"");

    fscanf(f," %s",a);
    temp.hp=atol(a);
    strcpy(a,"");

    fscanf(f," %s",a);
    temp.atk=atoi(a);
    strcpy(a,"");

    fscanf(f," %s",a);
    temp.def=atoi(a);
    strcpy(a,"");

    fscanf(f," %s",a);
    temp.agi=atoi(a);
    strcpy(a,"");

    fscanf(f," %s",a);
    temp.exp=atol(a);
    strcpy(a,"");

    fscanf(f," %s",a);
    temp.max_hp=atol(a);
    strcpy(a,"");

    fscanf(f," %s",a);
    temp.max_exp=atol(a);
    strcpy(a,"");

    fscanf(f," %s",a);
    temp.skill_points=atoi(a);
    strcpy(a,"");

    fscanf(f," %s",a);
    temp.death_count=atoi(a);
    strcpy(a,"");

    for(int i=0; i<INVENTORY_SIZE; i++)
    {
        fscanf(f," %s",a);
        temp.inventory[i]=atoi(a);
        strcpy(a,"");
    }

    fclose(f);
    player=temp;
    printf("\nOld game loaded successfully.\n");
    add_delay(1);
    printf("\nWelcome back %s.\n",player.name);
    add_delay(1);
    status_screen();
}

void save()
{
    FILE *f=fopen("player.dat","w");

    ///Export Data
    fprintf(f,"%s %d %ld %d %d %d %ld %ld %ld %d %d ",player.name,player.lvl,player.hp,player.atk,player.def,player.agi,player.exp,player.max_hp,player.max_exp,player.skill_points,player.death_count);
    for(int i=0; i<INVENTORY_SIZE; i++)
    {
        fprintf(f,"%d ",player.inventory[i]);
    }
    fclose(f);
    printf("\nGame saved successfully.\n");
}

void brewing_screen()
{
    system("cls");
    printf("____Potion__Brewery____\n");
    printf("\nSelect Potion\n");
    printf("\n1 : %s",name_of_item(low_grade_health_potion));
    printf("\n2 : %s",name_of_item(middle_grade_health_potion));
    printf("\n3 : %s",name_of_item(high_grade_health_potion));
    printf("\n4 : %s",name_of_item(supreme_grade_health_potion));
    printf("\n5 : Main Menu\n");

    int option =0;
    while(1)
    {
        scanf("%d",&option);
        while((c=getchar())!='\n' && c != EOF);
        switch(option)
        {
        case 1:
            printf("\n Ingredients required : \n");
            printf("2 %s\n",name_of_item(healing_herb));
            printf("\nProvide Ingredients (Y/else)\n");
            scanf("%c",&option);
            while((c=getchar())!='\n' && c != EOF);
            if(option=='Y' || option=='y')
            {
                int i = search_for_item(healing_herb);
                if(i==-1)
                {
                    printf("\nEnough ingredients not found.\n");
                    goToMainMenu();
                }
                else
                    player.inventory[i]=empty;

                int j = search_for_item(healing_herb);
                if(j==-1)
                {
                    printf("\nEnough ingredients not found.\n");
                    player.inventory[i]=healing_herb;  ///To restore previously taken items if an item is unavailable
                    goToMainMenu();
                }
                else
                    player.inventory[j]=empty;

                printf("\nBrewing %s .",name_of_item(low_grade_health_potion));
                add_delay(1); /// Delays are here just for feel
                printf(" .");
                add_delay(1);
                printf(" .");
                add_delay(1);
                printf("\n1 %s Obtained.\n",name_of_item(low_grade_health_potion));
                store_item(low_grade_health_potion);
                add_delay(2);
                brewing_screen();
            }
            else
            {
                goToMainMenu();
                break;
            }
            break;

        case 2:
            printf("\n Ingredients required : \n");
            printf("2 %s\n",name_of_item(healing_fruit));
            printf("\nProvide Ingredients (Y/else)\n");
            scanf("%c",&option);
            while((c=getchar())!='\n' && c != EOF);
            if(option=='Y' || option=='y')
            {
                int i = search_for_item(healing_fruit);
                if(i==-1)
                {
                    printf("\nEnough ingredients not found.\n");
                    goToMainMenu();
                }
                else
                    player.inventory[i]=empty;

                int j = search_for_item(healing_fruit);
                if(j==-1)
                {
                    printf("\nEnough ingredients not found.\n");
                    player.inventory[i]=healing_fruit;
                    goToMainMenu();
                }
                else
                    player.inventory[j]=empty;

                printf("\nBrewing %s .",name_of_item(middle_grade_health_potion));
                add_delay(1);
                printf(" .");
                add_delay(1);
                printf(" .");
                add_delay(1);
                printf("\n1 %s Obtained.\n",name_of_item(middle_grade_health_potion));
                store_item(middle_grade_health_potion);
                add_delay(1);
                brewing_screen();
            }
            else
            {
                goToMainMenu();
                break;
            }
            break;

        case 3:
            printf("\n Ingredients required : \n");
            printf("2 %s\n",name_of_item(healing_sand));
            printf("\nProvide Ingredients (Y/else)\n");
            scanf("%c",&option);
            while((c=getchar())!='\n' && c != EOF);
            if(option=='Y' || option=='y')
            {
                int i = search_for_item(healing_sand);
                if(i==-1)
                {
                    printf("\nEnough ingredients not found.\n");
                    goToMainMenu();
                }
                else
                    player.inventory[i]=empty;

                int j = search_for_item(healing_sand);
                if(j==-1)
                {
                    printf("\nEnough ingredients not found.\n");
                    player.inventory[i]=healing_sand;
                    goToMainMenu();
                }
                else
                    player.inventory[j]=empty;

                printf("\nBrewing %s .",name_of_item(high_grade_health_potion));
                add_delay(1);
                printf(" .");
                add_delay(1);
                printf(" .");
                add_delay(1);
                printf("\n1 %s Obtained.\n",name_of_item(high_grade_health_potion));
                store_item(high_grade_health_potion);
                add_delay(2);
                brewing_screen();
            }
            else
            {
                goToMainMenu();
                break;
            }
            break;

        case 4:
            printf("\n Ingredients required : \n");
            printf("2 %s\n",name_of_item(healing_gem));
            printf("\nProvide Ingredients (Y/else)\n");
            scanf("%c",&option);
            while((c=getchar())!='\n' && c != EOF);
            if(option=='Y' || option=='y')
            {
                int i = search_for_item(healing_gem);
                if(i==-1)
                {
                    printf("\nEnough ingredients not found.\n");
                    goToMainMenu();
                }
                else
                    player.inventory[i]=empty;

                int j = search_for_item(healing_gem);
                if(j==-1)
                {
                    printf("\nEnough ingredients not found.\n");
                    player.inventory[i]=healing_gem;
                    goToMainMenu();
                }
                else
                    player.inventory[j]=empty;

                printf("\nBrewing %s .",name_of_item(supreme_grade_health_potion));
                add_delay(1);
                printf(" .");
                add_delay(1);
                printf(" .");
                add_delay(1);
                printf("\n1 %s Obtained.\n",name_of_item(supreme_grade_health_potion));
                store_item(supreme_grade_health_potion);
                add_delay(2);
                brewing_screen();
            }
            else
            {
                goToMainMenu();
                break;
            }
            break;

        case 5:
            main_menu();
            break;

        default:
            printf("\nWrong Choice !!!\nTry Again\n");
            break;
        }
    }
    goToMainMenu();

}

int search_for_item(int item)
{
    printf("\nSearching for %s.....\n",name_of_item(item));
    add_delay(0.5);
    for(int i=0; i<INVENTORY_SIZE; i++)
    {
        if(player.inventory[i]==item)
        {
            return i; /// Return respective index if item is found
        }
        if (i==(INVENTORY_SIZE-1))
        {
            return -1; /// Return code -1 if item is not found
        }
    }
}

void store_item(int item)
{
    for(int i=0; i<INVENTORY_SIZE; i++)
    {
        if(player.inventory[i]==empty) /// Find the first empty slot
        {
            player.inventory[i]=item;
            printf("\n1 %s stored in inventory.\n",name_of_item(item));
            return;
        }
        if (i==(INVENTORY_SIZE-1)) /// If inventory is full
        {
            printf("\nCannot store %s Inventory is already full\n",name_of_item(item));
            add_delay(1);
            return;
        }
    }
}

/// To retrieve name of an item as a string
char* name_of_item(int item)
{
    switch(item)
    {
    case empty:
        return "Empty";
        break;

    case healing_herb:
        return "Healing Herb";
        break;

    case healing_fruit:
        return "Healing Fruit";
        break;

    case healing_sand:
        return "Healing Sand";
        break;

    case healing_gem:
        return "Healing Gem";
        break;

    case low_grade_health_potion:
        return "Low Grade Health Potion";
        break;

    case middle_grade_health_potion:
        return "Middle Grade Health Potion";
        break;

    case high_grade_health_potion:
        return "High Grade Health Potion";
        break;

    case supreme_grade_health_potion:
        return "Supreme Grade Health Potion";
    }
}

void inventory_screen()
{
    system("cls");
    printf("____INVENTORY____\n");
    printf("\n SLOT\tITEM\n\n");

    /// To display inventory
    for(int i=0; i<INVENTORY_SIZE; i++)
    {
        printf("  %d  \t%s\n",i+1,name_of_item(player.inventory[i]));
        add_delay(0.1);
    }

    ///To use an item
    printf("\nEnter slot number to use any item.\n");
    printf("Press 0 to return to main menu.\n");

    int option =0,i=0,temp=0;
    while(1)
    {
        scanf("%d",&option);
        while((c=getchar())!='\n' && c != EOF);
        if(option==0)
            main_menu();
        else
        {
            if(option<32)
            {
                int item = player.inventory[option-1];
                switch(item)
                {
                /// Healing consumables
                case healing_herb:
                    temp=player.hp;
                    if(player.max_hp-player.hp<10)
                        player.hp=player.max_hp;
                    else
                        player.hp+=10;
                    printf("\nConsumed 1 %s health increase by %d (%d >>%d)\n",name_of_item(healing_herb),player.hp-temp,temp,player.hp);
                    player.inventory[option-1]=empty;
                    add_delay(2);
                    inventory_screen();
                    break;

                case healing_fruit:
                    temp=player.hp;
                    if(player.max_hp-player.hp<30)
                        player.hp=player.max_hp;
                    else
                        player.hp+=30;
                    printf("\nConsumed 1 %s health increase by %d (%d >>%d)\n",name_of_item(healing_fruit),player.hp-temp,temp,player.hp);
                    player.inventory[option-1]=empty;
                    add_delay(2);
                    inventory_screen();
                    break;

                case healing_sand:
                    temp=player.hp;
                    if(player.max_hp-player.hp<200)
                        player.hp=player.max_hp;
                    else
                        player.hp+=200;
                    printf("\nConsumed 1 %s health increase by %d (%d >>%d)\n",name_of_item(healing_sand),player.hp-temp,temp,player.hp);
                    player.inventory[option-1]=empty;
                    add_delay(2);
                    inventory_screen();
                    break;

                case healing_gem:
                    temp=player.hp;
                    if(player.max_hp-player.hp<400)
                        player.hp=player.max_hp;
                    else
                        player.hp+=400;
                    printf("\nConsumed 1 %s health increase by %d (%d >>%d)\n",name_of_item(healing_gem),player.hp-temp,temp,player.hp);
                    player.inventory[option-1]=empty;
                    add_delay(2);
                    inventory_screen();
                    break;

                case low_grade_health_potion:
                    temp=player.hp;
                    if(player.max_hp-player.hp<40)
                        player.hp=player.max_hp;
                    else
                        player.hp+=40;
                    printf("\nConsumed 1 %s health increase by %d (%d >>%d)\n",name_of_item(low_grade_health_potion),player.hp-temp,temp,player.hp);
                    player.inventory[option-1]=empty;
                    add_delay(2);
                    inventory_screen();
                    break;

                case middle_grade_health_potion:
                    temp=player.hp;
                    if(player.max_hp-player.hp<100)
                        player.hp=player.max_hp;
                    else
                        player.hp+=100;
                    printf("\nConsumed 1 %s health increase by %d (%d >>%d)\n",name_of_item(middle_grade_health_potion),player.hp-temp,temp,player.hp);
                    player.inventory[option-1]=empty;
                    add_delay(2);
                    inventory_screen();
                    break;

                case high_grade_health_potion:
                    temp=player.hp;
                    if(player.max_hp-player.hp<500)
                        player.hp=player.max_hp;
                    else
                        player.hp+=500;
                    printf("\nConsumed 1 %s health increase by %d (%d >>%d)\n",name_of_item(high_grade_health_potion),player.hp-temp,temp,player.hp);
                    player.inventory[option-1]=empty;
                    add_delay(2);
                    inventory_screen();
                    break;

                case supreme_grade_health_potion:
                    temp=player.hp;
                    if(player.max_hp-player.hp<1000)
                        player.hp=player.max_hp;
                    else
                        player.hp+=1000;
                    printf("\nConsumed 1 %s health increase by %d (%d >>%d)\n",name_of_item(supreme_grade_health_potion),player.hp-temp,temp,player.hp);
                    player.inventory[option-1]=empty;
                    add_delay(2);
                    inventory_screen();
                    break;

                case empty:
                    printf("\n You selected an empty slot.\n");
                    add_delay(2);
                    inventory_screen();
                    break;

                default :
                    printf("\nNon Consumable Item !!!! \nTry Again \n");
                    break;
                }
            }
            else
            {
                printf("\nNo such inventory slot exists . \nTry Again \n");
            }
        }
    }
}

void status_screen()
{
    system("cls");
    printf("\n\tStatus\n");
    printf("\nName   : %s\n",player.name);
    printf("Level  : %d\n",player.lvl);
    printf("HP     : %ld / %ld\n",player.hp,player.max_hp);
    printf("ATK    : %d\n",player.atk);
    printf("DEF    : %d\n",player.def);
    printf("AGI    : %d\n",player.agi);
    printf("EXP    : %ld / %ld\n",player.exp,player.max_exp);
    printf("\nSkill Points : %d\n",player.skill_points);
    printf("\nDeath Count : %d\n",player.death_count);

    ///Allocate skill points in various attributes
    if(player.skill_points>0)
    {
        printf("\nYou have %d unused skill points.\n",player.skill_points);
        printf("1 skill point can increase any attribute by 1\n");
        printf("Press 1 to invest in Attack (ATK)\n");
        printf("Press 2 to invest in Defense (DEF)\n");
        printf("Press 3 to invest in Agility (AGI)\n");

        while(player.skill_points!=0)
        {
            int option=0,old;
            scanf("%d",&option);
            while((c=getchar())!='\n' && c != EOF);

            switch(option)
            {
            case 1:
                old = player.atk;
                player.atk+=1;
                printf("\nATK increased by 1 (%d >> %d)\n",old,player.atk);
                player.skill_points-=1;
                printf("\nRemaining Skill Points  = %d\n",player.skill_points);
                break;

            case 2:
                old = player.def;
                player.def+=1;
                printf("\nDEF increased by 1 (%d >> %d)\n",old,player.def);
                player.skill_points-=1;
                printf("\nRemaining Skill Points  = %d\n",player.skill_points);
                break;

            case 3:
                old = player.agi;
                player.agi+=1;
                printf("\nAGI increased by 1 (%d >> %d)\n",old,player.agi);
                player.skill_points-=1;
                printf("\nRemaining Skill Points  = %d\n",player.skill_points);
                break;

            default :
                printf("\nError in Input.\n");
                printf("\nRemaining Skill Points  = %d\n",player.skill_points);
                break;
            }
        }
        printf("\nAll skill points are distributed according to your arrangements.\n");
        status_screen();
    }
    goToMainMenu();
}

int player_attack(Monster monster)
{
    attack chosen_attack;
    printf("\n%s is going to attack.\n",player.name);
    chosen_attack=choose_attack(); ///Choose attack
    printf("\n%s is using %s on Monster\n",player.name,chosen_attack.name);

    /// Damage calculation
    int damage = (player.atk*chosen_attack.power)/100;
    int def_diff=monster.def - player.def;
    float def_factor;
    int chance_of_success = chosen_attack.accuracy+(player.agi-monster.agi)+((rand()%5)-2)*10;
    switch((monster.lvl-1)/5)
    {
    case 0:
        damage*=2;
        break;

    case 1:
        damage*=4;
        break;

    case 2:
        damage*=8;
        break;

    case 3:
        damage*=16;
        break;

    case 4:
        damage*=32;
        break;

    default :
        damage *= 64;
        break;
    }
    if(def_diff<20 && def_diff>0)
    {
        def_factor  =(100-(def_diff*3));
    }
    if(def_diff<=0)
    {
        def_factor = 100;
    }
    if(def_diff>=20)
    {
        def_factor = 40;
    }
    damage = damage*def_factor/100;
    damage=(damage*chance_of_success)/100;
    if(damage < 0)
        damage =0;

    if(monster.hp>damage)
        return(damage);
    else ///If opponent's hp is lower than calculated damage then reduce the damage to his remaining hp
        return(monster.hp);
}

int monster_attack(Monster monster)
{
    attack chosen_attack;
    printf("\n%s is going to attack.\n",monster.name);
    add_delay(0.5);
    switch(rand()%3) ///Randomly choose attack for monster
    {
    case 0:
        chosen_attack=attacks[head_bash];
        break;

    case 1:
        chosen_attack=attacks[punch];
        break;

    case 2:
        chosen_attack=attacks[kick];
        break;
    }
    printf("\n%s is using %s on %s\n",monster.name,chosen_attack.name,player.name);
    int damage = (monster.atk*chosen_attack.power)/100;
    int def_diff=player.def - monster.def;
    float def_factor;
    int chance_of_success = chosen_attack.accuracy+(monster.agi-player.agi)+((rand()%5)-2)*10;
    switch((monster.lvl-1)/5)
    {
    case 0:
        damage*=2;
        break;

    case 1:
        damage*=4;
        break;

    case 2:
        damage*=8;
        break;

    case 3:
        damage*=16;
        break;

    case 4:
        damage*=32;
        break;

    default :
        damage *= 64;
        break;
    }
    if(def_diff<20 && def_diff>0)
    {
        def_factor  =(100-(def_diff*3));
    }
    if(def_diff<=0)
    {
        def_factor = 100;
    }
    if(def_diff >= 20)
    {
        def_factor = 40;
    }
    damage = damage*def_factor/100;
    damage=(damage*chance_of_success)/100;
    if(damage < 0)
        damage =0;

    if(player.hp>damage)
        return(damage);
    else
        return(player.hp);
}

void fight(Monster monster)
{
    printf("\n____Battle Start____\n");
    int damage;
    while(1) /// To let the game going until one of the participant's hp reach 0
    {
        add_delay(1);
        damage = player_attack(monster);
        printf("\n%s suffered %d damage. (%d >> %d)\n",monster.name,damage,monster.hp,monster.hp-damage);
        monster.hp-=damage;
        if(monster.hp==0) /// Break the loop when one's hp reaches 0
        {
            monster_lost(monster);
            break;
        }
        add_delay(1);
        damage=monster_attack(monster);
        printf("\n%s suffered %d damage. (%d >> %d)\n",player.name,damage,player.hp,player.hp-damage);
        player.hp-=damage;
        if(player.hp==0)
        {
            player_lost();
            break;
        }

    }
}

void player_lost()
{
    printf("\n%s Lost\n",player.name);
    add_delay(1);

    player.death_count++;
    if(player.lvl <26) /// Before completing the game
    {
        player.lvl = ((player.lvl-1)/5)*5+1; /// Regress the level to area minimum
        printf("\nLevel Regressed to %d\n",player.lvl);
        calibrate();
        player.atk=ATTR_BASE_VALUE;
        player.def = ATTR_BASE_VALUE;
        player.agi = ATTR_BASE_VALUE;
        player.skill_points = BASE_SKILL_POINTS + (player.lvl-1)*SKILL_POINTS_PER_LEVEL;
        printf("\nPreviously allocated Skill Points are restored.\n");
        player.exp=0;
        player.hp=player.max_hp;
    }

    else /// For free play
    {
        player.exp = 0;
        player.hp = player.max_hp;
        printf("\nAll stored exp is lost.\n");
        printf("\nHP restored to maximum (%ld/%ld)\n",player.hp,player.max_hp);
    }
    save();
    add_delay(1);
    status_screen();
}

void monster_lost(Monster monster)
{
    printf("\n%s Lost.\n",monster.name);
    add_delay(1);
    if(player.lvl > 25 && isInFreeZone ==1)/// Do not give exp is player is over level 25 and is not in the free zone
    {
        player.exp+=monster.exp;
        printf("\n %s gained %ld exp.\n",player.name,monster.exp);
    }

    if(player.lvl == monster.lvl)
    {
        player.exp+=monster.exp;
        printf("\n %s gained %ld exp.\n",player.name,monster.exp);
    }
    if(player.exp>=player.max_exp) /// Level up
    {
        player.exp=0;
        player.lvl++;
        printf("\nLevel up!!! (%d >> %d)\n",player.lvl-1,player.lvl);
        add_delay(0.5);
        player.skill_points+=SKILL_POINTS_PER_LEVEL;
        printf("\n%d skill points gained.\n",SKILL_POINTS_PER_LEVEL);
        add_delay(0.5);
        calibrate();
        player.hp = player.max_hp;
        printf("\nHP returned to maximum.\n");
    }

    if(monster.lvl%5 == 0)
    {
        printf("\nYou defeated the Area Boss !!! \n\nNext area unlocked !!!\n");
        add_delay(2);
    }
    save();
    add_delay(1);
    status_screen();
}

attack choose_attack()
{
    printf("\nChoose Attack\n");
    printf("\n1 : Head Bash\n");
    printf("2 : Punch\n");
    printf("3 : Kick\n");

    int choice=0;

    while(1)
    {
        scanf("%d",&choice);
        while((c=getchar())!='\n' && c != EOF);
        switch(choice)
        {
        case 1:
            return attacks[head_bash];
            break;

        case 2:
            return attacks[punch];
            break;

        case 3:
            return attacks[kick];
            break;

        default :
            printf("\nWrong Choice !!!\nTry Again\n");
            break;
        }
    }
}

void initialize_monsters_area1()
{
    Monster temp1 = {"Slime",1,20,5,1,2,20};
    Monster temp2 = {"Goblin",1,50,4,5,5,50};
    Monster temp3 = {"Skeleton",1,40,4,6,3,40};
    Monster temp4 = {"Slime",2,25,7,3,3,25};
    Monster temp5 = {"Goblin",2,58,6,6,7,58};
    Monster temp6 = {"Skeleton",2,46,5,8,5,46};
    Monster temp7 = {"Slime",3,30,9,5,4,30};
    Monster temp8 = {"Goblin",3,66,8,7,9,66};
    Monster temp9 = {"Skeleton",3,52,6,10,7,52};
    Monster temp10 = {"Slime",4,35,11,7,5,35};
    Monster temp11 = {"Goblin",4,74,10,8,11,74};
    Monster temp12 = {"Skeleton",4,58,7,12,9,58};
    Monster temp13 = {"Hobgoblin",5,130,14,9,12,300};

    monsters[1][0]=temp1;
    monsters[1][1]=temp2;
    monsters[1][2]=temp3;
    monsters[2][0]=temp4;
    monsters[2][1]=temp5;
    monsters[2][2]=temp6;
    monsters[3][0]=temp7;
    monsters[3][1]=temp8;
    monsters[3][2]=temp9;
    monsters[4][0]=temp10;
    monsters[4][1]=temp11;
    monsters[4][2]=temp12;
    monsters[5][0]=temp13;

}

void initialize_monsters_area2()
{
    Monster temp1 = {"Poison viper",6,150,10,6,14,300};
    Monster temp2 = {"Giant Frog",6,200,17,8,10,400};
    Monster temp3 = {"Lizardman",6,250,10,14,12,500};
    Monster temp4 = {"Poison viper",7,160,12,7,16,320};
    Monster temp5 = {"Giant Frog",7,215,20,9,11,430};
    Monster temp6 = {"Lizardman",7,270,12,16,13,540};
    Monster temp7 = {"Poison viper",8,170,14,9,17,340};
    Monster temp8 = {"Giant Frog",8,230,23,10,12,460};
    Monster temp9 = {"Lizardman",8,290,14,18,14,580};
    Monster temp10 = {"Poison viper",9,180,16,11,18,360};
    Monster temp11 = {"Giant Frog",9,245,26,11,13,490};
    Monster temp12 = {"Lizardman",9,310,16,20,15,620};
    Monster temp13 = {"Scaly Naga",10,600,21,22,18,1400};

    monsters[6][0]=temp1;
    monsters[6][1]=temp2;
    monsters[6][2]=temp3;
    monsters[7][0]=temp4;
    monsters[7][1]=temp5;
    monsters[7][2]=temp6;
    monsters[8][0]=temp7;
    monsters[8][1]=temp8;
    monsters[8][2]=temp9;
    monsters[9][0]=temp10;
    monsters[9][1]=temp11;
    monsters[9][2]=temp12;
    monsters[10][0]=temp13;

}

void initialize_monsters_area3()
{
    Monster temp1 = {"Iron Tooth Piranha",11,500,28,8,18,1000};
    Monster temp2 = {"Murloc",11,1000,20,20,20,2000};
    Monster temp3 = {"Obsidian Tortoise",11,1500,18,33,13,3000};
    Monster temp4 = {"Iron Tooth Piranha",12,520,30,9,20,1040};
    Monster temp5 = {"Murloc",12,1030,22,22,21,2060};
    Monster temp6 = {"Obsidian Tortoise",12,1550,19,36,14,3100};
    Monster temp7 = {"Iron Tooth Piranha",13,540,32,10,22,1080};
    Monster temp8 = {"Murloc",13,1060,24,24,22,2120};
    Monster temp9 = {"Obsidian Tortoise",13,1600,20,39,15,3200};
    Monster temp10 = {"Iron Tooth Piranha",14,560,34,11,24,1120};
    Monster temp11 = {"Murloc",14,1090,26,26,23,2180};
    Monster temp12 = {"Obsidian Tortoise",14,1650,21,42,16,3300};
    Monster temp13 = {"Hydra",15,2500,28,28,28,8800};

    monsters[11][0]=temp1;
    monsters[11][1]=temp2;
    monsters[11][2]=temp3;
    monsters[12][0]=temp4;
    monsters[12][1]=temp5;
    monsters[12][2]=temp6;
    monsters[13][0]=temp7;
    monsters[13][1]=temp8;
    monsters[13][2]=temp9;
    monsters[14][0]=temp10;
    monsters[14][1]=temp11;
    monsters[14][2]=temp12;
    monsters[15][0]=temp13;

}

void initialize_monsters_area4()
{
    Monster temp1 = {"Mutant Zombie",16,2000,33,33,18,6000};
    Monster temp2 = {"Mutant Skeleton",16,1500,28,43,13,4500};
    Monster temp3 = {"Mutant Orc",16,3000,38,38,13,9000};
    Monster temp4 = {"Mutant Zombie",17,2050,35,35,19,6150};
    Monster temp5 = {"Mutant Skeleton",17,1530,29,46,14,4590};
    Monster temp6 = {"Mutant Orc",17,3100,40,40,14,9300};
    Monster temp7 = {"Mutant Zombie",18,2100,37,37,20,6300};
    Monster temp8 = {"Mutant Skeleton",18,1560,30,49,15,4680};
    Monster temp9 = {"Mutant Orc",18,3200,42,42,15,9600};
    Monster temp10 = {"Mutant Zombie",19,2150,39,39,21,6450};
    Monster temp11 = {"Mutant Skeleton",19,1590,31,52,16,4770};
    Monster temp12 = {"Mutant Orc",19,3300,44,44,16,9900};
    Monster temp13 = {"Orc Lord",20,4500,49,49,18,24000};

    monsters[16][0]=temp1;
    monsters[16][1]=temp2;
    monsters[16][2]=temp3;
    monsters[17][0]=temp4;
    monsters[17][1]=temp5;
    monsters[17][2]=temp6;
    monsters[18][0]=temp7;
    monsters[18][1]=temp8;
    monsters[18][2]=temp9;
    monsters[19][0]=temp10;
    monsters[19][1]=temp11;
    monsters[19][2]=temp12;
    monsters[20][0]=temp13;

}

void initialize_monsters_area5()
{
    Monster temp1 = {"Gargoyle",21,3000,28,58,18,15000};
    Monster temp2 = {"Ghost Armor",21,3500,38,48,23,17500};
    Monster temp3 = {"Wyvern",21,2500,48,13,48,12500};
    Monster temp4 = {"Gargoyle",22,3120,29,61,19,15600};
    Monster temp5 = {"Ghost Armor",22,3700,40,50,24,18500};
    Monster temp6 = {"Wyvern",22,2600,50,14,50,13000};
    Monster temp7 = {"Gargoyle",23,3240,30,64,20,16200};
    Monster temp8 = {"Ghost Armor",23,3900,42,52,25,19500};
    Monster temp9 = {"Wyvern",23,2800,52,15,52,14000};
    Monster temp10 = {"Gargoyle",24,3360,31,67,21,16800};
    Monster temp11 = {"Ghost Armor",24,4100,44,54,26,20500};
    Monster temp12 = {"Wyvern",24,2900,54,16,54,14500};
    Monster temp13 = {"Flame Dragon",25,7000,48,48,40,58000};


    monsters[21][0]=temp1;
    monsters[21][1]=temp2;
    monsters[21][2]=temp3;
    monsters[22][0]=temp4;
    monsters[22][1]=temp5;
    monsters[22][2]=temp6;
    monsters[23][0]=temp7;
    monsters[23][1]=temp8;
    monsters[23][2]=temp9;
    monsters[24][0]=temp10;
    monsters[24][1]=temp11;
    monsters[24][2]=temp12;
    monsters[25][0]=temp13;
}

void initialize_attacks()
{
    attack temp1 = {"Head Bash",100,50};
    attack temp2 = {"Punch",50,80};
    attack temp3 = {"Kick",80,60};
    attacks[head_bash]=temp1;
    attacks[punch]=temp2;
    attacks[kick]=temp3;
}

void add_delay(float sec)
{
    clock_t st = clock();
    while((clock()-st)<sec*CLOCKS_PER_SEC);
}

/// To calculate max exp and max hp for different levels
void calibrate()
{
    if(player.lvl<=5)
    {
        player.max_exp = 100 + (player.lvl-1)*50;
        player.max_hp = 100 + (player.lvl-1)*10;
    }
    if(player.lvl>5 && player.lvl<=10)
    {
        player.max_exp = 1000 + (player.lvl-6)*100;
        player.max_hp = 500 + (player.lvl-6)*25;
    }
    if(player.lvl > 10 && player.lvl <=15)
    {
        player.max_exp = 7000 + (player.lvl-11)*400;
        player.max_hp = 2500 + (player.lvl-11)*50;
    }
    if(player.lvl > 15 && player.lvl <=20)
    {
        player.max_exp = 20000 + (player.lvl-16)*1000;
        player.max_hp = 4000 + (player.lvl-16)*100;
    }
    if(player.lvl > 20 && player.lvl <=25)
    {
        player.max_exp =  50000+ (player.lvl-21)*2000;
        player.max_hp  =  6500 + (player.lvl-21)*200;
    }
    if(player.lvl >25)
    {
        player.max_exp = 100000 + (player.lvl-26)*1000;
        player.max_hp = 10000 + (player.lvl-26)*500;
    }

}

/// To wait for a response then redirect to main menu
void goToMainMenu()
{
    printf("\nPress any key to return to Main Menu.\n");
    getch();
    main_menu();
}

void help(int code)
{
    system("cls");
    printf("\n\t\tHELP\n");

    printf("\nStart Game:\n");
    printf("\n* Select \"New Game\" option if you don't have a save file.(You'll lose your old progress)\n");
    printf("* Continue your old progress by selecting \"Continue Old Game\".\n");

    printf("\nMap:\n");
    printf("\n* You can play on different maps according to your level.\n");
    printf("* Each map contains 3 unique monsters with unique attributes.\n");
    printf("* Sometimes you can also come across some item which will come in handy in your journey ahead.\n");
    printf("* After level 25 you can play in \"Free Zone\". In which you'll come across all monsters from previous maps.\n");

    printf("\nHow to play:\n");
    printf("\n* Open map screen.\n");
    printf("* Select map.\n");
    printf("* In case you came across a monster press any key to start the battle.\n");
    printf("* Choose attack.(The first turn will always be yours)\n");
    printf("* On the next turn monster will attack you.\n");
    printf("* First to reach 0 HP will lose.\n");

    printf("\nBrewery:\n");
    printf("\n* You can brew different potions here.\n");
    printf("* Select the potion you wanna brew and if you have enough items to brew it the potions will automatically be stored in your inventory.\n");

    printf("\nInventory:\n");
    printf("\n* All of your items will be stored here.\n");
    printf("* You can use consumable items from the inventory by entering their slot number.\n");

    printf("\nStatus:\n");
    printf("\n* You can view your status on status screen.\n");
    printf("* You can allocate your skill points in your desired attribute.\n");

    printf("\nAttributes:\n");
    printf("\n* HP - Current Health Points\n");
    printf("* Max HP - Maximum Health Points\n");
    printf("* ATK - Attack\n");
    printf("* DEF - Defense\n");
    printf("* AGI - Agility\n");
    printf("* EXP - Current Experience Points\n");
    printf("* Max EXP - Experience Points needed for level up\n");

    printf("\nAttacks:\n");
    printf("\n* Head Bash: Low accuracy - High damage\n");
    printf("* Punch: High accuracy - Low damage\n");
    printf("* Kick: Medium accuracy - Medium damage\n");

    if(code==0){
        printf("\nPress any key to return to previous screen.\n");
        getch();
        main();
    }
    if(code==1){
        goToMainMenu();
    }
}
