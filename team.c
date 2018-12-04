//
// EECS2031 team.c
//
// Program for maintaining a personal team.
//
//
// Uses a linked list to hold the team players.
//
// Author: <Baris Bagcilar>
// Student Info: <215271943>


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


//**********************************************************************
// Defines the player, which forms the data of the linked list
struct Players{
    char familyName[1024];
    char firstName[1024];
    char position[1024];
    int value;
};

//**********************************************************************
// Defines linked list node and pointer types
struct node{
    struct Players data; 
    struct node *next;
}tnode;



//**********************************************************************
// Linked List Function Declarations
//

struct node* insertNewNode (struct node *next, struct Players player);
void printLinkedList(struct node *head);
void searchByFamily(struct node *head);
int equalsChar(char string1[], char string2[]);
void valueCap(struct node *head);
void delete(struct node *head, char familyName[]);
int itemCounter(struct node *head);
int searchByFamilyBoolean(struct node *head, struct Players player);



//**********************************************************************
// Support Function Declarations
//

void safegets (char s[], int arraySize);        // gets without buffer overflow
void familyNameDuplicate (char familyName[]);   // marker/tester friendly 
void familyNameFound (char familyName[]);       //   functions to print
void familyNameNotFound (char familyName[]);    //     messages to user
void familyNameDeleted (char familyName[]);
void printTeamEmpty (void);
void printTeamTitle(void);
void printNoPlayersWithLowerValue(int value);

//**********************************************************************
// Program-wide Constants
//

const int MAX_LENGTH = 1023;
const char NULL_CHAR = '\0';
const char NEWLINE = '\n';
const char GOALKEEPER = 'G';
const char DEFENDER = 'D';
const char MIDFIELDER = 'M';
const char STRIKER = 'S';


//**********************************************************************
// Main Program
//

