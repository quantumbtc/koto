zcash_packages := libsodium utfcpp
packages := boost libevent zeromq $(zcash_packages) googletest curl
native_packages := native_clang native_ccache native_rust

qt_native_packages = native_protobuf
qt_packages = qrencode protobuf zlib

qt_linux_packages:=qt expat dbus libxcb xcb_proto libXau xproto freetype fontconfig libX11 xextproto libXext xtrans

qt_darwin_packages=qt
qt_mingw32_packages=qt

wallet_packages=bdb

darwin_native_packages = native_biplist native_ds_store native_mac_alias

ifneq ($(build_os),darwin)
darwin_native_packages += native_cctools native_cdrkit native_libdmg-hfsplus
endif

# We use a complete SDK for Darwin, which includes libc++.
ifneq ($(host_os),darwin)
packages += libcxx
endif
