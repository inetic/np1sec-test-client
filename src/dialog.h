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

namespace np1sec_plugin {

class Dialog {
public:
    Dialog(PurpleConversation* conv, const std::string&);

    ~Dialog();
private:
    static void on_response(GtkDialog*, gint response_id, Dialog*);

private:
    GtkWidget* _dialog;
};

//------------------------------------------------------------------------------
// Implementation
//------------------------------------------------------------------------------
Dialog::Dialog(PurpleConversation* conv, const std::string& msg)
{
    auto* gtkconv = PIDGIN_CONVERSATION(conv);

    if (!gtkconv) {
        assert(0 && "Not a pidgin conversation");
        return;
    }

    _dialog = gtk_message_dialog_new( GTK_WINDOW(gtkconv->win->window)
                                    , GTK_DIALOG_MODAL
                                    , GTK_MESSAGE_WARNING
                                    , GTK_BUTTONS_OK
                                    , "%s"
                                    , msg.c_str());
    gtk_widget_show_all(_dialog);

    g_signal_connect( GTK_DIALOG(_dialog)
                    , "response"
                    , G_CALLBACK(on_response)
                    , this);
}

Dialog::~Dialog()
{
    gtk_widget_destroy(_dialog);
}

void Dialog::on_response(GtkDialog*, gint response_id, Dialog* self)
{
    delete self;
}

} // np1sec_plugin namespace


