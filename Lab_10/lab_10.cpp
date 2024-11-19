#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// Estructura de un nodo en el heap binomial
struct BinomialNode {
    int key;
    int degree;
    BinomialNode* parent;
    BinomialNode* child;
    BinomialNode* sibling;

    BinomialNode(int k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

// Clase para manejar el heap binomial
class BinomialHeap {
private:
    BinomialNode* head;

     BinomialNode* mergeTrees(BinomialNode* t1, BinomialNode* t2) {
        cout << "Fusionando arboles con raices: " << t1->key << " y " << t2->key << endl;
        
        if (t1->key > t2->key) swap(t1, t2);
        t2->parent = t1;
        t2->sibling = t1->child;
        t1->child = t2;
        t1->degree++;
        return t1;
    }


    // Unir dos heaps binomiales
    BinomialNode* unionHeaps(BinomialNode* h1, BinomialNode* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        BinomialNode* newHead = nullptr;
        BinomialNode** pos = &newHead;

        // Fusionar las listas de raíces
        while (h1 && h2) {
            if (h1->degree <= h2->degree) {
                *pos = h1;
                h1 = h1->sibling;
            } else {
                *pos = h2;
                h2 = h2->sibling;
            }
            pos = &(*pos)->sibling;
        }

        // Agregar los nodos restantes
        *pos = h1 ? h1 : h2;
        return newHead;
    }

    // Ajustar el heap para que cumpla con las propiedades del heap binomial
    BinomialNode* adjustHeap(BinomialNode* head) {
        if (!head || !head->sibling) return head;

        BinomialNode* prev = nullptr;
        BinomialNode* curr = head;
        BinomialNode* next = curr->sibling;

        while (next) {
            if (curr->degree != next->degree || (next->sibling && next->sibling->degree == curr->degree)) {
                prev = curr;
                curr = next;
            } else {
                if (curr->key <= next->key) {
                    curr->sibling = next->sibling;
                    curr = mergeTrees(curr, next);
                } else {
                    if (!prev) head = next;
                    else prev->sibling = next;
                    next = mergeTrees(next, curr);
                    curr = next;
                }
            }
            next = curr->sibling;
        }

        return head;
    }

public:
    BinomialHeap() : head(nullptr) {}

    // Insertar un nuevo elemento en el heap (enqueue)
    void enqueue(int key) {
        BinomialNode* newNode = new BinomialNode(key);
        head = unionHeaps(head, newNode);
        head = adjustHeap(head);
    }

    // Mostrar el heap
    void printHeap() {
        if (!head) {
            cout << "El heap esta vacio." << endl;
            return;
        }
        cout<<endl;
        cout << "Heap binomial:" << endl;
        printHeapRec(head);
        cout << endl;
    }

private:
    // Función recursiva para imprimir el heap
    void printHeapRec(BinomialNode* node) {
        while (node) {
            cout << "Clave: " << node->key << ", Grado: " << node->degree << endl;
            if (node->child) {
                cout << "  Hijo de " << node->key << ": {" << endl;
                printHeapRec(node->child);
                cout << "  } Fin hijo de " << node->key << endl;
            }
            node = node->sibling;
        }
    }
};

int main() {
    BinomialHeap heap;

    // Insertar valores en el heap
    heap.enqueue(10);
    heap.enqueue(20);
    heap.enqueue(5);
    heap.enqueue(15);
    heap.enqueue(30);
    heap.enqueue(25);
    heap.enqueue(40);
    heap.enqueue(35);
    heap.enqueue(50);

    // Mostrar el heap
    heap.printHeap();

    return 0;
}
