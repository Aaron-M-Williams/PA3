#ifndef PA3_H
#define PA3_H
#include <iostream>
#include <stdexcept>
#include <string>
#include <math.h>

using namespace std;

class Stack
{
    struct node
    {
        string item;

        node* next;

        node(string item, node* next):
                item(item),
                next(next) {}

        ~node() {}
    };

    node* head = NULL; //top of stack
    node* tail = NULL; //bottom of stack

public:
    Stack() {}

    //removes all items in linked list
    ~Stack()
    {
        while (head != NULL)
        {
            node* curr = head;
            head = head->next;
            delete curr;
        }
    }

    int size()
    {
        int counter = 1;
        node* curr = tail;

        if (head == NULL)
        {
            return 0;
        }

        while (curr->next != NULL)
        {
            counter += 1;
            curr = curr->next;
        }

        return counter;
    }
    void insert(string newItem)
    {


        if (size() == 1)
        {
            head = new node(newItem, NULL);
            tail->next = head;
            return;
        }
        if (size() == 0)
        {
            head = new node(newItem, NULL);
            tail = head;
            return;
        }
        node* temp = head;
        head = new node(newItem, NULL);
        temp->next = head;

        return;
    }

    string top()
    {
        if (head == NULL)
        {
            return "";
        }

        return (head->item);
    }

    int depthOfLoops()
    {
        if (size() == 0)
            return 0;

        int fors = 0;
        int begins = 0;
        int ends = 0;

        node* curr = tail;

        for (int i = 0; i < size(); i++)
        {
            if (curr->item == "FOR")
            {
                fors=fors+ 1;
            }
            else if (curr->item == "BEGIN")
            {
                begins=begins+ 1;
            }
            else if (curr->item == "END")
            {
                ends=ends+ 1;
            }

            curr = curr->next;
        }

        if (fors == begins && begins == ends && ends==fors)
        {
            curr = tail;

            int continous = 0;
            int temp = 0;

            for (int i = 0; i < size(); i++)
            {
                if (curr->item == "BEGIN")
                {
                    temp += 1;
                }

                else if (curr->item == "END")
                {
                    continous = max(temp, continous);
                    temp = 0;
                }

                if (curr->next != NULL)
                {
                    curr = curr->next;
                }
            }

            return continous;
        }

        else
        {
            int pairs = 0; //number of FOR-BEGIN pairs in the stack

            curr = tail;

            for (int i = 0; i < size(); i++)
            {
                if (curr->item == "FOR" && (curr->next)->item == "BEGIN")
                {
                    pairs += 1;
                }

                if (curr->next != NULL)
                {
                    curr = curr->next;
                }
            }
            int minimum = min(pairs, ends);
            return minimum;
        }
    }
};



#endif // PA3_H
