#!/bin/bash -eux

git submodule update --init --recursive
# we are in ~/i-score
./tools/travis/deps.sh


case "$TRAVIS_OS_NAME" in
  linux)
    wget https://www.dropbox.com/s/lavj7413vfgrign/vtk_6.3-1_amd64.deb?dl=1 -O vtk.deb
    sudo dpkg -i vtk.deb
  ;;
  osx)
    set +e
    brew install homebrew/science/vtk
    set -e
  ;;
esac
