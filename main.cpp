#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

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
    //same as above but provides a list
    void genreRecs(string, Node*, vector<Node*>&);
    void genreRecs(string, Node*, vector<Node*>&, string);
    void genreRecs(string, Node*, vector<Node*>&, int);
    void genreRecs(string, Node*, vector<Node*>&, string, int);

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

void Tree::genreRecs(string g, Node* root, vector<Node*> &results)
{
    if (root == nullptr)
        return;

    if (root->genre == g)
    {
        results.push_back(root);
    }

    genreRecs(g, root->left, results);
    genreRecs(g, root->right, results);

}

void Tree::genreRecs(string g, Node* root, vector<Node*> &results, string r)
{
    if (root == nullptr)
        return;

    if (root->genre == g & root->rating == r)
    {
        results.push_back(root);
    }

    genreRecs(g, root->left, results,r);
    genreRecs(g, root->right, results,r);

}

void Tree::genreRecs(string g, Node* root, vector<Node*> &results, int y)
{
    string f = to_string(y);
    if (root == nullptr)
        return;

    if (root->genre == g & root->year == f)
    {
        results.push_back(root);
    }

    genreRecs(g, root->left, results,y);
    genreRecs(g, root->right, results,y);

}

void Tree::genreRecs(string g, Node* root, vector<Node*> &results, string r, int y)
{
    string f = to_string(y);
    if (root == nullptr)
        return;

    if (root->genre == g & root->rating == r & root->year == f)
    {
        results.push_back(root);
    }

    genreRecs(g, root->left, results,r,y);
    genreRecs(g, root->right, results,r,y);

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
//Same as above but list
vector<Node*> genreRecsUM(Node* table[], int tableSize, string g, vector<Node*> &results)
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
                temp = temp->right;
                if (results.size() >= 50)
                    return results;
            }
            else
                temp = temp->right;
        }
    }
    return results;
}

vector<Node*> genreRecsUM(Node* table[], int tableSize, string g, vector<Node*> &results, string r)
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
                temp = temp->right;
                if (results.size() >= 50)
                    return results;
            }
            else
                temp = temp->right;
        }
    }
    return results;
}

vector<Node*> genreRecsUM(Node* table[], int tableSize, string g, vector<Node*> &results, int y)
{
    Node* temp;
    string f = to_string(y);
    for (int i = 0; i < tableSize; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->genre == g & temp->year == f)
            {
                results.push_back(temp);
                temp = temp->right;
                if (results.size() >= 50)
                    return results;
            }
            else
                temp = temp->right;
        }
    }
    return results;
}

vector<Node*> genreRecsUM(Node* table[], int tableSize, string g, vector<Node*> &results, string r, int y)
{
    Node* temp;
    string f = to_string(y);
    for (int i = 0; i < tableSize; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->genre == g & temp->rating == r & temp->year == f)
            {
                results.push_back(temp);
                temp = temp->right;
                if (results.size() >= 50)
                    return results;
            }
            else
                temp = temp->right;
        }
    }
    return results;

}

