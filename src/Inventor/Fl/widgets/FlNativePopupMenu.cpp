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
#include <FL/Fl.H>
#include <FL/Fl_Menu_Item.H>
#include <string>
#include <vector>
#include <stdint.h>

struct ChildRecord {
    enum Type { ITEM, MENU, SEPARATOR };
    Type type = SEPARATOR;
    int id = -1;
};

struct MenuRecord {
    int menuid = -1;
    std::string name;
    std::string title;
    std::vector<ChildRecord> children;
};

struct ItemRecord {
    int itemid = -1;
    int flags = 0;
    std::string name;
    std::string title;
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
        MenuRecord * rec = static_cast<MenuRecord *>((*this->menus)[i]);
        delete rec;
    }

    const int numItems = this->items->getLength();
    for (i = 0; i < numItems; i++) {
        ItemRecord * rec = static_cast<ItemRecord *>((*this->items)[i]);
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
        while (this->getMenuRecord(id) != nullptr) id++;
    } else {
        assert(this->getMenuRecord(id) == nullptr &&
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
        MenuRecord * rec = static_cast<MenuRecord *>((*this->menus)[i]);
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
        while (this->getItemRecord(id) != nullptr) id++;
    } else {
        assert(this->getItemRecord(itemid) == nullptr &&
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
        ItemRecord * rec = static_cast<ItemRecord *>((*this->items)[i]);
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
    assert(rec && "no such item");
    rec->title = title;
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
        if (enabled)
            rec->flags |= ITEM_ENABLED;
        else
            rec->flags &= ~ITEM_ENABLED;
    }
}

/*!
*/
SbBool
FlNativePopupMenu::getMenuItemEnabled(int itemid) {
    ItemRecord * rec = this->getItemRecord(itemid);
    if (rec) {
        return (rec->flags & ITEM_ENABLED) ? TRUE : FALSE;
    }
    return (FALSE);
}

/*!
*/
void
FlNativePopupMenu::_setMenuItemMarked(int itemid, SbBool marked) {
    ItemRecord * rec = this->getItemRecord(itemid);
    if (rec == nullptr)
        return;
    if (marked)
        rec->flags |= ITEM_MARKED;
    else
        rec->flags &= ~ITEM_MARKED;
}

/*!
*/
SbBool
FlNativePopupMenu::getMenuItemMarked( int itemid) {
    ItemRecord * rec = this->getItemRecord(itemid);
    assert(rec && "no such menu");
    return (rec->flags & ITEM_MARKED) ? TRUE : FALSE;
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

    ChildRecord child;
    child.type = ChildRecord::MENU;
    child.id = submenuid;

    if (pos == -1 || pos >= (int)super->children.size()) {
        super->children.push_back(child);
    } else {
        super->children.insert(super->children.begin() + pos, child);
    }
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
    assert(item && "invalid child item id");

    ChildRecord child;
    child.type = ChildRecord::ITEM;
    child.id = itemid;

    if (pos == -1 || pos >= (int)menu->children.size()) {
        menu->children.push_back(child);
    } else {
        menu->children.insert(menu->children.begin() + pos, child);
    }
}

void
FlNativePopupMenu::addSeparator(int menuid,
                                int pos) {
    MenuRecord * menu = this->getMenuRecord(menuid);
    assert(menu && "no such menu");

    ChildRecord child;
    child.type = ChildRecord::SEPARATOR;
    child.id = -1;

    if (pos == -1 || pos >= (int)menu->children.size()) {
        menu->children.push_back(child);
    } else {
        menu->children.insert(menu->children.begin() + pos, child);
    }
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
        return;
    }

    // Remove from all parents
    const int numMenus = this->menus->getLength();
    for (int i = 0; i < numMenus; i++) {
        MenuRecord * m = static_cast<MenuRecord *>((*this->menus)[i]);
        for (auto it = m->children.begin(); it != m->children.end(); ) {
            if (it->type == ChildRecord::MENU && it->id == menuid) {
                it = m->children.erase(it);
            } else {
                ++it;
            }
        }
    }
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

    // Remove from all parents
    const int numMenus = this->menus->getLength();
    for (int i = 0; i < numMenus; i++) {
        MenuRecord * m = static_cast<MenuRecord *>((*this->menus)[i]);
        for (auto it = m->children.begin(); it != m->children.end(); ) {
            if (it->type == ChildRecord::ITEM && it->id == itemid) {
                it = m->children.erase(it);
            } else {
                ++it;
            }
        }
    }
}

