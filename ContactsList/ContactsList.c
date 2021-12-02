#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ContactList.h"

#define MAXCONTACTS 30 /* Max # of Contacts */
#define MAXPHONES 10 /* Max # of Phone Numbers for each Contact */
#define PHONE_NUMBER_MAX_LENGTH 12

//INCLUDE YOUR OWN "HELPER" ROUTINES AS YOU SEE FIT
struct contactName* NameList = NULL;

//Nested Loop for Phone Numbers and Contact Names
struct phoneNumbers
{
   char phoneNumber[PHONE_NUMBER_MAX_LENGTH];
   struct phoneNumbers *next; //Linkage
};

struct contactName
{
   char nameOfContact[256];
   struct phoneNumbers *phone_data; //Connects to PhoneNumbers Structure
   struct contactName* next; //Linkage
};

//Add Contact Function that takes the Name of the Person and Add it to the Node
void addContact(struct contactName **head, char name[256])
{
   if((*head) == NULL) //If Node is NULL, add the First Value
   {
      *head = malloc(sizeof(struct contactName));
      strcpy((*head)->nameOfContact,name);
      (*head)->next = NULL;
      (*head)->phone_data = NULL;
   }
   else //Constantly Add Additional Nodes and Shift Down
   {
      struct contactName *tempNode;
      tempNode = (*head);
      
      while(tempNode->next)
         tempNode=tempNode->next;

      (tempNode->next) = malloc(sizeof(struct contactName));
      strcpy(tempNode->next->nameOfContact,name);
      tempNode->next->next = NULL;
      tempNode->next->phone_data = NULL;
   }
}

//Attach Each Individual Phone Number to a Contact Name
void addPhonetoContact(struct contactName *head, char phone[PHONE_NUMBER_MAX_LENGTH])
{
   struct phoneNumbers* phoneNode = malloc(sizeof(struct phoneNumbers));
   strcpy(phoneNode->phoneNumber, phone);

   phoneNode->next = NULL;
   if (head->phone_data == NULL)
   {
      head->phone_data = phoneNode;
   }
   else
   {  
      phoneNode->next = head->phone_data;
      head->phone_data = phoneNode;
   }
}

struct contactName *timeToAdd (struct contactName *head, char name[256])
{
   struct contactName *tempNode = head;
   while (tempNode != NULL)
   {
      if (strcmp(tempNode->nameOfContact,name)==0)
         return tempNode;
      tempNode=tempNode->next;
   }
   return NULL;
}

void sortList(struct contactName *NameList)
{
   char temp[256];
   struct contactName* tempNode=NameList;
   struct contactName* tempNode1=tempNode->next;
   struct phoneNumbers *phoneTemp; //Connects to PhoneNumbers Structure

   while (tempNode!=NULL && tempNode1!=NULL)
   {
      if(strcmp(tempNode->nameOfContact,tempNode1->nameOfContact)>0)
      {
         strcpy(temp,tempNode1->nameOfContact);
         strcpy(tempNode1->nameOfContact,tempNode->nameOfContact);
         strcpy(tempNode->nameOfContact,temp);
         phoneTemp = tempNode1->phone_data;
         tempNode1->phone_data = tempNode->phone_data;
         tempNode->phone_data=phoneTemp;
      }
      tempNode=tempNode->next;
      tempNode1=tempNode->next;
   }
}

/*
 * convertInputToDataStructure
 * 
 */
void convertInputToDataStructure() 
{
   int sizeOfArray = sizeof contactList / sizeof contactList[0]; //Find Length of Array
   for (int i=0;i<sizeOfArray;)
   {
      char personName[256];
      if (isalpha(contactList[i][0])!=0)
      {
         strcpy(personName, contactList[i]);
         addContact(&NameList,personName);
      }

      while(++i && i<sizeOfArray)

      if (isdigit(contactList[i][0])!=0)
      {
         struct contactName *temporaryNode;
         temporaryNode = timeToAdd(NameList,personName);
         addPhonetoContact(temporaryNode,contactList[i]);
      }
      else
         break;
   }

   for (int i=0;i<100;i++)
      sortList(NameList);
}

void Display(struct contactName *head)
{
   struct contactName *currentNode;
   currentNode = head;
   while (currentNode!=NULL)
   {
      printf("Name: %s\n", currentNode->nameOfContact);
      struct phoneNumbers *currentPhone = currentNode->phone_data;
      printf("Phone(s):");

      while(currentPhone!=NULL)
      {
         printf(" %s", currentPhone->phoneNumber);
         currentPhone=currentPhone->next;
      }
      printf("\n");
      currentNode=currentNode->next;
   }
}

void ListAllContacts()
{
   Display(NameList);
}

/*
 * printMenu - Prints out the user menu
 */
void printMenu() 
{
   printf("\n\nEnter a command by number\n");
   printf("1. List all contacts in alphabetical order\n");
   printf("2. Print phone(s) for a contact\n");
   printf("3. Find duplicate entries\n");
   printf("4. Delete contact\n");
   printf("5. Exit the program\n");
   printf("Your input: ");
}

