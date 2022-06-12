#include "list.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

list *l_init() {
    list* newList = (list*) malloc(sizeof(list));
    newList->count = 0;
    newList->head = nullptr;
    return newList;
}

void l_insert(list * lst, char * word){
    element* newHead = (element*) malloc(sizeof(element));
    lst->count+=1;
    strcpy(newHead->word, word);
    newHead->next = lst->head;
    lst->head = newHead;
    
}

void l_delete(list * lst, char * word){
    element* target = lst->head;
    element* before = nullptr;
    for(; strcmp(target->word,word) != 0; target = target->next){
        before = target;
    }
    if(before == nullptr){
        lst->head=lst->head->next;
    }else{
        before->next=target->next;
    }
    lst->count-=1;
    free(target);
    
}

void l_print(list * lst) {
    for(element* current = lst->head; current->next != nullptr; current = current->next){
        for(size_t size = current->count, curr = 0; curr > size; curr++){
            printf("%c", current->word[curr]);
        }
         printf("\n");
    }

}

element *l_find(list * lst, char * word)
{
    element* target = lst->head;
    for(; strcmp(target->word,word) != 0; target = target->next){;}
    return target;
}

int readFile(list* list, char* filename){
    // Parser von und mit Kommentaren von Herbert Paulis:
    FILE * file;
    char line[300];        // sicher lang genug fuer eine Zeile
    file = fopen("alice.txt", "r");
    if (!file) // bei Fehler wird NULL zurueckgegeben
    {
        printf("Fehler beim Oeffnen\n");
        return 1;
    }
    while (!feof(file))    // Solange nicht Dateiende erreicht wurde
    {
        fgets(line, 300, file);
        if (isspace(line[0]))  // Eliminieren von Leerzeilen
            continue;
        line[strcspn(line, "\r\n")] = 0;// ersetzt beliebeige Kombination von CR&LF durch 0
   //     printf("%s\n", line);  // Testausgabe
  //      printf("1");
        // Alternative fuer die folgende while-schleife: C-Funktion strtok()
        // https://en.cppreference.com/w/c/string/byte/strtok
        
        int zeilen_index = 0; // Positionszaehler in der Zeile
        while (zeilen_index < strlen(line))
        {
            char wort[100];
            int wort_index = 0; // Positionszaehler im Wort
            int skip_extra = 0; // uebersprungene Zeichen
            for (int i = zeilen_index; i < strlen(line); i++)
            {
                if (line[i] == ' ')    // Leerzeichen
                {
                    if (i != zeilen_index)  // Wortende
                    {
                        break;
                    }
                    else                    // Zusätzliches Leerzeichen am Wort-/Zeilenanfang
                    {
                        skip_extra++;       // Merken, muessen spaeter dazugezaehlt werden
                        continue;
                    }
                }
                if (!isalpha(line[i])) // ueberspringe alles, was kein Buchstabe ist
                {
                    skip_extra++;       // Merken, muessen spaeter dazugezaehlt werden
                    continue;
                }
                wort[wort_index] = tolower(line[i]);   // Groszbuchsteben in Kleinbuchstaben umwandeln
                wort_index++;
            }
            wort[wort_index] = 0;
            zeilen_index += wort_index + skip_extra + 1;    // Zeilenindex korrigieren
            
            /*******************************************************************************/
            // Hier ist das jeweilige Wort in Variable wort enthalten zur spaeteren Weiterverarbeitung in der Liste
            // Achtung: wort kann Leerstring "" enthalten (z.B. wenn Zahl im Text war)!
            printf("%s:", wort);    // Testausgabe
           
            element * tmp;
            if(!l_find(list, tmp->word)){
                l_insert(list, tmp->word);
            }
            
            // TODO
            // Hier müssen die Elemente noch in die Liste hinzugefügt werden.
            // Aber Vorsicht. Nicht doppelt...
            // list wird als Parameter der Funktion readFile übergeben.
            
            /*******************************************************************************/
            if (tmp != nullptr){
                tmp->count++;
                continue;
            }
            l_insert(list, wort);
        }
      //  printf("\n");   // Nur fuer Testausgabe
    }
    return 0;
}

void frequencyCount(list *list, int *frequencies){
    element* elem = list->head;
    for(;elem!=nullptr;){
        for (int i = 0; elem->word[i] != '\0'; i++) {
            if (isalpha(elem->word[i])) {
                frequencies[(tolower(elem->word[i])-'a')]+=elem->count;
            }
        }
        elem = elem->next;
    }
}

int letterCount(list *list){
    int frequencies[26] = { 0 };
    frequencyCount(list, frequencies);
    int counter = 0;


    for (int i = 0; i < 26; i++) {
        counter += frequencies[i];
    }

    return counter;
    
}

int wordCount(list *list){
    element* elem = list->head;
    int counter = 0;
    for(;elem!=nullptr;){
        elem = elem->next;
        counter += elem->count;
    }
    return counter;
}

void deleteWords(list *list, int frequency){
    // Hinweis:
    // In unserem Fall wird für frequency z.B. 100 übergeben. Alle
    // Wörter mit einer geringeren Frequenz/Häufigkeit werden gelöscht.
    
    element* elem = list->head;
    element* tmp = nullptr;

    for(;elem!=nullptr;){
        if (elem->count < frequency) {
            tmp = elem->next;
            l_delete(list, elem->word);
            elem = tmp;
        }
        else {
            elem = elem->next;
        }
        
    }
}
