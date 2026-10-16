#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static Node *create_list()
{
    int values[] = {1, 1, 5, 5, 1, 4};
    int size = sizeof(values) / sizeof(values[0]);

    Node *head = NULL;
    Node *tail = NULL;

    for (int i = 0; i < size; ++i)
    {
        Node *new_node = (Node *)malloc(sizeof(Node));
        new_node->value = values[i];
        new_node->next = NULL;

        if (head == NULL)
        {
            head = tail = new_node;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }
    }
    return head;
}

static void print_list(Node *head)
{
    Node *current = head;

    while (current != NULL)
    {
        printf("%d ", current->value);
        current = current->next;
    }

    puts("");
}

static Node *reverse_list(Node *head)
{
    Node *previous = NULL;
    Node *current = head;

    while (current != NULL)
    {
        Node *next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    return previous;
}

static void delete_list(Node **head)
{
    Node *current = *head;

    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    *head = NULL;
}

static Node *find_first(Node *head, int target, int *position)
{
    while (head != NULL)
    {
        if (head->value == target)
            return head;

        ++(*position);
        head = head->next;
    }

    *position = -1;
    return NULL;
}

int main()
{
    Node *head = create_list();

    print_list(head);

    head = reverse_list(head);

    print_list(head);

    int first_position = 1;
    Node *first = find_first(head, 5, &first_position);

    if (first == NULL)
        puts("-1");
    else
        printf("%d\n", first_position);

    int second_position = first_position + 1;
    Node *second = NULL;

    if (first != NULL)
        second = find_first(first->next, 5, &second_position);

    if (second == NULL)
        puts("-1");
    else
        printf("%d\n", second_position);

    delete_list(&head);

    return 0;
}