char* rtrim(char* str)
{
   int i;
   for (i = strlen(str) - 1; i >= 0 && str[i] == '\n'; i--)
      str[i] = '\0';
   return str;
}

void DisplayContact(struct contactName *head, char *key)
{
   struct contactName *currentNode;
   currentNode = head;
   while (currentNode!=NULL)
   {
      if (strcmp(currentNode->nameOfContact,key)==0)
      {
         printf("Name: %s\n", currentNode->nameOfContact);
         struct phoneNumbers *currentPhone = currentNode->phone_data;
         printf("Phone(s):");

         while(currentPhone!=NULL)
         {
            printf(" %s", currentPhone->phoneNumber);
            currentPhone=currentPhone->next;
         }
         printf("\n"); 
      }
      currentNode=currentNode->next;
   }
}

void PrintContact() 
{
   printf("Enter name: ");
   char printName[256];
   char* d = fgets(printName, sizeof(printName), stdin);
   d = rtrim(d);   
   DisplayContact(NameList,d);

   struct contactName *currentNode;
   currentNode=NameList;
   int legalCheck=0;
   while (currentNode!=NULL)
   {
      if (strcmp(currentNode->nameOfContact,d)==0)
         legalCheck=1;
      currentNode=currentNode->next;
   }
        
   if (legalCheck==0)
      printf("Contact not found\n");

}

void deleteFirstNode(struct contactName *head, char *key)
{
   struct contactName *temp = head;
   if(strcmp(temp->nameOfContact,key) == 0)
   {
      NameList=head->next;
   }
}

void deleteNode(struct contactName *head, char *insert)
{
   // Store head node
   int legalCheck=0, nodePosition=0;
   struct contactName *tempNode = head;

   while(tempNode!=NULL)
   {
      if(strcmp(tempNode->nameOfContact,insert) == 0)
      {
         legalCheck=1;
         break;
      }
      else
      {
         tempNode=tempNode->next;
         nodePosition++;
      }
   }

   if(legalCheck==1)
   {
      tempNode=head;
      if(nodePosition==0)
      {
         NameList = tempNode->next;
         free(tempNode);
      }

      for(int i = 0; i < nodePosition-1; i++)
         tempNode = tempNode->next;

      struct contactName *tempNode1 = tempNode->next;
      tempNode->next = tempNode1->next;
      free(tempNode1);
   }
}

/*
 * DeleteContact deletes a contact from the list
 * The user must enter the name of the contact to delete
 * 
 */
void DeleteContact() 
{
   printf("Enter name: ");
   char deleteName[256];
   char* d = fgets(deleteName, sizeof(deleteName), stdin);
   d = rtrim(d);   

   struct contactName *currentNode;
   currentNode=NameList;
   int legalCheck=0;
   while (currentNode!=NULL)
   {
      if (strcmp(currentNode->nameOfContact,d)==0)
         legalCheck=1;
      currentNode=currentNode->next;
   }
        
   if (legalCheck==0)
      printf("Contact not found\n");

   deleteFirstNode(NameList,d);
   deleteNode(NameList, d);
}

void findDuplicates(struct contactName *head)
{
   int legalCheck=0;
   struct contactName *temp1 = head;
   while (temp1!=NULL)
   {
      struct phoneNumbers *CurrentPhone = temp1->phone_data;
      while (CurrentPhone!=NULL)
      {
         struct contactName *temp2 = temp1->next;
         while (temp2!=NULL)
         {
            struct phoneNumbers *CurrentPhone1 = temp2->phone_data;
            while (CurrentPhone1!=NULL)
            {
               if (strcmp(CurrentPhone->phoneNumber,CurrentPhone1->phoneNumber)==0)
               {
                  printf("%s and %s have a phone number in common\n", temp1,temp2);
                  legalCheck=1;
                  break;
               }
               CurrentPhone1=CurrentPhone1->next;
            }
            temp2=temp2->next;
         }
         CurrentPhone=CurrentPhone->next;
      }
      temp1=temp1->next;
   }
   if (legalCheck==0)
   {
      printf("No duplicates found\n");
   }
}

/*
 * findDuplicates finds the names of contacts with 
 * a phone number in common
 */
void FindDuplicates() 
{
   findDuplicates(NameList);
}

int main()
{
   //First Move Data from ContactList.h to a Data Structure
   convertInputToDataStructure();
   
   char notDone = 1;
   while (notDone) {
      printMenu();
      char userChoice[100];
      char* s = fgets(userChoice, sizeof(userChoice), stdin);
      if (!s)
      {
         printf("Wrong input\n");
         continue;
      }
      s = rtrim(s); 
      int userSelection = atoi(s);

      switch (userSelection) {
         case 1: ListAllContacts(); break;
         case 2: PrintContact(); break;
         case 3: FindDuplicates(); break;
         case 4: DeleteContact(); break;
         case 5: notDone = 0; break;
         default: printf("Bad command...\n"); break;
         } 
   }
   
   return 0;
}
