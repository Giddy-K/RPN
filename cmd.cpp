/* 
Author: Gideon Kipamet Kaiyian 
Student RegNo: SCT221-0255/2023
Author: Marylyne Nashipae Saoli 
Student RegNo: SCT221-0207/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SIZE 100

typedef struct {
    char elements[MAX_SIZE];
    int top;
} Stack;

void initialize(Stack *stk) {
    stk->top = -1;
}

int isEmpty(Stack *stk) {
    return (stk->top == -1);
}

void push(Stack *stk, char c) {
    if (stk->top == MAX_SIZE - 1) {
        printf("Sorry the was a stack overflow\n");
        exit(1);
    }
    stk->elements[++(stk->top)] = c;
}

char pop(Stack *stk) {
    if (isEmpty(stk)) {
        printf("Sorry the was a stack underflow\n");
        exit(1);
    }
    return stk->elements[(stk->top)--];
}

char peek(Stack *stk) {
    return stk->elements[stk->top];
}

int precedence(char op) {
    switch(op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        default:
            return 0;
    }
}

void infixToPostfix(char infix[], char postfix[]) {
    Stack s;
    initialize(&s);
    int i = 0, j = 0;

    while (infix[i] != '\0') {
        if (isdigit(infix[i])) {
            postfix[j++] = infix[i];
        } else if (infix[i] == '(') {
            push(&s, infix[i]);
        } else if (infix[i] == ')') {
            while (!isEmpty(&s) && peek(&s) != '(') {
                postfix[j++] = pop(&s);
            }
            pop(&s); // Discard '('
        } else {
            while (!isEmpty(&s) && precedence(infix[i]) <= precedence(peek(&s))) {
                postfix[j++] = pop(&s);
            }
            push(&s, infix[i]);
        }
        i++;
    }

    while (!isEmpty(&s)) {
        postfix[j++] = pop(&s);
    }
    postfix[j] = '\0';
}

int main() {
    char infixExpression[MAX_SIZE], postfixExpression[MAX_SIZE];
    
    printf("Enter the infix expression of your choice: ");
    fgets(infixExpression, MAX_SIZE, stdin);

    infixToPostfix(infixExpression, postfixExpression);

    printf("Postfix expression of your choice: %s\n", postfixExpression);

    return 0;
}
