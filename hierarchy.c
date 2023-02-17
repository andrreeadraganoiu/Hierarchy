#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"


/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, 
           atunci employee_name e primul om din ierarhie.
 */

Tree ReturnPerson(Tree tree, char *manager_name)
{
    if (tree == NULL)  
        return NULL;
    if (strcmp(tree->name, manager_name) == 0) // daca persoana cautata este radacina arborelui
        return tree;

    Tree person;
    for(int i = 0; i < tree->direct_employees_no; i++) 
    {
        person = ReturnPerson(tree->team[i], manager_name);
        if(person == NULL)
            continue;

        if (strcmp(person->name, manager_name) == 0) 
            return person;       
    }
    return NULL;
}
Tree init_node(char *employee_name)
{
    Tree Node = (Tree)malloc(sizeof(TreeNode));

    Node->name = (char*)malloc(100 * sizeof(char));
    strcpy(Node->name, employee_name);
    Node->manager = NULL;
    Node->team = NULL;
    Node->direct_employees_no = 0;

    return Node;
}
void Sort_by_name(Tree team[], int n)
{
    for(int i = 0; i < n - 1; i++)
    {   
        for(int j = i + 1; j < n; j++)
        {   
            if(strcmp(team[i]->name, team[j]->name) > 0)
            {
                Tree aux = team[i];
	            team[i] = team[j];
	            team[j] = aux;
            }
        }
    }
}
Tree hire(Tree tree, char *employee_name, char *manager_name) 
{
    if(tree == NULL  && manager_name == NULL)
    {
        tree = init_node(employee_name);  //creez nodul angajatului dat ca parametru
        return tree;
    }

    Tree Employee = init_node(employee_name);   //creez nodul angajatului dat ca parametru
    Tree Manager = ReturnPerson(tree, manager_name); //returnez adresa managerului angajatului

    Employee->manager = Manager;

    if(Manager == NULL) // daca managerul dat ca parametru nu exista
        return tree;

    Manager->team = (Tree*)realloc(Manager->team, (Manager->direct_employees_no + 1) * sizeof(Tree));
    Manager->team[Manager->direct_employees_no++] = Employee; //il adaug in echipa managerului sau
    Sort_by_name(Manager->team, Manager->direct_employees_no); //sortez in ordine alfabetica vectorul 
                                                               // de angajati
    return tree;
}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */

int get_position(Tree team[], Tree Employee, int n)
{
    int i, position;
    for(i = 0; i < n; i++)  
    {
        if(strcmp(team[i]->name, Employee->name)== 0 )
        {
            position = i;
            break;
        }   
    }
    return position;
}

