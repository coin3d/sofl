#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Pixmap.H>

// Esempio di icona XPM (una semplice "X" colorata)
static const char *const icon_xpm[] = {
    "16 16 3 1",
    "  c None",
    ". c #FF0000",
    "+ c #000000",
    "                ",
    "  ++++    ++++  ",
    "   ++++  ++++   ",
    "    ++++++++    ",
    "     ++++++     ",
    "      ++++      ",
    "     ++++++     ",
    "    ++++++++    ",
    "   ++++  ++++   ",
    "  ++++    ++++  ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                "
};

int main(int argc, char **argv) {
    // 1. Creiamo la finestra principale
    Fl_Double_Window *win = new Fl_Double_Window(150, 300, "Toolbar XPM");

    // 2. Usiamo Fl_Pack per disporre i bottoni verticalmente
    // Il Pack si occupa di posizionare i widget uno sotto l'altro
    Fl_Pack *pack = new Fl_Pack(25, 20, 100, 260);
    //pack->spacing(10); // Spazio tra i bottoni

    // 3. Creiamo l'oggetto Pixmap dai dati XPM
    Fl_Pixmap *image = new Fl_Pixmap(icon_xpm);

    // 4. Aggiungiamo i bottoni al pack
    for (int i = 1; i <= 4; i++) {
        // Creiamo un gruppo alto quanto il bottone ma largo quanto il Pack
        Fl_Group *g = new Fl_Group(0, 0, 150, 40);

        // Creiamo il bottone dentro il gruppo con dimensioni FISSE (es. 80x40)
        Fl_Button *btn = new Fl_Button(g->x(), g->y(), 40, 40, nullptr);
        btn->image(image);

        g->end(); // Chiudiamo il gruppo
    }

    pack->end();
    win->end();
    win->show(argc, argv);

    return Fl::run();
}