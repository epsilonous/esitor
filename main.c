/**
    ESITOR (Text-Editor)
    -------------------
    ESITOR est un editeur de texte, cree a base de la beblioteque (GTK+)

    main.c
    ------
    TP 01 : SDSF 2CPI Ecole Nationale Superieure d informatique
    par : ABIDI Souad et CHELGHOUM Aimen
    Groupe : 09

    Role :
        le fichier suivant contient :
        - le declaration des beblioteque externes et les defines
        - la declaration des prototypes des foncions
        - la declaration des variables globales
        - la fonction main
        - les autres fonctions

*/

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

/** la declaration des defines **/
#define SIZE_LINE 120
#define NUMBER_LINE 38
#define WINDOW_WIDTH 1150
#define WINDOW_HEITH 650
#define LEFT 0
#define CENTER 1
#define RIGHT 2
#define NEXT 1
#define PREVIOUS 0

/** la declaration de la structure utilisee dans la fonction (FindRemplace)**/
typedef struct {
    GtkWidget *pEntryNew;
    GtkWidget *pEntryLast;
    GtkWidget *pRadio;
}GtkReplace;

/** la declararion des prototypes des fonctions **/
void OnNew (GtkWidget *pWidget, gpointer data);
void OnOpen(GtkWidget *pWidget, gpointer data);
void OnSave(GtkWidget *pWidget, gpointer data);
void OnHelp(GtkWidget *pWidget, gpointer data);
void OnQuit(GtkWidget *pWidget, gpointer data);
void OnPage(GtkWidget *pWidget, gpointer data);
void OnFind(GtkWidget *pWidget, gpointer data);
void OnAligne(GtkWidget *pWidget, gpointer data);
void OnRefresh(GtkWidget *pWidget, gpointer data);
void OnFind(GtkWidget *pWidget, gpointer data);
void OnFindReplace(GtkWidget *pWidget, gpointer data);
void OnSearch(GtkWidget *pWidget, gpointer data);
void OnReplace(GtkWidget *pWidget, GtkReplace *pReplace);
void OpenFile(FILE *fichier);
void SaveFile(gchar *sName);
void JustifiText();

/** la delaration des variables globales **/
static GtkWidget *pWindow;
static GtkWidget *pTextView;
static GtkWidget *pLabelPosition;
static GtkWidget *pLabelPage;
gchar *sFileName = 0;
gint iPageCourante = 0;

