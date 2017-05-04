#!/bin/bash
set -eu

#
# Installs compiled binaries to /usr/local/bin, or DEST, if set
#

make
make -C remove_links
make -C latex_autocompile

binaries=(bin/* remove_links/rmln latex_autocompile/latex-autocompile)
strip "${binaries[@]}"
cp -Pv "${binaries[@]}" "${DEST:-/usr/local/bin}"