int main() {
    srand(time(nullptr));
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
    cout << "**********************************" << endl;
    cout << '\t' << "Welcome to DVDFinder 5000" << endl;
    cout << "**********************************" << endl;
    cout << "Your viewing needs will soon be met :)" << endl;
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
        getline(s, g, '\r');
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
    int turns = 0;
    while (turns != INT_MAX) {
        cout << "Would you like to:" << endl << "1. Search for a title" << endl << "2. Get a recommendation" << endl;
        int userInput;
        cin >> userInput;
        Tree show;
        if (userInput == 1) {
            cout << "Please enter a title: " << endl;
            string title;
            cin >> title;
            auto start = chrono::high_resolution_clock::now();
            Node* final = findTitleUM(table, 300, title);
            auto end = chrono::high_resolution_clock::now();
            auto time1 = chrono::duration_cast<chrono::nanoseconds>(end - start);


            start = chrono::high_resolution_clock::now();
            Node* finish = show.findTitle(title,root);
            end = chrono::high_resolution_clock::now();
            auto time2 = chrono::duration_cast<chrono::nanoseconds>(end - start);

            if (final != nullptr) {
                cout << "Title is in the DVD Collection!" << endl;
            }
            else {
                cout << "Sorry, it's not in the collection!" << endl;
            }

            cout << "Time Taken For Unordered map: " << time1.count() << endl;
            cout << "Time taken for BST: " << time2.count() << endl;
        }
        else if (userInput == 2) {
            cout << "Please enter the specifics you would like to search by: " << endl;
            cout << "1. Genre" << endl << "2. Genre and Rating" << endl;
            cout << "3. Genre and Year" << endl << "4. Genre, Year and Rating" << endl;
            int choice;
            cin >> choice;
            if (choice == 1) {
                cout << "Enter genre:" << endl;
                string x;
                cin >> x;
                cout << "Would you like:" << endl;
                cout << "1. A list of recommendations" << endl;
                cout << "2. One singular recommendation" << endl;
                int list;
                cin >> list;

                vector<Node *> nodes;
                auto start = chrono::high_resolution_clock::now();
                vector<Node *> final = genreRecsUM(table, tableSize, x, nodes);
                auto end = chrono::high_resolution_clock::now();
                auto time1 = chrono::duration_cast<chrono::nanoseconds>(end - start);
                if (!final.empty()) {
                    cout << "Unorderd Map Approach:" << endl;
                    if (list == 1) {
                        for (int i = 0; i < final.size(); i++) {
                            if (i == 50) {
                                break;
                            }
                            cout << final[i]->title << endl;
                        }
                    } else if (list == 2) {
                        //insert random number
                        int ran = rand() % (final.size()-1);
                        cout << final[ran]->title << endl;
                    }

                    vector<Node*> finish;
                    start = chrono::high_resolution_clock::now();
                    show.genreRecs(x,root,finish);
                    end = chrono::high_resolution_clock::now();
                    auto time2 = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    cout << "Binary Search Tree Approach:" << endl;
                    if (list == 1) {
                        for (int i = 0; i < finish.size(); i++) {
                            if (i == 50) {
                                break;
                            }
                            cout << finish[i]->title << endl;
                        }
                    }
                    else if (list == 2){
                        //insert ran number
                        int ran = rand() % (finish.size()-1);
                        cout << finish[ran]->title << endl;
                    }

                    cout << "Time Taken For Unordered map: " << time1.count() << endl;
                    cout << "Time taken for BST: " << time2.count() << endl;
                }
                else {
                    cout << "No movies match your specifications! Please Try Again :)" << endl;
                }

            } else if (choice == 2) {
                cout << "Enter Genre:" << endl;
                string x;
                cin >> x;
                cout << "Enter rating:" << endl;
                string m;
                cin >> m;
                cout << "Would you like:" << endl;
                cout << "1. A list of recommendations" << endl;
                cout << "2. One singular recommendation" << endl;
                int list;
                cin >> list;

                vector<Node *> nodes;
                auto start = chrono::high_resolution_clock::now();
                vector<Node *> final = genreRecsUM(table, tableSize, x, nodes, m);
                auto end = chrono::high_resolution_clock::now();
                auto time1 = chrono::duration_cast<chrono::nanoseconds>(end - start);
                if (!final.empty()) {
                    cout << "Unorderd Map Approach:" << endl;
                    if (list == 1) {
                        for (int i = 0; i < final.size(); i++) {
                            if (i == 50) {
                                break;
                            }
                            cout << final[i]->title << endl;
                        }
                    } else if (list == 2) {
                        //insert random number
                        int ran = rand() % (final.size()-1);
                        cout << final[ran]->title << endl;
                    }


                    vector<Node*> finish;
                    start = chrono::high_resolution_clock::now();
                    show.genreRecs(x,root,finish,m);
                    end = chrono::high_resolution_clock::now();
                    auto time2 = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    cout << "Binary Search Tree Approach:" << endl;
                    if (list == 1) {
                        for (int i = 0; i < finish.size(); i++) {
                            if (i == 50) {
                                break;
                            }
                            cout << finish[i]->title << endl;
                        }
                    }
                    else if (list == 2){
                        //insert ran number
                        int ran = rand() % (finish.size()-1);
                        cout << finish[ran]->title << endl;
                    }
                    cout << "Time Taken For Unordered map: " << time1.count() << endl;
                    cout << "Time taken for BST: " << time2.count() << endl;
                }
                else {
                    cout << "No movies match your specifications! Please Try Again :)" << endl;
                }

            } else if (choice == 3) {
                cout << "Enter Genre:" << endl;
                string x;
                cin >> x;
                cout << "Enter Year:" << endl;
                int c;
                cin >> c;
                cout << "Would you like:" << endl;
                cout << "1. A list of recommendations" << endl;
                cout << "2. One singular recommendation" << endl;
                int list;
                cin >> list;

                vector<Node *> nodes;
                auto start = chrono::high_resolution_clock::now();
                vector<Node *> final = genreRecsUM(table, tableSize, x, nodes, c);
                auto end = chrono::high_resolution_clock::now();
                auto time1 = chrono::duration_cast<chrono::nanoseconds>(end - start);
                if (!final.empty()) {
                    cout << "Unorderd Map Approach:" << endl;
                    if (list == 1) {
                        for (int i = 0; i < final.size(); i++) {
                            if (i == 50) {
                                break;
                            }
                            cout << final[i]->title << endl;
                        }
                    } else if (list == 2) {
                        //random number generator
                        int ran = rand() % (final.size()-1);
                        cout << final[ran]->title << endl;
                    }

                    vector<Node*> finish;
                    start = chrono::high_resolution_clock::now();
                    show.genreRecs(x,root,finish,c);
                    end = chrono::high_resolution_clock::now();
                    auto time2 = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    cout << "Binary Search Tree Approach:" << endl;
                    if (list == 1) {
                        for (int i = 0; i < finish.size(); i++) {
                            if (i == 50) {
                                break;
                            }
                            cout << finish[i]->title << endl;
                        }
                    }
                    else if (list == 2){
                        //insert ran number
                        int ran = rand() % (finish.size()-1);
                        cout << finish[ran]->title << endl;
                    }
                    cout << "Time Taken For Unordered map: " << time1.count() << endl;
                    cout << "Time taken for BST: " << time2.count() << endl;
                }
                else {
                    cout << "No movies match your specifications! Please Try Again :)" << endl;
                }

            }
            else if (choice == 4) {
                cout << "Enter Genre:" << endl;
                string x;
                cin >> x;
                cout << "Enter rating:" << endl;
                string m;
                cin >> m;
                cout << "Enter Year:" << endl;
                int c;
                cin >> c;
                cout << "Would you like:" << endl;
                cout << "1. A list of recommendations" << endl;
                cout << "2. One singular recommendation" << endl;
                int list;
                cin >> list;

                vector<Node *> nodes;
                auto start = chrono::high_resolution_clock::now();
                vector<Node *> final = genreRecsUM(table, tableSize, x, nodes, m, c);
                auto end = chrono::high_resolution_clock::now();
                auto time1 = chrono::duration_cast<chrono::nanoseconds>(end - start);
                if (!final.empty()) {
                    cout << "Unorderd Map Approach:" << endl;
                    if (list == 1) {
                        for (int i = 0; i < final.size(); i++) {
                            if (i == 50) {
                                break;
                            }
                            cout << final[i]->title << endl;
                        }
                    } else if (list == 2) {
                        //random number generator
                        int ran = rand() % (final.size()-1);
                        cout << final[ran]->title << endl;
                    }

                    vector<Node*> finish;
                    start = chrono::high_resolution_clock::now();
                    show.genreRecs(x,root,finish,m,c);
                    end = chrono::high_resolution_clock::now();
                    auto time2 = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    cout << "Binary Search Tree Approach:" << endl;
                    if (list == 1) {
                        for (int i = 0; i < finish.size(); i++) {
                            if (i == 50) {
                                break;
                            }
                            cout << finish[i]->title << endl;
                        }
                    }
                    else if (list == 2){
                        //insert ran number
                        int ran = rand() % (finish.size()-1);
                        cout << finish[ran]->title << endl;
                    }
                    cout << "Time Taken For Unordered map: " << time1.count() << endl;
                    cout << "Time taken for BST: " << time2.count() << endl;
                }
                else {
                    cout << "No movies match your specifications! Please Try Again :)" << endl;
                }

            }
        }
        turns++;
        cout << "Would you like to quit?" << endl;
        cout << "1. No" << endl << "2. Yes" << endl;
        int quit;
        cin >> quit;
        if (quit == 2) {
            turns = INT_MAX;
        }

    }
    return 0;
}

int hashing(string &title, int size) {
    return title.length() % size;
}

