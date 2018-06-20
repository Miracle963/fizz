/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <vector>

#include <fizz/record/Types.h>

namespace fizz {

inline std::vector<Extension>::const_iterator findExtension(
    const std::vector<Extension>& extensions,
    ExtensionType type) {
  for (auto it = extensions.begin(); it != extensions.end(); ++it) {
    if (it->extension_type == type) {
      return it;
    }
  }
  return extensions.end();
}

template <>
inline folly::Optional<SignatureAlgorithms> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::signature_algorithms);
  if (it == extensions.end()) {
    return folly::none;
  }
  SignatureAlgorithms sigs;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::readVector<uint16_t>(sigs.supported_signature_algorithms, cursor);
  return sigs;
}

template <>
inline folly::Optional<SupportedGroups> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::supported_groups);
  if (it == extensions.end()) {
    return folly::none;
  }
  SupportedGroups groups;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::readVector<uint16_t>(groups.named_group_list, cursor);
  return groups;
}

template <>
inline folly::Optional<ClientKeyShare> getExtension(
    const std::vector<Extension>& extensions) {
  ClientKeyShare share;
  auto it = findExtension(extensions, ExtensionType::key_share);
  if (it == extensions.end()) {
    it = findExtension(extensions, ExtensionType::key_share_old);
    if (it == extensions.end()) {
      return folly::none;
    }
    share.preDraft23 = true;
  }
  folly::io::Cursor cursor(it->extension_data.get());
  detail::readVector<uint16_t>(share.client_shares, cursor);
  return std::move(share);
}

template <>
inline folly::Optional<ServerKeyShare> getExtension(
    const std::vector<Extension>& extensions) {
  ServerKeyShare share;
  auto it = findExtension(extensions, ExtensionType::key_share);
  if (it == extensions.end()) {
    it = findExtension(extensions, ExtensionType::key_share_old);
    if (it == extensions.end()) {
      return folly::none;
    }
    share.preDraft23 = true;
  }
  folly::io::Cursor cursor(it->extension_data.get());
  detail::read(share.server_share, cursor);
  return std::move(share);
}

template <>
inline folly::Optional<HelloRetryRequestKeyShare> getExtension(
    const std::vector<Extension>& extensions) {
  HelloRetryRequestKeyShare share;
  auto it = findExtension(extensions, ExtensionType::key_share);
  if (it == extensions.end()) {
    it = findExtension(extensions, ExtensionType::key_share_old);
    if (it == extensions.end()) {
      return folly::none;
    }
    share.preDraft23 = true;
  }
  folly::io::Cursor cursor(it->extension_data.get());
  detail::read(share.selected_group, cursor);
  return std::move(share);
}

template <>
inline folly::Optional<ClientPresharedKey> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::pre_shared_key);
  if (it == extensions.end()) {
    return folly::none;
  }
  ClientPresharedKey share;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::readVector<uint16_t>(share.identities, cursor);
  detail::readVector<uint16_t>(share.binders, cursor);
  return std::move(share);
}

template <>
inline folly::Optional<ServerPresharedKey> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::pre_shared_key);
  if (it == extensions.end()) {
    return folly::none;
  }
  ServerPresharedKey share;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::read(share.selected_identity, cursor);
  return std::move(share);
}

template <>
inline folly::Optional<ClientEarlyData> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::early_data);
  if (it == extensions.end()) {
    return folly::none;
  }
  return ClientEarlyData();
}

template <>
inline folly::Optional<ServerEarlyData> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::early_data);
  if (it == extensions.end()) {
    return folly::none;
  }
  return ServerEarlyData();
}

template <>
inline folly::Optional<TicketEarlyData> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::early_data);
  if (it == extensions.end()) {
    return folly::none;
  }
  TicketEarlyData early;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::read(early.max_early_data_size, cursor);
  return std::move(early);
}

template <>
inline folly::Optional<Cookie> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::cookie);
  if (it == extensions.end()) {
    return folly::none;
  }
  Cookie cookie;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::readBuf<uint16_t>(cookie.cookie, cursor);
  return std::move(cookie);
}

template <>
inline folly::Optional<SupportedVersions> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::supported_versions);
  if (it == extensions.end()) {
    return folly::none;
  }
  SupportedVersions versions;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::readVector<uint8_t>(versions.versions, cursor);
  return versions;
}

template <>
inline folly::Optional<ServerSupportedVersions> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::supported_versions);
  if (it == extensions.end()) {
    return folly::none;
  }
  ServerSupportedVersions versions;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::read(versions.selected_version, cursor);
  return versions;
}

template <>
inline folly::Optional<PskKeyExchangeModes> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::psk_key_exchange_modes);
  if (it == extensions.end()) {
    return folly::none;
  }
  PskKeyExchangeModes modes;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::readVector<uint8_t>(modes.modes, cursor);
  return modes;
}

