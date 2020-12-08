#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/dir.h>
#include <wx/wxprec.h>
#include <wx/imaglist.h>
#include <wx/artprov.h>
#include <wx/dirdlg.h>
#include <wx/notebook.h>

class MainApp : public wxApp // MainApp is the class for our application
{ // MainApp just acts as a container for the frame in MainFrame
public: 
    virtual bool OnInit();
};

class MainFrame : public wxFrame // MainFrame is the class for our window,
{ // It contains the window and all objects in it
public:
    bool editorial = false;
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    wxDirDialog* dirdlg;
    wxNotebook* notebook;
    wxMenuBar* MainMenu;
    wxSplitterWindow* splittermain;
    wxBoxSizer* mainsizer;
    wxPanel* fileTree;
    wxPanel* editor;
    wxStaticBoxSizer* group1;
    wxStaticBoxSizer* group2;
    wxDirDialog* DirDialog;
    wxTextCtrl* TextCtrl;
    wxTreeCtrl* treeCtrl;
    wxTreeItemId root;
    wxImageList* imagelist;
    wxString path;

    void projectView(wxString& path);

    void Quit(wxCommandEvent& event);
    void NewProject(wxCommandEvent& event);
    void OpenProject(wxCommandEvent& event);
    void OpenFile(wxCommandEvent& event);
    void SaveFile(wxCommandEvent& event);
    void SaveFileAs(wxCommandEvent& event);
    void CloseFile(wxCommandEvent& event);
    void RunCode(wxCommandEvent& event);
    void DebugCode(wxCommandEvent& event);
    void AddClass(wxCommandEvent& event);
    void AddFile(wxCommandEvent& event);
    void DelFile(wxCommandEvent& event);
    void OnTreeSelectionChanged(wxTreeEvent& event);

    DECLARE_EVENT_TABLE()
};



enum
{
    MAINTEXT = wxID_HIGHEST + 1, // declares an id which will be used to call our button
    CONSOLE = wxID_HIGHEST + 2,
    EDITIOR = wxID_HIGHEST + 3,
    FILETREE = wxID_HIGHEST + 4,
    OUTPUT = wxID_HIGHEST +5,
    PANEL,
    NOTEBOOK,
    NEWPROJECT,
    OPENPROJECT,
    OPENFILE,
    MENU_Close,
    MENU_Save,
    MENU_SaveAs,
    MENU_Quit,
    RUN,
    DEBUGCODE,
    PROJECT_ADDCLASS,
    PROJECT_ADDFILE,
    PROJECT_DELETE
};