#include <ptrie.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node {
    struct ptriearray * next;
    char * string;
    int count;
    int max_count;

};

struct ptriearray {
    struct node s[256];

};


struct ptrie {
    struct ptriearray * root;
};


struct ptrie *ptrie_allocate(void)
{
    struct ptrie * pt = calloc(1, sizeof(struct ptrie));
    pt->root = calloc(1, sizeof(struct ptriearray));
    if(pt == NULL)
    {
        return NULL;
    }
    if (pt->root == NULL) 
    {
        free(pt);
        return NULL;
    }
    return pt;
}

static void freeTraversal(struct ptriearray * prt)
{
    if(prt == NULL)
    {
        return;
    }
    
    struct node *cur_node;
    for (int i = 0; i < 256; i++)
    {
        cur_node = &prt->s[i];
        //free the string 
        free(cur_node->string);

        if (cur_node->next != NULL)
        {
            freeTraversal(cur_node->next);
        }
        
    }
    free(prt);

}

void ptrie_free(struct ptrie *pt)
{
    if(pt == NULL)
    {
        return;
    }
    freeTraversal(pt->root);
    free(pt);
}

static void updateMax(struct ptrie *pt, const char * str, int max)
{
    int l = strlen(str);
    struct ptriearray *cur_array = pt->root;
    struct node *cur_node = NULL;
    int ind;
    for (int i = 0; i < l; i++)
    {
        ind = (int)str[i];
        cur_node = &cur_array->s[ind];
        if (cur_node->next != NULL)
        {
            cur_node->max_count = max;
        }
        cur_array = cur_node->next;
    }
}

int ptrie_add(struct ptrie *pt, const char *str)
{
    if (pt == NULL || str == NULL)
    {
        return -1;
    }

    //checking the ASCII characters
    int l = strlen(str);
    for (int i = 0; i < l; i++)
    {
        if (((int)str[i]) < 32)
        {
            return -1;
        }
    }

    char *c = strdup(str);
    if (c == NULL)
    {
        return -1;
    }

    struct ptriearray *cur_array = pt->root;
    struct node *cur_node = NULL;
    int ind;
    for (int i = 0; i < l; i++)
    {
        ind = (int)str[i];
        cur_node = &cur_array->s[ind];
        if (cur_node->next == NULL)
        {
            cur_node->next = calloc(1, sizeof(struct ptriearray));
            if (cur_node->next == NULL)
            {
                free(c);
                return -1;
            }
        }
        cur_array = cur_node->next;
    }


    if (cur_node->string == NULL)
    {
        cur_node->string = c;
        cur_node->count = 1;
    }
    else
    {
        cur_node->count++;

        if (cur_node->count > cur_node->max_count)
        {
            cur_node->max_count = cur_node->count;
            updateMax(pt, str, cur_node->max_count);
        }

        free(c);
        return 0;
    }

    if (cur_node->count > cur_node->max_count)
    {
        cur_node->max_count = cur_node->count;
        updateMax(pt, str, cur_node->max_count);
    }

    return 0;
}


char * ptrie_autocomplete(struct ptrie *pt, const char * str)
{
    if(pt == NULL || str == NULL)
    {
        return NULL;
    }

    int maximum = 0; 
    int ind = 0;
    int j = 0;
    struct ptriearray *cur = pt->root;
    int size = strlen(str);
    
    for(int i = 0; i < size; i ++)
    {
        if(str[i+1] == '\0')
        {
            if(cur->s[(int) str[i]].next == NULL)
            {
                return strdup(str);
            }
            if(cur->s[(int) str[i]].count == cur->s[(int)str[i]].max_count)
            {
                return strdup(cur->s[(int) str[i]].string);
            }
        }
        cur = cur->s[(int) str[i]].next;
        if(cur == NULL)
        {
            return strdup(str);
        }
    }

    for(j = 0; j < 256; j++)
    {
        if(cur->s[j].max_count > maximum)
        {
            ind = j;
            maximum = cur->s[j].max_count;
        }
        if(j > 254)
        {
            if(cur->s[ind].next == NULL)
            {
                return strdup(str);
            }

            if(cur->s[ind].string != NULL)
            {
                if(cur->s[ind].max_count == cur->s[ind].count)
                {
                    return strdup(cur->s[ind].string);
                }
            }

            cur = cur->s[ind].next;
            maximum = 0;
            ind = 0;
            j = 0;
        }
    }
    return strdup(str);
}


static void traversalWithRecur(struct ptriearray * prt, char * prefix, int index)
{
    if(prt == NULL)
    {
        return;
    }
    
    //print info
    
    struct node *cur_node;
    for (int i = 0; i < 256; i++)
    {
        cur_node = &prt->s[i];
        if (cur_node->string != NULL)
        {
            prefix[index] = (char)i;
            if (cur_node->next != NULL)
            {
                traversalWithRecur(cur_node->next, prefix, index + 1);
            }
            else
            {
                prefix[index + 1] = '\0';
                //prints it here
                printf("%s\n", prefix);
            }
        }
    }

}

void ptrie_print(struct ptrie *pt)
{
    if(pt == NULL)
    {
        return;
    }
    char string[1024];
    traversalWithRecur(pt->root, string, 0);
}