int main(int argc, char *argv[])
{
    /** la declaration des variables **/
    GtkWidget *pMainBox;
    GtkWidget *pMenuBar;
    GtkWidget *pMenu;
    GtkWidget *pMenuItem;
    GtkWidget *pToolBar;
    GtkWidget *pScrolled;

    /** l initialisation de la gtk **/
    gtk_init(&argc, &argv);

    /** la creation de la fenetre **/
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(pWindow), "Esitor");
    gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(pWindow), FALSE);
    gtk_widget_set_size_request(pWindow, WINDOW_WIDTH, WINDOW_HEITH);
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /** la cration du box principale **/
    pMainBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(pWindow), pMainBox);

    /** la creation des barres d etat **/
    pLabelPosition = gtk_label_new(NULL);
    gtk_box_pack_end(GTK_BOX(pMainBox), pLabelPosition, FALSE, FALSE, 0);
    pLabelPage = gtk_label_new(NULL);
    gtk_box_pack_end(GTK_BOX(pMainBox), pLabelPage, FALSE, FALSE, 0);

    /** la creation du menu principal **/
    /** la creation du menu barre **/
    pMenuBar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(pMainBox), pMenuBar, FALSE, FALSE, 0);
    /** la creation du menu (FILE)**/
    pMenu = gtk_menu_new();
    /** la creation des elements du menu (FILE)  et l insertion dans le menu(FILE) **/
    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnNew), NULL);

    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnOpen), NULL);

    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnSave), NULL);

    pMenuItem = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);

    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnQuit), NULL);

    /** la creation de l elements pour l associer au menu (FILE) et l insertion dans le menu (MenuBar) **/
    pMenuItem = gtk_menu_item_new_with_label("File\t");
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);
    /** l association du menuItem au menu **/
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);

    /** la creation du menu (Edit) **/
    pMenu = gtk_menu_new();
    /** la creation des elements du menu (EDIT)  et l insertion dans le menu(EDIT) **/
    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_JUSTIFY_LEFT,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnAligne), GINT_TO_POINTER(LEFT));

    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_JUSTIFY_CENTER,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnAligne), GINT_TO_POINTER(CENTER));

    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_JUSTIFY_RIGHT,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnAligne), GINT_TO_POINTER(RIGHT));

    pMenuItem = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);

    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_MEDIA_NEXT,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnPage), GINT_TO_POINTER(NEXT));

    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_MEDIA_PREVIOUS,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnPage), GINT_TO_POINTER(PREVIOUS));

    pMenuItem = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);

    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnFind), NULL);

    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND_AND_REPLACE,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnFindReplace), NULL);

    pMenuItem = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);

    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_REFRESH, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnRefresh), NULL);

    /** la creation de l elements pour l associer au menu (Edit) et l insertion dans le menu (MenuBar) **/
    pMenuItem = gtk_menu_item_new_with_label("Edit\t");
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);
    /** l association du menuItem au menu **/
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);

    /** la creation du menu (Help)**/
    pMenu = gtk_menu_new();
    /** la creation des elements de lu menu (Help)  et l insertion dans le menu(Help) **/
    pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP,NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
    g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnHelp), NULL);

    /** la creation de l elements pour l associer au menu (Help) et l insertion dans le menu (MenuBar) **/
    pMenuItem = gtk_menu_item_new_with_label("Help\t");
    gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);
    /** l association du menuItem au menu **/
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);

    /** la creation de la barre d outil **/
    pToolBar = gtk_toolbar_new();
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(pToolBar), GTK_ICON_SIZE_DND);
    gtk_toolbar_set_style(GTK_TOOLBAR(pToolBar), GTK_TOOLBAR_ICONS);
    gtk_box_pack_start(GTK_BOX(pMainBox), pToolBar, FALSE, FALSE, 0);

    /** la creation des elements de la barre a partir du stock **/
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_NEW, "New",
        NULL,G_CALLBACK(OnNew), NULL,-1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_OPEN,"Open",
        NULL,G_CALLBACK(OnOpen), NULL,-1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_SAVE,"Save",
        NULL,G_CALLBACK(OnSave), NULL,-1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_HELP,"Help",
        NULL,G_CALLBACK(OnHelp),NULL,-1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_QUIT,"Quit",
        NULL,G_CALLBACK(OnQuit), NULL,-1);
    gtk_toolbar_append_space(GTK_TOOLBAR(pToolBar));
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_JUSTIFY_LEFT,"Left",
        NULL,G_CALLBACK(OnAligne), GINT_TO_POINTER(LEFT),-1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_JUSTIFY_CENTER,"Center",
        NULL,G_CALLBACK(OnAligne), GINT_TO_POINTER(CENTER),-1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_JUSTIFY_RIGHT,"Right",
        NULL,G_CALLBACK(OnAligne), GINT_TO_POINTER(RIGHT),-1);
    gtk_toolbar_append_space(GTK_TOOLBAR(pToolBar));
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_MEDIA_PREVIOUS,"Previous",
        NULL,G_CALLBACK(OnPage),GINT_TO_POINTER(PREVIOUS),-1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_MEDIA_NEXT,"Next",
        NULL,G_CALLBACK(OnPage),GINT_TO_POINTER(NEXT),-1);
    gtk_toolbar_append_space(GTK_TOOLBAR(pToolBar));
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_FIND,"Find",
        NULL,G_CALLBACK(OnFind),NULL,-1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar),GTK_STOCK_FIND_AND_REPLACE,"Find and Replace",
        NULL,G_CALLBACK(OnFindReplace),NULL,-1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolBar), GTK_STOCK_REFRESH, "Refresh",
        NULL, G_CALLBACK(OnRefresh), NULL,  -1);

    /** la creation de la scrolled pour innserer la zone de texte dedans **/
    pScrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(pMainBox), pScrolled, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(pScrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    /** la creation de zone de texte **/
    pTextView = gtk_text_view_new();
    gtk_text_view_set_border_window_size(GTK_TEXT_VIEW(pTextView), GTK_TEXT_WINDOW_LEFT, 12);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(pTextView), 2);
    gtk_container_add(GTK_CONTAINER(pScrolled), pTextView);

    /** l affichage de la fenetre et la boucle main **/
    gtk_widget_show_all(pWindow);
    gtk_main();
    return EXIT_SUCCESS;
}
/** les fonctions principales du programme **/
/** la creation d une nouvelle page **/
void OnNew(GtkWidget *pWidget, gpointer data)
{
    GtkTextBuffer *pTextBuffer;
    GtkTextIter start, end;
    gchar *buffer = 0;

    /** la recuperation du texte buffer et son debut et sa fin **/
    pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pTextView));
    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(pTextBuffer), &start);
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(pTextBuffer), &end);

    /** la recuperation du buffer a partir du texte buffer **/
    buffer = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(pTextBuffer), &start, &end, FALSE);

    /** si le buffer est vide ou on a pas lu un fichier alors on fait rien **/
    /** si le buffer n est pas vide ou l utilisateur a lu un fichier on demmande a l utilisateur d enregistrer **/
    if((buffer[0] != '\0' )||(sFileName != 0) ){
        GtkWidget *pDialog;
        GtkWidget *pLabelDialog;

        /** la creation de la boite de dialogue pour savoir si l utilisateur veut enregistrer le fichier **/
        pDialog = gtk_dialog_new_with_buttons("Esitor", GTK_WINDOW(pWindow), GTK_DIALOG_MODAL,
            GTK_STOCK_OK, GTK_RESPONSE_OK,
            GTK_STOCK_NO, GTK_RESPONSE_NO, NULL);
        gtk_widget_set_size_request(pDialog, 300, 130);

        /** la creation du label de la boite **/
        pLabelDialog = gtk_label_new("Do you want to save the file ? ");
        gtk_label_set_justify(GTK_LABEL(pLabelDialog), GTK_JUSTIFY_CENTER);

        /** Insertion du label dans la boite de dialogue **/
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabelDialog, TRUE, TRUE, 0);

        /** l affichage de la fenetre **/
        gtk_widget_show_all(GTK_DIALOG(pDialog)->vbox);

        switch(gtk_dialog_run(GTK_DIALOG(pDialog))){
            case GTK_RESPONSE_NO:
            case GTK_RESPONSE_NONE:
                gtk_widget_destroy(pDialog);
                break;
            case GTK_RESPONSE_OK:
                /** la cas ou l utilisaeur veux enregistrer le buffer **/
                if(sFileName != 0)
                    SaveFile(sFileName);
                else{
                    const gchar *sName;
                    GtkWidget *pBoite;
                    GtkWidget *pEntry;
                    GtkWidget *pLabel;

                    /** creation de la boite du dialogue **/
                    pBoite = gtk_dialog_new_with_buttons("Save As", GTK_WINDOW(pWindow),
                        GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK,
                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
                    gtk_widget_set_size_request(pBoite, 300, 130);

                    /** Création de la zone de saisie et le label contenant le message */
                    pEntry = gtk_entry_new();
                    pLabel = gtk_label_new("\nEnter the path of the file\n");
                    gtk_label_set_justify(GTK_LABEL(pLabel), GTK_JUSTIFY_CENTER);

                    /** Insertion de la zone de saisie dans la boite de dialogue */
                    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pBoite)->vbox), pLabel, TRUE, TRUE, 0);
                    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pBoite)->vbox), pEntry, TRUE, TRUE, 0);

                    /** Affichage des éléments de la boite de dialogue */
                    gtk_widget_show_all(GTK_DIALOG(pBoite)->vbox);

                    /** On lance la boite de dialogue et on récupéré la réponse */
                    switch (gtk_dialog_run(GTK_DIALOG(pBoite))){
                        case GTK_RESPONSE_NONE:
                        case GTK_RESPONSE_CANCEL:
                            gtk_widget_destroy(pBoite);
                            break;
                        case GTK_RESPONSE_OK:
                            /** la recupeartion du nouveau mot **/
                            sName = gtk_entry_get_text(GTK_ENTRY(pEntry));
                            sFileName = g_strdup_printf("%s", sName);
                            SaveFile(sFileName);
                            gtk_widget_destroy(pBoite);
                            break;
                    }
                }
                gtk_widget_destroy(pDialog);
                break;
        }
    }
    /** on inisalize le TextView par un (NULL)**/
    pTextBuffer = gtk_text_buffer_new(NULL);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(pTextView), pTextBuffer);
    g_free(buffer);
    sFileName = 0;
}
/** l affichage de a boite dialogue (Open ) pour inserer le nom du fichier a lire **/
void OnOpen(GtkWidget *pWidget, gpointer datas)
{
    GtkWidget *pDialog;
    GtkWidget *pEntry;
    GtkWidget *pLabel;
    FILE* fichier = 0;
    const gchar *sName;

    /** creation de la boite du dialogue **/
    pDialog = gtk_dialog_new_with_buttons("Open", GTK_WINDOW(pWindow),
        GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK,
        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_widget_set_size_request(pDialog, 300, 130);

    /** Création de la zone de saisie et le label contenant le message */
    pEntry = gtk_entry_new();
    pLabel = gtk_label_new("Enter the path of the file : ");
    gtk_label_set_justify(GTK_LABEL(pLabel), GTK_JUSTIFY_CENTER);

    /** Insertion de la zone de saisie et le label dans la boite de dialogue */
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabel, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pEntry, TRUE, TRUE, 0);

    /** Affichage des éléments de la boite de dialogue */
    gtk_widget_show_all(GTK_DIALOG(pDialog)->vbox);

    /** On lance la boite de dialogue et on récupéré la réponse */
    switch (gtk_dialog_run(GTK_DIALOG(pDialog))){
        case GTK_RESPONSE_CANCEL:
        case GTK_RESPONSE_NONE:
            break;
        case GTK_RESPONSE_OK:
            /** la recuperation du nom du fichier **/
            sName = gtk_entry_get_text(GTK_ENTRY(pEntry));
            sFileName = g_strdup_printf("%s", sName);
            fichier = fopen(sFileName, "rt+");

            if (fichier == NULL){
                    /** si le fichier n existe pas on affiche un message d erreur **/
                    GtkWidget *pError;
                    pError = gtk_message_dialog_new(GTK_WINDOW(pWindow), GTK_DIALOG_MODAL,
                            GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "The file : ( %s ) does not exist",
                            g_locale_to_utf8(sFileName, -1, NULL, NULL, NULL));
                    gtk_dialog_run(GTK_DIALOG(pError));
                    gtk_widget_destroy(pError);
                    sFileName = 0;
            }
            else {
                /** si le fichier existe alors on le charge dans le GtkTextView  **/
                /** on fait une justification du fichier **/
                OpenFile(fichier);
                fclose(fichier);
                JustifiText();
                OnRefresh(NULL, NULL);
            }
            break;
    }
    gtk_widget_destroy(pDialog);
}
/** l enregistrement du buffer dans le fichier (sFileName)**/
void OnSave(GtkWidget *pWidget, gpointer data)
{
    /** si l utilisateur a entre deja le nom du fichier **/
    /** on enregistre le buffer dans le fichier donnee **/
    if(sFileName != 0){
        SaveFile(sFileName);
    }
    /** si l  utilisateur n a pas entre encore le nom du fichier **/
    /** on lit le nom du fichier depuis une boite de dialogue **/
    else{
        GtkWidget *pDialog;
        GtkWidget *pEntry;
        GtkWidget *pLabel;
        const gchar* sName;

        /** creation de la boite du dialogue **/
        pDialog = gtk_dialog_new_with_buttons("Save", GTK_WINDOW(pWindow),
            GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK,
            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

        gtk_widget_set_size_request(pDialog, 300, 130);

        /** Création de la zone de saisie et le label contenant le message */
        pEntry = gtk_entry_new();
        pLabel = gtk_label_new("Enter the path of the file");
        gtk_label_set_justify(GTK_LABEL(pLabel), GTK_JUSTIFY_CENTER);

        /** Insertion de la zone de saisie dans la boite de dialogue */
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabel, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pEntry, TRUE, TRUE, 0);

        /** Affichage des éléments de la boite de dialogue */
        gtk_widget_show_all(GTK_DIALOG(pDialog)->vbox);

        /** On lance la boite de dialogue et on récupéré la réponse */
        switch (gtk_dialog_run(GTK_DIALOG(pDialog))){
            case GTK_RESPONSE_CANCEL:
            case GTK_RESPONSE_NONE:
                break;
            case GTK_RESPONSE_OK:
                /** la recuperation du nom et l enregistrement dans (sFileName)**/
                sName  = gtk_entry_get_text(GTK_ENTRY(pEntry));
                sFileName = g_strdup_printf("%s", sName);
                SaveFile(sFileName);
                break;
        }
        gtk_widget_destroy(pDialog);
    }
}
/** l affichage de la boite du dialogue (Help)**/
void OnHelp(GtkWidget *pWidget, gpointer data)
{
    GtkWidget *pDialog;
    /** creation de la boite du help **/
    pDialog = gtk_message_dialog_new(GTK_WINDOW(pWindow), GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
            "Esitor is a text editor\nVersion Esitor 1.1\n\nWritten By ABIDI Souad and CHELGHOUM Aimen\nESI 2CPI 2016-2017\nGroupe : 09");
    gtk_dialog_run(GTK_DIALOG(pDialog));
    gtk_widget_destroy(pDialog);
}
/** l affichage de la boite pour la fermeture di programme **/
void OnQuit(GtkWidget *pWidget, gpointer data)
{
    GtkWidget *pDialog;
    GtkWidget *pLabelDialog;
    const gchar *sName;

    /** la creation de la boite de dialogue pour savooir si l utilisateur veut enregistrer le fichier **/
    pDialog = gtk_dialog_new_with_buttons("Esitor", GTK_WINDOW(pWindow), GTK_DIALOG_MODAL,
        GTK_STOCK_OK, GTK_RESPONSE_OK,
        GTK_STOCK_NO, GTK_RESPONSE_NO,
        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_widget_set_size_request(pDialog, 300, 130);
    /** la creation du label de la boite **/
    pLabelDialog = gtk_label_new("Do you want to save the file ? ");
    gtk_label_set_justify(GTK_LABEL(pLabelDialog), GTK_JUSTIFY_CENTER);
    /** Insertion du label dans la boite de dialogue */
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabelDialog, TRUE, TRUE, 0);

    gtk_widget_show_all(GTK_DIALOG(pDialog)->vbox);

    switch(gtk_dialog_run(GTK_DIALOG(pDialog))){
        case GTK_RESPONSE_NO:
            gtk_main_quit();
        case GTK_RESPONSE_CANCEL:
            gtk_widget_destroy(pDialog);
            break;
        case GTK_RESPONSE_OK:
            if (sFileName == 0){
                GtkWidget *pBoite;
                GtkWidget *pEntry;
                GtkWidget *pLabel;

                /** creation de la boite du dialogue **/
                pBoite = gtk_dialog_new_with_buttons("Save As", GTK_WINDOW(pWindow),
                    GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK,
                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
                gtk_widget_set_size_request(pBoite, 300, 130);

                /** Création de la zone de saisie et le label contenant le message */
                pEntry = gtk_entry_new();
                pLabel = gtk_label_new("\nEnter the path of the file\n");
                gtk_label_set_justify(GTK_LABEL(pLabel), GTK_JUSTIFY_CENTER);

                /** Insertion de la zone de saisie dans la boite de dialogue */
                gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pBoite)->vbox), pLabel, TRUE, TRUE, 0);
                gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pBoite)->vbox), pEntry, TRUE, TRUE, 0);

                /** Affichage des éléments de la boite de dialogue */
                gtk_widget_show_all(GTK_DIALOG(pBoite)->vbox);

                /** On lance la boite de dialogue et on récupéré la réponse */
                switch (gtk_dialog_run(GTK_DIALOG(pBoite))){
                    case GTK_RESPONSE_CANCEL:
                        gtk_widget_destroy(pBoite);
                        gtk_widget_destroy(pDialog);
                        break;
                    case GTK_RESPONSE_OK:
                        /** la recupeartion du nouveau mot **/
                        sName = gtk_entry_get_text(GTK_ENTRY(pEntry));
                        sFileName = g_strdup_printf("%s", sName);
                        SaveFile(sFileName);
                        break;
                }
            }
            else
                SaveFile(sFileName);
            gtk_main_quit();
            break;
    }
}
/** le changement de la page  courante **/
/** cette fonction est appele lors des singaux mis par (MenuItemNext) et (MenuItemPrevieus)**/
void OnPage(GtkWidget *pWidget, gpointer data)
{
    GtkTextBuffer *pTextBuffer;
    GtkTextIter where;
    gint tmp, NbLigne;
    gchar *sStatus;

    /** la recuperation du buffer et son debut et le nombre de ligne **/
    pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pTextView));
    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(pTextBuffer), &where);
    NbLigne = gtk_text_buffer_get_line_count(GTK_TEXT_BUFFER(pTextBuffer));

    /** on recupere la donnee suplementaire de la fonction **/
    tmp = GPOINTER_TO_INT(data);

    /** le changement de la valeur de iPageCourante **/
    iPageCourante += tmp ? +NUMBER_LINE : -NUMBER_LINE;
    iPageCourante -= (iPageCourante % NUMBER_LINE);
    if(iPageCourante < 0)
        iPageCourante = 0;
    if(iPageCourante >= NbLigne)
        iPageCourante = NbLigne;

    /** on scroll le TextView jusqu a la nouvelle place **/
    gtk_text_iter_set_line(&where, iPageCourante);
    gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(pTextView), &where ,0.0, TRUE, 0, 0);

    /** le changement du label pLabelPage **/
    sStatus = g_strdup_printf("Page Number : %d", (iPageCourante / NUMBER_LINE) + 1);
    gtk_label_set_justify(GTK_LABEL(pLabelPage), GTK_JUSTIFY_CENTER);
    gtk_label_set_text(GTK_LABEL(pLabelPage), sStatus);
}
/** affichage de la barre de status dans la fenetre **/
void OnRefresh(GtkWidget *pWidget, gpointer data)
{
    GtkTextBuffer *pTextBuffer;
    GtkTextIter start, end;
    gchar *buffer = 0;
    gchar *sStatus = 0;
    gboolean trouvMot = FALSE;
    gint NbMot = 1, NbLigne = 1, NbChar = 0, taille, i, newLine = 0;

    /** on fait une justification du textView **/
    JustifiText();

    /** la recuperation du texte buffer et son debut et sa fin **/
    pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pTextView));
    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(pTextBuffer), &start);
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(pTextBuffer), &end);

    /** la recuperation du buffer a partir du textebuffer **/
    buffer = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(pTextBuffer), &start, &end, FALSE);
    taille = strlen(buffer);
    iPageCourante = 1;
    /** le calcul du nombre de mot, ligne et caractere **/
    for(i=0; i<taille; i++){
        if(buffer[i] == '\t')
            newLine += 7;
        else
            newLine ++;

        if(buffer[i] > 32 && buffer[i] < 127){
            NbChar++;
            trouvMot = 1;
        }
        else if(trouvMot){
            trouvMot = 0;
            NbMot++;
        }
        if(buffer[i] == '\n')
            NbLigne++;
    }

    /** la modifiation du label position **/
    sStatus = g_strdup_printf("NB Word = %d\t\tNB Ligne = %d\t\tNB Char  = %d",
        NbMot, NbLigne, NbChar);
    gtk_label_set_text(GTK_LABEL(pLabelPosition), sStatus);
    gtk_label_set_justify(GTK_LABEL(pLabelPosition), GTK_JUSTIFY_CENTER);

    /** la modification du label page **/
    sStatus = g_strdup_printf("Page Number : %d", iPageCourante);
    gtk_label_set_text(GTK_LABEL(pLabelPage), sStatus);
    gtk_label_set_justify(GTK_LABEL(pLabelPage), GTK_JUSTIFY_CENTER);

    /** la leberation de la memoire **/
    g_free(buffer);
    g_free(sStatus);
}
/** changer l alignement du texte a savoir la valeur de data **/
void OnAligne(GtkWidget *pWidget, gpointer data)
{
    /** on fait une justification du textView **/
    JustifiText();

    gint i = GPOINTER_TO_INT(data);
    switch (i) {
    case LEFT:
        gtk_text_view_set_justification(GTK_TEXT_VIEW(pTextView), GTK_JUSTIFY_LEFT);
        break;
    case CENTER:
        gtk_text_view_set_justification(GTK_TEXT_VIEW(pTextView), GTK_JUSTIFY_CENTER);
        break;
    case RIGHT:
        gtk_text_view_set_justification(GTK_TEXT_VIEW(pTextView), GTK_JUSTIFY_RIGHT);
        break;
    }
}
/** afficher la boite de dialogue pour faire la recherche **/
void OnFind(GtkWidget *pWidget, gpointer data)
{
    GtkWidget *pDialog;
    GtkWidget *pLabel;
    GtkWidget *pEntry;
    GtkWidget *pButton;

    /** on fait une justification du textView **/
    JustifiText();

    /** la creation de la boite de dialogue **/
    pDialog = gtk_dialog_new_with_buttons("Find", GTK_WINDOW(pWindow),
        GTK_DIALOG_MODAL, NULL);
    gtk_widget_set_size_request(pDialog, 350, 200);

    /** la creation et l insertion des elements de la boite de dialogue **/
    pLabel = gtk_label_new("Enter word : ");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabel, TRUE, FALSE, 0);
    pEntry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pEntry, TRUE, FALSE, 0);
    pButton = gtk_button_new_with_label("\nSearch\n");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pButton, TRUE, FALSE, 0);

    /** la connexion a la fonction OnSearch au cas du clicke sur le button **/
    g_signal_connect(G_OBJECT(pButton), "clicked", G_CALLBACK(OnSearch), pEntry);

    /** l affichage de la boite et le(run) et la fermeture **/
    gtk_widget_show_all(GTK_DIALOG(pDialog)->vbox);
    gtk_dialog_run(GTK_DIALOG(pDialog));
    gtk_widget_destroy(pDialog);
}
/** l affichage de la biote de  dialogue pour faire la recherhce et le remplacement **/
void OnFindReplace(GtkWidget *pWidget, gpointer data)
{
    GtkWidget *pDialog;
    GtkWidget *pLabel;
    GtkWidget *pButton;
    GtkWidget *pRadioAll;
    GtkReplace *pReplace = g_slice_new(GtkReplace);

    /** on fait une justification du textView **/
    JustifiText();

    /** la creation de la boite de dialogue **/
    pDialog = gtk_dialog_new_with_buttons("Find and replace", GTK_WINDOW(pWindow),
        GTK_DIALOG_MODAL, NULL);
    gtk_widget_set_size_request(pDialog, 400, 300);

    /**la creation et l insertion des zones a inserer dans la boite de dialogue **/
    pLabel = gtk_label_new("Enter the word : ");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabel, TRUE, TRUE, 0);
    pReplace->pEntryLast =  gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pReplace->pEntryLast, TRUE, TRUE, 0);
    pLabel = gtk_label_new("Enter the New word : ");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabel, TRUE, TRUE, 0);
    pReplace->pEntryNew =  gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pReplace->pEntryNew, TRUE, TRUE, 0);
    pLabel = gtk_label_new("Do you want to replace one or all the words");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabel, TRUE, TRUE, 0);
    pReplace->pRadio = gtk_radio_button_new_with_label(NULL, "One Iteration");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pReplace->pRadio, TRUE, TRUE, 0);
    pRadioAll = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(pReplace->pRadio), "All the Iretations");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pRadioAll, TRUE, TRUE, 0);
    pButton = gtk_button_new_with_label("Replace");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pButton, TRUE, TRUE, 0);

    g_signal_connect(G_OBJECT(pButton), "clicked", G_CALLBACK(OnReplace), (gpointer*)pReplace);

    /** l affichage et le (run) de la boite de dialogue pDilaog **/
    gtk_widget_show_all(GTK_DIALOG(pDialog)->vbox);
    gtk_dialog_run(GTK_DIALOG(pDialog));
    gtk_widget_destroy(pDialog);

}
/** les fonctions secondaires **/
void OnSearch(GtkWidget *pWidget, gpointer data)
{
    GtkWidget *pDialog;
    GtkWidget *pLabel, *pLabelEnd;
    GtkWidget *pScrolled;
    GtkWidget *pBox;
    GtkTextBuffer *pTextBuffer;
    GtkTextIter start, begin, end;
    gchar *sFind;
    const gchar *sWord;
    gint cpt = 0, position, ligne;
    gboolean success;

    /** la recuperation du buffer et le debut **/
    pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pTextView));
    gtk_text_buffer_get_start_iter(pTextBuffer, &start);

    /** la recupration du mot de la GtkEntry **/
    sWord = gtk_entry_get_text(GTK_ENTRY(data));

    /** la creation de la boite de dialogue et ses elements **/
    pDialog = gtk_dialog_new_with_buttons("Search", GTK_WINDOW(pWindow),
            GTK_DIALOG_MODAL, NULL);
    gtk_widget_set_size_request(pDialog, 350, 400);

    /** la creation des widgets a inserer dans la boite de dialoque **/
    pLabel = gtk_label_new("\nThe Search of the word\n");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabel, FALSE, FALSE, 0);
    pLabel = gtk_label_new("|-------------------------------------------------------------|");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabel, FALSE, FALSE, 0);
    pLabel = gtk_label_new("The Word\t  |  \tThe Ligne\t  |  \tThe Position");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabel, FALSE, FALSE, 0);
    pLabel = gtk_label_new("|-------------------------------------------------------------|");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabel, FALSE, FALSE, 0);
    pLabelEnd = gtk_label_new(NULL);
    gtk_box_pack_end(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pLabelEnd, FALSE, FALSE, 0);
    pScrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(pScrolled),
        GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pDialog)->vbox), pScrolled, TRUE, TRUE, 0);

    /** la creation du box qui vas contenir les labels **/
    pBox = gtk_vbox_new(TRUE, 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(pScrolled), pBox);

    success = gtk_text_iter_forward_search (&start, (gchar*) sWord, 0,
            &begin, &end, NULL);

    while (success){
        gtk_text_iter_forward_char (&start);
        success = gtk_text_iter_forward_search (&start, (gchar*) sWord, 0,
            &begin, &end, NULL);
        ligne = gtk_text_iter_get_line(&begin)+1;
        position  = gtk_text_iter_get_line_offset(&begin)+1;
        sFind = g_strdup_printf("%s\t\t%d\t\t%d", sWord, ligne, position);
        pLabel = gtk_label_new(sFind);
        gtk_box_pack_start(GTK_BOX(pBox), pLabel, TRUE, TRUE, 0);

        start = begin;
        cpt++;
    }
    sFind = g_strdup_printf("The Word has found %d times", cpt);
    gtk_label_set_text(GTK_LABEL(pLabelEnd), sFind);
    /** l affichage et le (run) de la boite de dialogue pDialog **/
    gtk_widget_show_all(GTK_DIALOG(pDialog)->vbox);
    gtk_dialog_run(GTK_DIALOG(pDialog));
    gtk_widget_destroy(pDialog);
}

