#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "list_sort.h"

typedef struct __element {
    char *value;
    struct list_head list;
} list_ele_t;

static bool validate(struct list_head *q)
{
    struct list_head *node;
    list_for_each(node, q)
    {
        if (node->next == q)
            break;
        if (strcmp(list_entry(node, list_ele_t, list)->value,
                   list_entry(node->next, list_ele_t, list)->value) > 0)
            return false;
    }
    return true;
}

static void q_free(struct list_head *q)
{
    if (!q)
        return;

    list_ele_t *tmp = NULL;
    while (!list_empty(q)) {
        tmp = list_first_entry(q, list_ele_t, list);
        list_del(&tmp->list);
        free(tmp->value);
        free(tmp);
    }
}

bool q_insert_head(struct list_head *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    char *new_value = strdup(s);
    if (!new_value) {
        free(newh);
        return false;
    }

    newh->value = new_value;
    list_add_tail(&newh->list, q);

    return true;
}

typedef enum { ascend, descend } arrangement;

int ele_cmp(void *priv, struct list_head *a, struct list_head *b)
{
    if (*(arrangement *) priv == ascend) {
        return strcmp(list_entry(a, list_ele_t, list)->value,
                      list_entry(b, list_ele_t, list)->value);
    } else {
        return strcmp(list_entry(b, list_ele_t, list)->value,
                      list_entry(a, list_ele_t, list)->value);
    }
}

int main(void)
{
    FILE *fp = fopen("cities.txt", "r");
    if (!fp) {
        perror("failed to open cities.txt");
        exit(EXIT_FAILURE);
    }

    struct list_head q;
    INIT_LIST_HEAD(&q);
    char buf[256];
    while (fgets(buf, 256, fp))
        q_insert_head(&q, buf);
    fclose(fp);

    arrangement priv = ascend;
    list_sort(&priv, &q, ele_cmp);
    assert(validate(&q));

    q_free(&q);

    return 0;
}