Tree fire(Tree tree, char *employee_name) 
{
    Tree Employee = ReturnPerson(tree, employee_name); //returnez adresa angajatului dat ca parametru

    if(Employee == NULL) //daca angajatul nu exista
        return tree;
    if(Employee->manager == NULL) //daca angajatul este primul om din ierarhie, adica managerul 
                                  //sau este NULL 
        return tree;

    Tree Manager = Employee->manager;

    Manager->team = (Tree*)realloc(Manager->team, (Manager->direct_employees_no + Employee->direct_employees_no) * sizeof(Tree)); 
    for(int i = 0; i < Employee->direct_employees_no; i++)
    {
        Employee->team[i]->manager = Manager; //subordonatii angajatului concediat vor avea ca manager 
                                              //managerul angajatului in cauza
        Manager->team[Manager->direct_employees_no++] = Employee->team[i]; //adaug in echipa managerului angajatului 
                                                                           //subordonatii sai
    }
    int poz, n;
    n = Manager->direct_employees_no;

    //caut pozitia angajatului care trebuie concediat in vectorul teams al managerului
    poz = get_position(Manager->team, Employee, n);
    
    //refac vectorul de angajati
    for(int i = poz + 1; i < n ; i++)
        Manager->team[i - 1] = Manager->team[i];
    Manager->direct_employees_no--;
    Sort_by_name(Manager->team, Manager->direct_employees_no); //sortez in ordine alfabetica 
                                                               //vectorul de angajati

    free(Employee->team);
    free(Employee->name);
    free(Employee);

    return tree;
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */
Tree promote(Tree tree, char *employee_name) 
{
    Tree Employee = ReturnPerson(tree, employee_name); //returnez adresa angajatului dat ca parametru
    Tree Manager = Employee->manager;

    if(Employee == NULL) //daca angajatul nu exista
        return tree;
    if(Manager == NULL || Manager->manager == NULL ) // daca angajatul se afla pe nivelul 0 sau 1
        return tree;

    Manager->team = (Tree*)realloc(Manager->team, (Manager->direct_employees_no + Employee->direct_employees_no) * sizeof(Tree)); 
    for(int i = 0; i < Employee->direct_employees_no; i++)
    {
        Employee->team[i]->manager = Manager; //subordonatii angajatului promovat vor avea ca manager managerul angajatului in cauza
        Manager->team[Manager->direct_employees_no++] = Employee->team[i]; //adaug in echipa managerului angajatului subordonatii sai
    }
    int poz, n;
    n = Manager->direct_employees_no;

    //caut pozitia angajatului care trebuie promovat in vectorul teams al managerului
    poz = get_position(Manager->team, Employee, n);

    //refac vectorul de angajati
    for(int i = poz + 1; i < n ; i++)
        Manager->team[i - 1] = Manager->team[i];
    Manager->direct_employees_no--;
    Sort_by_name(Manager->team, Manager->direct_employees_no); //sortez in ordine alfabetica vectorul de angajati

    Manager->manager->team = (Tree*)realloc(Manager->manager->team, (Manager->manager->direct_employees_no + 1) * sizeof(Tree));
 
    Employee->manager = Manager->manager;
    Manager->manager->team[Manager->manager->direct_employees_no++] = Employee;

    for(int i = 0; i < Employee->direct_employees_no; i++)
        Employee->team[i] = NULL;
    Employee->direct_employees_no = 0;
    Sort_by_name(Manager->manager->team, Manager->manager->direct_employees_no); //sortez in ordine alfabetica vectorul de angajati

    return tree;
}

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name) 
{
    Tree Employee = ReturnPerson(tree, employee_name);
    Tree NewManager = ReturnPerson(tree, new_manager_name);
    Tree Manager = Employee->manager;

    if(Employee == NULL) //daca angajatul nu exista
        return tree;
    if(NewManager == NULL) //daca managerul nu exista
        return tree;
    if(Manager == NULL) //daca angajatul este primul om din ierarhie, adica managerul sau este NULL 
        return tree;
    if(Employee->manager == NewManager) //daca managerul angajatului este deja managerul dat ca parametru
        return tree;

    Manager->team = (Tree*)realloc(Manager->team, (Manager->direct_employees_no + Employee->direct_employees_no) * sizeof(Tree)); 
    for(int i = 0; i < Employee->direct_employees_no; i++)
    {
        Employee->team[i]->manager = Manager;
        Manager->team[Manager->direct_employees_no++] = Employee->team[i];
    }
    int poz, n;
    n = Manager->direct_employees_no;
    //caut pozitia angajatului care trebuie mutat in vectorul teams al managerului
    poz = get_position(Manager->team, Employee, n);
    for(int i = poz + 1; i < n ; i++)
        Manager->team[i - 1] = Manager->team[i];
    Manager->direct_employees_no--;
    Sort_by_name(Manager->team, Manager->direct_employees_no); //sortez in ordine alfabetica vectorul de angajati

    NewManager->team = (Tree*)realloc(NewManager->team, (NewManager->direct_employees_no + 1) * sizeof(Tree));
 
    Employee->manager = NewManager;
    NewManager->team[NewManager->direct_employees_no++] = Employee;
    for(int i = 0; i < Employee->direct_employees_no; i++)
        Employee->team[i] = NULL;
    Employee->direct_employees_no = 0;
    Sort_by_name(NewManager->team, NewManager->direct_employees_no); //sortez in ordine alfabetica vectorul de angajati
    return tree;
}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name) 
{
    Tree Employee = ReturnPerson(tree, employee_name);
    Tree NewManager = ReturnPerson(tree, new_manager_name);
    Tree Manager = Employee->manager;

    if(Employee == NULL) //daca angajatul nu exista
        return tree;
    if(NewManager == NULL) //daca managerul nu exista
        return tree;
    if(Manager == NULL) //daca angajatul este primul om din ierarhie, adica managerul sau este NULL 
        return tree;
    if(Employee->manager == NewManager) //daca managerul angajatului este deja managerul dat ca parametru
        return tree;
    
    int poz, n;
    n = Manager->direct_employees_no;
    //caut pozitia angajatului care trebuie mutat impreuna cu echipa sa 
    poz = get_position(Manager->team, Employee, n);
    for(int i = poz + 1; i < n ; i++)
        Manager->team[i - 1] = Manager->team[i];
    Manager->direct_employees_no--;

    NewManager->team = (Tree*)realloc(NewManager->team, (NewManager->direct_employees_no + 1) * sizeof(Tree));
    Employee->manager = NewManager;
    NewManager->team[NewManager->direct_employees_no++] = Employee;
    Sort_by_name(NewManager->team, NewManager->direct_employees_no);
    return tree;
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */
void free_team(Tree tree)
{
    if(tree == NULL)
        return;
    for(int i = 0; i < tree->direct_employees_no; i++)
        destroy_tree(tree->team[i]);
    free(tree->team);
    free(tree->name);
    free(tree);
}
Tree fire_team(Tree tree, char *employee_name) 
{
    Tree Employee = ReturnPerson(tree, employee_name);
    Tree Manager = Employee->manager;

    if(Employee == NULL) //daca angajatul nu exista
        return tree;
    if(Manager == NULL) //daca angajatul este primul om din ierarhie, adica managerul sau este NULL 
        return tree;
    
    int poz, n;
    n = Manager->direct_employees_no;
    //caut pozitia angajatului care trebuie concediat in vectorul teams al managerului
    poz = get_position(Manager->team, Employee, n);
    for(int i = poz + 1; i < n ; i++)
        Manager->team[i - 1] = Manager->team[i];
    Manager->direct_employees_no--;
    free_team(Employee);
    Sort_by_name(Manager->team, Manager->direct_employees_no);

    return tree;
}

/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */
Tree* fill_in_vector(Tree* v, Tree Employee, int *n)
{
    if(Employee == NULL)
        return NULL;
    else
    {
        for(int i = 0; i < Employee->direct_employees_no; i++)
            fill_in_vector(v, Employee->team[i], n);
        v[(*n)++] = Employee;
    }
    return v;
}
void print_name(FILE *f, Tree v[], int n)
{
    for (int i = 0; i < n; i++)
        fprintf(f, "%s ", v[i]->name);
    fprintf(f, "\n");
}
void get_employees_by_manager(FILE *f, Tree tree, char *employee_name) 
{
    Tree Employee = ReturnPerson(tree, employee_name);
    if(Employee == NULL)
    {
        fprintf(f, "\n");
        return;
    }
    Tree* v = (Tree*)malloc(100*sizeof(Tree));
    int n = 0;
    v = fill_in_vector(v, Employee, &n); //pun toate elementele din arbore in v
    Sort_by_name(v, n);
    print_name(f, v, n);
    free(v);
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */

void get_level(Tree tree, int level, int lvl, Tree v[], int *n)
{
    if (tree == NULL)
        return;
    
    if (lvl == level)
       v[(*n)++] = tree;
    else
        for(int i = 0; i < tree->direct_employees_no; i++)
            get_level(tree->team[i], level, lvl + 1, v, n);
}

void get_employees_by_level(FILE *f, Tree tree, int level) 
{
    Tree* v = (Tree*)malloc(100*sizeof(Tree));
    int n = 0;
    get_level(tree, level, 0, v, &n); //pun elementele cu nivelul level in v
    Sort_by_name(v, n);
    print_name(f, v, n);
    free(v);
}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void get_best_manager(FILE *f, Tree tree) 
{
    Tree* v = (Tree*)malloc(100*sizeof(Tree));
    int n = 0;
    v = fill_in_vector(v, tree, &n); //pun toate elementele din arbore in v
    Sort_by_name(v, n);
    int i, max = 0;
    for(i = 0; i < n; i++)
    {
        Tree Employee = ReturnPerson(tree, v[i]->name);
        if(Employee->direct_employees_no > max)
            max = Employee->direct_employees_no;
    }
    for(i = 0; i < n; i++)
    {
        Tree Employee = ReturnPerson(tree, v[i]->name);
        if(Employee->direct_employees_no == max)
            fprintf(f,"%s ", Employee->name);
    }
    fprintf(f,"\n");
    free(v);
}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
Tree reorganize(Tree tree, char *employee_name) 
{
    Tree Employee = ReturnPerson(tree, employee_name);
    if(Employee == NULL || Employee->manager == NULL) //angajatul nu exista sau daca este primul din ierarhie(managerul sau e NULL)
        return tree;

    Tree x = Employee;
    Tree y = x->manager;
    Tree z = y->manager;
    
    while(z != NULL)
    {
        x->team = (Tree*)realloc(x->team, (x->direct_employees_no + 1) * sizeof(Tree)); //mai am nevoie de o pozitie unde il voi pune pe y
        x->team[x->direct_employees_no++] = y; 
        Sort_by_name(x->team, x->direct_employees_no); //sortez vectorul unde l-am pus pe y
        int poz = get_position(y->team, x , y->direct_employees_no); //caut pozitia lui x in vectorul team al lui y pentru a-l elimina
        for(int i = poz + 1; i < y->direct_employees_no ; i++)
            y->team[i - 1] = y->team[i];
        y->direct_employees_no--;
        
        y->manager = x;//z
        printf("adresa y->manager: %p %p\n", y->manager, x);
        x = y;
        //y = y->manager; //la fel
        //y++;// y se duce la alta adresa dar asta nu inseamna ca se muta si x 
        printf("adresa x, y->manager, y: %p %p %p\n", x, y->manager, y);
        y = z;
        printf("adresa y,z,y->manager: %p %p %p\n", y, z, y->manager);
        z = z->manager;
        printf("adresa y->manager,x, y, z : %p %p %p %p\n\n\n", y->manager, x, y, z);
        
    }
    
    if(z == NULL) // execut ultimul pas sau angajatul care trebuie mutat are nivelul 1 in arbore 
    {
       x->team = (Tree*)realloc(x->team, (x->direct_employees_no + 1) * sizeof(Tree)); //mai am nevoie de o pozitie unde il voi pune pe y
       x->team[x->direct_employees_no++] = y;
       Sort_by_name(x->team, x->direct_employees_no); //sortez vectorul unde l-am pus pe y
       int poz = get_position(y->team, x , y->direct_employees_no); //caut pozitia lui x in vectorul team al lui y pentru a-l elimina
       for(int i = poz + 1; i < y->direct_employees_no; i++)
            y->team[i - 1] = y->team[i];
        y->direct_employees_no--;
        y->manager = x;

    }
    Employee->manager = NULL;
    tree = Employee;
    return tree;
}

void preorder_rec(FILE *f, Tree tree)
{
    if(tree == NULL)
        return;
    for(int i = 0; i < tree->direct_employees_no; i++)
    {
        fprintf(f, "%s-%s ", tree->team[i]->name, tree->name);
        preorder_rec(f, tree->team[i]);
    }
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void preorder_traversal(FILE *f, Tree tree)
{
    fprintf(f, "%s ", tree->name);
    preorder_rec(f, tree);
    fprintf(f, "\n");
    return;
}
/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */
void destroy_tree(Tree tree) 
{
    if(tree == NULL)
        return;
    for(int i = 0; i < tree->direct_employees_no; i++ )
        destroy_tree(tree->team[i]);
    free(tree->team);
    free(tree->name);
    free(tree);
}

