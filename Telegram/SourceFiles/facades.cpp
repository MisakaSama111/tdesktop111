/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "facades.h"

#include "api/api_bot.h"
#include "info/info_memento.h"
#include "inline_bots/bot_attach_web_view.h"
#include "core/click_handler_types.h"
#include "core/application.h"
#include "media/clip/media_clip_reader.h"
#include "window/window_session_controller.h"
#include "window/window_peer_menu.h"
#include "history/history_item_components.h"
#include "base/platform/base_platform_info.h"
#include "data/data_peer.h"
#include "data/data_user.h"
#include "mainwindow.h"
#include "mainwidget.h"
#include "apiwrap.h"
#include "main/main_session.h"
#include "main/main_domain.h"
#include "ui/boxes/confirm_box.h"
#include "boxes/url_auth_box.h"
#include "ui/layers/layer_widget.h"
#include "lang/lang_keys.h"
#include "history/history.h"
#include "history/history_item.h"
#include "history/view/media/history_view_media.h"
#include "payments/payments_checkout_process.h"
#include "data/data_session.h"
#include "styles/style_chat.h"

namespace {

[[nodiscard]] MainWidget *CheckMainWidget(not_null<Main::Session*> session) {
	if (const auto m = App::main()) { // multi good
		if (&m->session() == session) {
			return m;
		}
	}
	if (&Core::App().domain().active() != &session->account()) {
		Core::App().domain().activate(&session->account());
	}
	if (const auto m = App::main()) { // multi good
		if (&m->session() == session) {
			return m;
		}
	}
	return nullptr;
}

} // namespace

namespace App {

void searchByHashtag(const QString &tag, PeerData *inPeer, PeerData *from) {
	const auto m = inPeer
		? CheckMainWidget(&inPeer->session())
		: App::main(); // multi good
	if (m) {
		if (m->controller()->openedFolder().current()) {
			m->controller()->closeFolder();
		}
		App::wnd()->ui_hideSettingsAndLayer(anim::type::instant);
		Core::App().hideMediaView();
		if (tag.startsWith("#")) {
			m->searchMessages(
				tag,
				(inPeer && !inPeer->isUser())
				? inPeer->owner().history(inPeer).get()
				: Dialogs::Key(),
				from);
		} else {
			m->searchMessages(
				tag,
				(inPeer && !inPeer->isUser())
				? inPeer->owner().history(inPeer).get()
				: Dialogs::Key(),
				from ? from : inPeer);
		}
	}
}

} // namespace App
