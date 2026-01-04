#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Pixmap.H>

// Assicurati che l'array sia formattato esattamente così:
static const char *mio_xpm_data[] = {
    "10 10 2 1",       // Larghezza, Altezza, Numero Colori, Caratteri per pixel
    "  c None",        // Colore trasparente
    "@ c #FF0000",     // Rosso
    "  @@@@@@  ",
    " @@@@@@@@ ",
    "@@@@@@@@@@",
    "@@@@@@@@@@",
    "@@@@@@@@@@",
    "@@@@@@@@@@",
    "@@@@@@@@@@",
    "@@@@@@@@@@",
    " @@@@@@@@ ",
    "  @@@@@@  "
};

int main() {
    Fl_Window *win = new Fl_Window(300, 200, "Fix Crash XPM");

    // Il segreto è il cast a (char**) per Fl_Pixmap
    // Questo evita l'incompatibilità dei tipi che spesso causa crash
    Fl_Pixmap *img = new Fl_Pixmap((char**)mio_xpm_data);

    // Verifica di sicurezza: se img->w() o img->h() sono 0, il parsing è fallito
    if (img->w() == 0) {
        /* Gestisci errore di parsing qui */
    }

    Fl_Box *box = new Fl_Box(50, 50, 200, 100);
    box->image(img);

    win->end();
    win->show();
    return Fl::run();
}