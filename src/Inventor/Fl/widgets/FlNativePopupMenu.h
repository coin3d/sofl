/**************************************************************************\
 * BSD 3-Clause License
 *
 * Copyright (c) 2025, Fabrizio Morciano
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifndef SOFL_WXNATIVEPOPUPMENU_H
#define SOFL_WXNATIVEPOPUPMENU_H

#include <Inventor/Fl/widgets/SoFlPopupMenu.h>

#include <vector>
#include <FL/Fl_Menu_Item.H>

struct MenuRecord;
struct ItemRecord;

class FlNativePopupMenu
        : public SoFlPopupMenu {
    typedef SoFlPopupMenu inherited;
public:
    FlNativePopupMenu();
    virtual ~FlNativePopupMenu();
    virtual int newMenu(const char * name, int menuid = -1) override;
    virtual int getMenu(const char * name) override;
    virtual void setMenuTitle(int id, const char * title) override;
    virtual const char * getMenuTitle(int id) override;

    virtual int newMenuItem(const char * name, int itemid = -1) override;
    virtual int getMenuItem(const char * name) override;
    virtual void setMenuItemTitle(int itemid, const char * title) override;
    virtual const char * getMenuItemTitle(int itemid) override;
    virtual void setMenuItemEnabled(int itemid, SbBool enabled) override;
    virtual SbBool getMenuItemEnabled(int itemid) override;
    void setMenuItemMarked(int itemid, SbBool marked);
    virtual SbBool getMenuItemMarked(int itemid) override;

    virtual void addMenu(int menuid, int submenuid, int pos = -1) override;
    virtual void addMenuItem(int menuid, int itemid, int pos = -1) override;
    virtual void addSeparator(int menuid, int pos = -1) override;
    virtual void removeMenu(int menuid) override;
    virtual void removeMenuItem(int itemid) override;

    virtual void popUp(Fl_Window* inside, int x, int y) override;

protected:

    virtual void _setMenuItemMarked(int itemid, SbBool marked) override;

    MenuRecord * getMenuRecord(int menuid);
    ItemRecord * getItemRecord(int itemid);
    ItemRecord * getItemRecordFromId(int itemid);
    MenuRecord * createMenuRecord(const char * name);
    ItemRecord * createItemRecord(const char * name);

    void itemActivation(int);

    void build_fltk_menu(int menuid, std::vector<Fl_Menu_Item> & items);

private:
    SbPList * menus;
    SbPList * items;
};


#endif //SOFL_WXNATIVEPOPUPMENU_H
