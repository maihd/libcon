#include <stdio.h>
#include <stdlib.h>

#include "../include/link.h"
#include "../include/macros.h"

struct person
{
    const char* name;
    Link sibling;  
};

int main(int argc, char* argv[])
{
    struct person* person;
    const char* person_names[] = {
        "David", "Albert", "Alice", "Becky", "Peter", "Robert"
    };

    int i, n;
    struct person* root_person = NULL;
    struct person* prev_person = NULL;
    for (i = 0, n = countof(person_names); i < n; i++)
    {
        person = malloc(sizeof(struct person));
        if (person)
        {
            person->name = person_names[i];
            person->sibling.next = NULL;
            person->sibling.prev = NULL;

            if (!root_person) root_person = person;
            if (prev_person)
            {
                Link_attachPrev(&prev_person->sibling, &person->sibling);
            }
            prev_person = person;
        }
    }

    person = root_person;
    while (person)
    {
        printf("person->name: %s\n", person->name);

        person = Link_container(person->sibling.next, struct person, sibling);
    }

    person = root_person;
    while (person)
    {
        struct person* next = Link_container(person->sibling.next, struct person, sibling);

        free(person);

        person = next;
    }

    return 0;
}