int main (void)
{ 
    const char bannerString[]
        = "Personal Team Maintenance Program.\n\n";
    const char commandList[]
        = "Commands are I (insert), D (delete), S (search by name),\n"
          "  V (search by value), P (print), Q (quit).\n";

    // Declares linked list head.
    struct node *head = NULL;
 
    // announce start of program
    printf("%s",bannerString);
    printf("%s",commandList);
    
    char response;
    char input[MAX_LENGTH+1];

    // declares the fields that will be obtained from the user
    char familyName[MAX_LENGTH+1];
    char firstName[MAX_LENGTH+1];
    char position[MAX_LENGTH+1];
    char valueChar[MAX_LENGTH+1];
    int isDigit, value, i, isDuplicate = 1;
    do
    {
        printf("\nCommand?: ");
        safegets(input,MAX_LENGTH+1);
        // Response is first char entered by user.
        // Convert to uppercase to simplify later comparisons.
        response = toupper(input[0]);

        if (response == 'I')
        {
            // Inserts a player entry into the linked list.
            // Maintains the list in correct order (G, D, M, S).

            // Obtains surname, prints error message if input is blank or if family name already exists in the team
            do{
                printf("  family name: ");
                safegets(familyName, MAX_LENGTH+1);                        
                if(strlen(familyName) == 0){
                    printf("  Input cannot be blank. \n");
                }
            }while(strlen(familyName) == 0);
            
            // Obtains given name, prints error message if input is blank
            do{
                printf("  first name: ");
                safegets(firstName, MAX_LENGTH+1);
                if(strlen(firstName) == 0){
                    printf("  Input cannot be blank. \n");
                }
            }while(strlen(firstName) == 0);

            // Obitains position, prints error message if position is not one of G, D, M, S
            do{
                printf("  position: ");
                safegets(position, MAX_LENGTH+1);
                if(((toupper(position[0]) != 'G' && toupper(position[0]) != 'D' && toupper(position[0]) != 'M' && toupper(position[0]) != 'S') || strlen(position) != 1) || !isalpha(position[0])){
                    printf("  Position must be one of G, D, M, S \n  (Goalkeeper, Defender, Midfielder, Striker) \n");
                }else{
                    position[0] = toupper(position[0]);
                }
            }while(((toupper(position[0]) != 'G' && toupper(position[0]) != 'D' && toupper(position[0]) != 'M' && toupper(position[0]) != 'S') || strlen(position) != 1)|| !isalpha(position[0]));
            
            //Obtains value, prints error if input contains nondigits, or if less than 0, or if greater than INT_MAX
            do{
                printf("  value: ");
                safegets(valueChar, MAX_LENGTH+1);
                value = atoi(valueChar);
                isDigit = 1;
                for (i = 0; i < strlen(valueChar); i++){
                    if (!isdigit(valueChar[i]) || value < 0 || value > 2147483647){
                        isDigit = 0;
                        printf("  The value must be an integer between 0 and 2147483647, inclusive \n");
                        break;
                    }
                }
            }while (isDigit == 0);
            
            // Initializes the player structure
            struct Players player;
            strcpy(player.familyName, familyName);
            strcpy(player.firstName, firstName);
            strcpy(player.position, position);
            player.value = value;

            if (searchByFamilyBoolean(head, player) == 1){
                familyNameDuplicate(familyName);
            }else{
                // If the linked list is empty, initializes the head
                if(head == NULL){
                    head = insertNewNode(NULL, player);
                // If position is goalie, enters at the end of goalies 
                }else if (position[0] == 'G'){
                    if(head->data.position[0] != 'G'){
                        struct node *temp = insertNewNode(head, player);
                        head = temp;
                    }else{
                        struct node *temp = head;
                        while(temp->next != NULL && temp->next->data.position[0] == 'G'){
                            temp = temp->next;
                        }
                        struct node *temp2 = insertNewNode(temp->next, player);
                        temp->next = temp2;
                    }
                    
                // If position is defender, enters at the end of defenders 
                }else if (position[0] == 'D'){
                        if(head->data.position[0] != 'G' && head->data.position[0] != 'D'){
                        struct node *temp = insertNewNode(head, player);
                        head = temp;
                        }else{
                            struct node *temp = head;
                            while((temp->next != NULL) && (temp->next->data.position[0] == 'G' || temp->next->data.position[0] == 'D')){
                                temp = temp->next;
                            }
                            struct node *temp2 = insertNewNode(temp->next, player);
                            temp->next = temp2;
                        }

                // If position is midfielder, enters at the end of midfielders 
                }else if (position[0] == 'M'){
                        if(head->data.position[0] == 'S'){
                            struct node *temp = insertNewNode(head, player);
                            head = temp;
                        }else{
                            struct node *temp = head;
                            while((temp->next != NULL) && (temp->next->data.position[0] == 'G' || temp->next->data.position[0] == 'D'|| temp->next->data.position[0] == 'M')){
                            temp = temp->next;
                            }
                            struct node *temp2 = insertNewNode(temp->next, player);
                            temp->next = temp2;
                        }


                // If position is striker, enters at the end the linked list
                }else if (position[0] == 'S'){
                    struct node *temp = head;
                    while(temp->next != NULL){
                        temp = temp->next;
                    }
                    struct node *temp2 = insertNewNode(NULL, player);
                    temp->next = temp2;
                }
            }
        }
        else if (response == 'D')
        {

             char familyName[MAX_LENGTH+1];

            do{
                printf("  family name: ");
                safegets(familyName, MAX_LENGTH+1);
                if(strlen(familyName) == 0){
                    printf("  Input cannot be blank. \n");
                }
            }while(strlen(familyName) == 0);

            // If family name belongs to head, deletes it. If not, calls the delete function.
            if(equalsChar(head->data.familyName, familyName)){
                struct node *temp = head;
                head = head->next;
                free(temp);
                familyNameDeleted(familyName);
                if (head == NULL){
                    printTeamEmpty();
                }
            }            
            else{
                delete(head, familyName);
            }

        }
        else if (response == 'S')
        {
            // Search for a player by family name.
            searchByFamily(head);

        }
        else if (response == 'V')
        {
            // Search for players that are worth less than or equal a value.
            valueCap(head);

        }
        else if (response == 'P')
        {
            //Prints the team
            printLinkedList(head);

        }
        else if (response == 'Q')
        {
            ; // do nothing, we'll catch this case below
        }
        else 
        {
            // do this if no command matched ...
            printf("\nInvalid command.\n%s\n",commandList);
        }
    } while (response != 'Q');
  
    // Deletes all items in the linked list
        while (head != NULL){
        struct node *temp = head;
        head = head->next;
        free(temp);
    }     
    
    //  Prints the linked list to confirm deletion
    printf("\nAll items deleted");
    printLinkedList(head);
    printf("\n");

    return 0;
}

//**********************************************************************
// Support Function Definitions

// Inserts a new node
struct node* insertNewNode(struct node* next, struct Players player){
    struct node *newNode = NULL;
    newNode = malloc(sizeof(tnode));
    newNode->next = next;

