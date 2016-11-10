/**
 * Multiparty Off-the-Record Messaging library
 * Copyright (C) 2016, eQualit.ie
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#pragma once

#include "defer.h"

namespace np1sec_plugin {

class ChannelView;

class UserView {
public:
    UserView(ChannelView&, const User&);
    ~UserView();

    UserView(const UserView&) = delete;
    UserView& operator=(const UserView&) = delete;

    UserView(UserView&&) = delete;
    UserView& operator=(UserView&&) = delete;

    PopupActions popup_actions;

    void update(const User&);

private:
    void expand(GtkTreeIter& iter);

private:
    ChannelView& _channel_view;
    GtkTreeIter _iter;
    bool _was_promoted = false;
};

} // np1sec_plugin namespace

#include "channel_view.h"
#include "user.h"

namespace np1sec_plugin {
//------------------------------------------------------------------------------
// Implementation
//------------------------------------------------------------------------------
UserView::UserView(ChannelView& channel, const User& user)
    : _channel_view(channel)
{
    auto& rv = _channel_view._room_view;
    auto* tree_store = rv._tree_store;

    gtk_tree_store_append(tree_store, &_iter, &_channel_view._iter);
    update(user);

    expand(_iter);

    auto path = util::tree_iter_to_path(_iter, rv._tree_store);

    rv._show_popup_callbacks[path] = [this] (GdkEventButton* e) {
        show_popup(e, popup_actions);
    };
}

UserView::~UserView() {
    gtk_tree_store_remove(_channel_view._room_view._tree_store, &_iter);
}

inline void UserView::update(const User& user)
{
    auto new_name = (user.is_myself() ? "*" + user.name() : user.name());

    //if (user.was_promoted()) {
    //    new_name += " (p)";
    //}
    //else {
        new_name += " " + util::str(user.authorized_by());
    //}

    gtk_tree_store_set(_channel_view._room_view._tree_store,
                       &_iter,
                       0, new_name.c_str(), -1);
}

inline
void UserView::expand(GtkTreeIter& iter) {
    auto& rv = _channel_view._room_view;

    auto tree_store = rv._tree_store;
    auto tree_view  = rv._tree_view;

    auto model = GTK_TREE_MODEL(tree_store);

    gchar* path_str = gtk_tree_model_get_string_from_iter(model, &iter);
    auto free_path_str = defer([path_str] { g_free(path_str); });

    GtkTreePath* path = gtk_tree_path_new_from_string(path_str);
    auto free_path = defer([path] { gtk_tree_path_free(path); });

    gtk_tree_view_expand_to_path(tree_view, path);
}

} // np1sec_plugin namespace