// Helper to build FLTK menu items
void
FlNativePopupMenu::build_fltk_menu(int menuid,
                                   std::vector<Fl_Menu_Item> & fl_items) {
    MenuRecord * menu = this->getMenuRecord(menuid);
    if (!menu) return;

    for (const auto & child : menu->children) {
        if (child.type == ChildRecord::ITEM) {
            ItemRecord * item = this->getItemRecord(child.id);
            if (item) {
                Fl_Menu_Item fl_item;
                memset(&fl_item, 0, sizeof(fl_item));
                fl_item.text = item->title.c_str();
                fl_item.user_data_ = reinterpret_cast<void *>(static_cast<uintptr_t>(item->itemid));
                fl_item.flags = 0;
                if (!(item->flags & ITEM_ENABLED)) fl_item.flags |= FL_MENU_INACTIVE;
                if (item->flags & ITEM_MARKED) fl_item.flags |= FL_MENU_TOGGLE | FL_MENU_VALUE;
                fl_items.push_back(fl_item);
            }
        } else if (child.type == ChildRecord::MENU) {
            MenuRecord * sub = this->getMenuRecord(child.id);
            if (sub) {
                Fl_Menu_Item fl_item;
                memset(&fl_item, 0, sizeof(fl_item));
                fl_item.text = sub->title.c_str();
                fl_item.flags = FL_SUBMENU;
                fl_items.push_back(fl_item);

                this->build_fltk_menu(child.id, fl_items);

                // Close submenu
                memset(&fl_item, 0, sizeof(fl_item));
                fl_item.text = nullptr;
                fl_items.push_back(fl_item);
            }
        } else if (child.type == ChildRecord::SEPARATOR) {
            if (!fl_items.empty()) {
                fl_items.back().flags |= FL_MENU_DIVIDER;
            }
        }
    }
}

// Doc in superclass.
void
FlNativePopupMenu::popUp(Fl_Window * inside, int x, int y) {
    std::vector<Fl_Menu_Item> fl_items;
    this->build_fltk_menu(0, fl_items);

    // End of menu
    Fl_Menu_Item end_item;
    memset(&end_item, 0, sizeof(end_item));
    fl_items.push_back(end_item);

    const Fl_Menu_Item * picked = fl_items.data()->popup(x, y);
    if (picked && picked->user_data_) {
        int itemid = static_cast<int>(reinterpret_cast<uintptr_t>(picked->user_data_));
        this->invokeMenuSelection(itemid);
    }
}

/*!
*/
MenuRecord *
FlNativePopupMenu::getMenuRecord(int menuid) {
    const int numMenus = this->menus->getLength();
    int i;
    for (i = 0; i < numMenus; i++)
        if (static_cast<MenuRecord *>((*this->menus)[i])->menuid == menuid)
            return static_cast<MenuRecord *>((*this->menus)[i]);
    return nullptr;
}

/*!
*/
ItemRecord *
FlNativePopupMenu::getItemRecord(int itemid) {
    const int numItems = this->items->getLength();
    for (int i = 0; i < numItems; i++) {
        const int recid = static_cast<ItemRecord *>((*this->items)[i])->itemid;
        if (recid == itemid) {
            return static_cast<ItemRecord *>((*this->items)[i]);
        }
    }
    return nullptr;
}

/*!
*/
MenuRecord *
FlNativePopupMenu::createMenuRecord(const char * name) {
    MenuRecord * rec = new MenuRecord;
    rec->menuid = -1;
    rec->name =  name;
    rec->title = name;
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
    return rec;
}

ItemRecord *
FlNativePopupMenu::getItemRecordFromId(int itemid) {
    const int numItems = this->items->getLength();
    for (int i = 0; i < numItems; i++) {
        const auto * rec = static_cast<const ItemRecord *>((*this->items)[i]);
        if (rec->itemid == itemid) { return static_cast<ItemRecord *>((*this->items)[i]); }
    }

    return nullptr;
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