template <>
inline folly::Optional<ProtocolNameList> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(
      extensions, ExtensionType::application_layer_protocol_negotiation);
  if (it == extensions.end()) {
    return folly::none;
  }
  ProtocolNameList names;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::readVector<uint16_t>(names.protocol_name_list, cursor);
  return std::move(names);
}

template <>
inline folly::Optional<ServerNameList> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::server_name);
  if (it == extensions.end()) {
    it = findExtension(extensions, ExtensionType::alternate_server_name);
    if (it == extensions.end()) {
      return folly::none;
    }
  }
  ServerNameList names;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::readVector<uint16_t>(names.server_name_list, cursor);
  return std::move(names);
}

template <>
inline folly::Optional<CertificateAuthorities> getExtension(
    const std::vector<Extension>& extensions) {
  auto it = findExtension(extensions, ExtensionType::certificate_authorities);
  if (it == extensions.end()) {
    return folly::none;
  }
  CertificateAuthorities authorities;
  folly::io::Cursor cursor(it->extension_data.get());
  detail::readVector<uint16_t>(authorities.authorities, cursor);
  return std::move(authorities);
}

template <>
inline Extension encodeExtension(const SignatureAlgorithms& sig) {
  Extension ext;
  ext.extension_type = ExtensionType::signature_algorithms;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::writeVector<uint16_t>(sig.supported_signature_algorithms, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const SupportedGroups& groups) {
  Extension ext;
  ext.extension_type = ExtensionType::supported_groups;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::writeVector<uint16_t>(groups.named_group_list, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const ClientKeyShare& share) {
  Extension ext;
  ext.extension_type = share.preDraft23 ? ExtensionType::key_share_old
                                        : ExtensionType::key_share;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::writeVector<uint16_t>(share.client_shares, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const ServerKeyShare& share) {
  Extension ext;
  ext.extension_type = share.preDraft23 ? ExtensionType::key_share_old
                                        : ExtensionType::key_share;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::write(share.server_share, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const HelloRetryRequestKeyShare& share) {
  Extension ext;
  ext.extension_type = share.preDraft23 ? ExtensionType::key_share_old
                                        : ExtensionType::key_share;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::write(share.selected_group, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const ClientPresharedKey& share) {
  Extension ext;
  ext.extension_type = ExtensionType::pre_shared_key;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::writeVector<uint16_t>(share.identities, appender);
  detail::writeVector<uint16_t>(share.binders, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const ServerPresharedKey& share) {
  Extension ext;
  ext.extension_type = ExtensionType::pre_shared_key;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::write(share.selected_identity, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const ClientEarlyData&) {
  Extension ext;
  ext.extension_type = ExtensionType::early_data;
  ext.extension_data = folly::IOBuf::create(0);
  return ext;
}

template <>
inline Extension encodeExtension(const ServerEarlyData&) {
  Extension ext;
  ext.extension_type = ExtensionType::early_data;
  ext.extension_data = folly::IOBuf::create(0);
  return ext;
}

template <>
inline Extension encodeExtension(const TicketEarlyData& early) {
  Extension ext;
  ext.extension_type = ExtensionType::early_data;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::write(early.max_early_data_size, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const Cookie& cookie) {
  Extension ext;
  ext.extension_type = ExtensionType::cookie;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::writeBuf<uint16_t>(cookie.cookie, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const SupportedVersions& versions) {
  Extension ext;
  ext.extension_type = ExtensionType::supported_versions;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::writeVector<uint8_t>(versions.versions, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const ServerSupportedVersions& versions) {
  Extension ext;
  ext.extension_type = ExtensionType::supported_versions;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::write(versions.selected_version, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const PskKeyExchangeModes& modes) {
  Extension ext;
  ext.extension_type = ExtensionType::psk_key_exchange_modes;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::writeVector<uint8_t>(modes.modes, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const ProtocolNameList& names) {
  Extension ext;
  ext.extension_type = ExtensionType::application_layer_protocol_negotiation;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::writeVector<uint16_t>(names.protocol_name_list, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const ServerNameList& names) {
  Extension ext;
  if (names.useAlternateCodePoint) {
    ext.extension_type = ExtensionType::alternate_server_name;
  } else {
    ext.extension_type = ExtensionType::server_name;
  }
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::writeVector<uint16_t>(names.server_name_list, appender);
  return ext;
}

template <>
inline Extension encodeExtension(const CertificateAuthorities& authorities) {
  Extension ext;
  ext.extension_type = ExtensionType::certificate_authorities;
  ext.extension_data = folly::IOBuf::create(0);
  folly::io::Appender appender(ext.extension_data.get(), 10);
  detail::writeVector<uint16_t>(authorities.authorities, appender);
  return ext;
}

inline size_t getBinderLength(const ClientHello& chlo) {
  if (chlo.extensions.empty() ||
      chlo.extensions.back().extension_type != ExtensionType::pre_shared_key) {
    throw FizzException(
        "psk not at end of client hello", AlertDescription::decode_error);
  }
  folly::io::Cursor cursor(chlo.extensions.back().extension_data.get());
  uint16_t identitiesLen;
  detail::read(identitiesLen, cursor);
  cursor.skip(identitiesLen);
  uint16_t binderLen;
  detail::read(binderLen, cursor);
  if (cursor.totalLength() != binderLen) {
    throw FizzException(
        "malformed binder length", AlertDescription::decode_error);
  }
  return sizeof(binderLen) + binderLen;
}

namespace detail {

template <>
struct Reader<KeyShareEntry> {
  template <class T>
  size_t read(KeyShareEntry& out, folly::io::Cursor& cursor) {
    size_t len = 0;
    len += detail::read(out.group, cursor);
    len += readBuf<uint16_t>(out.key_exchange, cursor);
    return len;
  }
};

template <>
struct Writer<KeyShareEntry> {
  template <class T>
  void write(const KeyShareEntry& share, folly::io::Appender& out) {
    detail::write(share.group, out);
    detail::writeBuf<uint16_t>(share.key_exchange, out);
  }
};

template <>
struct Sizer<KeyShareEntry> {
  template <class T>
  size_t getSize(const KeyShareEntry& share) {
    return sizeof(NamedGroup) + getBufSize<uint16_t>(share.key_exchange);
  }
};

template <>
struct Reader<PskIdentity> {
  template <class T>
  size_t read(PskIdentity& out, folly::io::Cursor& cursor) {
    size_t len = 0;
    len += readBuf<uint16_t>(out.psk_identity, cursor);
    len += detail::read(out.obfuscated_ticket_age, cursor);
    return len;
  }
};

template <>
struct Writer<PskIdentity> {
  template <class T>
  void write(const PskIdentity& ident, folly::io::Appender& out) {
    writeBuf<uint16_t>(ident.psk_identity, out);
    detail::write(ident.obfuscated_ticket_age, out);
  }
};

template <>
struct Sizer<PskIdentity> {
  template <class T>
  size_t getSize(const PskIdentity& ident) {
    return getBufSize<uint16_t>(ident.psk_identity) + sizeof(uint32_t);
  }
};

template <>
struct Reader<PskBinder> {
  template <class T>
  size_t read(PskBinder& out, folly::io::Cursor& cursor) {
    return readBuf<uint8_t>(out.binder, cursor);
  }
};

template <>
struct Writer<PskBinder> {
  template <class T>
  void write(const PskBinder& binder, folly::io::Appender& out) {
    writeBuf<uint8_t>(binder.binder, out);
  }
};

template <>
struct Sizer<PskBinder> {
  template <class T>
  size_t getSize(const PskBinder& binder) {
    return getBufSize<uint8_t>(binder.binder);
  }
};

template <>
struct Reader<ProtocolName> {
  template <class T>
  size_t read(ProtocolName& name, folly::io::Cursor& cursor) {
    return readBuf<uint8_t>(name.name, cursor);
  }
};

template <>
struct Writer<ProtocolName> {
  template <class T>
  void write(const ProtocolName& name, folly::io::Appender& out) {
    writeBuf<uint8_t>(name.name, out);
  }
};

template <>
struct Sizer<ProtocolName> {
  template <class T>
  size_t getSize(const ProtocolName& name) {
    return getBufSize<uint8_t>(name.name);
  }
};

template <>
struct Reader<ServerName> {
  template <class T>
  size_t read(ServerName& name, folly::io::Cursor& cursor) {
    size_t size = 0;
    size += detail::read(name.name_type, cursor);
    size += readBuf<uint16_t>(name.hostname, cursor);
    return size;
  }
};

template <>
struct Writer<ServerName> {
  template <class T>
  void write(const ServerName& name, folly::io::Appender& out) {
    detail::write(name.name_type, out);
    writeBuf<uint16_t>(name.hostname, out);
  }
};

template <>
struct Sizer<ServerName> {
  template <class T>
  size_t getSize(const ServerName& name) {
    return sizeof(ServerNameType) + getBufSize<uint16_t>(name.hostname);
  }
};

template <>
struct Reader<DistinguishedName> {
  template <class T>
  size_t read(DistinguishedName& dn, folly::io::Cursor& cursor) {
    return readBuf<uint16_t>(dn.encoded_name, cursor);
  }
};

template <>
struct Writer<DistinguishedName> {
  template <class T>
  void write(const DistinguishedName& dn, folly::io::Appender& out) {
    writeBuf<uint16_t>(dn.encoded_name, out);
  }
};

template <>
struct Sizer<DistinguishedName> {
  template <class T>
  size_t getSize(const DistinguishedName& dn) {
    return getBufSize<uint16_t>(dn.encoded_name);
  }
};
} // namespace detail
} // namespace fizz