#include <stdio.h>
#include <stdlib.h>

typedef struct Binomial_Tree {
    int valor;
    int k;
    struct Binomial_Tree *hijo;
    struct Binomial_Tree *padre;
    struct Binomial_Tree *bro;
} nodo_Bt;

typedef struct _heap_ {
    nodo_Bt *head;
} heap_t;

nodo_Bt *init_nodo(int valor) {
    nodo_Bt *nodo = malloc(sizeof(nodo_Bt));
    nodo->valor = valor;
    nodo->k = 0;
    nodo->hijo = NULL;
    nodo->padre = NULL;
    nodo->bro = NULL;
    return nodo;
}

heap_t *heap_ini() {
    heap_t *heap = malloc(sizeof(heap_t));
    heap->head = NULL;
    return heap;
}

nodo_Bt *merge_trees(nodo_Bt *t1, nodo_Bt *t2) {
    if (t1->valor > t2->valor) {
        nodo_Bt *temp = t1;
        t1 = t2;
        t2 = temp;
    }
    t2->padre = t1;
    t2->bro = t1->hijo;
    t1->hijo = t2;
    t1->k++;
    return t1;
}

nodo_Bt *union_heaps(nodo_Bt *h1, nodo_Bt *h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    nodo_Bt *new_head = NULL, *tail = NULL, *next_h1 = h1, *next_h2 = h2;

    while (next_h1 && next_h2) {
        nodo_Bt *min_tree = NULL;
        if (next_h1->k <= next_h2->k) {
            min_tree = next_h1;
            next_h1 = next_h1->bro;
        } else {
            min_tree = next_h2;
            next_h2 = next_h2->bro;
        }

        if (!new_head) {
            new_head = min_tree;
            tail = new_head;
        } else {
            tail->bro = min_tree;
            tail = min_tree;
        }
    }

    if (next_h1) tail->bro = next_h1;
    if (next_h2) tail->bro = next_h2;

    nodo_Bt *prev = NULL, *curr = new_head, *next = new_head->bro;
    while (next) {
        if (curr->k != next->k || (next->bro && next->bro->k == curr->k)) {
            prev = curr;
            curr = next;
        } else {
            if (curr->valor <= next->valor) {
                curr->bro = next->bro;
                curr = merge_trees(curr, next);
            } else {
                if (!prev) {
                    new_head = next;
                } else {
                    prev->bro = next;
                }
                curr = merge_trees(next, curr);
            }
        }
        next = curr->bro;
    }

    return new_head;
}

void enqueue(heap_t *heap, int valor) {
    nodo_Bt *new_tree = init_nodo(valor);
    heap->head = union_heaps(heap->head, new_tree);
}

void print_heap(nodo_Bt *head) {
    if (!head) return;

    printf("Nodo valor: %d, grado: %d\n", head->valor, head->k);

    if (head->hijo) {
        printf("Hijos del nodo %d:\n", head->valor);
        print_heap(head->hijo);
    }
    if (head->bro) {
        printf("Siguiente árbol:\n");
        print_heap(head->bro);
    }
}

void visualizar_heap(heap_t *heap) {
    if (!heap->head) {
        printf("Heap vacío.\n");
        return;
    }
    printf("Contenido del heap:\n");
    print_heap(heap->head);
}

int main() {
    heap_t *heap = heap_ini();
    enqueue(heap, 13);
    enqueue(heap, 7);
    enqueue(heap, 21);
    enqueue(heap, 5);

    visualizar_heap(heap);

    return 0;
}