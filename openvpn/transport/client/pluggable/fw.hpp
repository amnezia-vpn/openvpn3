//    OpenVPN -- An application to securely tunnel IP networks
//               over a single port, with support for SSL/TLS-based
//               session authentication and key exchange,
//               packet encryption, packet authentication, and
//               packet compression.
//
//    Copyright (C) 2012-2020 OpenVPN Inc.
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

#ifndef OPENVPN_TRANSPORT_CLIENT_PLUGGABLE_FW_H
#define OPENVPN_TRANSPORT_CLIENT_PLUGGABLE_FW_H

#define OPENVPN_PLUGGABLE_TRANSPORTS 1

#ifdef OPENVPN_PLUGGABLE_TRANSPORTS
#include <openvpn/transport/client/pluggable/pt.hpp>
#include <string>

#include "ck-ovpn-plugin.h"

#endif

namespace openvpn {
namespace PluggableTransports {

#ifdef OPENVPN_PLUGGABLE_TRANSPORTS
class CloakTransport : public PluggableTransports::Connection, public PluggableTransports::Transport, public RC<thread_unsafe_refcount> {
 public:
  CloakTransport(){};
  CloakTransport(openvpn_io::ip::tcp::endpoint address) {
  
    int ret = 0;

    char* config = getenv("CLOAK_CONFIG");

    if (config == nullptr){
      return;
    }

    if (*config == '\0') {
      return;
    }

    // Setup cloak config
    ret = Initialize_cloak_c_client(config);

    if (ret < 0) {
      return;
    }

    // Start cloak connection
    client_id = Cloak_dial();

    inited = true;
  };

  size_t send(const openvpn_io::const_buffer& buffer) {
    if (!inited) {
      return -1;
    }

    std::lock_guard<std::mutex> lock(send_mt);

    GoInt number_of_characters_sent =
        Cloak_write(client_id, (void*)buffer.data(), (int)buffer.size());

    if (number_of_characters_sent < 0) {
      goto error;
    }

    return number_of_characters_sent;

  error:
    return -1;
  };

  size_t receive(const openvpn_io::mutable_buffer& buffer) {
    if (!inited) {
      return -1;
    }
    
    std::lock_guard<std::mutex> lock(recv_mt);

    GoInt number_of_bytes_read =
        Cloak_read(client_id, (void*)buffer.data(), (int)buffer.size());

    if (number_of_bytes_read < 0) {
      return -1;
    }

    return number_of_bytes_read;
  };

  void close() { 
       Cloak_close_connection(client_id); 
       inited = false;     
  };

  int native_handle() { return Cloak_native_handle(); };

  PluggableTransports::Connection::Ptr dial(
      openvpn_io::ip::tcp::endpoint address) {
    return new CloakTransport(address);
  };

 private:

  std::mutex recv_mt;
  std::mutex send_mt;
  GoInt client_id;
  
  bool inited = false;
};

struct Factory {
  virtual openvpn::PluggableTransports::Transport::Ptr new_transport_factory() {
    return new CloakTransport();
  }
  virtual ~Factory() {}
};
#else
struct Factory {};
#endif
}  // namespace PluggableTransports
}  // namespace openvpn
#endif