void OnReplace(GtkWidget *pWidget, GtkReplace *pReplace)
{
    GtkTextBuffer *pTextBuffer;
    GtkTextIter start, end, begin;
    const gchar *sLastWord, *sNewWord;
    GSList *pListRadio;


    pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pTextView));
    gtk_text_buffer_get_start_iter(pTextBuffer, &start);

    sLastWord = gtk_entry_get_text(GTK_ENTRY(pReplace->pEntryLast));
    sNewWord = gtk_entry_get_text(GTK_ENTRY(pReplace->pEntryNew));

    pListRadio = gtk_radio_button_get_group(GTK_RADIO_BUTTON(pReplace->pRadio));

    /** si le button a ete selection ie (Une seul iteration) on remplace une seule fois**/
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pListRadio->next->data))){
        if (gtk_text_iter_forward_search(&start, (gchar*)sLastWord, 0, &begin,&end, NULL)){
            gtk_text_buffer_delete_interactive(GTK_TEXT_BUFFER(pTextBuffer),
                &begin, &end, TRUE);
            gtk_text_buffer_insert(GTK_TEXT_BUFFER(pTextBuffer), &begin,
                sNewWord, strlen(sNewWord));
        }
    }
    /*** sinon on remplace plusieurs fois **/
    else{
        while(gtk_text_iter_forward_search(&start, (gchar*)sLastWord, 0, &begin,&end, NULL)){
            gtk_text_iter_forward_char (&start);
            gtk_text_buffer_delete_interactive(GTK_TEXT_BUFFER(pTextBuffer),
                &begin, &end, TRUE);
            gtk_text_buffer_insert(GTK_TEXT_BUFFER(pTextBuffer), &begin,
                sNewWord, strlen(sNewWord));
            start = begin;
        }
    }
}
/** enregistrer le texte du buffer (pTextView) dans le fichier (sFileName)**/
void SaveFile(gchar *sName)
{
    FILE *fichier = 0;
    GtkTextBuffer *pTextBuffer;
    GtkTextIter start, end;
    char *buffer = 0;
    fichier = fopen(sName, "wt+");
    if (fichier != NULL){
        /** la recuperation du texte buffer et son debut et sa fin **/
        pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pTextView));
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(pTextBuffer), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(pTextBuffer), &end);

        /** la recuperation du buffer a partir du textebuffer **/
        buffer = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(pTextBuffer), &start, &end, FALSE);
        fprintf(fichier, "%s", buffer);
        fclose(fichier);
        g_free(buffer);
    }
}
/** la justification du texte (ie le nombre de caracetre dans chaque ligne)**/
/** on appelle cette fonction chaque fois qu on veux faire un traitemet **/
void JustifiText()
{
    FILE *fichier;
    GtkTextBuffer *pTextBuffer;
    GtkTextIter start, end;
    gchar *buffer = 0;
    gint i, newLigne = 0;

    /** on recupere le buffer dans une chaine de caractere **/
    /** et on l ecrit dans un fichier temporaire **/
    fichier = fopen("D:\tmp.txt", "wt+");
    if (fichier != NULL){
        /** la recuperation du texte buffer et son debut et sa fin **/
        pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pTextView));
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(pTextBuffer), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(pTextBuffer), &end);

        /** la recuperation du buffer a partir du textebuffer **/
        buffer = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(pTextBuffer), &start, &end, FALSE);
        /** si le nombre de caracetre depasse le nombre maximum alors on insert un saut de ligne **/
        for(i=0; i < strlen(buffer); i++){
            if((newLigne > SIZE_LINE) && buffer[i] != '\n'){
                fputc('\n', fichier);
                newLigne = 0;
            }
            if(buffer[i] == '\n')
                newLigne = -1;
            fputc(buffer[i], fichier);
            newLigne++;
        }
        OpenFile(fichier);
        fclose(fichier);
        g_free(buffer);
        system("DEL D:\tmp.txt");
    }
}
/** la lecture du fichier (fichier) et l insertion dans le GtkTextView **/
void OpenFile(FILE *fichier)
{
    GtkTextBuffer *Buffer = 0;
    GtkTextIter start, end;
    gchar lecture[1024];

    /** la recuperation du buffer **/
    Buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pTextView));

    fseek(fichier, 0, SEEK_SET);
    /** la recuperation du debut et la fin du buffer **/
    gtk_text_buffer_get_start_iter(Buffer,&start);
    gtk_text_buffer_get_end_iter(Buffer,&end);
    gtk_text_buffer_delete(Buffer, &start, &end);

    /** on  recupere succesivement des chaines de caracetres de taille 1024 et on les insert dans le buffer **/
    while(fgets(lecture, 1024, fichier)){
        gtk_text_buffer_get_end_iter(Buffer,&end);
        gtk_text_buffer_insert(Buffer, &end, g_locale_to_utf8(lecture, -1, NULL, NULL, NULL), -1);
    }
}
