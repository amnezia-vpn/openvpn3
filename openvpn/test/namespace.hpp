//    OpenVPN -- An application to securely tunnel IP networks
//               over a single port, with support for SSL/TLS-based
//               session authentication and key exchange,
//               packet encryption, packet authentication, and
//               packet compression.
//
//    Copyright (C) 2012-2022 OpenVPN Inc.
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

#pragma once

// Enable OPENVPN_NAMESPACE_ANON to compile all participating
// source files into an anonymous namespace under the openvpn
// namespace.  This is very useful for isolating the class
// hierarchy of different compilation units, and is mostly
// intended for unit testing.
// 
// Source files that would like to participate in
// OPENVPN_NAMESPACE_ANON should change code that looks
// like this:
//
//   namespace openvpn {
//     class Foo {
//     . . .
//     };
//   }
//
// to this:
//
//   BEGIN_NAMESPACE_OPENVPN
//   {
//     class Foo {
//     . . .
//     };
//   }
//   END_NAMESPACE_OPENVPN
//
// When OPENVPN_NAMESPACE_ANON is not defined, the above macro
// usage will expand to a standard namespace declaration:
//
//   namespace openvpn {
//     class Foo {
//     . . .
//     };
//   }
// 
// However, when OPENVPN_NAMESPACE_ANON is defined, we insert
// an anonymous namespace under openvpn to get compilation-unit
// isolation of all global symbols.
//
// This is helpful in unit testing, where all tests under a
// given repo are compiled into a single binary, but where each
// compilation unit might modify the definition of classes in a
// way that would create conflicts at the linker stage.

#ifdef OPENVPN_NAMESPACE_ANON
#define BEGIN_NAMESPACE_OPENVPN namespace openvpn { namespace
#define END_NAMESPACE_OPENVPN }
#else
#define BEGIN_NAMESPACE_OPENVPN namespace openvpn
#define END_NAMESPACE_OPENVPN
#endif
