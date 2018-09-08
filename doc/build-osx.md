Mac OS X Build Instructions and Notes
====================================
The commands in this guide should be executed in a Terminal application.
The built-in one is located in `/Applications/Utilities/Terminal.app`.

Preparation
-----------

Install the OS X command line tools:

`xcode-select --install`

When the popup appears, click `Install`.

Then install [Homebrew](https://brew.sh).

Dependencies
----------------------

    brew install autoconf automake libtool boost openssl pkg-config protobuf python libevent 

Build Koto
------------------------

1. Clone the koto source code and cd into `koto`

        git clone https://github.com/KotoDevelopers/koto.git
        cd koto

2.  Build koto:

        ./zcutil/build-mac.sh

For further documentation on the depends system see [README.md](../depends/README.md) in the depends directory.

Running
-------

Koto is now available at `./src/kotod`

Before running, it's recommended you create an RPC configuration file.

    echo -e "rpcuser=rpc\nrpcpassword=$(xxd -l 16 -p /dev/urandom)" > "/Users/${USER}/Library/Application Support/Koto/koto.conf"
    chmod 600 "/Users/${USER}/Library/Application Support/Koto/koto.conf"

The first time you run kotod, it will start downloading the blockchain. This process could take several hours.

You can monitor the download process by looking at the debug.log file:

    tail -f $HOME/Library/Application\ Support/Koto/debug.log

Other commands:
-------

    ./kotod -daemon    # to start the koto daemon.
    ./koto-cli --help  # for a list of command-line options.
    ./koto-cli help    # When the daemon is running, to get a list of RPC commands

Notes
-----

* Tested on OS X 10.10 through 10.13 on 64-bit Intel processors only.
