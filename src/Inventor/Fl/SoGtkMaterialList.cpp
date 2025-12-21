/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/
#if 0
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoMaterial.h>

#include <sogtkdefs.h>

#include <Inventor/Fl/SoAnyMaterialList.h>
#include <Inventor/Fl/SoFlMaterialList.h>

/*!
  \class SoFlMaterialList Inventor/Gtk/SoFlMaterialList.h
  \brief The SoFlMaterialList class is a component for selecting a material
  from a list of predefined materials.
*/

// *************************************************************************

SOWX_OBJECT_SOURCE(SoFlMaterialList);

// *************************************************************************

SoFlMaterialList::SoFlMaterialList(
  GtkWidget * parent,
  const char * const name,
  const SbBool embed,
  const char * const dir)
  : inherited(parent, name, embed)
{
  this->constructor(dir, TRUE);
} // SoFlMaterialList()

SoFlMaterialList::SoFlMaterialList(// protected
  GtkWidget * parent,
  const char * const name,
  const SbBool embed,
  const char * const dir,
  const SbBool build )
  : inherited(parent, name, embed)
{
  this->constructor(dir, build);
} // SoFlMaterialList()

void
SoFlMaterialList::constructor(// private
  const char * const dir,
  const SbBool build)
{
  this->common = new SoAnyMaterialList(dir);

  this->listwidget = NULL;
  this->setClassName("SoFlMaterialList");
  this->setSize(SbVec2s(200, 300));
  if (! build) return;
  GtkWidget * parent = this->getParentWidget();
  GtkWidget * materiallist = this->buildWidget(parent);
  this->setBaseWidget(materiallist);
} // constructor()

SoFlMaterialList::~SoFlMaterialList(
  void)
{
  delete this->common;
} // ~SoFlMaterialList()

// *************************************************************************

/*!
*/

GtkWidget *
SoFlMaterialList::buildWidget(// protected
  GtkWidget * parent)
{
  GtkWidget * listroot = GTK_WIDGET(gtk_vbox_new(FALSE, 0));
  GtkWidget * menubar = GTK_WIDGET(gtk_menu_bar_new());
  gtk_widget_set_usize(menubar, 0, 30);
  gtk_widget_show(menubar);
  gtk_box_pack_start(GTK_BOX(listroot), GTK_WIDGET(menubar), 0, 0, 0);
  GtkWidget * materials = 
    gtk_menu_item_new_with_label(_("Materials"));
  gtk_widget_show(materials);
  gtk_menu_bar_append(GTK_MENU_BAR(menubar), materials);

  GtkWidget * menu = this->buildPulldownMenu(materials);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(materials), menu);

  /* value, lower, upper, step_increment, page_increment, page_size */
  GtkAdjustment * vertical = GTK_ADJUSTMENT(gtk_adjustment_new(0, 0, 1000, 10, 100, 100));
  GtkAdjustment * horizontal = GTK_ADJUSTMENT(gtk_adjustment_new(0, 0, 1000, 10, 100, 1000));

  GtkWidget * scrolled = gtk_scrolled_window_new(horizontal, vertical);
  gtk_scrolled_window_set_policy(
    GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_widget_show(scrolled);

  this->listwidget = GTK_WIDGET(gtk_clist_new(1));
  gtk_widget_show(this->listwidget);
  gtk_signal_connect(GTK_OBJECT(this->listwidget), "select_row",
    GTK_SIGNAL_FUNC(SoFlMaterialList::itemactivationCB), (gpointer) this);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled), this->listwidget);

  SoGtkMaterialDirectory * dir = common->getMaterialDirectory();
  if (dir && dir->numGroups > 0) {
    SoGtkMaterialGroup * group = dir->groups[dir->current];
    for (int i = 0; i < group->numMaterials; i++) {
      const char ** ptr = &(group->materials[i]->name);
      gtk_clist_append(GTK_CLIST(this->listwidget), (char **) ptr);
    }
  }

  gtk_box_pack_start(GTK_BOX(listroot), scrolled, 1, 1, 0);

  return listroot;
} // buildWidget()

/*!
*/