    strcpy(newNode->data.familyName, player.familyName);
    strcpy(newNode->data.firstName, player.firstName);
    strcpy(newNode->data.position, player.position);
    newNode->data.value = player.value;

    return newNode;
}

//Prints the linked list
void printLinkedList(struct node *head){
    int size = itemCounter(head);
    
    // If list if empty, print error
    if (head == NULL){
        printTeamEmpty();
    // If size is one, print head
    }else if (size == 1){
        printTeamTitle();
        printf("\n");
        printf("%s\n", head->data.familyName);
        printf("%s\n", head->data.firstName);
        printf("%s\n", head->data.position);
        printf("%d\n", head->data.value);  
    // If size is greater than one, first print head, then print the rest of the team
    }else{
        printTeamTitle();
        printf("\n");
        printf("%s\n", head->data.familyName);
        printf("%s\n", head->data.firstName);
        printf("%s\n", head->data.position);
        printf("%d\n", head->data.value);  

        struct node *temp = head;
        while(temp->next != NULL){
            temp = temp->next; 
            printf("\n");                  
            printf("%s\n", temp->data.familyName);
            printf("%s\n", temp->data.firstName);
            printf("%s\n", temp->data.position);
            printf("%d\n", temp->data.value);
        }
    }

}

// Deletes en entry from the team
void delete(struct node *head, char familyName[]){
    int found = 0;

    // Searches for the family name in the linked list
    // Error is printed if player with family name is not in team

    // If team is empty, print error
    if (head == NULL){
        printTeamEmpty();

    // Head is deleted within the main method
    // If team is not empty and player to be removed is not at head, traverse through the list to find the node to be removed
    // If cannot find the player to be removed in the team, print error message
    }else if(head->next != NULL){
        struct node *temp1 = head;
        struct node *temp2 = head->next;
        if (equalsChar(temp2->data.familyName, familyName)){
            found = 1;
            if (temp2->next == NULL){
                free(temp2);
                temp1->next = NULL;
            }else{
                temp1->next = temp2->next;
                free(temp2);
            }
            found = 1;
            familyNameDeleted(familyName);
        }else{
            while(temp2->next != NULL){
                temp1 = temp1->next;
                temp2 = temp2->next;
                if (equalsChar(temp2->data.familyName, familyName)){
                    found = 1;
                    if (temp2->next == NULL){
                        free(temp2);
                        temp1->next = NULL;
                    }else{
                        temp1->next = temp2->next;
                        free(temp2);
                    }
                    familyNameDeleted(familyName);
                }                
            }
        }
    }

    if (found == 0){
        familyNameNotFound(familyName);
    }
}

//Returns the number of items in the linked list
int itemCounter(struct node *head){
    int counter = 0;
    struct node *temp = head;
    if(head == NULL){
        return 0;
    }else{
        while(temp->next!=NULL){
            counter++;
            temp = temp->next;
        }
    }

}

//Returns 1 if char[] parameters are identical, 0 otherwise
int equalsChar(char string1[], char string2[]){
    int i, size = strlen(string1);
    if(strlen(string1) != strlen(string2)){
        return 0;
    }else{
        for(i = 0; i < size; i++){
            if (toupper(string1[i]) != toupper(string2[i])){
                return 0;
            } 
        }
    }
    return 1;
}

//Searches the linked list by family name
void searchByFamily(struct node *head){
    int found = 0;
    char searchToken[MAX_LENGTH+1];

    // If team is empty, prints the appropriate message
    if (head == NULL){
        printTeamEmpty();
    // If team is not empty, first checks the head
    // If the searched player is not at head, iteratively checks all entries in team
    }else{
        do{
            found = 0;
            printf("\nEnter family name to search for: ");
            safegets(searchToken, MAX_LENGTH+1);
                
            // If search token is empty, print error message
            if(strlen(searchToken) == 0){
                printf("\nSeach value cannot be blank");
            // Checks if the head contains the search token
            }else if (equalsChar(head->data.familyName, searchToken)){
                found = 1;
                familyNameFound(searchToken);
                printf("\n");
                printf("%s\n", head->data.familyName);
                printf("%s\n", head->data.firstName);
                printf("%s\n", head->data.position);
                printf("%d\n", head->data.value);
            // If head doesn't contain the search token, linked list is traversed
            }else{
                struct node *temp = head;
                while(temp->next != NULL){
                    found = 0;
                    temp = temp->next;
                    if (equalsChar(temp->data.familyName, searchToken)){
                        found = 1;
                        familyNameFound(searchToken);
                        printf("\n");
                        printf("%s\n", temp->data.familyName);
                        printf("%s\n", temp->data.firstName);
                        printf("%s\n", temp->data.position);
                        printf("%d\n", temp->data.value);
                        break;
                    }
                }
            }if (strlen(searchToken) != 0 && found == 0){
                familyNameNotFound(searchToken);
            }

        }while(strlen(searchToken) == 0);
    }
}

