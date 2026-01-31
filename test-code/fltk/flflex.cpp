//
// Created by fab on 1/31/26.
//
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

int main(int argc, char **argv) {
    // 1. Creiamo la finestra principale
    Fl_Window *win = new Fl_Window(600, 400, "Esempio Fl_Flex");

    // 2. Creiamo un layout flessibile orizzontale (ROW) che riempie la finestra
    Fl_Flex *root_flex = new Fl_Flex(0, 0, 600, 400, Fl_Flex::ROW);

    // Aggiungiamo un widget "Side Bar"
    Fl_Button *side_bar = new Fl_Button(0, 0, 0, 0, "Side Bar");
    side_bar->box(FL_FLAT_BOX);
    side_bar->color(fl_rgb_color(200, 200, 200));

    // Aggiungiamo un'area di contenuto (un altro Flex, ma verticale)
    Fl_Flex *content_flex = new Fl_Flex(Fl_Flex::COLUMN);

    Fl_Box *top_box = new Fl_Box(0, 0, 0, 0, "Area Superiore (Flessibile)");
    top_box->box(FL_UP_BOX);

    Fl_Button *bottom_btn = new Fl_Button(0, 0, 0, 0, "Bottone Inferiore (Fisso)");

    content_flex->end();

    // 3. Configurazione del dimensionamento
    // Rendiamo la Side Bar larga esattamente 150px (non si restringe)
    root_flex->fixed(side_bar, 150);

    // Nel content_flex, rendiamo il bottone alto esattamente 50px
    content_flex->fixed(bottom_btn, 50);

    root_flex->end();

    // Rendiamo il layout ridimensionabile insieme alla finestra
    win->resizable(root_flex);
    win->show(argc, argv);

    return Fl::run();
}