GtkWidget *
SoFlMaterialList::buildPulldownMenu(// protected
  GtkWidget * parent)
{
  GtkWidget * menu = GTK_WIDGET(gtk_menu_new());
  gtk_widget_show(menu);

  SoGtkMaterialDirectory * data = common->getMaterialDirectory();
  if (data) {
    for (int i = 0; i < data->numGroups; i++) {
      GtkWidget * menuitem = GTK_WIDGET(gtk_menu_item_new_with_label(data->groups[i]->name));
      gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
        GTK_SIGNAL_FUNC(SoFlMaterialList::menuactivationCB), (gpointer) this);
      gtk_widget_show(menuitem);
      gtk_menu_append(GTK_MENU(menu), menuitem);
      data->groups[i]->menuitem = menuitem;
    }
  }

  return menu;
} // buildPulldownMenu()

// *************************************************************************

void
SoFlMaterialList::addCallback(
  SoFlMaterialListCB * const callback,
  void * const closure)
{
  common->addCallback(callback, closure);
} // addCallback()

void
SoFlMaterialList::removeCallback(
  SoFlMaterialListCB * const callback,
  void * const closure)
{
  common->removeCallback(callback, closure);
} // removeCallback()

// *************************************************************************

void
SoFlMaterialList::menuactivation(// private
  GtkWidget * menuitem)
{
  SoGtkMaterialDirectory * dir = common->getMaterialDirectory();
  if (dir) {
    for (int i = 0; i < dir->numGroups; i++) {
      SoGtkMaterialGroup * group = dir->groups[i];
      if (group->menuitem == menuitem) {
        gtk_clist_clear(GTK_CLIST(this->listwidget));
        for (int j = 0; j < group->numMaterials; j++) {
          const char ** ptr = &(group->materials[j]->name);
          gtk_clist_append(GTK_CLIST(this->listwidget), (char **) ptr);
        }
        dir->current = i;
        return;
      }
    }
  }
  SoDebugError::postWarning("SoFlMaterialList::menuactivation",
    _("invalid menu item"));
} // menuactivation()

void
SoFlMaterialList::menuactivationCB(// static, private
  GtkObject * obj,
  gpointer closure)
{
  assert(closure != NULL);
  SoFlMaterialList * component = (SoFlMaterialList *) closure;
  component->menuactivation(GTK_WIDGET(obj));
} // menuactivationCB()

// *************************************************************************

void
SoFlMaterialList::itemactivation(// private
  int materialid)
{
  SoGtkMaterialDirectory * data = common->getMaterialDirectory();
  assert(data != NULL);
  assert(materialid >= 0 &&
          materialid < data->groups[data->current]->numMaterials);
  const char * materialdata =
    data->groups[data->current]->materials[materialid]->data;

  SoInput reader;
  if (data->flags & SOGTK_BUILTIN_MATERIALS) {
    reader.setBuffer((void *) materialdata, strlen(materialdata));
  } else {
    if (! reader.openFile(materialdata, FALSE)) {
      SoDebugError::postWarning("SoFlMaterialList::itemactivation",
        "could not open file: \"%s\"", materialdata);
      return;
    }
  }

  SoNode * material = NULL;
  if (! SoDB::read(&reader, material)) {
    SoDebugError::postWarning("SoFlMaterialList::itemactivation",
      "failed to read material");
    return;
  }
  if (! material) {
    SoDebugError::postWarning("SoFlMaterialList::itemactivation",
      "read returned no data");
    return;
  }

  material->ref();
  if (! material->isOfType(SoMaterial::getClassTypeId())) {
    SoDebugError::postWarning("SoFlMaterialList::itemactivation",
      "not a material node!");
    material->unref();
    return;
  }
  common->invokeCallbacks((SoMaterial *) material);
  material->unref();
} // itemactivation()

void
SoFlMaterialList::itemactivationCB(// static, private
  GtkObject * obj,
  gint row,
  gint column,
  GdkEvent * event,
  gpointer closure)
{
  assert(closure != NULL);
  SoFlMaterialList * component = (SoFlMaterialList *) closure;
  component->itemactivation(row);
} // itemactivationCB()

// *************************************************************************

const char *
SoFlMaterialList::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoFlMaterialList";
  return defaultWidgetName;
} // getDefaultWidgetName()

const char *
SoFlMaterialList::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = N_("Material List");
  return _(defaultTitle);
} // getDefaultTitle()

const char *
SoFlMaterialList::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = N_("Material List");
  return _(defaultIconTitle);
} // getDefaultIconTitle()

// *************************************************************************
#endif