package=curl
$(package)_version=7.64.0
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_download_path=https://curl.haxx.se/download
$(package)_sha256_hash=cb90d2eb74d4e358c1ed1489f8e3af96b50ea4374ad71f143fa4595e998d81b5

define $(package)_set_vars
  $(package)_config_opts=--disable-shared --without-ssl
  $(package)_cflags_aarch64_linux=-fPIC
  $(package)_cflags_darwin=-mmacosx-version-min=$(OSX_MIN_VERSION)
endef

define $(package)_config_cmds
  $($(package)_autoconf)
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef
