#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <stdio.h>

using namespace std;

class Node{
    //Put implementation here
public:
    Node* left;
    Node* right;
    string title;
    string rating;
    string year;
    string genre;
    bool found = false;
    Node() {
        left = nullptr;
        right = nullptr;
        title = "";
        rating = "";
        year = "";
        genre = "";
    }
    Node(string &t, string &r, string &y, string &g) {
        left = nullptr;
        right = nullptr;
        title = t;
        rating = r;
        year = y;
        genre = g;
    }
    Node(Node* l, Node* ri, string &t, string &r, string &y, string &g) {
        left = l;
        right = ri;
        title = t;
        rating = r;
        year = y;
        genre = g;
    }
};


class Tree {
public:
    Tree();
    Tree(string, string, string, string);
    Node* insert(string&, Node*, vector<string>&);
    Node* findTitle(string, Node*);
    //recommends titles based on genre, has capability to accept a rating, a year, or both as extra filters
    Node* genreRec(string, Node*);
    Node* genreRec(string, Node*, string);
    Node* genreRec(string, Node*, int);
    Node* genreRec(string, Node*, string, int);
    //same as above but provides a list
    void genreRecs(string, Node*, vector<Node*>&);
    void genreRecs(string, Node*, vector<Node*>&, string);
    void genreRecs(string, Node*, vector<Node*>&, int);
    void genreRecs(string, Node*, vector<Node*>&, string, int);

    int ranNum(int, int);
    string title;
    string rating;
    string year;
    string genre;
};

Tree::Tree() {
    title = "";
    rating = "";
    year = "";
    genre = "";
}

Tree::Tree(string t, string r, string y, string g) {
    title = t;
    rating = r;
    year = y;
    genre = g;
}

Node* Tree::insert(string &t, Node* root, vector<string> &info) { //ideas from AVL Project 1
    if (root == nullptr) { //creates a new node at leaf or stem with only one child
        Node* node = new Node(t, info[0], info[1], info[2]);
        return node;
    }
    if (t < root->title) {
        root->left = insert(t, root->left, info);
    }
    else {
        root->right = insert(t, root->right, info);
    }
    return root;
}

Node* Tree::findTitle(string t, Node* root)
{
    // root is null or is the desired movie
    if (root == NULL || root->title == t)
       return root;
    
    // title is greater than root's title
    if (root->title < t)
       return findTitle(t, root->right);
 
    // title is smaller than root's title
    return findTitle(t, root->left);
}

//recommendation
Node* Tree::genreRec(string t, Node* root)
{
    if (root == nullptr)
        return nullptr;

    if (root->genre == t)
    {
        return root;
    }
    Node* left = genreRec(t, root->left);
    Node* right = genreRec(t, root->right);
    if (left != nullptr)
        return left;
    else if (right != nullptr)
        return right;
    return nullptr;
}

Node* Tree::genreRec(string t, Node* root, string r)
{
    if (root == nullptr)
        return nullptr;

    if (root->genre == t & root->rating == r)
    {
        return root;
    }
    Node* left = genreRec(t, root->left, r);
    Node* right = genreRec(t, root->right, r);
    if (left != nullptr)
        return left;
    else if (right != nullptr)
        return right;
    return nullptr;
}

Node* Tree::genreRec(string t, Node* root, int y)
{
    if (root == nullptr)
        return nullptr;

    if (root->genre == t & stoi(root->year) == y)
    {
        return root;
    }
    Node* left = genreRec(t, root->left, y);
    Node* right = genreRec(t, root->right, y);
    if (left != nullptr)
        return left;
    else if (right != nullptr)
        return right;
    return nullptr;
}

Node* Tree::genreRec(string t, Node* root, string r, int y)
{
    if (root == nullptr)
        return nullptr;

    if (root->genre == t & root->rating == r & stoi(root->year) == y)
    {
        return root;
    }
    Node* left = genreRec(t, root->left, r, y);
    Node* right = genreRec(t, root->right, r, y);
    if (left != nullptr)
        return left;
    else if (right != nullptr)
        return right;
    return nullptr;
}

void Tree::genreRecs(string t, Node* root, vector<Node*> &results)
{
    if (root == nullptr)
        return;

    if (results.size()<=50)
    {
        if (root->genre == t)
        {
            results.push_back(root);
        }

        genreRecs(t, root->left, results);
        genreRecs(t, root->right, results);
    }

}

void Tree::genreRecs(string t, Node* root, vector<Node*> &results, string r)
{
    if (root == nullptr)
        return;

    if (results.size()<=50)
    {
        if (root->genre == t & root->rating == r)
        {
            results.push_back(root);
        }

        genreRecs(t, root->left, results);
        genreRecs(t, root->right, results);
    }

}

void Tree::genreRecs(string t, Node* root, vector<Node*> &results, int y)
{
    if (root == nullptr)
        return;

    if (results.size()<=50 )
    {
        if (root->genre == t & stoi(root->year) == y)
        {
            results.push_back(root);
        }

        genreRecs(t, root->left, results);
        genreRecs(t, root->right, results);
    }

}

void Tree::genreRecs(string t, Node* root, vector<Node*> &results, string r, int y)
{
    if (root == nullptr)
        return;

    if (results.size()<=50)
    {
        if (root->genre == t & root->rating == r & stoi(root->year) == y)
        {
            results.push_back(root);
        }

        genreRecs(t, root->left, results);
        genreRecs(t, root->right, results);
    }

}

class unorderedMap {
private:
    int lF = -1;
    int tableSize = 300; //starting table size
    int key = -1;
    int currentSize = 0;
    string title;
    Node* data;
public:
    unorderedMap();
    unorderedMap(int, Node*);
    float calcLF(int, int);
};

int hashing(string&, int);
unorderedMap::unorderedMap() {
    key = -1;
    data = nullptr;
}

unorderedMap::unorderedMap(int key, Node* n) {
    this->key = key;
    data = n;
}

float unorderedMap::calcLF(int cS, int tS) {
    return (float)cS/tS;
}

//UM search functions
Node* findTitleUM(Node* table[], int tableSize, string t)
{
    int key = hashing(t, tableSize);
    if (table[key] != nullptr) {
        if (table[key]->title == t) {
            return table[key];
        }
        else {
            Node* temp = table[key]->right;
            while (temp!= nullptr) {
                if (temp->title == t)
                    return temp;
                temp = temp->right;
            }
        }
    }
    return nullptr;
}

Node* genreRecUM(Node* table[], int tableSize, string g)
{
    Node* temp;
    for (int i = 0; i < tableSize; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->genre == g)
                return temp;
            else 
                temp = temp->right;
        }
    }
    return nullptr;
}

Node* genreRecUM(Node* table[], int tableSize, string g, string r)
{
    Node* temp;
    for (int i = 0; i < tableSize; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->genre == g & temp->rating == r)
                return temp;
            else 
                temp = temp->right;
        }
    }
    return nullptr;
}

Node* genreRecUM(Node* table[], int tableSize, string g, int y)
{
    Node* temp;
    for (int i = 0; i < tableSize; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->genre == g & stoi(temp->year) == y)
                return temp;
            else 
                temp = temp->right;
        }
    }
    return nullptr;
}

Node* genreRecUM(Node* table[], int tableSize, string g, string r, int y)
{
    Node* temp;
    for (int i = 0; i < tableSize; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->genre == g & temp->rating == r & stoi(temp->year) == y)
                return temp;
            else 
                temp = temp->right;
        }
    }
    return nullptr;
}

//Same as above but list
void genreRecsUM(Node* table[], int tableSize, string g, vector<Node*> &results)
{
    Node* temp;
    for (int i = 0; i < tableSize; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->genre == g)
            {
                results.push_back(temp);
                if (results.size() >= 50)
                    break;
            }
            else
                temp = temp->right;
        }
    }
}

void genreRecsUM(Node* table[], int tableSize, string g, vector<Node*> &results, string r)
{
    Node* temp;
    for (int i = 0; i < tableSize; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->genre == g & temp->rating == r)
            {
                results.push_back(temp);
                if (results.size() >= 50)
                    break;
            }
            else 
                temp = temp->right;
        }
    }
}

void genreRecsUM(Node* table[], int tableSize, string g, vector<Node*> &results, int y)
{
    Node* temp;
    for (int i = 0; i < tableSize; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->genre == g & stoi(temp->year) == y)
            {
                results.push_back(temp);
                if (results.size() >= 50)
                    break;
            }
            else 
                temp = temp->right;
        }
    }
}

void genreRecsUM(Node* table[], int tableSize, string g, vector<Node*> &results, string r, int y)
{
    Node* temp;
    for (int i = 0; i < tableSize; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->genre == g & temp->rating == r & stoi(temp->year) == y)
            {
                results.push_back(temp);
                if (results.size() >= 50)
                    break;
            }
            else 
                temp = temp->right;
        }
    }
}

int main() {
    fstream dvdFile;
    dvdFile.open("DVDlist.csv");
    string line;
    getline(dvdFile,line); //gets the headers
    //cout << line;
    string t;
    string r;
    string y;
    string g;
    string extra;
    int tableSize = 300;
    int currentSize = 0;
    Node* table[300] = {nullptr};
    Node* root = nullptr;
    for (int i = 0; i < 100005; i++) {
        vector<string> data;
        getline(dvdFile,line);
        stringstream s(line);
        if (line[0] == '"') { //accounts for titles with commas in them
            getline(s, extra, '"');
            getline(s, t, '"');
            getline(s, extra, ',');
        }
        else {
            getline(s, t, ',');
        }
        getline(s, r, ',');
        data.push_back(r);
        getline(s, y, ',');
        data.push_back(y);
        getline(s, g, ',');
        data.push_back(g);
        //making bst tree
        Tree movie(t,r,y,g);
        root = movie.insert(t,root,data);
        //making unordered map
        int k = hashing(t, tableSize);
        Node* node = new Node(t,r,y,g);
        unorderedMap uM(k,node);
        if (table[k] != nullptr) {
            if (table[k]->right == nullptr) {
                table[k]->right = node;
            }
            else {
                Node* holder = table[k];
                Node* j = nullptr;
                while (holder != nullptr) {
                    j = holder;
                    holder = holder->right;
                }
                j->right = node;
            }

        }
        else {
            table[k] = node;
            currentSize++;
        }
    }
    return 0;
}

int hashing(string &title, int size) {
    return title.length() % size;
}

