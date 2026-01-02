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

/*!
  \class FlNativePopupMenu Inventor/Fl/widgets/FlNativePopupMenu.h
  \brief The SoFlNativePopupMenu class implements a common interface for pop-up
  menu management for all the Coin GUI toolkit libraries.
*/

#include "FlNativePopupMenu.h"
#include "Inventor/Fl/viewers/SoFlExaminerViewerP.h"
#include "sofldefs.h"
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Menu_Item.H>
#include <string>


struct MenuRecord {
    int menuid;
    std::string name;
    std::string title;
    Fl_Menu_Window* menu;
    Fl_Menu_Window* parent;
};
struct ItemRecord {
    int itemid;
    int flags;
    std::string name;
    std::string title;
    Fl_Menu_Window* parent;
    Fl_Menu_Item * action;
};

#define ITEM_MARKED       0x0001
#define ITEM_SEPARATOR    0x0002
#define ITEM_ENABLED      0x0004

/*!
  The constructor.
*/
FlNativePopupMenu::FlNativePopupMenu(void) {
    this->menus = new SbPList;
    this->items = new SbPList;
}

/*!
  Destructor.
*/
FlNativePopupMenu::~FlNativePopupMenu() {
    const int numMenus = this->menus->getLength();

    int i;
    for (i = 0; i < numMenus; i++) {
        MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
        delete rec->menu;
        delete rec;
    }

    const int numItems = this->items->getLength();
    for (i = 0; i < numItems; i++) {
        ItemRecord * rec = (ItemRecord *) (*this->items)[i];
        delete rec;
    }

    delete this->menus;
    delete this->items;

}

/*!
*/
int
FlNativePopupMenu::newMenu(const char * name,
                           int menuid) {
    // FIXME: this function is the same in the other So-libraries --

    int id = menuid;
    if (id == -1) {
        id = 1;
        while (this->getMenuRecord(id) != NULL) id++;
    } else {
        assert(this->getMenuRecord(id) == NULL &&
               "requested menuid already taken");
    }

    // id contains ok ID
    MenuRecord * rec = this->createMenuRecord(name);
    rec->menuid = id;
    this->menus->append((void *) rec);
    return id;
}

/*!
*/
int
FlNativePopupMenu::getMenu(const char * name) {
    const int numMenus = this->menus->getLength();
    int i;
    for (i = 0; i < numMenus; i++) {
        MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
        if (rec->name == name)
            return rec->menuid;
    }
    return -1;
}

/*!
*/
void
FlNativePopupMenu::setMenuTitle(int menuid,
                                const char * title) {
    MenuRecord * rec = this->getMenuRecord(menuid);
    assert(rec && "no such menu");
    rec->title = title;

    if (rec->parent)  {
        rec->menu->label(rec->title.c_str());
        getMenuRecord(rec->menuid)->menu->label(rec->title.c_str());
    }
}

/*!
*/
const char *
FlNativePopupMenu::getMenuTitle(int menuid) {
    MenuRecord * rec = this->getMenuRecord(menuid);
    assert(rec && "no such menu");
    return rec->title.c_str();
}

/*!
*/
int
FlNativePopupMenu::newMenuItem(const char * name,
                               int itemid) {
    int id = itemid;
    if (id == -1) {
        id = 1;
        while (this->getItemRecord(id) != NULL) id++;
    } else {
        assert(this->getItemRecord(itemid) == NULL &&
               "requested itemid already taken");
    }
    ItemRecord * rec = createItemRecord(name);
    rec->itemid = id;
    this->items->append(rec);
    return id;
}

/*!
*/
int
FlNativePopupMenu::getMenuItem(const char * name) {
    const int numItems = this->items->getLength();
    int i;
    for (i = 0; i < numItems; i++) {
        ItemRecord * rec = (ItemRecord *) (*this->items)[i];
        if (rec->name == name)
            return rec->itemid;
    }
    return -1;
}

/*!
*/
void
FlNativePopupMenu::setMenuItemTitle(int itemid,
                                    const char * title) {
    ItemRecord * rec = this->getItemRecord(itemid);
    assert(rec && "no such menu");
    rec->title = title;
    if (rec->parent) {
        rec->action->label(rec->title.c_str());
    }
}

/*!
*/
const char *
FlNativePopupMenu::getMenuItemTitle(int itemid) {
    ItemRecord * rec = this->getItemRecord(itemid);
    assert(rec && "no such menu");
    return rec->title.c_str();
}

/*!
*/
void
FlNativePopupMenu::setMenuItemEnabled(int itemid,
                                      SbBool enabled) {
    ItemRecord * rec = this->getItemRecord(itemid);
    if (rec) {
        rec->action->value(enabled ? true : false);
        return;
    }
}

/*!
*/
SbBool
FlNativePopupMenu::getMenuItemEnabled(int itemid) {
    ItemRecord * rec = this->getItemRecord(itemid);

    if (rec) {
        return  (rec->action->value());
    }
    return (FALSE);
}

/*!
*/
void
FlNativePopupMenu::_setMenuItemMarked(int itemid, SbBool marked) {
    ItemRecord * rec = this->getItemRecord(itemid);
    if (rec == NULL)
        return;
    if (marked)
        rec->flags |= ITEM_MARKED;
    else
        rec->flags &= ~ITEM_MARKED;

    if (rec->parent  && rec->action) {
        rec->action->value(marked != 0);
    }
}

/*!
*/
SbBool
FlNativePopupMenu::getMenuItemMarked( int itemid) {
    ItemRecord * rec = this->getItemRecord(itemid);
    assert(rec && "no such menu");
    if (rec->parent == NULL)
        return (rec->flags & ITEM_MARKED) ? TRUE : FALSE;

    return (rec->action->value());
}

