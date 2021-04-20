#pragma once
#include "wx/wx.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	// main menu
	wxStaticText* m_title = nullptr;
	wxStaticText* m_question = nullptr;
	wxButton* m_btn1 = nullptr;
	wxButton* m_btn2 = nullptr;
	wxButton* m_btn3 = nullptr;
	wxButton* m_btn4 = nullptr;
	wxListBox* m_recs = nullptr;
	wxStaticText* m_recsTitle = nullptr;

	// recommendation menu buttons, texts, etc
	wxStaticText* promptQ = nullptr;
	wxStaticText* yearTxt = nullptr;
	wxStaticText* genreTxt = nullptr;
	wxStaticText* ratingTxt = nullptr;
	wxTextCtrl* yearInput = nullptr;
	wxTextCtrl* genreInput = nullptr;
	wxTextCtrl* ratingInput = nullptr;
	wxButton* generateButton = nullptr;

	// search menu buttons, texts, etc
	wxButton* titleButton = nullptr;
	wxButton* yearButton = nullptr;
	wxButton* genreButton = nullptr;
	wxButton* ratingButton = nullptr;
	wxStaticText* askFor = nullptr;
	wxTextCtrl* searchInfo = nullptr;



	// events
	void OnButtonClickedRec(wxCommandEvent& evt);
	void OnButtonClickedSearch(wxCommandEvent& evt);
	void OnButtonClickedFindRec(wxCommandEvent& evt);
	void OnButtonClickedReset(wxCommandEvent& evt);
	void OnButtonClickedTitle(wxCommandEvent& evt);
	void OnButtonClickedYear(wxCommandEvent& evt);
	void OnButtonClickedGenre(wxCommandEvent& evt);
	void OnButtonClickedRating(wxCommandEvent& evt);
	void OnButtonClickedSearchTitle(wxCommandEvent& evt);
	void OnButtonClickedSearchYear(wxCommandEvent& evt);
	void OnButtonClickedSearchGenre(wxCommandEvent& evt);
	void OnButtonClickedSearchRating(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

