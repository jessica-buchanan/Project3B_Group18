#include "cMain.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <stdio.h>
#include <wx/stopwatch.h>
#include <ctime>
#include <chrono>

using namespace std;

class Node {
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
    Node(string& t, string& r, string& y, string& g) {
        left = nullptr;
        right = nullptr;
        title = t;
        rating = r;
        year = y;
        genre = g;
    }
    Node(Node* l, Node* ri, string& t, string& r, string& y, string& g) {
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

    void genreSearch(string g, Node* root, vector<Node*>& results);
    void yearSearch(string _year, Node* root, vector<Node*>& results);
    void ratingSearch(string g, Node* root, vector<Node*>& results);
    void recommendation(Node* root, vector<Node*>& results, string _genre, string _year, string _rating);

    int ranNum(int, int);
    string title, rating, year, genre;
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

Node* Tree::insert(string& t, Node* root, vector<string>& info) { //ideas from AVL Project 1
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

void Tree::genreSearch(string t, Node* root, vector<Node*>& results)
{
    if (root == nullptr)
        return;

    if (results.size() <= 50)
    {
        if (root->genre == t)
        {
            results.push_back(root);
        }

        genreSearch(t, root->left, results);
        genreSearch(t, root->right, results);
    }

}

void Tree::yearSearch(string _year, Node* root, vector<Node*>& results)
{
    if (root == nullptr)
        return;

    if (results.size() <= 50)
    {
        if (root->year == _year)
        {
            results.push_back(root);
        }

        yearSearch(_year, root->left, results);
        yearSearch(_year, root->right, results);
    }

}

void Tree::ratingSearch(string r, Node* root, vector<Node*>& results)
{
    if (root == nullptr)
        return;

    if (results.size() <= 50)
    {
        if (root->rating == r)
        {
            results.push_back(root);
        }
        ratingSearch(r, root->left, results);
        ratingSearch(r, root->right, results);
    }
}

void Tree::recommendation(Node* root, vector<Node*>& results, string _genre, string _year, string _rating)
{
    if (root == nullptr)
        return;

    if (results.size() <= 50)
    {
        if (root->rating == _rating && root->year == _year && root->genre == _genre)
        {
            results.push_back(root);
        }

        recommendation(root->left, results, _genre, _year, _rating);
        recommendation(root->right, results, _genre, _year, _rating);
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
    vector<Node*> yearSearch(Node* table[], string _year, vector<Node*>& results);
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

vector<Node*> unorderedMap::yearSearch(Node* table[], string _year, vector<Node*>& results)
{
    Node* temp;
    for (int i = 0; i < 300; i++)
    {
        temp = table[i];
        while (temp != nullptr)
        {
            if (temp->year == _year)
                results.push_back(temp);
            temp = temp->right;
        }
    }
    return results;
}

int hashing(string& title, int size) {
    return title.length() % size;
}

// BUTTON EVENTS
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClickedRec)
	EVT_BUTTON(10002, OnButtonClickedSearch)
	EVT_BUTTON(10003, OnButtonClickedFindRec)
	EVT_BUTTON(10004, OnButtonClickedReset)
	EVT_BUTTON(10005, OnButtonClickedTitle)
	EVT_BUTTON(10006, OnButtonClickedYear)
	EVT_BUTTON(10007, OnButtonClickedGenre)
	EVT_BUTTON(10008, OnButtonClickedRating)
	EVT_BUTTON(10009, OnButtonClickedSearchTitle)
    EVT_BUTTON(10010, OnButtonClickedSearchYear)
    EVT_BUTTON(10011, OnButtonClickedSearchGenre)
    EVT_BUTTON(10012, OnButtonClickedSearchRating)
wxEND_EVENT_TABLE()

Node* loadData(Tree& _movie, unorderedMap& _map, Node _mapTable[])
{
    fstream dvdFile;
    dvdFile.open("DVDlist.csv");
    string line;
    getline(dvdFile, line); //gets the headers
    //cout << line;
    string t, r, y, g, extra;
    int tableSize = 300;
    int currentSize = 0;

    Node* table[300] = { nullptr };
    Node* root = nullptr;

    for (int i = 0; i < 100005; i++) {
        vector<string> data;
        getline(dvdFile, line);
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
        Tree movie(t, r, y, g);
        _movie = movie;
        root = movie.insert(t, root, data);

        //making unordered map
        int k = hashing(t, tableSize);
        Node* node = new Node(t, r, y, g);
        unorderedMap uM(k, node);
        _map = uM;

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
    _mapTable = *table;
    return root;

}

Node* table;
Node* mapTable;
Tree movie;
unorderedMap map;

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Movie Recommendation Generator", wxPoint(30, 40), wxSize(800, 800))
{
    table = loadData(movie, map, mapTable);

	m_title = new wxStaticText(this, wxID_ANY, "Welcome to the DVDFinder 5000!", wxPoint(250, 70), wxSize(50, 50));
	m_question = new wxStaticText(this, wxID_ANY, "Do you want to search for a movie or receive a recommendation?", wxPoint(90, 100), wxSize(50, 50));
	wxFont titleFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont questionFont(11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	m_title->SetFont(titleFont);
	m_question->SetFont(questionFont);

	m_btn1 = new wxButton(this, 10001, "Recommendation", wxPoint(50, 180), wxSize(150, 50));
	m_btn2 = new wxButton(this, 10002, "Search", wxPoint(200, 180), wxSize(150, 50));
	m_btn3 = new wxButton(this, 10004, "Reset", wxPoint(350, 180), wxSize(150, 50));

    m_recsTitle = new wxStaticText(this, wxID_ANY, "List of Recommendations: ", wxPoint(540, 180), wxSize(200, 25));
    m_recs = new wxListBox(this, wxID_ANY, wxPoint(540, 205), wxSize(200, 400));

}

cMain::~cMain()
{
}

void cMain::OnButtonClickedRec(wxCommandEvent& evt)
{
	promptQ = new wxStaticText(this, wxID_ANY, "Please answer the following: ", wxPoint(50, 300), wxSize(150, 50));
	yearTxt = new wxStaticText(this, wxID_ANY, "Year: ", wxPoint(90, 350), wxSize(50, 50));
	genreTxt = new wxStaticText(this, wxID_ANY, "Genre: ", wxPoint(90, 400), wxSize(50, 50));
	ratingTxt = new wxStaticText(this, wxID_ANY, "Minimum rating: ", wxPoint(90, 450), wxSize(100, 50));
	yearInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(20, 350), wxSize(50, 20));
	genreInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(20, 400), wxSize(50, 20));
	ratingInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(20, 450), wxSize(50, 20));
	generateButton = new wxButton(this, 10003, "Find Recommendation!", wxPoint(20, 500), wxSize(150, 50));

}

void cMain::OnButtonClickedFindRec(wxCommandEvent& evt)
{
	// take input and call function
    vector<Node*> results;

    wxString genreWx(genreInput->GetValue());
    wxString ratingWx(ratingInput->GetValue());
    wxString yearWx(yearInput->GetValue());
    string genString = std::string(genreWx.mb_str());
    string rString = std::string(ratingWx.mb_str());
    string yearString = std::string(yearWx.mb_str());
    
    movie.recommendation(table, results, genString, yearString, rString);
    
    for (int i = 0; i < results.size(); i++) {
        wxString mystring(results.at(i)->title);
        m_recs->AppendString(mystring);
    }
}

void cMain::OnButtonClickedSearch(wxCommandEvent& evt)
{
	promptQ = new wxStaticText(this, wxID_ANY, "Search for a movie by choosing one of the following categories ", wxPoint(50, 300), wxSize(350, 20));
	titleButton = new wxButton(this, 10005, "Title", wxPoint(50, 330), wxSize(100, 50));
	yearButton = new wxButton(this, 10006, "Year", wxPoint(150, 330), wxSize(100, 50));
	genreButton = new wxButton(this, 10007, "Genre", wxPoint(250, 330), wxSize(100, 50));
	ratingButton = new wxButton(this, 10008, "Rating", wxPoint(350, 330), wxSize(100, 50));
}

void cMain::OnButtonClickedTitle(wxCommandEvent& evt)
{
	askFor = new wxStaticText(this, wxID_ANY, "Enter the name of the movie: ", wxPoint(50, 400), wxSize(200, 20));
	searchInfo = new wxTextCtrl(this, wxID_ANY, "", wxPoint(50, 420), wxSize(200, 20));
    m_btn4 = new wxButton(this, 10009, "Search now!", wxPoint(300, 400), wxSize(150, 50));
}

void cMain::OnButtonClickedSearchTitle(wxCommandEvent& evt)
{
    wxString wString(searchInfo->GetValue());
    string stdString = std::string(wString.mb_str());
    wxStopWatch sw;

    // BST function call
    sw.Start(0);
    Node* temp = movie.findTitle(stdString, table);
    wxLogMessage("BST search took %ldms in all ", sw.Time());
    sw.Pause();

    if (temp != nullptr) {
        wxString mystring(temp->title);
        m_recs->AppendString(mystring);
    }
}

void cMain::OnButtonClickedYear(wxCommandEvent& evt)
{
	askFor = new wxStaticText(this, wxID_ANY, "Enter the year of the movie: ", wxPoint(50, 400), wxSize(200, 20));
	searchInfo = new wxTextCtrl(this, wxID_ANY, "", wxPoint(50, 420), wxSize(200, 20));
    m_btn4 = new wxButton(this, 10010, "Search now!", wxPoint(300, 400), wxSize(150, 50));
}

void cMain::OnButtonClickedSearchYear(wxCommandEvent& evt)
{
    wxString wString(searchInfo->GetValue());
    string stdString = std::string(wString.mb_str());

    // BST Search
    //vector<Node*> results;
    //movie.yearSearch(stdString, table, results);

    // Unordered function call
    wxStopWatch sw;
    sw.Start(0);
    vector<Node*> results;
    map.yearSearch(&mapTable, stdString, results);
    wxLogMessage("Unordered search took %ldms in all ", sw.Time());
    sw.Pause();

    for (int i = 0; i < results.size(); i++) {
        wxString mystring(results.at(i)->title);
        m_recs->AppendString(mystring);
    }
}

void cMain::OnButtonClickedGenre(wxCommandEvent& evt)
{
	askFor = new wxStaticText(this, wxID_ANY, "Enter the genre of the movie: ", wxPoint(50, 400), wxSize(200, 20));
	searchInfo = new wxTextCtrl(this, wxID_ANY, "", wxPoint(50, 420), wxSize(200, 20));
	m_btn4 = new wxButton(this, 10011, "Search now!", wxPoint(300, 400), wxSize(150, 50));
}

void cMain::OnButtonClickedSearchGenre(wxCommandEvent& evt)
{
    wxString wString(searchInfo->GetValue());
    string stdString = std::string(wString.mb_str());
    vector<Node*> results;
    movie.genreSearch(stdString, table, results);

    for (int i = 0; i < results.size(); i++) {
        wxString mystring(results.at(i)->title);
        m_recs->AppendString(mystring);
    }
}

void cMain::OnButtonClickedRating(wxCommandEvent& evt)
{
	askFor = new wxStaticText(this, wxID_ANY, "Enter the rating: (NR, PG, PG-13, MA13, MA17) ", wxPoint(50, 400), wxSize(250, 20));
	searchInfo = new wxTextCtrl(this, wxID_ANY, "", wxPoint(50, 430), wxSize(200, 20));
	m_btn4 = new wxButton(this, 10012, "Search now!", wxPoint(300, 400), wxSize(150, 50));
	searchInfo->GetValue();
}

void cMain::OnButtonClickedSearchRating(wxCommandEvent& evt)
{
    wxString wString(searchInfo->GetValue());
    string stdString = std::string(wString.mb_str());
    vector<Node*> results;

    movie.ratingSearch(stdString, table, results);


    for (int i = 0; i < results.size(); i++) {
        wxString mystring(results.at(i)->title);
        m_recs->AppendString(mystring);
    }
}

void cMain::OnButtonClickedReset(wxCommandEvent& evt)
{
    // recommendation menu buttons, texts, etc
    if (promptQ != nullptr) {
        promptQ->Destroy();
        promptQ = nullptr;
    }
    if (yearTxt != nullptr) {
        yearTxt->Destroy();
        yearTxt = nullptr;
    }
    if (genreTxt != nullptr) {
        genreTxt->Destroy();
        genreTxt = nullptr;
    }
    if (ratingTxt != nullptr) {
        ratingTxt->Destroy();
        ratingTxt = nullptr;
    }
    if (yearInput != nullptr) {
        yearInput->Destroy();
        yearInput = nullptr;
    }
    if (genreInput != nullptr) {
        genreInput->Destroy();
        genreInput = nullptr;
    }
    if (ratingInput != nullptr) {
        ratingInput->Destroy();
        ratingInput = nullptr;
    }
    if (generateButton != nullptr) {
        generateButton->Destroy();
        generateButton = nullptr;
    }
    if (titleButton != nullptr) {
        titleButton->Destroy();
        titleButton = nullptr;
    }
    if (yearButton != nullptr) {
        yearButton->Destroy();
        yearButton = nullptr;
    }
    if (genreButton != nullptr) {
        genreButton->Destroy();
        genreButton = nullptr;
    }
    if (ratingButton != nullptr) {
        ratingButton->Destroy();
        ratingButton = nullptr;
    }
    if (askFor != nullptr) {
        askFor->Destroy();
        askFor = nullptr;
    }
    if (searchInfo != nullptr) {
        searchInfo->Destroy();
        searchInfo = nullptr;
    }
    if (m_btn4 != nullptr) {
        m_btn4->Destroy();
        m_btn4 = nullptr;
    }
}