/*!
*/
void
FlNativePopupMenu::addMenu(int menuid,
                           int submenuid,
                           int pos) {
    MenuRecord * super = this->getMenuRecord(menuid);
    MenuRecord * sub = this->getMenuRecord(submenuid);
    assert(super && sub && "no such menu");

    /*
    wxMenuItem* action = 0;
    if (pos == -1) {
        action = super->menu->AppendSubMenu(sub->menu, sub->title);
    }
    else {
        super->menu->Insert(pos, sub->menuid,  sub->title, sub->menu);
    }
    action->SetItemLabel(sub->title);
    */
    sub->parent = super->menu;
}

/*!
*/
void
FlNativePopupMenu::addMenuItem(int menuid,
                               int itemid,
                               int pos) {
    MenuRecord * menu = this->getMenuRecord(menuid);
    assert(menu && "invalid parent menu id");
    ItemRecord * item = this->getItemRecord(itemid);
    assert(item && "invalid child menu id");
/*
    if (pos == -1) {
        item->action = menu->menu->AppendCheckItem(itemid, item->title);
    }
    else {
        item->action =  menu->menu->InsertCheckItem(pos,itemid,  item->title);
    }
    assert(item->action);
    item->parent = menu->menu;

    item->action->Check(true);
    if (item->flags & ITEM_MARKED) {
        item->action->Check(true);
    }
    */
}

void
FlNativePopupMenu::addSeparator(int menuid,
                                int pos) {
    MenuRecord * menu = this->getMenuRecord(menuid);
    assert(menu && "no such menu");

    ItemRecord * rec = createItemRecord("separator");
    /*
    menu->menu->InsertSeparator(pos);
    */
    rec->flags |= ITEM_SEPARATOR;
    this->items->append(rec);
}

/*!
  This method removes the submenu with the given \a menuid.

  A removed menu can be attached again later - its menu id will still be
  allocated.
*/
void
FlNativePopupMenu::removeMenu(int menuid) {
    MenuRecord * rec = this->getMenuRecord(menuid);
    assert(rec && "no such menu");

    if (rec->menuid == 0) {
#if SOFL_DEBUG && 0
        SoDebugError::postInfo("SoFlNativePopupMenu::RemoveMenu", "can't remove root");
#endif
        return;
    }
    if (rec->parent == NULL) {
#if SOFL_DEBUG && 0
        SoDebugError::postInfo("SoFlNativePopupMenu::RemoveMenu", "menu not attached");
#endif
        return;
    }
 //   rec->parent->Delete(rec->menuid);
    rec->parent = NULL;
}

/*!
  This method removes the menu item with the given \a itemid.

  A removed menu item can be attached again later - its item id will still
  be allocated.
*/
void
FlNativePopupMenu::removeMenuItem(int itemid) {
    ItemRecord * rec = this->getItemRecord(itemid);
    assert(rec && "no such item");

    if (rec->parent == NULL) {
#if SOFL_DEBUG && 0
        SoDebugError::postInfo("SoFlNativePopupMenu::RemoveMenu", "item not attached");
#endif
        return;
    }
    //rec->parent->Remove(rec->itemid);
    rec->parent = NULL;
}

// Doc in superclass.
void
FlNativePopupMenu::popUp(Fl_Window * inside, int x, int y) {
    MenuRecord * rec = this->getMenuRecord(0);
    //inside->PopupMenu(rec->menu);
}

/*!
*/
MenuRecord *
FlNativePopupMenu::getMenuRecord(int menuid) {
    const int numMenus = this->menus->getLength();
    int i;
    for (i = 0; i < numMenus; i++)
        if (((MenuRecord *) (*this->menus)[i])->menuid == menuid)
            return (MenuRecord *) (*this->menus)[i];
    return (MenuRecord *) NULL;
}

/*!
*/
ItemRecord *
FlNativePopupMenu::getItemRecord(int itemid) {
    const int numItems = this->items->getLength();
    for (int i = 0; i < numItems; i++) {
        const int recid = ((ItemRecord *) (*this->items)[i])->itemid;
        if (recid == itemid) {
            return (ItemRecord *) (*this->items)[i];
        }
    }
    return (ItemRecord *) NULL;
}

/*!
*/
MenuRecord *
FlNativePopupMenu::createMenuRecord(const char * name) {
    MenuRecord * rec = new MenuRecord;
    rec->menuid = -1;
    rec->name =  name;
    rec->title = name;

    /*
    rec->menu = new  wxMenu("");
    rec->menu->Connect(wxEVT_COMMAND_MENU_SELECTED,
                       wxCommandEventHandler(FlNativePopupMenu::itemActivation),
                       0,
                       this);
*/
    rec->parent = NULL;
    return rec;
}

/*!
*/
ItemRecord *
FlNativePopupMenu::createItemRecord(const char * name) {
    ItemRecord * rec = new ItemRecord;
    rec->itemid = -1;
    rec->flags = 0;
    rec->name = name;
    rec->title =  name;
    rec->parent = NULL;
    rec->action = NULL;
    return rec;
}

ItemRecord *
FlNativePopupMenu::getItemRecordFromId(int itemid) {
    const int numItems = this->items->getLength();
    for (int i = 0; i < numItems; i++) {
        const ItemRecord * rec = static_cast<ItemRecord *>((*this->items)[i]);
        if (rec->itemid == itemid) { return (ItemRecord *) (*this->items)[i]; }
    }

    return (ItemRecord *) NULL;
}

void
FlNativePopupMenu::itemActivation(int action) {
    /*
    int id = action.GetId();
    ItemRecord *rec = getItemRecordFromId(id);
    assert(rec);
    inherited::invokeMenuSelection(rec->itemid);
    */
}

