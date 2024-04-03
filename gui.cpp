/* 
Author: Gideon Kipamet Kaiyian 
Student RegNo: SCT221-0255/2023
Author: Marylyne Nashipae Saoli 
Student RegNo: SCT221-0207/2023

How to run the code:
Type this in your terminal: gcc -o gui gui.cpp `pkg-config --cflags --libs gtk+-3.0`
 then type this: ./file1

*/

#include <gtk/gtk.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    char items[MAX_SIZE];
    int top;
} Stack;

void initialize(Stack *s) {
    s->top = -1;
}

int isEmpty(Stack *s) {
    return (s->top == -1);
}

void push(Stack *s, char c) {
    if (s->top == MAX_SIZE - 1) {
        printf("Stack overflow\n");
        exit(1);
    }
    s->items[++(s->top)] = c;
}

char pop(Stack *s) {
    if (isEmpty(s)) {
        printf("Stack underflow\n");
        exit(1);
    }
    return s->items[(s->top)--];
}

char peek(Stack *s) {
    return s->items[s->top];
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

void show_postfix(GtkWidget *widget, gpointer data) {
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
    GtkTextIter start, end;
    char *infix, postfix[MAX_SIZE];

    gtk_text_buffer_get_bounds(buffer, &start, &end);
    infix = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    infixToPostfix(infix, postfix);
    g_free(infix);

    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Postfix expression: %s", postfix);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *vbox, *label, *textview, *button, *scroll;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Change gtk_vbox_new to gtk_box_new
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label = gtk_label_new("Enter the infix expression:");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    textview = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(vbox), textview, TRUE, TRUE, 0);
    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scroll), textview);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Convert to Postfix");
    g_signal_connect(button, "clicked", G_CALLBACK(show_postfix), gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)));
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
