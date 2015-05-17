//    OpenVPN -- An application to securely tunnel IP networks
//               over a single port, with support for SSL/TLS-based
//               session authentication and key exchange,
//               packet encryption, packet authentication, and
//               packet compression.
//
//    Copyright (C) 2012-2015 OpenVPN Technologies, Inc.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License Version 3
//    as published by the Free Software Foundation.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program in the COPYING file.
//    If not, see <http://www.gnu.org/licenses/>.

#ifndef OPENVPN_AUTH_AUTHCREDS
#define OPENVPN_AUTH_AUTHCREDS

#include <utility> // for std::move
#include <string>

#include <openvpn/common/types.hpp>
#include <openvpn/common/exception.hpp>
#include <openvpn/common/rc.hpp>
#include <openvpn/common/options.hpp>
#include <openvpn/common/unicode.hpp>
#include <openvpn/buffer/safestr.hpp>

namespace openvpn {

    class AuthCreds : public RC<thread_unsafe_refcount>
    {
    public:
      typedef boost::intrusive_ptr<AuthCreds> Ptr;

      AuthCreds(std::string&& username_arg,
		SafeString&& password_arg,
		const std::string& peer_info_str)
	: username(std::move(username_arg)),
	  password(std::move(password_arg))
      {
	peer_info.parse_from_peer_info(peer_info_str, nullptr);
	peer_info.update_map();
      }

      void wipe_password()
      {
	password.wipe();
      }

      std::string to_string() const
      {
	std::ostringstream os;
	os << "*** AuthCreds ***" << std::endl;
	os << "user: '" << username << "'" << std::endl;
	os << "pass: '" << password << "'" << std::endl; // fixme
	os << "peer info:" << std::endl;
	os << peer_info.render(Option::RENDER_BRACKET|Option::RENDER_NUMBER);
	return os.str();
      }

      std::string username;
      SafeString password;
      OptionList peer_info;
    };

}

#endif