//Searches the linked list by family name
int searchByFamilyBoolean(struct node *head, struct Players player){

    // If team is empty, returns 0
    if (head == NULL){
        return 0;

    // If team is not empty, first checks the head
    // If the searched player is not at head, iteratively checks all entries in team
    }else if (equalsChar(head->data.familyName, player.familyName)){
            return 1;
    // If head doesn't contain the search token, linked list is traversed
    }else{
        struct node *temp = head;
        while(temp->next != NULL){
            temp = temp->next;
            if (equalsChar(temp->data.familyName, player.familyName)){
                 return 1;
            }
        }

    }
    return 0;
}

//Prints the entries that correspond to players with values less than indicated in the parameter
void valueCap(struct node *head){
    
    char valueChar[MAX_LENGTH+1];
    int isDigit, value, i, found = 0;

    // If team is empty, appropriate error is printed
    if (head == NULL){
        printTeamEmpty();
        printNoPlayersWithLowerValue(value);
    // If team is not empty, value is read from the user.
    // If the value satisfies the requirements, first the head is checked, then the linked list is traversed
    }else{    
        // Reads value from the user
        // Prints error message if value is not an integer between 0 and 2147483647   
        do{
            printf("\nEnter value: ");
            safegets(valueChar, MAX_LENGTH+1);
            value = atoi(valueChar);
            isDigit = 1;
            for (i = 0; i < strlen(valueChar); i++){
                if (!isdigit(valueChar[i]) || value < 0 || value > 2147483647){
                    isDigit = 0;
                    printf("  The value must be an integer between 0 and 2147483647, inclusive \n");
                    break;
                }
            }
        }while (isDigit == 0);
        
        // If entered value satisfies the requirements, head is checked
        if (head->data.value <= value){
            found = 1;
            printf("\n");
            printf("%s\n", head->data.familyName);
            printf("%s\n", head->data.firstName);
            printf("%s\n", head->data.position);
            printf("%d\n", head->data.value);            
        }
        // List is traversed for players with appropriate values
        struct node *temp = head;
        while(temp->next != NULL){
            temp = temp->next;
            if (temp->data.value <= value){
                found = 1;
                printf("\n");
                printf("%s\n", temp->data.familyName);
                printf("%s\n", temp->data.firstName);
                printf("%s\n", temp->data.position);
                printf("%d\n", temp->data.value);
            }
        }
    }
    if (head !=NULL && found == 0){
        printNoPlayersWithLowerValue(value);
    }
}

// Function to get a line of input without overflowing target char array.
void safegets (char s[], int arraySize)
{
    int i = 0, maxIndex = arraySize-1;
    char c;
    while (i < maxIndex && (c = getchar()) != NEWLINE)
    {
        s[i] = c;
        i = i + 1;
    }
    s[i] = NULL_CHAR;
}

// Function to call when user is trying to insert a family name 
// that is already in the book.
void familyNameDuplicate (char familyName[])
{
    printf("\nAn entry for <%s> is already in the team!\n"
           "New entry not entered.\n",familyName);
}

// Function to call when a player with this family name was found in the team.
void familyNameFound (char familyName[])
{
    printf("\nThe player with family name <%s> was found in the team.\n",
             familyName);
}

// Function to call when a player with this family name was not found in the team.
void familyNameNotFound (char familyName[])
{
    printf("\nThe player with family name <%s> is not in the team.\n",
             familyName);
	
}

// Function to call when a family name that is to be deleted
// was found in the team.
void familyNameDeleted (char familyName[])
{
    printf("\nDeleting player with family name <%s> from the team.\n",
             familyName);
}

// Function to call when printing an empty team.
void printTeamEmpty (void)
{
    printf("\nThe team is empty.\n");
}

// Function to call to print title when whole team being printed.
void printTeamTitle (void)
{
    printf("\nMy Team: \n");
}

// Function to call when no player in the team has lower or equal value to the given value
void printNoPlayersWithLowerValue(int value)
{
	printf("\nNo player(s) in the team is worth less than or equal to <%d>.\n", value);
}

//**********************************************************************
// Add your functions below this line.

